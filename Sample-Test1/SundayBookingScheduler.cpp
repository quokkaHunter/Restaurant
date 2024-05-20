#include "../Restuarant/BookingScheduler.cpp"

class SundayBookingScheduler : public BookingScheduler
{
public:
	SundayBookingScheduler(int capacityPerHour) :
		BookingScheduler{ capacityPerHour } {}

	time_t getNow() override
	{
		return getTime(2024, 5, 19, 9, 0);
	}
private:
	time_t getTime(int year, int mon, int mday, int hour, int min)
	{
		tm stTm{ 0, min, hour, mday, mon - 1, year - 1900, 0, 0, -1 };
		return mktime(&stTm);
	}
};