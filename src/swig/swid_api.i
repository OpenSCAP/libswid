%module swid_api
%{
#include <libswid>
%}

%include "std_string.i"
%include "std_vector.i"
%include "exception.i"

%template(links_vector) std::vector<SWIDLink>;
%template(entities_vector) std::vector<SWIDEntity>;


%catches (std::runtime_error) SWIDTagIO::setBackend;
%catches (XMLIOError, std::runtime_error) SWIDTagIO::load;
%catches (XMLIOError, std::runtime_error) SWIDTagIO::save;

/* Can't use -includeall because it would include STL include files */
%include <libswid-base.h>
%include <SWIDStruct.h>
%include <swid-common.h>
