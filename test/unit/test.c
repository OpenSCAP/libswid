#include <string.h>

#include "acutest.h"

#include "libswid.h"


const size_t MAX_LEN = 128;


const char * str_name = "ACME RoadRunner Management Suite";
const char * str_tagId = "com.acme.rms-ce-v4-1-5-0";
const char * str_version = "1.2.3";
const char * str_versionScheme = "semver";
const char * str_xml_lang = "en";

const char * entity_name = "The ACME corporation";
const char * entity_regid = "acme.com";
unsigned int entity_role = SWID_ROLE_SOFTWARE_CREATOR | SWID_ROLE_TAG_CREATOR;

void create_io(void)
{
	SWIDIOHandle io = swid_create_io("xerces");
	TEST_CHECK(swid_destroy_io(io) == 0);
}


void create_xml(const char * backend)
{
	char output_fname[MAX_LEN];
	snprintf(output_fname, MAX_LEN, "%s-c.xml", backend);

	SWIDHandle swid = swid_get_empty_data();
	SWIDHandle swid_loaded = swid_get_empty_data();

	SWIDEntityHandle entity = swid_get_empty_entity();

        swid_entity_set_name(entity, entity_name);
        swid_entity_set_regid(entity, entity_regid);
	swid_entity_set_role(entity, entity_role);

        swid_append_entity_data(swid, entity);
	swid_destroy_entity(entity);

	swid_set_name(swid, str_name);
	swid_set_tagId(swid, str_tagId);
	swid_set_version(swid, str_version);
	swid_set_versionScheme(swid, str_versionScheme);
	swid_set_xml_lang(swid, str_xml_lang);

	swid_set_type(swid, SWID_TYPE_CORPUS);

	SWIDIOHandle io = swid_create_io(backend);
	swid_save_data(io, output_fname, swid);
	swid_load_data(io, output_fname, swid_loaded);
	swid_destroy_io(io);

	TEST_CHECK(strcmp(str_name, swid_get_name(swid_loaded)) == 0);
	TEST_CHECK(strcmp(str_tagId, swid_get_tagId(swid_loaded)) == 0);
	TEST_CHECK(strcmp(str_version, swid_get_version(swid_loaded)) == 0);
	TEST_CHECK(strcmp(str_versionScheme, swid_get_versionScheme(swid_loaded)) == 0);
	TEST_CHECK(strcmp(str_xml_lang, swid_get_xml_lang(swid_loaded)) == 0);
	TEST_CHECK(SWID_TYPE_CORPUS == swid_get_type(swid_loaded));

        /* There is exactly one entity. */
        entity = swid_get_entity(swid, 0);
	TEST_CHECK(entity != NULL);
	TEST_CHECK(swid_get_entity(swid, 1) == NULL);

	TEST_CHECK(strcmp(swid_entity_get_name(entity), entity_name) == 0);
	TEST_CHECK(strcmp(swid_entity_get_regid(entity), entity_regid) == 0);
	TEST_CHECK(swid_entity_get_role(entity) == entity_role);

	swid_destroy_data(swid_loaded);
	swid_destroy_data(swid);
}


void create_tinyxml() {
	return create_xml("tinyxml");
}


void create_xerces() {
	return create_xml("xerces");
}


TEST_LIST = {
    { "create-xerces", create_xerces },
    { "create-tinyxml", create_tinyxml },
    { NULL, NULL }
};
