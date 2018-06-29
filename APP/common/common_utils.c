#include "common_utils.h"

void log_array(const u8 *array, u16 size)
{
	u16 i;
	for(i = 0; i < size; i++) {
		LOG("%02x ", array[i]);
	}
	LOG("\r\n");
}



