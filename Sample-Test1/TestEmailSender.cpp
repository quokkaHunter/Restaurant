#include "../Restuarant/MailSender.cpp"
#include <iostream>

using namespace std;

class TesttableEmailSender : public MailSender
{
public:
	void sendMail(Schedule* schedule) override
	{
		if (schedule == nullptr)
		{
			cout << "Hasn't schedule" << endl;
			return;
		}
		cout << "YEAR " << schedule->getDateTime().tm_year  + 1900 << endl;
		cout << "MONTH " << schedule->getDateTime().tm_mon << endl;
		cout << "MDAY " << schedule->getDateTime().tm_mday << endl;
		cout << "HOUR " << schedule->getDateTime().tm_hour << endl;
		cout << "MIN " << schedule->getDateTime().tm_min << endl;
		sendEmMail = true;
	}
	bool isSendEmail() { return sendEmMail;  }

private:
	bool sendEmMail = false;
};