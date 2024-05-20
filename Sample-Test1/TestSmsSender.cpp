#include <iostream>
#include "../Restuarant/SmsSender.cpp"

using namespace std;

class TesttableSmsSender : public SmsSender
{
public:
	void send(Schedule* schedule) override {
		cout << "Send SMS" << endl;
		sendSms = true;
	}
	bool isSendSms()
	{
		return sendSms;
	}

private:
	bool sendSms = false;
};