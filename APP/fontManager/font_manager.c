#include "font_manager.h"
#include "uart_manager.h"
#include "common_utils.h"
#include "msg_parser.h"
#include "24cxx.h"

u8	fontPrepareFlag = 0;
u8 	fontBuffer[FONT_BUFFER_SIZE];
u16 fontLen;
u8 	fontColor;

u8 read_font()
{
	fontLen = AT24CXX_ReadLenByte(FONT_LEN_ADDR, 2);
	fontColor = AT24CXX_ReadOneByte(FONT_COLOR_ADDR);
	
	if(fontLen < FONT_BUFFER_SIZE) {
		AT24CXX_ReadBytes(FONT_START_ADDR, fontBuffer, fontLen);
		fontPrepareFlag = 1;
		return 1;
	}  else {
		fontPrepareFlag = 0;
		return 0;
	}
}

u8 update_font(u8 len, u8 *fontData)
{
	u8 nextIndex;
	Font_Msg_t *pFontMsg = (Font_Msg_t *)fontData;
	pFontMsg->offset = BigLittleSwap16(pFontMsg->offset);
	
	LOG("save font, offset:%d, index:%d/%d, len:%d\r\n", pFontMsg->offset, pFontMsg->index, pFontMsg->total, len - 4);
	AT24CXX_WriteBytes(FONT_START_ADDR + pFontMsg->offset, pFontMsg->font, len - 4);
	memcpy(fontBuffer + pFontMsg->offset, pFontMsg->font, len - 4);

	nextIndex = pFontMsg->index + 1;
	ble_send(ACK_FONT_UPDATE, 1, &nextIndex);
	
	if(nextIndex == pFontMsg->total) {
		fontLen = pFontMsg->offset + len - 4;
		AT24CXX_WriteLenByte(FONT_LEN_ADDR, fontLen, 2);
		fontPrepareFlag = 1;
	} else {
		fontPrepareFlag = 0;
	}
	
	return 1;
}

void update_color(u8 color) 
{
	AT24CXX_WriteOneByte(FONT_COLOR_ADDR, color);
	fontColor = color;
}

