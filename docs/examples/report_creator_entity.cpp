#include <iostream>

#include <libswid>

using std::cout;


int main(int argc, const char ** argv) {
	if (argc != 2) {
		cout << "No input file has been specified.\n";
		cout << "Specify exactly one SWID tag filename.\n";
		return 1;
	}
	SWIDStruct tag_data;

	const char * xml_backend = "xerces";
	auto * io = get_swidtagio(xml_backend);
	if (io == nullptr) {
		cout << "Error getting XML backend '";
	        cout << xml_backend << "', try another one.\n";
		return 1;
	}
	try {
		tag_data = io->load(argv[1]);
	} catch (XMLIOError & exc) {
		cout << exc.what() << "\n";
		delete io;
		return 1;
	}
	delete io;

	bool creator_not_found = true;
	for (auto it = tag_data.entities.begin(); it != tag_data.entities.end(); it++) {
		if (it->role & SWID_ROLE_SOFTWARE_CREATOR) {
			creator_not_found = false;
			cout << "Creator of " << tag_data.name << " software: ";
			cout << it->name << " [" << it->regid << "]\n";
			break;
		}
	}
	if (creator_not_found) {
		cout << "Creator of " << tag_data.name << " software has not been found.\n";
	}
	return 0;
}
