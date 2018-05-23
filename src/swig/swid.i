%module swid
%{
#include <libswid>
%}

%include "std_string.i"
%include "std_vector.i"
%include "exception.i"

%template(links_vector) std::vector<SWIDLink>;
%template(entities_vector) std::vector<SWIDEntity>;

/* Can't use -includeall because it would include STL include files */
%include <lib.h>
%include <SWIDStruct.h>
%include <swid-common.h>
