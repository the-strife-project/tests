#include <common.hpp>
#include <userspace/ramblock.hpp>

void ramblock() {
	testing("Storage > ramblock");
	std::PID pid = std::resolve("ramblock");
	assert(pid);
	passed();

	testing("Storage > ramblock > read (1 page)"); {
		// At this point, superblock is there, so signature must be
		std::SMID smid = std::smMake();
		char* buffer = (char*)std::smMap(smid);
		std::smAllow(smid, pid);

		assert(std::rpc(pid, std::ramblock::READ, smid, 0));

		buffer[8] = 0;
		assert(std::string(buffer) == "StrifeFS");

		std::munmap(buffer);
		std::smDrop(smid);
	} passed();

	testing("Storage > ramblock > read (2 pages)"); {
		std::SMID smid = std::smMake(2);
		char* buffer = (char*)std::smMap(smid);
		std::smAllow(smid, pid);

		assert(std::rpc(pid, std::ramblock::READ, smid, 0));

		// Check first page was actually read
		buffer[8] = 0;
		assert(std::string(buffer) == "StrifeFS");
		// Check second page (inode bitmap) is not empty (first bit MBO)
		assert(buffer[PAGE_SIZE] & (1 << 7));

		std::munmap(buffer, 2);
		std::smDrop(smid);
	} passed();

	testing("Storage > ramblock > write"); {
		// Plan: read superblock, write something at the end of the page, read again
		std::SMID smid = std::smMake();
		char* buffer = (char*)std::smMap(smid);
		std::smAllow(smid, pid);

		assert(std::rpc(pid, std::ramblock::READ, smid, 0));
		buffer[PAGE_SIZE-1] = 0x69;
		assert(std::rpc(pid, std::ramblock::WRITE, smid, 0));
		memset(buffer, 0, PAGE_SIZE); // Don't trick me
		assert(std::rpc(pid, std::ramblock::READ, smid, 0));
		assert(buffer[PAGE_SIZE-1] == 0x69);

		std::munmap(buffer);
		std::smDrop(smid);
	} passed();
}
