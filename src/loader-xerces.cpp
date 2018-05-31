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
static XMLCh * create_str(const char * str) {
	return XMLString::transcode(str);
}


static char * create_str(const XMLCh * str) {
	return XMLString::transcode(str);
}


static void destroy_str(XMLCh * str) {
	XMLString::release(& str);
}


static void destroy_str(char * str) {
	XMLString::release(& str);
}


// Taken from https://stackoverflow.com/a/2897419/592892
static void OutputXML(DOMDocument * pmyDOMDocument, string filePath)
{
	auto * ls = create_str("LS");
	//Return the first registered implementation that has the desired features. In this case, we are after a DOM implementation that has the LS feature... or Load/Save.
	auto * implementation = DOMImplementationRegistry::getDOMImplementation(ls);
	destroy_str(ls);

	// Create a DOMLSSerializer which is used to serialize a DOM tree into an XML document.
	auto * serializer = ((DOMImplementationLS*)implementation)->createLSSerializer();

	// Make the output more human readable by inserting line feeds.
	if (serializer->getDomConfig()->canSetParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true))
		serializer->getDomConfig()->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true);

	auto * xml_endl = create_str("\n");
	// The end-of-line sequence of characters to be used in the XML being written out.
	serializer->setNewLine(xml_endl);

	destroy_str(xml_endl);

	// Convert the path into Xerces compatible XMLCh*.
	auto * tempFilePath = create_str(filePath.c_str());

	// Specify the target for the XML output.
	LocalFileFormatTarget * formatTarget;

	try {
		// Can throw e.g. when the filename is invalid
		formatTarget = new LocalFileFormatTarget(tempFilePath);
	} catch (...) {
		// Cleanup.
		serializer->release();
		destroy_str(tempFilePath);
		throw;
	}

	// Create a new empty output destination object.
	auto * output = ((DOMImplementationLS*)implementation)->createLSOutput();

	// Set the stream to our target.
	output->setByteStream(formatTarget);

	// Write the serialized output to the destination.
	serializer->write(pmyDOMDocument, output);

	// Cleanup.
	destroy_str(tempFilePath);

	serializer->release();
	output->release();
	delete formatTarget;
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
	shared_parser = nullptr;
	shared_error_handler = nullptr;
	shared_document = nullptr;

	swid_ns = create_str(SWID_NS);
	xmlch_entity = create_str("Entity");
	xmlch_link = create_str("Link");

	element_strings[SWID_ELEMENT_ENTITY] = xmlch_entity;
	element_strings[SWID_ELEMENT_LINK] = xmlch_link;
}


XercesSWIDTagIO::~XercesSWIDTagIO() {
	destroy_str(xmlch_link);
	destroy_str(xmlch_entity);
	destroy_str(swid_ns);

	XMLPlatformUtils::Terminate();
}


void XercesSWIDTagIO::deleteParser() {
	delete shared_parser;
	shared_parser = nullptr;
	delete shared_error_handler;
	shared_error_handler = nullptr;
}


void XercesSWIDTagIO::createParser() {
	shared_parser = new XercesDOMParser();

	shared_parser->setValidationScheme(XercesDOMParser::Val_Always);
	shared_parser->setDoNamespaces(true);  // optional

	shared_error_handler = (ErrorHandler*) new HandlerBase();
	shared_parser->setErrorHandler(shared_error_handler);
}


void XercesSWIDTagIO::createDocument() {
	auto * xml_core = create_str("Core");
	auto * implementation = DOMImplementationRegistry::getDOMImplementation(xml_core);

	auto * xml_swid = create_str("SoftwareIdentity");
	shared_document = implementation->createDocument(swid_ns, xml_swid, 0);
	destroy_str(xml_swid);
	destroy_str(xml_core);
}


void XercesSWIDTagIO::deleteDocument() {
	delete shared_document;
	shared_document = nullptr;
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
	createDocument();
	try {
		XMLIO::save(filename, what);
	} catch (const XMLException & toCatch) {
		deleteDocument();
		auto message = create_str(toCatch.getMessage());
		auto exception = create_save_error(filename, message);
		destroy_str(message);
		throw exception;
	} catch (...) {
		deleteDocument();
		throw create_save_error(filename, "Unknown error.");
	}
	deleteDocument();
}


validity XercesSWIDTagIO::isXSDValid(const std::string & filename) {
	validity result = SWID_VALIDITY_VALID;
	createParser();

	shared_parser->setValidationSchemaFullChecking(true);
	shared_parser->setExternalNoNamespaceSchemaLocation(SCHEMA_ABS_PATH);

	shared_parser->parse(filename.c_str());
	if(shared_parser->getErrorCount() != 0) {
		result = SWID_VALIDITY_INVALID;
	}
	deleteParser();
	return result;
}


DOMElement * XercesSWIDTagIO::readRoot(const string & filename) {
	shared_parser->parse(filename.c_str());
	shared_document = shared_parser->getDocument();

	return shared_document->getDocumentElement();
}


DOMElement * XercesSWIDTagIO::createRoot() {
	return shared_document->getDocumentElement();
}


void XercesSWIDTagIO::setAttrValue(DOMElement * el, const char * name, const string & value) {
	auto * xml_name = create_str(name);
	auto * xml_val = create_str(value.c_str());
	el->setAttribute(xml_name, xml_val);
	destroy_str(xml_val);
	destroy_str(xml_name);
}


string XercesSWIDTagIO::extractAttrValue(DOMElement * el, const char * name) const {
	auto * xml_name = create_str(name);
	auto * xml_val = el->getAttribute(xml_name);
	const char * text_val = create_str(xml_val);
	string ret(text_val);
	delete text_val;
	destroy_str(xml_name);
	return ret;
}


DOMElement * XercesSWIDTagIO::createSubElement(DOMElement * pRoot, int element_type) {
	auto * el = shared_document->createElementNS(swid_ns, element_strings[element_type]);
	pRoot->appendChild(el);
	return el;
}


void XercesSWIDTagIO::saveToFile(const std::string & filename) {
	OutputXML(shared_document, filename);
}
