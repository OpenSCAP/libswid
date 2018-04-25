#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/sax/HandlerBase.hpp>

// Other include files, declarations, and non-Xerces-C++ initializations.

#include "loader-xerces.h"


XercesSWIDTagLoader::XercesSWIDTagLoader():SWIDTagLoader() {
	try {
		XMLPlatformUtils::Initialize();
	}
	catch (const XMLException& toCatch) {
		return;
	}
	parser = NULL;
}


XercesSWIDTagLoader::~XercesSWIDTagLoader() {
	if (parser != NULL)
		delete parser;
	parser = NULL;
	XMLPlatformUtils::Terminate();
}


void XercesSWIDTagLoader::load(const string & filename) {
	if (parser == NULL)
		parser = new XercesDOMParser();

	parser->setValidationScheme(XercesDOMParser::Val_Always);
	parser->setDoNamespaces(true);  // optional

	ErrorHandler* errHandler = (ErrorHandler*) new HandlerBase();
	parser->setErrorHandler(errHandler);

	parser->parse(filename.c_str());
}

