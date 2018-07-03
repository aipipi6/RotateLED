#ifndef __MSG_PARSER_H__
#define __MSG_PARSER_H__

#include "common_utils.h"

#define MSG_START_TAG 	0x9096

#define CMD_FONT_UPDATE 	0x01
#define ACK_FONT_UPDATE 	0x11

#define CMD_COLOR_UPDATE		0x02

typedef struct {
	u16 	startTag;
	u8  	cmdType;
	u8 		len;
	
} Msg_Header_t;




u8 calc_crc(u8 *buff, u8 len);
u8 parser_uart_msg(void);


#endif

