#pragma once
#include "pch.h"

class GameProcess : public ContentsProcess
{
public:
	GameProcess();
private:
	void registSubPacketFunc();
	static void C_REQ_GAME_NAME(Session *session, Packet *rowPacket);
	static void C_REQ_ENEMY_NOTEHIT(Session *session, Packet *rowPacket);

	static void C_REQ_GAME_CONNECT(Session* session, Packet* rowPacket);
	static void C_REQ_GAME_CONNECT_CANCEL(Session* session, Packet* rowPacket);


	static void C_REQ_EXIT_GAME(Session* session, Packet* rowPacket);
};