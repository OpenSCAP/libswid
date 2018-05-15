#include <string.h>

#include "acutest.h"

#include "libswid.h"


const size_t MAX_LEN = 128;

/*
SWIDHandle swid_create_data();
int swid_load_data(SWIDIOHandle io_handle, SWIDHandle swid_structure);
int swid_save_data(SWIDIOHandle io_handle, SWIDHandle swid_structure);
*/



void create_io(void)
{
	SWIDIOHandle io = swid_create_io("xerces");
	TEST_CHECK(swid_destroy_io(io) == 0);
}


void create_xml(const char * backend)
{
	char output_fname[MAX_LEN];
	snprintf(output_fname, MAX_LEN, "%s-c.xml", backend);

	CSWIDStruct swid = swid_get_empty_data();
	CSWIDStruct swid_loaded;

	swid.name = malloc(MAX_LEN);
	strcpy(swid.name, "ACME RoadRunner Management Suite");

	swid.tagId = malloc(MAX_LEN);
	strcpy(swid.tagId, "com.acme.rms-ce-v4-1-5-0");

	swid.version = malloc(MAX_LEN);
	strcpy(swid.version, "1.2.3");

	swid.versionScheme = malloc(MAX_LEN);
	strcpy(swid.versionScheme, "semver");

	swid.xml_lang = malloc(MAX_LEN);
	strcpy(swid.xml_lang, "en-us");

	swid.type = SWID_TYPE_CORPUS;

	SWIDIOHandle io = swid_create_io(backend);
	swid_save_data(io, output_fname, & swid);
	swid_load_data(io, output_fname, & swid_loaded);
	swid_destroy_io(io);

	TEST_CHECK(strncmp(swid.name, swid_loaded.name, MAX_LEN) == 0);
	TEST_CHECK(strncmp(swid.tagId, swid_loaded.tagId, MAX_LEN) == 0);
	TEST_CHECK(strncmp(swid.version, swid_loaded.version, MAX_LEN) == 0);
	TEST_CHECK(strncmp(swid.versionScheme, swid_loaded.versionScheme, MAX_LEN) == 0);
	TEST_CHECK(strncmp(swid.xml_lang, swid_loaded.xml_lang, MAX_LEN) == 0);
	TEST_CHECK(swid.type == swid_loaded.type);

	swid_destroy_data(& swid_loaded);
	swid_destroy_data(& swid);
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
