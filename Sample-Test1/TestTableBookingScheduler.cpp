#include "../Restuarant/BookingScheduler.cpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

class TestTableBookingScheduler : public BookingScheduler
{
public:
	TestTableBookingScheduler(int capacityPerHour) :
		BookingScheduler{ capacityPerHour }/*,
		datetime{ datetime }*/
	{}
	MOCK_METHOD(time_t, getNow, (), (override));
	//time_t getNow() override
	//{
	//	return mktime(&datetime);
	//}
//private:
//	tm datetime;
};