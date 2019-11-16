#pragma once
#include "stdafx.h"
#include "packetHeader.h"
#include "Stream.h"

// 패킷의 종류들
class Packet {
public:
	virtual PacketType type() = 0;
	virtual void encode(Stream &stream) { stream << (Int64)this->type(); };
	virtual void decode(Stream &stream) { };
};

class PK_C_REQ_EXIT : public Packet
{
public:
	PacketType type() { return E_C_REQ_EXIT; }

};

class PK_S_ANS_EXIT : public Packet
{
public:
	PacketType type() { return E_S_ANS_EXIT; }

};

class PK_I_NOTIFY_TERMINAL : public Packet
{
public:
	PacketType type() { return E_I_NOTIFY_TERMINAL; }

};

class PK_C_NOTIFY_HEARTBEAT : public Packet
{
public:
	PacketType type() { return E_C_NOTIFY_HEARTBEAT; }

};

class PK_C_REQ_ID_PW : public Packet
{
public:
	PacketType type() { return E_C_REQ_ID_PW; }

	std::string     id_;
	std::string     password_;

	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << id_;
		stream << password_;

	}

	void decode(Stream &stream) {
		stream >> &id_;
		stream >> &password_;
	}
};

class PK_S_ANS_ID_PW_FAIL : public Packet
{
public:
	PacketType type() { return E_S_ANS_ID_PW_FAIL; }

};

class PK_S_ANS_ID_PW_SUCCESS : public Packet
{
public:
	PacketType type() { return E_S_ANS_ID_PW_SUCCESS; }

	std::string     ip_;
	UInt32     port_;
	std::string     name_;

	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << ip_;
		stream << port_;
		stream << name_;
	}

	void decode(Stream &stream) {
		stream >> &ip_;
		stream >> &port_;
		stream >> &name_;
	}
};

class PK_I_DB_REQ_ID_PW : public Packet
{
public:
	PacketType type() { return E_I_DB_REQ_ID_PW; }

	UInt64     clientId_;
	std::string     id_;
	std::string     password_;

	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << clientId_;
		stream << id_;
		stream << password_;
	}

	void decode(Stream &stream) {
		stream >> &clientId_;
		stream >> &id_;
		stream >> &password_;
	}
};

class PK_I_DB_ANS_ID_PW : public Packet
{
public:
	PacketType type() { return E_I_DB_ANS_ID_PW; }

	UInt64     clientId_;
	UInt64     oidAccountId_;
	Byte     result_;

	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << clientId_;
		stream << oidAccountId_;
		stream << result_;
	}

	void decode(Stream &stream) {
		stream >> &clientId_;
		stream >> &oidAccountId_;
		stream >> &result_;
	}
};

class PK_I_CHTTING_NOTIFY_ID : public Packet
{
public:
	PacketType type() { return E_I_CHTTING_NOTIFY_ID; }

	UInt64     clientId_;
	UInt64     oidAccountId_;

	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << clientId_;
		stream << oidAccountId_;
	}

	void decode(Stream &stream) {
		stream >> &clientId_;
		stream >> &oidAccountId_;
	}
};

class PK_I_DB_REQ_LOAD_DATA : public Packet
{
public:
	PacketType type() { return E_I_DB_REQ_LOAD_DATA; }

	UInt64     clientId_;
	UInt64     oidAccountId_;

	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << clientId_;
		stream << oidAccountId_;
	}

	void decode(Stream &stream) {
		stream >> &clientId_;
		stream >> &oidAccountId_;
	}
};

class PK_I_DB_ANS_PARSE_DATA : public Packet
{
public:
	PacketType type() { return E_I_DB_ANS_PARSE_DATA; }

	UInt64     clientId_;
	std::string     name_;
	Byte     result_;

	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << clientId_;
		stream << name_;
		stream << result_;
	}

	void decode(Stream &stream) {
		stream >> &clientId_;
		stream >> &name_;
		stream >> &result_;
	}
};

class PK_I_LOGIN_NOTIFY_ID_LOADED : public Packet
{
public:
	PacketType type() { return E_I_LOGIN_NOTIFY_ID_LOADED; }

	UInt64     clientId_;
	std::string     name_;
	Byte     result_;

	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << clientId_;
		stream << name_;
		stream << result_;
	}

	void decode(Stream &stream) {
		stream >> &clientId_;
		stream >> &name_;
		stream >> &result_;
	}
};

class PK_C_REQ_REGIST_CHATTING_NAME : public Packet
{
public:
	PacketType type() { return E_C_REQ_REGIST_CHATTING_NAME; }

	std::string     name_;

	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << name_;
	}

	void decode(Stream &stream) {
		stream >> &name_;
	}
};

class PK_C_REQ_CHATTING : public Packet
{
public:
	PacketType type() { return E_C_REQ_CHATTING; }

	std::string     text_;

	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << text_;
	}

	void decode(Stream &stream) {
		stream >> &text_;
	}
};

class PK_S_ANS_CHATTING : public Packet
{
public:
	PacketType type() { return E_S_ANS_CHATTING; }

	std::string     name_;
	std::string     text_;

	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << name_;
		stream << text_;
	}

	void decode(Stream &stream) {
		stream >> &name_;
		stream >> &text_;
	}
};

// 여기까지 기존 패킷

class PK_C_REQ_MOVING : public Packet
{
public:
	PacketType type() { return E_C_REQ_MOVING; }

	float	posX_ = 0;
	float	posY_ = 0;
	float	posZ_ = 0;

	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << posX_;
		stream << posY_;
		stream << posZ_;
	}

	void decode(Stream &stream) {
		stream >> &posX_;
		stream >> &posY_;
		stream >> &posZ_;
	}
};

class PK_S_ANS_MOVING : public Packet
{
public:
	PacketType type() { return E_S_ANS_MOVING; }

	float	posX_;
	float	posY_;
	float	posZ_;

	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << posX_;
		stream << posY_;
		stream << posZ_;
	}

	void decode(Stream &stream) {
		stream >> &posX_;
		stream >> &posY_;
		stream >> &posZ_;
	}
};


class PK_C_REQ_GAME_NAME : public Packet
{
public:
	PacketType type() { return E_C_REQ_GAME_NAME; }

	std::string     name_;
	UINT64     enemySessionID_;

	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << name_;
		stream << enemySessionID_;
	}

	void decode(Stream &stream) {
		stream >> &name_;
		stream >> &enemySessionID_;
	}
};


class PK_C_REQ_ENEMY_NOTEHIT : public Packet
{
public:
	PacketType type() { return E_C_REQ_ENEMY_NOTEHIT; }

	UInt32     keycode_;
	UInt32     judge_;
	UINT64     enemySessionID_;

	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << keycode_;
		stream << judge_;
		stream << enemySessionID_;
	}

	void decode(Stream &stream) {
		stream >> &keycode_;
		stream >> &judge_;
		stream >> &enemySessionID_;
	}


};


class PK_C_REQ_GAME_CONNECT : public Packet
{
public:
	PacketType type() { return E_C_REQ_GAME_CONNECT; }

	string title_;

	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << title_;
	}

	void decode(Stream &stream) {
		stream >> &title_;
	}


};


class PK_S_ANS_GAME_CONNECT : public Packet
{
public:
	PacketType type() { return E_S_ANS_GAME_CONNECT; }

	string title_;
	UINT64 enemySessionID_;

	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << title_;
		stream << enemySessionID_;
	}

	void decode(Stream &stream) {
		stream >> &title_;
		stream >> &enemySessionID_;
	}


};

class PK_C_REQ_GAME_CONNECT_CANCEL : public Packet
{
public:
	PacketType type() { return E_C_REQ_GAME_CONNECT_CANCEL; }

	string title_;

	void encode(Stream &stream) {
		stream << (Int64)this->type();
		stream << title_;
	}

	void decode(Stream &stream) {
		stream >> &title_;
	}


};


class PK_C_REQ_EXIT_GAME : public Packet
{
public:
	PacketType type() { return E_C_REQ_EXIT_GAME; }

	void encode(Stream &stream) {
		stream << (Int64)this->type();

	}

	void decode(Stream &stream) {

	}


};
