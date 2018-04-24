#include <iostream>
#include <fstream>
#include <string>
// there are other ways of getting a path relative to the executable, but this
// one is part of the STL soon (if you can talk about soon in c++ development)

// when testing, the main of gtest and this main function will collide,
// this prevents the collision, but it's an ugly hack like all ifdef branches

int main(int argc, char** argv) {
	return 0;
}
