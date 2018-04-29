#include <stdexcept>

#include "loader-tinyxml.h"
#include "Translator.h"


using std::runtime_error;


TiXML2SWIDTagLoader::TiXML2SWIDTagLoader():SWIDTagLoader() {
}


TiXML2SWIDTagLoader::~TiXML2SWIDTagLoader() {
}


SWIDStruct TiXML2SWIDTagLoader::load(const string & filename) {
	TiXmlDocument doc;

	auto result = doc.LoadFile(filename.c_str());

	if (! result) {
		throw std::runtime_error(doc.ErrorDesc());
	}

	auto ret = SWIDStruct();
	auto pRoot = doc.RootElement();

	ret.name = pRoot->Attribute("name");
	ret.tagId = pRoot->Attribute("tagId");

	ret.type = SWID_TYPE_PRIMARY;
	if (check_bool(pRoot->Attribute("corpus"))) {
		ret.type = SWID_TYPE_CORPUS;
	} else if (check_bool(pRoot->Attribute("patch"))) {
		ret.type = SWID_TYPE_PATCH;
	} else if (check_bool(pRoot->Attribute("supplemental"))) {
		ret.type = SWID_TYPE_SUPPLEMENTAL;
	}

	auto entity = SWIDEntity();
	for (auto it = pRoot->FirstChildElement("Entity"); it != NULL; it = it->NextSiblingElement("Entity")) {
		entity.name = it->Attribute("name");
		const char * role = it->Attribute("role");
		entity.role = Role(role).RoleAsId();
		ret.entities.push_back(entity);
	}

	return ret;
}


void TiXML2SWIDTagLoader::save(const string & filename, const SWIDStruct & what) {
	TiXmlDocument doc;
	auto decl = new TiXmlDeclaration( "1.0", "", "" );
	doc.LinkEndChild( decl );

	auto pRoot = new TiXmlElement("SoftwareIdentity");

	pRoot->SetAttribute("name", what.name.c_str());
	pRoot->SetAttribute("tagId", what.tagId.c_str());

	pRoot->SetAttribute("corpus", bool_to_string(what.type == SWID_TYPE_CORPUS));
	pRoot->SetAttribute("patch", bool_to_string(what.type == SWID_TYPE_PATCH));
	pRoot->SetAttribute("supplemental", bool_to_string(what.type == SWID_TYPE_SUPPLEMENTAL));

	for (auto it = what.entities.begin(); it != what.entities.end(); it++) {
		auto entity_el = new TiXmlElement("Entity");
		entity_el->SetAttribute("name", it->name.c_str());
		entity_el->SetAttribute("role", Role(it->role).RoleAsString());
		pRoot->LinkEndChild(entity_el);
	}

	doc.LinkEndChild(pRoot);

	auto result = doc.SaveFile(filename.c_str());
	if (! result) {
		throw std::runtime_error(doc.ErrorDesc());
	}
}


const char * TiXML2SWIDTagLoader::bool_to_string(bool value) const {
	if (value) {
		return "true";
	}
	return "false";
}


bool TiXML2SWIDTagLoader::check_bool(const char * value) const {
	if (strcmp("true", value) == 0) {
		return true;
	}
	return false;
}
