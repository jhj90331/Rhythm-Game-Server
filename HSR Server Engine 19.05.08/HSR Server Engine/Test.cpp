#include "pch.h"
#include "Test.h"
Test::Test()
	: testLock(L"testLocking")
{
	cout << "Test Class Work" << endl;


	MAKE_THREAD(Test, Number1);

	MAKE_THREAD(Test, Number2);

	MAKE_THREAD(Test, Number3);

}

Test::~Test()
{
	cout << "Test Class Finished" << endl;
}

void Test::Number1()
{
	SAFE_LOCK(testLock);
	for (int i = 1; i <= 10; i++)
	{
		cout << "value : " << i << endl;
	}
}

void Test::Number2()
{
	SAFE_LOCK(testLock);
	for (int i = 11; i <= 20; i++)
	{
		cout << "value : " << i << endl;
	}
}

void Test::Number3()
{
	SAFE_LOCK(testLock);
	for (int i = 21; i <= 30; i++)
	{
		cout << "value : " << i << endl;
	}
}