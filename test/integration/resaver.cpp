#include "SWIDStruct.h"
#include "lib.h"



void shuffle_stuff(SWIDTagIO * io, const char * src, const char * dest) {
	auto data = io->load(src);
	io->save(dest, data);
}


// $1: Source
// $2: First resave
// $3: Second resave (optional)
int main(int argc, const char ** argv) {
	auto * io = get_a_swidtagio("xerces");
	for (int i = 1; i < argc - 1; i++) {
		shuffle_stuff(io, argv[i], argv[i + 1]);
	}
	return 0;
}
