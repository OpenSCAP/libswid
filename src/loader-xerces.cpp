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
	//Return the first registered implementation that has the desired features. In this case, we are after a DOM implementation that has the LS feature... or Load/Save.
	auto * implementation = DOMImplementationRegistry::getDOMImplementation(XMLString::transcode("LS"));

	// Create a DOMLSSerializer which is used to serialize a DOM tree into an XML document.
	auto * serializer = ((DOMImplementationLS*)implementation)->createLSSerializer();

	// Make the output more human readable by inserting line feeds.
	if (serializer->getDomConfig()->canSetParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true))
		serializer->getDomConfig()->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true);

	// The end-of-line sequence of characters to be used in the XML being written out.
	serializer->setNewLine(XMLString::transcode("\n"));

	// Convert the path into Xerces compatible XMLCh*.
	auto * tempFilePath = X(filePath.c_str());

	// Specify the target for the XML output.
	auto * formatTarget = new LocalFileFormatTarget(tempFilePath);

	// Create a new empty output destination object.
	auto * output = ((DOMImplementationLS*)implementation)->createLSOutput();

	// Set the stream to our target.
	output->setByteStream(formatTarget);

	// Write the serialized output to the destination.
	serializer->write(pmyDOMDocument, output);

	// Cleanup.
	serializer->release();

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
}


XercesSWIDTagIO::~XercesSWIDTagIO() {
	if (parser != NULL) {
		deleteParser();
	}
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

	ret.name = Y(pRoot->getAttribute(X("name")));
	ret.tagId = Y(pRoot->getAttribute(X("tagId")));

	ret.type = determine_type_id(
		Y(pRoot->getAttribute(X("corpus"))),
		Y(pRoot->getAttribute(X("patch"))),
		Y(pRoot->getAttribute(X("supplemental"))));

	auto entity = SWIDEntity();
	for (auto it = pRoot->getFirstElementChild(); it != NULL; it = it->getNextElementSibling()) {
		entity.name = Y(it->getAttribute(X("name")));
		const char * role = Y(it->getAttribute(X("role")));
		entity.role = Role(role).RoleAsId();
		ret.entities.push_back(entity);
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
	auto * implementation = DOMImplementationRegistry::getDOMImplementation(X("Core"));

	auto * doc = implementation->createDocument(X("http://standards.iso.org/iso/19770/-2/2015/schema.xsd"), X("SoftwareIdentity"), 0);
	auto * pRoot = doc->getDocumentElement();

	pRoot->setAttribute(X("name"), X(what.name.c_str()));
	pRoot->setAttribute(X("tagId"), X(what.tagId.c_str()));

	string corpus, patch, supplemental;
	set_strings_to_match_type(what.type, corpus, patch, supplemental);
	pRoot->setAttribute(X("corpus"), X(corpus.c_str()));
	pRoot->setAttribute(X("patch"), X(patch.c_str()));
	pRoot->setAttribute(X("supplemental"), X(supplemental.c_str()));

	for (auto it = what.entities.begin(); it != what.entities.end(); it++) {
		auto * entity_el = doc->createElement(X("Entity"));
		entity_el->setAttribute(X("name"), X(it->name.c_str()));
		entity_el->setAttribute(X("role"), X(Role(it->role).RoleAsString().c_str()));
		pRoot->appendChild(entity_el);
	}

	OutputXML(doc, filename);
	doc->release();

	if (false) {
		// throw XMLReadError(doc.ErrorDesc());
	}
}
