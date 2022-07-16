#include <common.hpp>
#include <userspace/AHCI.hpp>

void ahci() {
	testing("Storage > AHCI");
	std::PID pid = std::resolve("AHCI");
	assert(pid);
	passed();

	std::vector<std::UUID> ret;
	testing("Storage > AHCI > get ATAPIs"); {
		size_t devices = std::rpc(pid, std::AHCI::GET_ATAPIS);
		assert(devices == 1);
	} passed();

	testing("Storage > AHCI > read (1 sector)"); {
		std::SMID smid = std::smMake();
		char* buffer = (char*)std::smMap(smid);
		std::smAllow(smid, pid);

		// Check CD001 in first volume descriptor
		assert(std::rpc(pid, std::AHCI::READ_ATAPI, smid, 0, 0x10, 1));
		buffer[6] = 0;
		assert(std::string(buffer+1) == "CD001");

		std::munmap(buffer);
		std::smDrop(smid);
	} passed();

	const size_t ASS = 2048; // ATAPI sector size
	testing("Storage > AHCI > read (2 sectors)"); {
		std::SMID smid = std::smMake();
		char* buffer = (char*)std::smMap(smid);
		std::smAllow(smid, pid);

		// Check CD001 in first volume descriptor
		assert(std::rpc(pid, std::AHCI::READ_ATAPI, smid, 0, 0x10, 2));
		buffer[6] = 0;
		assert(std::string(buffer+1) == "CD001");
		buffer[ASS+6] = 0;
		assert(std::string(buffer+ASS+1) == "CD001");

		std::munmap(buffer);
		std::smDrop(smid);
	} passed();
}
