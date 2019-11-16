#include "pch.h"
#include "GameProcess.h"

GameProcess::GameProcess()
{
	this->registSubPacketFunc();
}
void GameProcess::registSubPacketFunc()
{
#define INSERT_PACKET_PROCESS(type)		runFuncTable_.insert(make_pair(E_##type, &GameProcess::##type))

	INSERT_PACKET_PROCESS(C_REQ_GAME_NAME);
	INSERT_PACKET_PROCESS(C_REQ_ENEMY_NOTEHIT);
	INSERT_PACKET_PROCESS(C_REQ_GAME_CONNECT);
	INSERT_PACKET_PROCESS(C_REQ_GAME_CONNECT_CANCEL);
	INSERT_PACKET_PROCESS(C_REQ_EXIT_GAME);
}

//---------------------------------------------------------------//
//패킷 처리 정의

void GameProcess::C_REQ_EXIT_GAME(Session* session, Packet *rowPacket) {

	PK_C_REQ_EXIT_GAME* packet = (PK_C_REQ_EXIT_GAME*)rowPacket;

	User* user = UserManager::getInstance().at(session->id());
	UserManager::getInstance().remove(session->id());
	delete user;
}

void GameProcess::C_REQ_GAME_NAME(Session *session, Packet *rowPacket)
{
	PK_C_REQ_GAME_NAME *packet = (PK_C_REQ_GAME_NAME *)rowPacket;
	
	string name = packet->name_;
	UINT64 enemySesionID = packet->enemySessionID_;

	if (UserManager::getInstance().at(enemySesionID) != nullptr)
	{
		UserManager::getInstance().at(enemySesionID)->session()->sendPacket(packet);
		session->sendPacket(packet);
	}
			
}

void GameProcess::C_REQ_ENEMY_NOTEHIT(Session *session, Packet *rowPacket)
{
	PK_C_REQ_ENEMY_NOTEHIT *packet = (PK_C_REQ_ENEMY_NOTEHIT *)rowPacket;

	
	int keycode = packet->keycode_;

	PK_C_REQ_ENEMY_NOTEHIT retPacket;
	retPacket.keycode_ = packet->keycode_;
	retPacket.judge_ = packet->judge_;
	retPacket.enemySessionID_ = packet->enemySessionID_;


	if(UserManager::getInstance().at(packet->enemySessionID_) != nullptr)
		UserManager::getInstance().at(packet->enemySessionID_)->session()->sendPacket(packet);


	SLog(L"C_REQ_ENEMY_NOTEHIT packet Received key code : %d, enemySessionID : %d", packet->keycode_, packet->enemySessionID_);
}


void GameProcess::C_REQ_GAME_CONNECT(Session* session, Packet* rowPacket) 
{

	PK_C_REQ_GAME_CONNECT* packet = (PK_C_REQ_GAME_CONNECT*)rowPacket;

	User* user = UserManager::getInstance().CreateUser(session);

	UserManager::getInstance().PushPlayer(user);

	

	SLog(L"C_REQ_GAME_CONNECT packet Received");

}

void GameProcess::C_REQ_GAME_CONNECT_CANCEL(Session* session, Packet* rowPacket)
{
	PK_C_REQ_GAME_CONNECT_CANCEL* packet = (PK_C_REQ_GAME_CONNECT_CANCEL*)rowPacket;

	User* user = UserManager::getInstance().at(session->id());

	UserManager::getInstance().remove(user);

	SLog(L"Removed From Matching Queue Complete!");
}