/*
 * libswid, The SWID Tag Manipulation Library
 * Copyright (C) 2018, Red Hat Security Compliance Team.
 *
 * Red Hat, Inc., Durham, North Carolina hereby disclaims
 * all copyright interest in the library 'libswid'.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */
#include <sstream>
#include <map>

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/sax/HandlerBase.hpp>

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationRegistry.hpp>
#include <xercesc/dom/DOMLSSerializer.hpp>
#include <xercesc/dom/DOMLSOutput.hpp>

#include <xercesc/framework/LocalFileFormatTarget.hpp>

// Other include files, declarations, and non-Xerces-C++ initializations.

#include "loader-xerces.h"
#include "Translator.h"

#include "loader-generic.cpp"

using namespace xercesc;
using std::ostringstream;
using std::string;
using std::map;


// TODO: Xerces is bad with memory, because transcoded strings have to be released and aren't.
// See also: https://stackoverflow.com/questions/9826518/purpose-of-xmlstringtranscode
// and https://alfps.wordpress.com/2010/05/27/cppx-xerces-strings-simplified-by-ownership-part-i/#ownership
// See https://stackoverflow.com/a/2897419/592892

#define X2(str) XStr(str).unicodeForm()

#define Y(str) XMLString::transcode(str)
#define X(str) XMLString::transcode(str)
#define R(var) XMLString::release(& var)

// Taken from https://stackoverflow.com/a/2897419/592892
static void OutputXML(DOMDocument * pmyDOMDocument, string filePath)
{
	auto * ls = X("LS");
	//Return the first registered implementation that has the desired features. In this case, we are after a DOM implementation that has the LS feature... or Load/Save.
	auto * implementation = DOMImplementationRegistry::getDOMImplementation(ls);
	R(ls);

	// Create a DOMLSSerializer which is used to serialize a DOM tree into an XML document.
	auto * serializer = ((DOMImplementationLS*)implementation)->createLSSerializer();

	// Make the output more human readable by inserting line feeds.
	if (serializer->getDomConfig()->canSetParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true))
		serializer->getDomConfig()->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true);

	auto * xml_endl = X("\n");
	// The end-of-line sequence of characters to be used in the XML being written out.
	serializer->setNewLine(xml_endl);

	// Convert the path into Xerces compatible XMLCh*.
	auto * tempFilePath = X(filePath.c_str());

	// Specify the target for the XML output.
	auto * formatTarget = new LocalFileFormatTarget(tempFilePath);

	R(tempFilePath);

	// Create a new empty output destination object.
	auto * output = ((DOMImplementationLS*)implementation)->createLSOutput();

	// Set the stream to our target.
	output->setByteStream(formatTarget);

	// Write the serialized output to the destination.
	serializer->write(pmyDOMDocument, output);

	// Cleanup.
	serializer->release();

	R(xml_endl);
	delete formatTarget;
	output->release();
	pmyDOMDocument->release();
}


XercesSWIDTagIO::XercesSWIDTagIO() {
	try {
		XMLPlatformUtils::Initialize();
	}
	catch (const XMLException & toCatch) {
		ostringstream msg;
		auto message = XMLString::transcode(toCatch.getMessage());
		msg << "Error during initialization: ";
		msg << message << "\n";
		XMLString::release(&message);
		throw(msg.str());
	}
	parser = nullptr;
	doc = nullptr;

	swid_ns = X(SWID_NS);
	xmlch_entity = X("Entity");
	xmlch_link = X("Link");

	element_strings[SWID_ELEMENT_ENTITY] = xmlch_entity;
	element_strings[SWID_ELEMENT_LINK] = xmlch_link;
}


XercesSWIDTagIO::~XercesSWIDTagIO() {
	if (parser != nullptr) {
		deleteParser();
	}

	R(xmlch_link);
	R(xmlch_entity);
	R(swid_ns);

	XMLPlatformUtils::Terminate();
}


void XercesSWIDTagIO::deleteParser() {
	delete parser;
	parser = nullptr;
	delete errHandler;
	errHandler = nullptr;
}


void XercesSWIDTagIO::createParser() {
	parser = new XercesDOMParser();

	parser->setValidationScheme(XercesDOMParser::Val_Always);
	parser->setDoNamespaces(true);  // optional

	errHandler = (ErrorHandler*) new HandlerBase();
	parser->setErrorHandler(errHandler);
}


std::map<int, std::vector<DOMElement *> > XercesSWIDTagIO::subElementsOf(DOMElement * el) const {
	auto result = createEmptyMap();
	for (auto * sub_el = el->getFirstElementChild(); sub_el != nullptr; sub_el = sub_el->getNextElementSibling()) {
		if (XMLString::compareIString(sub_el->getTagName(), xmlch_entity) == 0) {
			result[SWID_ELEMENT_ENTITY].push_back(sub_el);
		} else if (XMLString::compareIString(sub_el->getTagName(), xmlch_link) == 0) {
			result[SWID_ELEMENT_LINK].push_back(sub_el);
		}
	}
	return result;
}


SWIDStruct XercesSWIDTagIO::load(const string & filename) {
	if (parser != nullptr) {
		deleteParser();
	}
	createParser();

	SWIDStruct result;
	try {
		result = XMLIO<DOMElement>::load(filename);
	} catch (const XMLException & toCatch) {
		deleteParser();
		auto message = XMLString::transcode(toCatch.getMessage());
		throw create_read_error(filename, message);
	} catch (...) {
		deleteParser();
		throw create_read_error(filename, "Unknown error.");
	}

	deleteParser();

	return result;
}


void XercesSWIDTagIO::save(const string & filename, const SWIDStruct & what) {
	try {
		XMLIO::save(filename, what);
	} catch (const XMLException & toCatch) {
		auto message = XMLString::transcode(toCatch.getMessage());
		throw create_save_error(filename, message);
	} catch (...) {
		throw create_save_error(filename, "Unknown error.");
	}
}


validity XercesSWIDTagIO::isXSDValid(const std::string & filename) {
	if (parser != nullptr) {
		deleteParser();
	}
	createParser();

	parser->setValidationSchemaFullChecking(true);
	parser->setExternalNoNamespaceSchemaLocation(SCHEMA_ABS_PATH);

	parser->parse(filename.c_str());
	if(parser->getErrorCount() != 0) {
		return SWID_VALIDITY_INVALID;
	}
	return SWID_VALIDITY_VALID;
}


DOMElement * XercesSWIDTagIO::readRoot(const string & filename) {
	parser->parse(filename.c_str());
	doc = parser->getDocument();

	return doc->getDocumentElement();
}


DOMElement * XercesSWIDTagIO::createRoot() {
	auto * xml_core = X("Core");
	auto * implementation = DOMImplementationRegistry::getDOMImplementation(xml_core);

	auto * xml_swid = X("SoftwareIdentity");
	doc = implementation->createDocument(swid_ns, xml_swid, 0);
	R(xml_swid);
	R(xml_core);
	return doc->getDocumentElement();
}


void XercesSWIDTagIO::setAttrValue(DOMElement * el, const char * name, const string & value) {
	auto * xml_name = X(name);
	auto * xml_val = X(value.c_str());
	el->setAttribute(xml_name, xml_val);
	R(xml_val);
	R(xml_name);
}


string XercesSWIDTagIO::extractAttrValue(DOMElement * el, const char * name) const {
	auto * xml_name = X(name);
	auto * xml_val = el->getAttribute(xml_name);
	const char * text_val = Y(xml_val);
	string ret(text_val);
	delete text_val;
	R(xml_name);
	return ret;
}


DOMElement * XercesSWIDTagIO::createSubElement(DOMElement * pRoot, int element_type) {
	auto * el = doc->createElementNS(swid_ns, element_strings[element_type]);
	pRoot->appendChild(el);
	return el;
}


void XercesSWIDTagIO::saveToFile(const std::string & filename) {
	OutputXML(doc, filename);
}
