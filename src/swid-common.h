#ifndef INCLUDED_SWID_COMMON_H
#define INCLUDED_SWID_COMMON_H

/**
 * SWID Tag Types
 *
 * A tag may have exactly one type.
 */
enum type_id {SWID_TYPE_PRIMARY, SWID_TYPE_CORPUS, SWID_TYPE_PATCH, SWID_TYPE_SUPPLEMENTAL};


/**
 * SWID validity codes
 *
 * A SWID tag may be valid, invalid, or we can't tell
 */
enum validity {SWID_VALIDITY_VALID, SWID_VALIDITY_INVALID, SWID_VALIDITY_UNKNOWN};

/**
 * SWID Entity Roles
 *
 * An entity has at least one role. More than one role at the same time are allowed.
 */
enum role_id {
	SWID_ROLE_NONE = 0,
	SWID_ROLE_AGGREGATOR
		= 1 << 0,
	SWID_ROLE_DISTRIBUTOR
		= 1 << 1,
	SWID_ROLE_LICENSOR
		= 1 << 2,
	SWID_ROLE_SOFTWARE_CREATOR
		= 1 << 3,
	SWID_ROLE_TAG_CREATOR
		= 1 << 4
};


#endif /* INCLUDED_SWID_COMMON_H */
