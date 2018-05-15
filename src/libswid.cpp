#include <cstring>

#include "lib.h"

#include "libswid.h"


static SWIDStruct C2CXX(const CSWIDStruct & src) {
	SWIDStruct ret;
	if (src.name != nullptr) {
		ret.name = src.name;
	}
	if (src.tagId != nullptr) {
		ret.tagId = src.tagId;
	}
	if (src.version != nullptr) {
		ret.version = src.version;
	}
	if (src.versionScheme != nullptr) {
		ret.versionScheme = src.versionScheme;
	}
	if (src.xml_lang != nullptr) {
		ret.xml_lang = src.xml_lang;
	}
	ret.type = (type_id)src.type;
	return ret;
}


static char * copy_string(const std::string & src) {
	const size_t str_len = src.size() + 1;
	char * dest = (char *)malloc(str_len * sizeof(char));
	memcpy(dest, src.c_str(), str_len);
	return dest;
}


extern "C" {

SWIDIOHandle swid_create_io(const char * backend) {
	return (SWIDIOHandle)get_swidtagio(backend);
}


int swid_destroy_io(SWIDIOHandle io_handle) {
	delete (SWIDTagIO *)io_handle;
	return 0;
}


int swid_load_data(SWIDIOHandle io_handle, const char * fname, CSWIDStruct * swid_structure) {
	auto data = ((SWIDTagIO *)io_handle)->load(fname);
	* swid_structure = swid_get_empty_data();
	swid_structure->name = copy_string(data.name);
	swid_structure->tagId = copy_string(data.tagId);
	swid_structure->version = copy_string(data.version);
	swid_structure->versionScheme = copy_string(data.versionScheme);
	swid_structure->xml_lang = copy_string(data.xml_lang);
	swid_structure->type = data.type;
	return 0;
}


int swid_save_data(SWIDIOHandle io_handle, const char * fname, CSWIDStruct * const swid_structure) {
	auto data = C2CXX(* swid_structure);
	auto * io = (SWIDTagIO *)io_handle;
	io->save(fname, data);
	return 0;
}


CSWIDStruct swid_get_empty_data() {
	CSWIDStruct ret;
	ret.name = nullptr;
	ret.tagId = nullptr;
	ret.version = nullptr;
	ret.versionScheme = nullptr;
	ret.xml_lang = nullptr;
	ret.type = SWID_TYPE_PRIMARY;

	ret.entities = nullptr;
	ret.entities_count = 0;

	ret.links = nullptr;
	ret.links_count = 0;

	return ret;
}


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
}
