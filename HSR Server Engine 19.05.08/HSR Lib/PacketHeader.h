#pragma once
#include "stdafx.h"

#define PACKET_MAKE_DATE "2015/03/14 13:14:41" 
enum PacketType : Int64 {
	/*128*/    E_C_REQ_EXIT = 128,
	/*129*/    E_S_ANS_EXIT = 129,
	/*130*/    E_I_NOTIFY_TERMINAL = 130,
	/*131*/    E_C_NOTIFY_HEARTBEAT = 131,
	/*132*/    E_C_REQ_ID_PW = 132,
	/*133*/    E_S_ANS_ID_PW_FAIL = 133,
	/*134*/    E_S_ANS_ID_PW_SUCCESS = 134,
	/*135*/    E_I_DB_REQ_ID_PW = 135,
	/*136*/    E_I_DB_ANS_ID_PW = 136,
	/*137*/    E_I_CHTTING_NOTIFY_ID = 137,
	/*138*/    E_I_DB_REQ_LOAD_DATA = 138,
	/*139*/    E_I_DB_ANS_PARSE_DATA = 139,
	/*140*/    E_I_LOGIN_NOTIFY_ID_LOADED = 140,
	/*141*/    E_C_REQ_REGIST_CHATTING_NAME = 141,
	/*142*/    E_C_REQ_CHATTING = 142,
	/*143*/    E_S_ANS_CHATTING = 143,

	// 여기까지 기존 패킷 헤더

	/*144*/    E_C_REQ_MOVING = 144,
	/*145*/    E_S_ANS_MOVING = 145,
	/*146*/    E_C_REQ_GAME_NAME = 146,
	/*147*/	   E_C_REQ_ENEMY_NOTEHIT = 147,
	/*148*/    E_C_REQ_GAME_CONNECT = 148,
	/*149*/    E_S_ANS_GAME_CONNECT = 149,
	/*150*/    E_C_REQ_GAME_CONNECT_CANCEL = 150,
	/*151*/    E_C_REQ_EXIT_GAME = 151,


};
