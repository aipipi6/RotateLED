#ifndef __MSG_PARSER_H__
#define __MSG_PARSER_H__

#include "common_utils.h"

#define MSG_START_TAG 	0x9096

#define CMD_FONT_UPDATE 	0x01
#define ACK_FONT_UPDATE 	0x11

#define CMD_COLOR_BLUE		0x02
#define CMD_COLOR_RED		0x03
#define CMD_COLOR_DOUBLE	0x04

typedef struct {
	u16 	startTag;
	u8  	cmdType;
	u8 		len;
	
} Msg_Header_t;




u8 calc_crc(u8 *buff, u8 len);
u8 parser_uart_msg(void);


#endif

