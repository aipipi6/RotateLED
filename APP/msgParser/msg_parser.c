#include "msg_parser.h"
#include "font_manager.h"
#include "uart_manager.h"

#define PARSER_MSG_CACHE_SIZE 512
u8	parser_msg_cache_buff[PARSER_MSG_CACHE_SIZE];
u16	parser_msg_cache_len;

u8 calc_crc(u8 *buff, u8 len) {
	u8 crc = 0;
	u8 i;
	for(i = 0; i < len; i++) {
		crc ^= buff[i];
	}
	return crc;
}

u8 parser_uart_msg()
{
	u8 				dataLen;
	u8 				offset;
	u8				crcCode;
	u16 			startTag;
	Msg_Header_t	*header;
	
	dataLen = get_uart_ring_buf(&parser_msg_cache_buff[parser_msg_cache_len]);
	parser_msg_cache_len += dataLen;

	if(dataLen <= 0) {
		return 0;
	}
	
	offset = 0;
	
	while(offset + sizeof(Msg_Header_t) <= parser_msg_cache_len) {
		header = (Msg_Header_t *)&parser_msg_cache_buff[offset];
		// 修改成小端模式
		startTag = BigLittleSwap16(header->startTag);
		if(startTag == MSG_START_TAG) {
			if(offset + sizeof(Msg_Header_t) + header->len + 1 > parser_msg_cache_len) {
				break;
			}
			crcCode = parser_msg_cache_buff[offset + sizeof(Msg_Header_t) + header->len];
			u8 calcCrcCode = calc_crc(&parser_msg_cache_buff[offset], sizeof(Msg_Header_t) + header->len);
			if(crcCode != calcCrcCode) {
			
				LOG("CRC error, read:0x%02x, calc:0x%02x\r\n", crcCode, calcCrcCode);
				log_array((u8 *)header, sizeof(Msg_Header_t) + header->len + 1);
				offset += sizeof(Msg_Header_t) + header->len + 1;
				break;
			}
			
			switch(header->cmdType) {
				case CMD_FONT_UPDATE:
					update_font(header->len, (u8 *)&parser_msg_cache_buff[offset + sizeof(Msg_Header_t)]);
					break;
				
				case CMD_COLOR_UPDATE:
					update_color(parser_msg_cache_buff[offset + sizeof(Msg_Header_t)]);
					break;
				
				default:
					break;
			}
			
			 offset += sizeof(Msg_Header_t) + header->len + 1;
		} else {
			offset++;
		}
	
	}
	
	if (offset > 0) {
		memmove(parser_msg_cache_buff, &parser_msg_cache_buff[offset], parser_msg_cache_len - offset);
	}
	
	parser_msg_cache_len -= offset;
	
	if(parser_msg_cache_len > PARSER_MSG_CACHE_SIZE) {
		parser_msg_cache_len = 0;
	}
	
	
	return 1;
}

