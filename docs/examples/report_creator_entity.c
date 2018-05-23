#include <stdio.h>

#include <libswid.h>


int main(int argc, const char ** argv) {
	if (argc != 2) {
		printf("No input file has been specified.\nSpecify exactly one SWID tag filename.\n");
		return 1;
	}

	SWIDHandle swid = swid_create_root();

	const char * xml_backend = "xerces";
	const char * swid_fname = argv[1];
	SWIDIOHandle io = swid_create_io(xml_backend);

	if (io == NULL) {
		printf("Error getting XML backend '%s', try another one.\n", xml_backend);
		return 1;
	}
	int rc;
	rc = swid_load_root(io, argv[1], swid);
	swid_destroy_io(io);
	if (rc != 0) {
		printf("Error loading SWID '%s'\n", swid_fname);
		swid_destroy_root(swid);
		return 1;
	}


	char creator_not_found = 1;
	SWIDEntityHandle entity;
	for (int i = 0; (entity = swid_root_get_entity(swid, i)); i++) {
		if (swid_entity_get_role(entity) & SWID_ROLE_SOFTWARE_CREATOR) {
			creator_not_found = 0;
			printf("Creator of %s software: %s [%s]\n", swid_root_get_name(swid), swid_entity_get_name(entity), swid_entity_get_regid(entity));
			break;
		}
	}

	if (creator_not_found) {
		printf("Creator of %s software has not been found.\n", swid_root_get_name(swid));
	}
	swid_destroy_root(swid);
	return 0;
}

