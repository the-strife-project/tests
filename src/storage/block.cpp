#include <common.hpp>
#include <userspace/block.hpp>
#include <unordered_map>

void block() {
	testing("Storage > block");
	std::PID pid = std::resolve("block");
	assert(pid);
	passed();

	std::unordered_map<size_t, std::vector<std::UUID>> devices;
	testing("Storage > block > probe"); {
		std::SMID smid = std::smMake();
		uint64_t* buffer = (uint64_t*)std::smMap(smid);
		std::smAllow(smid, pid);

		// Copy-pasted from VFS
		size_t page=0;
		while(true) {
			size_t n = std::rpc(pid,
								std::block::LIST_DEVICES,
								smid,
								page);

			if(n == 0)
				break; // That's all

			while(n--) {
				std::UUID uuid;
				uuid.a = *(buffer++);
				uuid.b = *(buffer++);

				auto type = *(buffer++);
				devices[type].push_back(uuid);
			}

			++page;
		}

		std::munmap(buffer);
		std::smDrop(smid);

		// Must be two entries
		assert(devices.size() == 2);
		// One of type AHCIATAPI
		assert(devices[std::block::DeviceTypes::AHCIATAPI].size() == 1);
		// One of type RAMBLOCK
		assert(devices[std::block::DeviceTypes::RAMBLOCK].size() == 1);
	} passed();

	testing("Storage > block > select RAMBLOCK"); {
		std::UUID uuid = *(devices[std::block::DeviceTypes::RAMBLOCK].begin());
		assert(std::rpc(pid, std::block::SELECT, uuid.a, uuid.b));
	} passed();

	// These are the same as ramblock tests
	testing("Storage > block > read RAMBLOCK (1 page)"); {
		std::SMID smid = std::smMake();
		char* buffer = (char*)std::smMap(smid);
		std::smAllow(smid, pid);

		assert(std::rpc(pid, std::block::READ, smid, 0, PAGE_SIZE));
		buffer[8] = 0;
		assert(std::string(buffer) == "StrifeFS");

		std::munmap(buffer);
		std::smDrop(smid);
	} passed();

	testing("Storage > block > read RAMBLOCK (2 pages)"); {
		std::SMID smid = std::smMake(2);
		char* buffer = (char*)std::smMap(smid);
		std::smAllow(smid, pid);

		assert(std::rpc(pid, std::block::READ, smid, 0, 2 * PAGE_SIZE));
		buffer[8] = 0;
		assert(std::string(buffer) == "StrifeFS");
		assert(buffer[PAGE_SIZE] & (1 << 7));

		std::munmap(buffer, 2);
		std::smDrop(smid);
	} passed();

	testing("Storage > block > write RAMBLOCK"); {
		std::SMID smid = std::smMake();
		char* buffer = (char*)std::smMap(smid);
		std::smAllow(smid, pid);

		assert(std::rpc(pid, std::block::READ, smid, 0, PAGE_SIZE));
		buffer[PAGE_SIZE-1] = 0x6A;
		assert(std::rpc(pid, std::block::WRITE, smid, 0, PAGE_SIZE));
		memset(buffer, 0, PAGE_SIZE); // Don't trick me
		assert(std::rpc(pid, std::block::READ, smid, 0, PAGE_SIZE));
		assert(buffer[PAGE_SIZE-1] == 0x6A);

		std::munmap(buffer);
		std::smDrop(smid);
	} passed();

	testing("Storage > block > select AHCIATAPI"); {
		std::UUID uuid = *(devices[std::block::DeviceTypes::AHCIATAPI].begin());
		assert(std::rpc(pid, std::block::SELECT, uuid.a, uuid.b));
	} passed();

	// These are very similar to AHCI's
	const size_t ASS = 2048; // ATAPI sector size
	testing("Storage > block > read AHCIATAPI (1 page)"); {
		std::SMID smid = std::smMake();
		char* buffer = (char*)std::smMap(smid);
		std::smAllow(smid, pid);

		// Read sector 0x10, check CD001
		assert(std::rpc(pid, std::block::READ, smid, 0x10, ASS));
		buffer[6] = 0;
		assert(std::string(buffer+1) == "CD001");

		std::munmap(buffer);
		std::smDrop(smid);
	} passed();

	testing("Storage > block > read AHCIATAPI (2 pages)"); {
		std::SMID smid = std::smMake();
		char* buffer = (char*)std::smMap(smid);
		std::smAllow(smid, pid);

		// Similar to above
		assert(std::rpc(pid, std::block::READ, smid, 0x10, 2 * ASS));
		buffer[6] = 0;
		assert(std::string(buffer+1) == "CD001");
		buffer[ASS + 6] = 0;
		assert(std::string(buffer+ASS+1) == "CD001");

		std::munmap(buffer);
		std::smDrop(smid);
	} passed();
}
