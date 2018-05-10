#include <libswid>


void shuffle_stuff(SWIDTagIO * io, const char * source_filename, const char * dest_filename) {
	auto data = io->load(source_filename);
	io->save(dest_filename, data);
}


// $1: Source
// $2: First resave
// $3: Second resave (optional)
int main(int argc, const char ** argv) {
	auto * io = get_swidtagio("xerces");
	for (int i = 1; i < argc - 1; i++) {
		shuffle_stuff(io, argv[i], argv[i + 1]);
	}
	delete io;
	return 0;
}
