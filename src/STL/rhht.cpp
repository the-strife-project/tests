#include <common.hpp>
#include <RHHT>

void rhht() {
	std::RHHT<int> a;
	testing("STL > RHHT > 1k int insertion"); {
		for(int i=0; i<1000; ++i)
			a.add(i);
		assert(a.size() == 1000);
	} passed();

	testing("STL > RHHT > 1k int iteration"); {
		size_t items = 0;
		for(auto x : a) {
			assert(x >= 0 && x < 1000);
			++items;
		}
		assert(items == 1000);
	} passed();

	testing("STL > RHHT > 1k int const iteration"); {
		size_t items = 0;
		for(auto const& x : a) {
			assert(x >= 0 && x < 1000);
			++items;
		}
		assert(items == 1000);
	} passed();

	std::RHHT<std::pair<int, int>> b;
	testing("STL > RHHT > 1k pair insertion"); {
		for(int i=0; i<1000; ++i)
			b.add({i, 1000-i});
		assert(b.size() == 1000);
	} passed();

	testing("STL > RHHT > 1k pair iteration"); {
		size_t items = 0;
		for(auto& x : b) {
			assert(x.f >= 0 && x.f < 1000 && x.s == 1000 - x.f);
			++items;
		}
		assert(items == 1000);
	} passed();

	testing("STL > RHHT > 1k pair const iteration"); {
		size_t items = 0;
		for(auto const& x : b) {
			assert(x.f >= 0 && x.f < 1000 && x.s == 1000 - x.f);
			++items;
		}
		assert(items == 1000);
	} passed();

	testing("STL > RHHT > 1k pair find"); {
		for(int i=0; i<1000; ++i) {
			auto x = b.find({i, 0});
			assert(x != b.end());
			auto& y = *x;
			assert(y.f == i);
			assert(y.s == 1000 - i);
		}
	} passed();

	/*testing("STL > RHHT > 1k pair const find"); {
		auto const x = b.find({69, 0});
		assert(x != b.end());
		auto const& y = *x;
		assert(y.f == 69);
		assert(y.s == 1000 - 69);
	} passed();*/
}
