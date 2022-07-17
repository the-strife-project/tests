#include <common.hpp>
#include <vector>

void vector() {
	std::vector<size_t> vec;
	testing("STL > vector > 1k insertion"); {
		for(size_t i=0; i<1000; ++i)
			vec.push_back(i);
	} passed();
}
