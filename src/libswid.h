#ifndef LIBSWID_H
#define LIBSWID_H

#include <stddef.h>

#include "swid-common.h"


struct CSWIDTagIO;
typedef struct CSWIDTagIO * SWIDIOHandle;


struct CSWIDEntity
{
	char * name;
	char * regid;
	int role;
};
typedef struct CSWIDEntity CSWIDEntity;


struct CSWIDLink
{
	char * href;
	char * rel;
};
typedef struct CSWIDLink CSWIDLink;


struct CSWIDStruct
{
	/// Name of the product or component as it would normally be referenced. HARD REQUIREMENT.
	char * name;
	/// Globally unique identifier of the tag. HARD REQUIREMENT.
	char * tagId;
	/// Detailed version of the product. SOFT REQUIREMENT, defaults to "0.0"
	char * version;
	/// Encoding of the version, e.g. "semver", "decimal", "unknown". SOFT REQUIREMENT, defaults to "multipartnumeric"
	char * versionScheme;
	/// Language of the tag in form of W3C-langtag. Automatically inherited to sub-elements. SOFT REQUIREMENT without default.
	char * xml_lang;
	/// OPTIONAL, defaults to SWID_TYPE_PRIMARY.
	int type;

	/// Ordered list of entities
	CSWIDEntity * entities;
	size_t entities_count;

	/// Ordered list of links
	CSWIDLink * links;
	size_t links_count;
};
typedef struct CSWIDStruct CSWIDStruct;
typedef struct SWIDStruct * SWIDHandle;


#ifdef __cplusplus
extern "C" {
#endif

SWIDIOHandle swid_create_io(const char * backend);
int swid_destroy_io(SWIDIOHandle io_handle);

int swid_load_data(SWIDIOHandle io_handle, const char * fname, SWIDHandle swid_structure);
int swid_save_data(SWIDIOHandle io_handle, const char * fname, SWIDHandle swid_structure);


int swid_set_name(SWIDHandle data, const char * name);
const char * swid_get_name(SWIDHandle data);

int swid_set_tagId(SWIDHandle data, const char * tagId);
const char * swid_get_tagId(SWIDHandle data);

int swid_set_version(SWIDHandle data, const char * version);
const char * swid_get_version(SWIDHandle data);

int swid_set_versionScheme(SWIDHandle data, const char * versionScheme);
const char * swid_get_versionScheme(SWIDHandle data);

int swid_set_xml_lang(SWIDHandle data, const char * xml_lang);
const char * swid_get_xml_lang(SWIDHandle data);

int swid_set_type(SWIDHandle data, unsigned int type);
unsigned int swid_get_type(SWIDHandle data);


SWIDHandle swid_get_empty_data();
int swid_destroy_data(SWIDHandle data);

CSWIDEntity swid_get_empty_entity();
int swid_destroy_entity(CSWIDEntity * entity);

int swid_push_back_entity(CSWIDStruct * data, CSWIDEntity * entity);

#ifdef __cplusplus
}
#endif

#endif /* LIBSWID_H */
