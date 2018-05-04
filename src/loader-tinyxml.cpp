#include "loader-tinyxml.h"
#include "Translator.h"


using std::string;


TiXMLSWIDTagIO::TiXMLSWIDTagIO():SWIDTagIO() {
}


TiXMLSWIDTagIO::~TiXMLSWIDTagIO() {
}


/*
void add_entity(TiXMLNode * entity_node, std::vector<SWIDEntity>) {
}
*/


SWIDStruct TiXMLSWIDTagIO::load(const string & filename) {
	TiXmlDocument doc;

	auto result = doc.LoadFile(filename.c_str());

	if (! result) {
		throw XMLReadError(doc.ErrorDesc());
	}

	auto ret = SWIDStruct();
	auto * pRoot = doc.RootElement();

	ret.name = pRoot->Attribute("name");
	ret.tagId = pRoot->Attribute("tagId");
	ret.version = pRoot->Attribute("version");
	ret.xml_lang = pRoot->Attribute("xml:lang");

	ret.type = determine_type_id(
		pRoot->Attribute("corpus"), pRoot->Attribute("patch"), pRoot->Attribute("supplemental"));

	auto entity = SWIDEntity();
	for (auto it = pRoot->FirstChildElement("Entity"); it != NULL; it = it->NextSiblingElement("Entity")) {
		entity.name = it->Attribute("name");
		entity.regid = it->Attribute("regid");
		const char * role = it->Attribute("role");
		entity.role = Role(role).RoleAsId();
		ret.entities.push_back(entity);
	}

	return ret;
}


void TiXMLSWIDTagIO::save(const string & filename, const SWIDStruct & what) {
	TiXmlDocument doc;
	auto * decl = new TiXmlDeclaration( "1.0", "", "" );
	doc.LinkEndChild( decl );

	auto * pRoot = new TiXmlElement("SoftwareIdentity");

	pRoot->SetAttribute("name", what.name.c_str());
	pRoot->SetAttribute("tagId", what.tagId.c_str());
	pRoot->SetAttribute("version", what.version.c_str());
	pRoot->SetAttribute("xml:lang", what.xml_lang.c_str());

	string corpus, patch, supplemental;
	set_strings_to_match_type(what.type, corpus, patch, supplemental);
	pRoot->SetAttribute("corpus", corpus);
	pRoot->SetAttribute("patch", patch);
	pRoot->SetAttribute("supplemental", supplemental);

	for (auto it = what.entities.begin(); it != what.entities.end(); it++) {
		auto * entity_el = new TiXmlElement("Entity");
		entity_el->SetAttribute("name", it->name.c_str());
		entity_el->SetAttribute("regid", it->regid.c_str());
		entity_el->SetAttribute("role", Role(it->role).RoleAsString());
		pRoot->LinkEndChild(entity_el);
	}

	doc.LinkEndChild(pRoot);

	auto result = doc.SaveFile(filename.c_str());
	if (! result) {
		throw XMLReadError(doc.ErrorDesc());
	}
}
