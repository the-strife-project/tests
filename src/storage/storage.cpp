#include <common.hpp>

void ramblock();
void ahci();
void block();
void iso9660();
void strifefs();

void storage() {
	ramblock();
	ahci();
	block();
	iso9660();
	strifefs();
}
