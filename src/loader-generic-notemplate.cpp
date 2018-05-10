#include <sstream>

#include "loader-generic.h"

using std::ostringstream;


static XMLIOError create_xml_io_error(const std::string & filename, const std::string & error_intro, const std::string & what_happened) {
	ostringstream msg;
	msg << error_intro;
	msg << " '";
	msg << filename;
	msg << "': ";
	msg << what_happened;
	return XMLIOError(msg.str());
}


XMLIOError create_read_error(const std::string & filename, const std::string & what_happened) {
	return create_xml_io_error(filename, "Could not load from", what_happened);
}


XMLIOError create_save_error(const std::string & filename, const std::string & what_happened) {
	return create_xml_io_error(filename, "Could not save to", what_happened);
}
