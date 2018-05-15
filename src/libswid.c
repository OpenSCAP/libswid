#include <stdlib.h>
#include <string.h>

#include "libswid.h"


int swid_destroy_data(CSWIDStruct * data) {
	if (data->name) {
		free(data->name);
		data->name = 0;
	}
	if (data->tagId) {
		free(data->tagId);
		data->tagId = 0;
	}
	if (data->version) {
		free(data->version);
		data->version = 0;
	}
	if (data->versionScheme) {
		free(data->versionScheme);
		data->versionScheme = 0;
	}
	if (data->xml_lang) {
		free(data->xml_lang);
		data->xml_lang = 0;
	}
	return 0;
}


char * copy_string(const char * src, size_t full_str_len) {
	char * dest = (char *)malloc(full_str_len * sizeof(char));
	memcpy(dest, src, full_str_len);
	return dest;
}
