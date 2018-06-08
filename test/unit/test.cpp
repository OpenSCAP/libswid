#include <regex>

#include "catch.hpp"

#include "SWIDStruct.h"
#include "Translator.h"
#include "libswid-base.h"

using std::string;
using std::regex;
using std::regex_match;


TEST_CASE( "Utilities" ) {
	SECTION("Role parsing") {
		auto role = Role();
		REQUIRE( role.roleAsString() == "" );

		role = Role(SWID_ROLE_AGGREGATOR | SWID_ROLE_LICENSOR);
		REQUIRE( role.roleAsString() == "aggregator licensor" );

		role = Role(SWID_ROLE_DISTRIBUTOR | SWID_ROLE_SOFTWARE_CREATOR | SWID_ROLE_TAG_CREATOR);
		REQUIRE( role.roleAsString() == "distributor softwareCreator tagCreator" );

		role = Role("  distributor ");
		REQUIRE( (role.roleAsId() & SWID_ROLE_DISTRIBUTOR) );

		role = Role("  distributor_");
		REQUIRE( role.roleAsId() == SWID_ROLE_NONE );

		role = Role("licensor softwarecreator aggregator tagCreator");
		REQUIRE( (role.roleAsId() & SWID_ROLE_SOFTWARE_CREATOR) == SWID_ROLE_NONE );

		auto real_role = SWID_ROLE_AGGREGATOR | SWID_ROLE_LICENSOR | SWID_ROLE_TAG_CREATOR;
		REQUIRE( (role.roleAsId() & real_role) == real_role);
		REQUIRE( (role.roleAsId() & (~real_role)) == 0);

		real_role = SWID_ROLE_TAG_CREATOR | SWID_ROLE_SOFTWARE_CREATOR;
		REQUIRE( (Role("softwareCreator tagCreator").roleAsId() & (~real_role)) == 0 );
		REQUIRE( (Role("softwareCreator tagCreator").roleAsId() & real_role) == real_role );
	}

	SECTION("Backends") {
		SWIDTagIO loader;
		CHECK_THROWS( loader.setBackend("trololol") );
		try {
			loader.setBackend("trololol");
			REQUIRE( false );
		} catch (const XMLIOBackendError & toCatch) {
			REQUIRE(regex_search(toCatch.what(),
				regex("trololol.*not known\\.")));
		} catch (...) {
			REQUIRE( false );
		}

		CHECK_THROWS( loader.load("") );
		try {
			loader.load("");
			REQUIRE( false );
		} catch (const XMLIOBackendError & toCatch) {
			REQUIRE(regex_search(toCatch.what(),
				regex("[Nn]o backend has been set")));
		} catch (...) {
			REQUIRE( false );
		}
	}
}


void check(string parser_name) {
	SWIDTagIO loader;
	loader.setBackend(parser_name.c_str());
	SWIDStruct swid;
	SECTION(parser_name + ": Sanity") {
		CHECK_THROWS( loader.load("") );
		try {
			loader.load("");
		} catch (const XMLIOError & toCatch) {
			regex good_msg_start("Could not load from '':.*");
			REQUIRE(regex_match(toCatch.what(), good_msg_start));
		}
		CHECK_THROWS( loader.save("/", swid) );
		try {
			loader.save("/", swid);
		} catch (const XMLIOError & toCatch) {
			regex good_msg_start("Could not save to '/':.*");
			REQUIRE(regex_match(toCatch.what(), good_msg_start));
		}
	}

	swid.name = "ACME RoadRunner Management Suite";
	swid.tagId = "com.acme.rms-ce-v4-1-5-0";
	swid.xml_lang = "en-us";
	string fname = parser_name;
	fname += ".xml";

	SWIDEntity entity;
	entity.name = "The ACME corporation";
	entity.regid = "acme.com";
	entity.role = Role(SWID_ROLE_SOFTWARE_CREATOR | SWID_ROLE_TAG_CREATOR).roleAsId();

	swid.entities.push_back(entity);

	SECTION(parser_name + ": Basic metadata") {
		swid.type = SWID_TYPE_CORPUS;

		loader.save(fname, swid);

		REQUIRE( loader.isXSDValid(fname) != SWID_VALIDITY_VALID );

		auto loaded_swid = loader.load(fname);
		REQUIRE( loaded_swid.name == swid.name );
		REQUIRE( loaded_swid.tagId == swid.tagId );
		REQUIRE( loaded_swid.type == swid.type );
		REQUIRE( loaded_swid.xml_lang == swid.xml_lang );
		REQUIRE( swid.version == loaded_swid.version );
		REQUIRE( loaded_swid.version == string("") );
		REQUIRE( loaded_swid.versionScheme == string("") );

		loaded_swid.applyDefaults();
		REQUIRE( loaded_swid.version == string("0.0") );
		REQUIRE( loaded_swid.versionScheme == string("multipartnumeric") );

		REQUIRE( loaded_swid.entities.size() == 1 );
		REQUIRE( loaded_swid.entities[0] == swid.entities[0] );


		SWIDEntity entity2;
		entity2.name = "Red Hat";
		entity2.role = Role("distributor").roleAsId();
		swid.entities.push_back(entity2);

		swid.version = "1.2.3";
		swid.versionScheme = "semver";
		swid.type = SWID_TYPE_PATCH;

		SWIDLink link;
		link.href = "swid.com.acme.rms-ce-v4-1-5-0";
		link.rel = "patches";
		swid.links.push_back(link);

		loader.save(fname, swid);

		loaded_swid = loader.load(fname);

		REQUIRE( loaded_swid.links.size() == 1 );
		REQUIRE( loaded_swid.links[0] == swid.links[0] );

		REQUIRE( loaded_swid.entities.size() == 2 );
		REQUIRE( loaded_swid.entities[0] == swid.entities[0] );
		REQUIRE( loaded_swid.entities[1] == swid.entities[1] );
		REQUIRE( loaded_swid.version == swid.version);
		REQUIRE( loaded_swid.versionScheme == swid.versionScheme );
	}

	SECTION(parser_name + ": Tag types") {
		swid.type = SWID_TYPE_PRIMARY;

		loader.save(fname, swid);

		auto loaded_swid = loader.load(fname);
		REQUIRE( loaded_swid.type == swid.type );

		swid.type = SWID_TYPE_SUPPLEMENTAL;
		loader.save(fname, swid);

		loaded_swid = loader.load(fname);
		REQUIRE( loaded_swid.type == swid.type );

		swid.type = SWID_TYPE_PATCH;
		loader.save(fname, swid);

		loaded_swid = loader.load(fname);
		REQUIRE( loaded_swid.type == swid.type );
	}
}


TEST_CASE( "Basic I/O", "[parsers]" ) {
#ifdef HAVE_TINYXML
	check("tinyxml");
#endif
#ifdef HAVE_XERCES
	check("xerces");
#endif
}

