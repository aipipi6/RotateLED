#ifndef __FONT_UPDATE_H__
#define __FONT_UPDATE_H__

#include "common_utils.h"

#define FONT_START_ADDR 	0x100
#define FONT_LEN_ADDR 		0
#define FONT_COLOR_ADDR		3

#define SINGLE_FONT_SIZE	72							// 
#define FONT_BUFFER_SIZE	(SINGLE_FONT_SIZE * 100)

#pragma pack (1) // 指定按一字节对齐

typedef struct {
	u8 		index;
	u8 		total;
	u16 	offset;
	u8		font[SINGLE_FONT_SIZE];
} Font_Msg_t;

#pragma pack () // 取消指定结构体对齐

extern u8	fontPrepareFlag;
extern u8 	fontBuffer[FONT_BUFFER_SIZE];
extern u16 	fontLen;
extern u8 	fontColor;

u8 read_font(void);
u8 update_font(u8 len, u8 *fontData);
void update_color(u8 color) ;
#endif
