#include <regex>

#include "catch.hpp"

#include "SWIDStruct.h"
#include "loader-tinyxml.h"
#include "Translator.h"

using std::string;
using std::regex;
using std::regex_match;


TEST_CASE( "Utilities" ) {
	SECTION("Role parsing") {
		auto role = Role();
		REQUIRE( role.RoleAsString() == "" );

		role = Role(SWID_ROLE_AGGREGATOR | SWID_ROLE_LICENSOR);
		REQUIRE( role.RoleAsString() == "aggregator licensor" );

		role = Role(SWID_ROLE_DISTRIBUTOR | SWID_ROLE_SOFTWARE_CREATOR | SWID_ROLE_TAG_CREATOR);
		REQUIRE( role.RoleAsString() == "distributor softwareCreator tagCreator" );

		role = Role("  distributor ");
		REQUIRE( (role.RoleAsId() & SWID_ROLE_DISTRIBUTOR) );

		role = Role("  distributor_");
		REQUIRE( role.RoleAsId() == SWID_ROLE_NONE );

		role = Role("licensor softwarecreator aggregator tagCreator");
		REQUIRE( (role.RoleAsId() & SWID_ROLE_SOFTWARE_CREATOR) == SWID_ROLE_NONE );

		auto real_role = SWID_ROLE_AGGREGATOR | SWID_ROLE_LICENSOR | SWID_ROLE_TAG_CREATOR;
		REQUIRE( (role.RoleAsId() & real_role) == real_role);
		REQUIRE( (role.RoleAsId() & (~real_role)) == 0);

		real_role = SWID_ROLE_TAG_CREATOR | SWID_ROLE_SOFTWARE_CREATOR;
		REQUIRE( (Role("softwareCreator tagCreator").RoleAsId() & (~real_role)) == 0 );
		REQUIRE( (Role("softwareCreator tagCreator").RoleAsId() & real_role) == real_role );
	}

}


void check(string parser_name) {
	auto * loader = get_a_swidtagio(parser_name.c_str());
	SECTION(parser_name + ": Sanity") {
		CHECK_THROWS( loader->load("") );
		try {
			loader->load("");
		} catch (const XMLReadError & toCatch) {
			regex good_msg_start("Error loading from '':.*");
			REQUIRE(regex_match(toCatch.what(), good_msg_start));
		}
	}
	SWIDStruct swid;

	swid.name = "ACME RoadRunner Management Suite";
	swid.tagId = "com.acme.rms-ce-v4-1-5-0";
	swid.xml_lang = "en-us";
	string fname = parser_name;
	fname += ".xml";

	SWIDEntity entity;
	entity.name = "The ACME corporation";
	entity.regid = "acme.com";
	entity.role = Role(SWID_ROLE_SOFTWARE_CREATOR | SWID_ROLE_TAG_CREATOR).RoleAsId();

	swid.entities.push_back(entity);

	SECTION(parser_name + ": Basic metadata") {
		swid.type = SWID_TYPE_CORPUS;

		loader->save(fname, swid);

		auto loaded_swid = loader->load(fname);
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
		entity2.role = Role("distributor").RoleAsId();
		swid.entities.push_back(entity2);

		swid.version = "1.2.3";
		swid.versionScheme = "semver";
		swid.type = SWID_TYPE_PATCH;

		SWIDLink link;
		link.href = "swid.com.acme.rms-ce-v4-1-5-0";
		link.rel = "patches";
		swid.links.push_back(link);

		loader->save(fname, swid);

		loaded_swid = loader->load(fname);

		REQUIRE( loaded_swid.links.size() == 1 );

		REQUIRE( loaded_swid.entities.size() == 2 );
		REQUIRE( loaded_swid.entities[0] == swid.entities[0] );
		REQUIRE( loaded_swid.entities[1] == swid.entities[1] );
		REQUIRE( loaded_swid.version == swid.version);
		REQUIRE( loaded_swid.versionScheme == swid.versionScheme );
	}

	SECTION(parser_name + ": Tag types") {
		swid.type = SWID_TYPE_PRIMARY;

		loader->save(fname, swid);

		auto loaded_swid = loader->load(fname);
		REQUIRE( loaded_swid.type == swid.type );

		swid.type = SWID_TYPE_SUPPLEMENTAL;
		loader->save(fname, swid);

		loaded_swid = loader->load(fname);
		REQUIRE( loaded_swid.type == swid.type );

		swid.type = SWID_TYPE_PATCH;
		loader->save(fname, swid);

		loaded_swid = loader->load(fname);
		REQUIRE( loaded_swid.type == swid.type );
	}
	delete loader;
}


TEST_CASE( "Basic I/O", "[parsers]" ) {
	check("tinyxml");
	check("xerces");
}

