#ifndef INCLUDED_LIBSWID_H
#define INCLUDED_LIBSWID_H

#include <stddef.h>

#include "swid-common.h"

typedef struct SWIDTagIO * SWIDIOHandle;

typedef struct SWIDStruct * SWIDHandle;

typedef struct SWIDEntity * SWIDEntityHandle;
typedef struct SWIDLink SWIDLinkHandle;


#ifdef __cplusplus
extern "C" {
#endif

SWIDIOHandle swid_create_io(const char * backend);
int swid_destroy_io(SWIDIOHandle io_handle);

int swid_load_root(SWIDIOHandle io_handle, const char * fname, SWIDHandle swid_structure);
int swid_save_root(SWIDIOHandle io_handle, const char * fname, SWIDHandle swid_structure);


int swid_root_set_name(SWIDHandle data, const char * name);
const char * swid_root_get_name(SWIDHandle data);

int swid_root_set_tagId(SWIDHandle data, const char * tagId);
const char * swid_root_get_tagId(SWIDHandle data);

int swid_root_set_version(SWIDHandle data, const char * version);
const char * swid_root_get_version(SWIDHandle data);

int swid_root_set_versionScheme(SWIDHandle data, const char * versionScheme);
const char * swid_root_get_versionScheme(SWIDHandle data);

int swid_root_set_xml_lang(SWIDHandle data, const char * xml_lang);
const char * swid_root_get_xml_lang(SWIDHandle data);

int swid_root_set_type(SWIDHandle data, unsigned int type);
unsigned int swid_root_get_type(SWIDHandle data);


SWIDHandle swid_create_root();
int swid_destroy_root(SWIDHandle data);

SWIDEntityHandle swid_create_entity();
int swid_destroy_entity(SWIDEntityHandle entity);


SWIDEntityHandle swid_root_get_entity(SWIDHandle swid, size_t index);
int swid_append_entity_data(SWIDHandle swid, SWIDEntityHandle entity);

int swid_entity_set_name(SWIDEntityHandle entity, const char * name);
const char * swid_entity_get_name(SWIDEntityHandle entity);

int swid_entity_set_regid(SWIDEntityHandle entity, const char * regid);
const char * swid_entity_get_regid(SWIDEntityHandle entity);

int swid_entity_set_role(SWIDEntityHandle entity, unsigned int role);
unsigned int swid_entity_get_role(SWIDEntityHandle entity);

int swid_root_append_entity(SWIDHandle data, SWIDEntityHandle entity);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDED_LIBSWID_H */
