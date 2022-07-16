#ifndef COMMON_HPP
#define COMMON_HPP

#include <cstdio>
#include <shared_memory>

extern size_t ntests;
extern size_t npassed;

inline void testing(const char* name) {
	std::printf("\033\x09%s\033\x0A ", name);
	++ntests;
}

inline void passed() {
	std::printf("\033\x0APASSED\n");
	++npassed;
}

inline void failed() {
	std::printf("\033\x0C""Failed!\033\x0A\n");
	std::exit(0x100);
}

inline void assert(bool v) { if(!v) failed(); }

// Areas
void storage();

#endif
