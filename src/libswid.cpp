#include <cstring>

#include "lib.h"

#include "libswid.h"


int swid_root_set_name(SWIDHandle data, const char * name) {
	data->name = name;
	return 0;
}


const char * swid_root_get_name(SWIDHandle data) {
	return data->name.c_str();
}


int swid_root_set_tagId(SWIDHandle data, const char * tagId) {
	data->tagId = tagId;
	return 0;
}


const char * swid_root_get_tagId(SWIDHandle data) {
	return data->tagId.c_str();
}


int swid_root_set_version(SWIDHandle data, const char * version) {
	data->version = version;
	return 0;
}


const char * swid_root_get_version(SWIDHandle data) {
	return data->version.c_str();
}


int swid_root_set_versionScheme(SWIDHandle data, const char * versionScheme) {
	data->versionScheme = versionScheme;
	return 0;
}


const char * swid_root_get_versionScheme(SWIDHandle data) {
	return data->versionScheme.c_str();
}


int swid_root_set_xml_lang(SWIDHandle data, const char * xml_lang) {
	data->xml_lang = xml_lang;
	return 0;
}


const char * swid_root_get_xml_lang(SWIDHandle data) {
	return data->xml_lang.c_str();
}


int swid_root_set_type(SWIDHandle data, unsigned int type) {
	data->type = (type_id)type;
	return 0;
}


unsigned int swid_root_get_type(SWIDHandle data) {
	return data->type;
}


/*
 * I/O stuff
 */

SWIDIOHandle swid_create_io(const char * backend) {
	return (SWIDIOHandle)get_swidtagio(backend);
}


int swid_destroy_io(SWIDIOHandle io_handle) {
	delete (SWIDTagIO *)io_handle;
	return 0;
}


int swid_load_root(SWIDIOHandle io_handle, const char * fname, SWIDHandle swid_structure) {
	auto data = SWIDStruct();
	try {
		data = ((SWIDTagIO *)io_handle)->load(fname);
	} catch (const XMLIOError & err) {
		return 1;
	}
	* swid_structure = data;
	return 0;
}


int swid_save_root(SWIDIOHandle io_handle, const char * fname, SWIDHandle data) {
	auto * io = (SWIDTagIO *)io_handle;
	try {
		io->save(fname, * data);
	} catch (const XMLIOError & err) {
		return 1;
	}
	return 0;
}


SWIDHandle swid_create_root() {
	auto * ret = new SWIDStruct();
	return ret;
}


int swid_destroy_root(SWIDHandle data) {
	delete ((SWIDStruct *)data);
	return 0;
}

/*
 * Entity stuff
 */


SWIDEntityHandle swid_create_entity() {
	return new SWIDEntity();
}


int swid_destroy_entity(SWIDEntityHandle entity) {
	delete entity;
	return 0;
}


SWIDEntityHandle swid_root_get_entity(SWIDHandle swid, size_t index) {
	if (index >= swid->entities.size()) {
		return nullptr;
	}
	return & swid->entities[index];
}


unsigned int swid_entity_get_role(SWIDEntityHandle entity) {
	return entity->role;
}


int swid_entity_set_name(SWIDEntityHandle entity, const char * name) {
	entity->name = name;
	return 0;
}


int swid_entity_set_role(SWIDEntityHandle entity, unsigned int role) {
	return entity->role = (role_id)role;
}


const char * swid_entity_get_name(SWIDEntityHandle entity) {
	return entity->name.c_str();
}


int swid_entity_set_regid(SWIDEntityHandle entity, const char * regid) {
	entity->regid = regid;
	return 0;
}


const char * swid_entity_get_regid(SWIDEntityHandle entity) {
	return entity->regid.c_str();
}


int swid_append_entity_data(SWIDHandle swid, SWIDEntityHandle entity) {
	swid->entities.push_back(* entity);
	return 0;
}
