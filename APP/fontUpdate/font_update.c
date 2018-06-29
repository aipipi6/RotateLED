#include "font_update.h"
#include "uart_manager.h"
#include "msg_parser.h"

u8 save_font(u8 index, u8 len, u8 *data)
{
	LOG("save font, index:%d, len:%d\r\n", index, len);
	return 1;
}

u8 update_font(u8 len, u8 *fontData)
{
	u8 nextIndex;
	Font_Msg_t *pFontMsg = (Font_Msg_t *)fontData;
	
	save_font(pFontMsg->index, len - 2, pFontMsg->font);

	nextIndex = pFontMsg->index + 1;
	ble_send(ACK_FONT_UPDATE, 1, &nextIndex);
	
	return 1;
}

