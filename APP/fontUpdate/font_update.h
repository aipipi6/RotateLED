#ifndef __FONT_UPDATE_H__
#define __FONT_UPDATE_H__

#include "common_utils.h"

#define FONT_START_ADDR 	0x100

#pragma pack (1) // 指定按一字节对齐
#define FONT_BUFF_SIZE		72
typedef struct {
	u8 		index;
	u8 		total;
	u16 	offset;
	u8		font[FONT_BUFF_SIZE];
} Font_Msg_t;
#pragma pack () // 取消指定结构体对齐

u8 update_font(u8 len, u8 *fontData);
#endif
