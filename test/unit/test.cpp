#include "catch.hpp"

#include "SWIDStruct.h"
#include "loader-tinyxml.h"
#include "Translator.h"

using std::string;


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
	CHECK_THROWS( loader->load("") );
	SWIDStruct swid;

	swid.name = "ACME RoadRunner Management Suite";
	swid.tagId = "com.acme.rms-ce-v4-1-5-0";
	string fname = parser_name;
	fname += ".xml";

	SWIDEntity entity;
	entity.name = "The ACME corporation";
	entity.role = Role(SWID_ROLE_SOFTWARE_CREATOR | SWID_ROLE_TAG_CREATOR).RoleAsId();

	swid.entities.push_back(entity);

	SECTION(parser_name + ": Basic metadata") {
		swid.type = SWID_TYPE_CORPUS;

		loader->save(fname, swid);

		auto loaded_swid = loader->load(fname);
		REQUIRE( loaded_swid.name == swid.name );
		REQUIRE( loaded_swid.tagId == swid.tagId );
		REQUIRE( loaded_swid.type == swid.type );

		REQUIRE( loaded_swid.entities.size() == 1 );
		REQUIRE( loaded_swid.entities[0] == swid.entities[0] );


		SWIDEntity entity2;
		entity2.name = "Red Hat";
		entity2.role = Role("distributor").RoleAsId();
		swid.entities.push_back(entity2);

		loader->save(fname, swid);

		loaded_swid = loader->load(fname);

		REQUIRE( loaded_swid.entities.size() == 2 );
		REQUIRE( loaded_swid.entities[0] == swid.entities[0] );
		REQUIRE( loaded_swid.entities[1] == swid.entities[1] );
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

