#include <sstream>

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/sax/HandlerBase.hpp>

// Other include files, declarations, and non-Xerces-C++ initializations.

#include "loader-xerces.h"


using std::ostringstream;


XercesSWIDTagLoader::XercesSWIDTagLoader():SWIDTagLoader() {
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


XercesSWIDTagLoader::~XercesSWIDTagLoader() {
	if (parser != NULL) {
		deleteParser();
	}
	XMLPlatformUtils::Terminate();
}


SWIDStruct XercesSWIDTagLoader::load(const string & filename) {
	if (parser != NULL) {
		deleteParser();
	}
	createParser();

	parser->parse(filename.c_str());

	return SWIDStruct();
}


void XercesSWIDTagLoader::deleteParser() {
	delete parser;
	parser = NULL;
}


void XercesSWIDTagLoader::createParser() {
	parser = new XercesDOMParser();

	parser->setValidationScheme(XercesDOMParser::Val_Always);
	parser->setDoNamespaces(true);  // optional

	ErrorHandler* errHandler = (ErrorHandler*) new HandlerBase();
	parser->setErrorHandler(errHandler);
}


void XercesSWIDTagLoader::save(const string & filename, const SWIDStruct & what) {
	if (parser != NULL) {
		deleteParser();
	}
	createParser();


}

