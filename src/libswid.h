#ifndef LIBSWID_H
#define LIBSWID_H

#include <stddef.h>

#include "swid-common.h"


struct CSWIDTagIO;
typedef struct CSWIDTagIO * SWIDIOHandle;

typedef struct CSWIDLink CSWIDLink;
typedef struct CSWIDEntity CSWIDEntity;


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


#ifdef __cplusplus
extern "C" {
#endif

SWIDIOHandle swid_create_io(const char * backend);
int swid_destroy_io(SWIDIOHandle io_handle);

int swid_load_data(SWIDIOHandle io_handle, const char * fname, CSWIDStruct * swid_structure);
int swid_save_data(SWIDIOHandle io_handle, const char * fname, CSWIDStruct * swid_structure);

CSWIDStruct swid_get_empty_data();
int swid_destroy_data(CSWIDStruct * data);

#ifdef __cplusplus
}
#endif

#endif /* LIBSWID_H */
