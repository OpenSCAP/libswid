#include <map>
#include <regex>

#include "Translator.h"


using std::string;
using std::map;


const map<const char *, role_id> role_id_map = {
	{"aggregator", SWID_ROLE_AGGREGATOR},
	{"distributor", SWID_ROLE_DISTRIBUTOR},
	{"licensor", SWID_ROLE_LICENSOR},
	{"softwareCreator", SWID_ROLE_SOFTWARE_CREATOR},
	{"tagCreator", SWID_ROLE_TAG_CREATOR}
};


static void rtrim_string(string & str) {
	str.erase(str.find_last_not_of(" \n\r\t") + 1);
}


Role::Role():role(SWID_ROLE_NONE) {
}


Role::Role(string desc):role(SWID_ROLE_NONE) {
	for (auto it = role_id_map.begin(); it != role_id_map.end(); it++)
	{
		string regexp_text = "\\b";
		regexp_text = it->first;
		regexp_text += "\\b";
		auto regexp = std::regex(regexp_text);
		if (std::regex_search(desc, regexp)) {
			role = role | it->second;
		}
	}
}


Role::Role(role_id id): role(id) {
}


string Role::RoleAsString() const {
	auto ret = string();
	for (auto it = role_id_map.begin(); it != role_id_map.end(); it++)
	{
		if (role & it->second) {
			ret += it->first;
			ret += " ";
		}
	}

	rtrim_string(ret);
	return ret;
}


role_id Role::RoleAsId() const {
	return role;
}
