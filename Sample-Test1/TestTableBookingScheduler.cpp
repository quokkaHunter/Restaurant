#include "../Restuarant/BookingScheduler.cpp"

class TestTableBookingScheduler : public BookingScheduler
{
public:
	TestTableBookingScheduler(int capacityPerHour, tm datetime) :
		BookingScheduler{ capacityPerHour },
		datetime{ datetime }
	{}

	time_t getNow() override
	{
		return mktime(&datetime);
	}
private:
	tm datetime;
};