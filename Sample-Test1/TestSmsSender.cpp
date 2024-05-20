#include <iostream>
#include "../Restuarant/SmsSender.cpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace std;

class TesttableSmsSender : public SmsSender
{
public:
	/*void send(Schedule* schedule) override {
		cout << "Send SMS" << endl;
		sendSms = true;
	}*/
	MOCK_METHOD(void, send, (Schedule*), (override));
//	bool isSendSms()
//	{
//		return sendSms;
//	}
//
//private:
//	bool sendSms = false;
};