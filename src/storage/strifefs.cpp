#include <common.hpp>
#include <userspace/StrifeFS.hpp>

void strifefs() {
	testing("Storage > StrifeFS");
	std::PID pid = std::resolve("StrifeFS");
	assert(pid);
	passed();

	
}
