#include <common.hpp>
#include <RHHT>
#include <set>
#include <random>

void rhht() {
	std::BadRNG gen;

	testing("STL > RHHT > 1k sequence check"); {
		gen.reset();
		std::set<uint64_t> vals;
		for(size_t i=0; i<1000; ++i)
			vals.insert(gen.next());
		assert(vals.size() == 1000);
	} passed();

	std::RHHT<uint64_t> a;
	testing("STL > RHHT > 1k int insertion"); {
		gen.reset();
		for(size_t i=0; i<1000; ++i)
			a.add(gen.next());
		assert(a.size() == 1000);
	} passed();

	testing("STL > RHHT > 1k int has"); {
		gen.reset();
		for(size_t i=0; i<1000; ++i)
			assert(a.has(gen.next()));
	} passed();
	a.clear();

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
}
