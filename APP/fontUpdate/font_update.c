#include "font_update.h"
#include "uart_manager.h"
#include "common_utils.h"
#include "msg_parser.h"
#include "24cxx.h"


u8 update_font(u8 len, u8 *fontData)
{
	u8 nextIndex;
	Font_Msg_t *pFontMsg = (Font_Msg_t *)fontData;
	pFontMsg->offset = BigLittleSwap16(pFontMsg->offset);
	
	LOG("save font, index:%d, len:%d\r\n", pFontMsg->index, len - 4);
//	AT24CXX_Write(FONT_START_ADDR + pFontMsg->offset, pFontMsg->font, len - 4);

	nextIndex = pFontMsg->index + 1;
	ble_send(ACK_FONT_UPDATE, 1, &nextIndex);
	
	return 1;
}

