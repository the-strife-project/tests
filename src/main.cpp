#include <common.hpp>

size_t ntests = 0;
size_t npassed = 0;

extern "C" void _start() {
	std::printf("\033\x0ERunning all unit tests\033\x0A\n");

	stl();
	storage();

	std::printf("\033\x0E""All %d tests passed\033\x0A\n", ntests);
	std::exit(0);
}
