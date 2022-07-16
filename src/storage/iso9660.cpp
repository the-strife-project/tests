#include <common.hpp>
#include <userspace/ISO9660.hpp>

void iso9660() {
	testing("Storage > ISO9660");
	std::PID pid = std::resolve("_bsISO");
	assert(pid);
	passed();

	size_t root = 0;
	testing("Storage > ISO9660 > get root"); {
		root = std::rpc(pid, std::ISO9660::GET_ROOT);
		assert(root);
	} passed();

	size_t npages = 0;
	testing("Storage > ISO9660 > public list size (root)"); {
		npages = std::rpc(pid, std::ISO9660::LIST_SIZE, root);
		assert(npages);
	} passed();

	testing("Storage > ISO9660 > list (root)"); {
		std::SMID smid = std::smMake(npages);
		char* buffer = (char*)std::smMap(smid);
		std::smAllow(smid, pid);

		assert(std::rpc(pid, std::ISO9660::LIST, smid, root));
		// For now, just check it's not empty
		assert(*(uint64_t*)buffer);

		std::munmap(buffer, npages);
		std::smDrop(smid);
	} passed();
}
