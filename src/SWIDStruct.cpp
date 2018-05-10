#include "SWIDStruct.h"


// Took from https://stackoverflow.com/a/1448478/592892
role_id operator | (role_id a, role_id b) {
	return static_cast<role_id>(static_cast<int>(a) | static_cast<int>(b));
}


role_id operator & (role_id a, role_id b) {
	return static_cast<role_id>(static_cast<int>(a) & static_cast<int>(b));
}


SWIDBase::SWIDBase():
	name(), tagId(), version(), versionScheme(), xml_lang(), type(SWID_TYPE_PRIMARY)
{
}


void SWIDBase::applyDefaults() {
	if (versionScheme.size() == 0) {
		versionScheme = "multipartnumeric";
	}
	if (version.size() == 0) {
		version = "0.0";
	}
}


SWIDStruct::SWIDStruct():
	SWIDBase(),
	entities(), links()
{
}


bool SWIDEntity::operator == (const SWIDEntity & rhs) const
{
	return name == rhs.name && role == rhs.role && regid == rhs.regid;
}


bool SWIDLink::operator == (const SWIDLink & rhs) const
{
	return href == rhs.href && rel == rhs.rel;
}
