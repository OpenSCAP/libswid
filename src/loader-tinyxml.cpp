#include "loader-tinyxml.h"
#include "Translator.h"

#include "loader-generic.cpp"

using std::string;


TiXMLSWIDTagIO::TiXMLSWIDTagIO() {
	element_strings[SWID_ELEMENT_ENTITY] = "Entity";
	element_strings[SWID_ELEMENT_LINK] = "Link";
}


void TiXMLSWIDTagIO::setAttrValue(TiXmlElement * el, const char * name, const string & value) {
	el->SetAttribute(name, value);
}


string TiXMLSWIDTagIO::extractAttrValue(TiXmlElement * el, const char * name) const {
	return el->Attribute(name);
}


std::map<int, std::vector<TiXmlElement *> > TiXMLSWIDTagIO::subElementsOf(TiXmlElement * el) const {
	auto result = createEmptyMap();
	for (auto it = el->FirstChildElement(); it != NULL; it = it->NextSiblingElement()) {
		for (int i = 0; i < TOTAL_SWID_ELEMENT_COUNT; i++) {
			if (strcmp(it->Value(), element_strings[i]) == 0) {
				result[i].push_back(it);
				continue;
			}
		}
	}
	return result;
}


TiXmlElement * TiXMLSWIDTagIO::createRoot() {
	doc = TiXmlDocument();;

	auto * decl = new TiXmlDeclaration( "1.0", "", "" );
	doc.LinkEndChild( decl );
	auto * pRoot = new TiXmlElement("SoftwareIdentity");
	setAttrValue(pRoot, "xmlns", SWID_NS);
	doc.LinkEndChild( pRoot );
	return pRoot;
}


TiXmlElement * TiXMLSWIDTagIO::readRoot(const std::string & filename) {
	doc = TiXmlDocument();;

	auto result = doc.LoadFile(filename.c_str());
	if (! result) {
		throw create_read_error(filename, doc.ErrorDesc());
	}
	return doc.RootElement();
}


TiXmlElement * TiXMLSWIDTagIO::createSubElement(TiXmlElement * parent, int element_type) {
	auto * entity_el = new TiXmlElement(element_strings[element_type]);
	parent->LinkEndChild(entity_el);
	return entity_el;
}


void TiXMLSWIDTagIO::saveToFile(const std::string & filename) {
	auto result = doc.SaveFile(filename.c_str());
	if (! result) {
		throw create_save_error(filename, doc.ErrorDesc());
	}
}


/*
SWIDStruct TiXMLSWIDTagIO::load(const string & filename) {
	TiXmlDocument doc;



	auto ret = SWIDStruct();
	auto * pRoot = doc.RootElement();

	ret.name = pRoot->Attribute("name");
	ret.tagId = pRoot->Attribute("tagId");
	ret.version = pRoot->Attribute("version");
	ret.versionScheme = pRoot->Attribute("versionScheme");
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
	auto link = SWIDLink();
	for (auto it = pRoot->FirstChildElement("Link"); it != NULL; it = it->NextSiblingElement("Link")) {
		link.href = it->Attribute("href");
		link.rel = it->Attribute("rel");
		ret.links.push_back(link);
	}

	return ret;
}


void TiXMLSWIDTagIO::save(const string & filename, const SWIDStruct & what) {
	TiXmlDocument doc;

	auto * pRoot = new TiXmlElement("SoftwareIdentity");

	pRoot->SetAttribute("name", what.name.c_str());
	pRoot->SetAttribute("tagId", what.tagId.c_str());
	pRoot->SetAttribute("version", what.version.c_str());
	pRoot->SetAttribute("versionScheme", what.versionScheme.c_str());
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

	for (auto it = what.links.begin(); it != what.links.end(); it++) {
		auto * link_el = new TiXmlElement("Link");
		link_el->SetAttribute("href", it->href.c_str());
		link_el->SetAttribute("rel", it->rel.c_str());
		pRoot->LinkEndChild(link_el);
	}

	doc.LinkEndChild(pRoot);

	auto * decl = new TiXmlDeclaration( "1.0", "", "" );
	doc.LinkFirstChild( decl );
	auto result = doc.SaveFile(filename.c_str());
	if (! result) {
		throw XMLReadError(doc.ErrorDesc());
	}
}
*/
