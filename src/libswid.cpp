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


static char * copy_std_string(const std::string & src) {
	const size_t str_len = src.size() + 1;
	return copy_string(src.c_str(), str_len + 1);
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
	SWIDStruct data;
	try {
		data = ((SWIDTagIO *)io_handle)->load(fname);
	} catch (const XMLIOError & err) {
		return 1;
	}
	* swid_structure = swid_get_empty_data();
	swid_structure->name = copy_std_string(data.name);
	swid_structure->tagId = copy_std_string(data.tagId);
	swid_structure->version = copy_std_string(data.version);
	swid_structure->versionScheme = copy_std_string(data.versionScheme);
	swid_structure->xml_lang = copy_std_string(data.xml_lang);
	swid_structure->type = data.type;
	return 0;
}


int swid_save_data(SWIDIOHandle io_handle, const char * fname, CSWIDStruct * const swid_structure) {
	auto data = C2CXX(* swid_structure);
	auto * io = (SWIDTagIO *)io_handle;
	try {
		io->save(fname, data);
	} catch (const XMLIOError & err) {
		return 1;
	}
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
}
