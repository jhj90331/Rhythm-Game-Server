#pragma once
#include "pch.h"
class User;
class Session;



class UserManager : public Singleton < UserManager >
{
	unordered_map<oid_t, User*> userPool_;
	list<User*> playerMatchingQueue;
	int queueCount;
	CRITICAL_SECTION cs;



public:
	
	UserManager() {
		InitializeCriticalSection(&cs);
		queueCount = 0;
	}

	void insert(User *user)
	{
		oid_t key = user->session()->id();
		userPool_.insert(make_pair(key, user));
	}

	User* CreateUser(Session* session) {
		User* user = nullptr;
		if ((user = at(session->id())) != nullptr) {
			return user;
		}
		user = new User(session);
		insert(user);
		return user;
		
	}


	void remove(oid_t id)
	{
		userPool_.erase(id);
	}

	User* at(oid_t id)
	{
		auto itr = userPool_.lower_bound(id);
		if (itr == userPool_.end()) {
			return nullptr;
		}
		return itr->second;
	}

	size_t size()
	{
		return userPool_.size();
	}


	void PushPlayer(User* user) {
		EnterCriticalSection(&cs);
		playerMatchingQueue.push_back(user);
		queueCount += 1;
		LeaveCriticalSection(&cs);
		if (queueCount >= 2) {
			PopPlayers();
		}
	}

	



	void PopPlayers() {

		EnterCriticalSection(&cs);
		queueCount -= 2;
		User* firstUser = playerMatchingQueue.front();
		playerMatchingQueue.pop_front();
		User* secondUser = playerMatchingQueue.front();
		playerMatchingQueue.pop_front();


		// 유저가 S_ANS_GAME_CONNECT 를 받는다면 Change Game Scene;

		PK_S_ANS_GAME_CONNECT firstPacket;
		firstPacket.title_ = "Lovely_Summer";
		firstPacket.enemySessionID_ = secondUser->session()->id();
		firstUser->session()->sendPacket(&firstPacket);
		SLog(L"FirstUserSessoin ID : %d", firstUser->session()->id());


		PK_S_ANS_GAME_CONNECT secondPacket;
		secondPacket.title_ = "Lovely_Summer";
		secondPacket.enemySessionID_ = firstUser->session()->id();
		secondUser->session()->sendPacket(&secondPacket);
		SLog(L"SecondUserSession ID : %d", secondUser->session()->id());
		

		remove(firstUser);
		remove(secondUser);

		LeaveCriticalSection(&cs);
	}

	bool remove(User* user) {
		if (user == nullptr)
		{
			return false;
		}

		EnterCriticalSection(&cs);
		list<User*>::iterator it = playerMatchingQueue.begin();

		for (; it != playerMatchingQueue.end(); it++) {
			if (*it == user) {
				queueCount -= 1;
				playerMatchingQueue.erase(it);
				
				LeaveCriticalSection(&cs);
				return true;
			}
		}
		LeaveCriticalSection(&cs);

		return true;
	}


};