#include <sstream>

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


using namespace xercesc;
using std::ostringstream;
using std::string;


// TODO: Xerces leaks memory, because transcoded strings have to be released and aren't.
// See also: https://stackoverflow.com/questions/9826518/purpose-of-xmlstringtranscode
// and https://alfps.wordpress.com/2010/05/27/cppx-xerces-strings-simplified-by-ownership-part-i/#ownership


// Taken from: https://stackoverflow.com/a/2897419/592892
class XStr
{
public :
	XStr(const char * const toTranscode)
	{
		// Call the private transcoding method
		fUnicodeForm = XMLString::transcode(toTranscode);
	}

	~XStr()
	{
		XMLString::release(&fUnicodeForm);
	}

	const XMLCh * unicodeForm() const
	{
		return fUnicodeForm;
	}

private :
	XMLCh * fUnicodeForm;
};


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
}


XercesSWIDTagIO::XercesSWIDTagIO():SWIDTagIO() {
	try {
		XMLPlatformUtils::Initialize();
	}
	catch (const XMLException& toCatch) {
		ostringstream msg;
		auto message = XMLString::transcode(toCatch.getMessage());
		msg << "Error during initialization: ";
		msg << message << "\n";
		XMLString::release(&message);
		throw(msg.str());
	}
	parser = NULL;

	swid_ns = X("http://standards.iso.org/iso/19770/-2/2015/schema.xsd");
	xmlch_entity = X("Entity");
	xmlch_link = X("Link");
}


XercesSWIDTagIO::~XercesSWIDTagIO() {
	if (parser != NULL) {
		deleteParser();
	}

	R(xmlch_link);
	R(xmlch_entity);
	R(swid_ns);

	XMLPlatformUtils::Terminate();
}


SWIDStruct XercesSWIDTagIO::load(const string & filename) {
	if (parser != NULL) {
		deleteParser();
	}
	createParser();

	parser->parse(filename.c_str());

	auto * doc = parser->getDocument();
	auto * pRoot = doc->getDocumentElement();

	auto ret = SWIDStruct();

	ret.name = extractAttrValue(pRoot, "name");
	ret.tagId = extractAttrValue(pRoot, "tagId");
	ret.version = extractAttrValue(pRoot, "version");
	ret.versionScheme = extractAttrValue(pRoot, "versionScheme");
	ret.xml_lang = extractAttrValue(pRoot, "xml:lang");

	ret.type = determine_type_id(
		extractAttrValue(pRoot, "corpus").c_str(),
		extractAttrValue(pRoot, "patch").c_str(),
		extractAttrValue(pRoot, "supplemental").c_str());

	auto entity = SWIDEntity();
	string role;
	for (auto it = pRoot->getFirstElementChild(); it != NULL; it = it->getNextElementSibling()) {
		if (XMLString::compareIString(it->getTagName(), xmlch_entity)) {
			continue;
		}
		entity.name = extractAttrValue(it, "name");
		entity.regid = extractAttrValue(it, "regid");
		role = extractAttrValue(it, "role");
		entity.role = Role(role).RoleAsId();
		ret.entities.push_back(entity);
	}

	auto link = SWIDLink();
	for (auto it = pRoot->getFirstElementChild(); it != NULL; it = it->getNextElementSibling()) {
		if (XMLString::compareIString(it->getTagName(), xmlch_link)) {
			continue;
		}
		link.href = extractAttrValue(it, "href");
		link.rel = extractAttrValue(it, "rel");
		ret.links.push_back(link);
	}

	return ret;
}


void XercesSWIDTagIO::deleteParser() {
	delete parser;
	parser = NULL;
	delete errHandler;
	errHandler = NULL;
}


void XercesSWIDTagIO::createParser() {
	parser = new XercesDOMParser();

	parser->setValidationScheme(XercesDOMParser::Val_Always);
	parser->setDoNamespaces(true);  // optional

	errHandler = (ErrorHandler*) new HandlerBase();
	parser->setErrorHandler(errHandler);
}


void XercesSWIDTagIO::save(const string & filename, const SWIDStruct & what) {
	auto * xml_core = X("Core");
	auto * implementation = DOMImplementationRegistry::getDOMImplementation(xml_core);

	auto * xml_swid = X("SoftwareIdentity");
	auto * doc = implementation->createDocument(swid_ns, xml_swid, 0);
	auto * pRoot = doc->getDocumentElement();

	setAttrValue(pRoot, "name", what.name);
	setAttrValue(pRoot, "tagId", what.tagId);
	setAttrValue(pRoot, "version", what.version);
	setAttrValue(pRoot, "versionScheme", what.versionScheme);
	setAttrValue(pRoot, "xml:lang", what.xml_lang);

	string corpus, patch, supplemental;
	set_strings_to_match_type(what.type, corpus, patch, supplemental);
	setAttrValue(pRoot, "corpus", corpus);
	setAttrValue(pRoot, "patch", patch);
	setAttrValue(pRoot, "supplemental", supplemental);

	for (auto it = what.entities.begin(); it != what.entities.end(); it++) {
		auto * entity_el = doc->createElementNS(swid_ns, xmlch_entity);
		setAttrValue(entity_el, "name", it->name);
		setAttrValue(entity_el, "regid", it->regid);
		setAttrValue(entity_el, "role", Role(it->role).RoleAsString());
		pRoot->appendChild(entity_el);
	}

	for (auto it = what.links.begin(); it != what.links.end(); it++) {
		auto * el = doc->createElementNS(swid_ns, xmlch_link);
		setAttrValue(el, "href", it->href);
		setAttrValue(el, "rel", it->rel);
		pRoot->appendChild(el);
	}

	OutputXML(doc, filename);
	doc->release();
	R(xml_swid);
	R(xml_core);

	if (false) {
		// throw XMLReadError(doc.ErrorDesc());
	}
}


void XercesSWIDTagIO::setAttrValue(DOMElement * el, const char * name, const string & value) {
	auto * xml_name = X(name);
	auto * xml_val = X(value.c_str());
	el->setAttribute(xml_name, xml_val);
	R(xml_val);
	R(xml_name);
}


string XercesSWIDTagIO::extractAttrValue(DOMElement * el, const char * name) {
	auto * xml_name = X(name);
	auto * xml_val = el->getAttribute(xml_name);
	const char * text_val = Y(xml_val);
	string ret(text_val);
	delete text_val;
	R(xml_name);
	return ret;
}
