#include "../Restuarant/BookingScheduler.cpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "TestSmsSender.cpp"
#include "TestEmailSender.cpp"
#include "SundayBookingScheduler.cpp"
#include "MondayBookingScheduler.cpp"
#include "TestTableBookingScheduler.cpp"

using namespace std;
using namespace testing;

class MockCustomer : public Customer
{
public:
    MockCustomer() : Customer("MockCustomer", "MockNumber") {}
    MOCK_METHOD(string, getEmail, (), (override));
};

class BookingSchedulerTest : public Test
{
public:
    void SetUp() override
    {
        NOT_ON_THE_HOUR = getTime(2021, 3, 26, 9, 5);
        ON_THE_HOUR = getTime(2021, 3, 26, 9, 0);
        DIFF_ON_THE_HOUR = getTime(2022, 3, 26, 9, 0);
        SUNDAY_HOUR = getTime(2024, 5, 19, 9, 0);
        MONDAY_HOUR = getTime(2024, 5, 20, 9, 0);

        booking.setSmsSender(&smsSender);
        booking.setMailSender(&emailSender);

        EXPECT_CALL(customer, getEmail())
            .WillRepeatedly(Return(""));
        EXPECT_CALL(customerWithEmail, getEmail())
            .WillRepeatedly(Return("test@test.com"));
    }

    tm getTime(int year, int mon, int mday, int hour, int min)
    {
        tm stTm{ 0, min, hour, mday, mon - 1, year - 1900, 0, 0, -1 };
        mktime(&stTm);
        return stTm;
    }

    tm NOT_ON_THE_HOUR;
    tm ON_THE_HOUR;
    tm SUNDAY_HOUR;
    tm DIFF_ON_THE_HOUR;
    tm MONDAY_HOUR;

    NiceMock<TesttableSmsSender> smsSender;
    NiceMock<TesttableEmailSender> emailSender;

    /*Customer customer{ "Fake Customer", "010-2323-3434" };
    Customer customerWithEmail{"Fake Customer", "010-2323-3434", "email"};*/
    MockCustomer customer;
    MockCustomer customerWithEmail;

    const int UNDER_CAPACITY_PER_HOUR = 1;
    const int CAPACITY_PER_HOUR = 3;
    BookingScheduler booking{ CAPACITY_PER_HOUR };
    tm stTmDate;
};

TEST_F(BookingSchedulerTest, 예약은_정시에만_가능하다_정시가_아닌경우_예약불가) {
    Schedule schedule(NOT_ON_THE_HOUR, UNDER_CAPACITY_PER_HOUR, customer);
    EXPECT_THROW(booking.addSchedule(&schedule), runtime_error);
}

TEST_F(BookingSchedulerTest, 예약은_정시에만_가능하다_정시인_경우_예약가능) {
    Schedule schedule(ON_THE_HOUR, UNDER_CAPACITY_PER_HOUR, customer);
    booking.addSchedule(&schedule);
    EXPECT_THAT(booking.hasSchedule(&schedule), Eq(true));
}

TEST_F(BookingSchedulerTest, 시간대별_인원제한이_있다_같은_시간대에_Capacity_초과할_경우_예외발생) {
    Schedule schedule1(ON_THE_HOUR, CAPACITY_PER_HOUR, customer);
    Schedule schedule2(ON_THE_HOUR, UNDER_CAPACITY_PER_HOUR, customer);
    booking.addSchedule(&schedule1);
    EXPECT_THROW(booking.addSchedule(&schedule2), runtime_error);
}

TEST_F(BookingSchedulerTest, 시간대별_인원제한이_있다_같은_시간대가_다르면_Capacity_차있어도_스케쥴_추가_성공) {
    Schedule schedule1(ON_THE_HOUR, UNDER_CAPACITY_PER_HOUR, customer);
    Schedule schedule2(DIFF_ON_THE_HOUR, UNDER_CAPACITY_PER_HOUR, customer);
    booking.addSchedule(&schedule1);
    booking.addSchedule(&schedule2);
    EXPECT_THAT(booking.hasSchedule(&schedule1), Eq(true));
    EXPECT_THAT(booking.hasSchedule(&schedule2), Eq(true));
}

TEST_F(BookingSchedulerTest, 예약완료시_SMS는_무조건_발송) {
    Schedule schedule(ON_THE_HOUR, UNDER_CAPACITY_PER_HOUR, customer);
    
    EXPECT_CALL(smsSender, send(&schedule))
        .Times(1);
    booking.addSchedule(&schedule);

    //EXPECT_THAT(smsSender.isSendSms(), Eq(true));
}

TEST_F(BookingSchedulerTest, 이메일이_없는_경우에는_이메일_미발송) {
    Schedule schedule(ON_THE_HOUR, UNDER_CAPACITY_PER_HOUR, customer);
    booking.addSchedule(&schedule);
    //EXPECT_THAT(emailSender.isSendEmail(), Eq(false));
}

TEST_F(BookingSchedulerTest, 이메일이_있는_경우에는_이메일_발송) {
    Schedule schedule(ON_THE_HOUR, UNDER_CAPACITY_PER_HOUR, customerWithEmail);
    EXPECT_CALL(emailSender, sendMail(&schedule))
        .Times(1);
    booking.addSchedule(&schedule);
    
    //EXPECT_THAT(emailSender.isSendEmail(), Eq(true));
}

TEST_F(BookingSchedulerTest, 현재날짜가_일요일인_경우_예약불가_예외처리) {
    //tm stTmNotOnTheHour = getTime(2024, 5, 19, 9, 0);
    Schedule schedule(ON_THE_HOUR, UNDER_CAPACITY_PER_HOUR, customer);

    TestTableBookingScheduler booking{ CAPACITY_PER_HOUR };
    EXPECT_CALL(booking, getNow())
        .WillRepeatedly(Return(mktime(&SUNDAY_HOUR)));
    EXPECT_THROW(booking.addSchedule(&schedule), runtime_error);
}

TEST_F(BookingSchedulerTest, 현재날짜가_일요일이_아닌경우_예약가능) {
    Schedule schedule(ON_THE_HOUR, UNDER_CAPACITY_PER_HOUR, customer);
    TestTableBookingScheduler booking{ CAPACITY_PER_HOUR };
    EXPECT_CALL(booking, getNow())
        .WillRepeatedly(Return(mktime(&MONDAY_HOUR)));

    booking.addSchedule(&schedule);
    EXPECT_THAT(booking.hasSchedule(&schedule), Eq(true));
}