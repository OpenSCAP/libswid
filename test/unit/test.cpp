#include "catch.hpp"

#include "loader-xerces.h"

TEST_CASE( "Basic I/O", "[io]" ) {
	auto loader = XercesSWIDTagLoader();
	CHECK_THROWS( loader.load("") );
}
