#include "Test_Shared.h"

extern "C"
{
#include "App_Timer.h"
}

class TimerTest : public testing::Test
{
  protected:
    void SetUp() override
    {
        current_time_ms = 0;
        App_Timer_SetCurrentTimeMS(0);
    }

    void PassTime(uint32_t delta_time_ms)
    {
        current_time_ms += delta_time_ms;
        App_Timer_SetCurrentTimeMS(current_time_ms);
    }

    uint32_t     current_time_ms;
    TimerChannel timer;
};

TEST_F(TimerTest, test_start)
{
    // Init timer, should be in idle
    App_Timer_InitTimer(&timer, 1000); // 1s duration
    ASSERT_EQ(App_Timer_UpdateAndGetState(&timer), TIMER_STATE_IDLE);

    // Start timer
    App_Timer_Restart(&timer);
    ASSERT_EQ(App_Timer_UpdateAndGetState(&timer), TIMER_STATE_RUNNING);

    // Confirm timer is running for 1s
    for (int t = 0; t < 1000; t += 100)
    {
        ASSERT_EQ(App_Timer_UpdateAndGetState(&timer), TIMER_STATE_RUNNING);
        PassTime(100);
    }

    // Duration has elapsed, timer should be expired
    ASSERT_EQ(App_Timer_UpdateAndGetState(&timer), TIMER_STATE_EXPIRED);

    // Confirm timer stays expired
    PassTime(100);
    ASSERT_EQ(App_Timer_UpdateAndGetState(&timer), TIMER_STATE_EXPIRED);
}

TEST_F(TimerTest, test_stop_while_running)
{
    // Init timer, should be in idle
    App_Timer_InitTimer(&timer, 1000); // 1s duration
    ASSERT_EQ(App_Timer_UpdateAndGetState(&timer), TIMER_STATE_IDLE);

    // Stop timer, still idle
    App_Timer_Stop(&timer);
    ASSERT_EQ(App_Timer_UpdateAndGetState(&timer), TIMER_STATE_IDLE);

    // Start timer, don't let duration elapse
    App_Timer_Restart(&timer);
    ASSERT_EQ(App_Timer_UpdateAndGetState(&timer), TIMER_STATE_RUNNING);

    for (int t = 0; t < 900; t += 100)
    {
        ASSERT_EQ(App_Timer_UpdateAndGetState(&timer), TIMER_STATE_RUNNING);
        PassTime(100);
    }

    // Stop timer before duration elapses, should go back to idle
    App_Timer_Stop(&timer);

    PassTime(100);
    ASSERT_EQ(App_Timer_UpdateAndGetState(&timer), TIMER_STATE_IDLE);

    PassTime(100);
    ASSERT_EQ(App_Timer_UpdateAndGetState(&timer), TIMER_STATE_IDLE);
}

TEST_F(TimerTest, test_stop_while_expired)
{
    // Init timer, should be in idle
    App_Timer_InitTimer(&timer, 500); // 0.5s duration
    ASSERT_EQ(App_Timer_UpdateAndGetState(&timer), TIMER_STATE_IDLE);

    // Start timer, let duration elapse
    App_Timer_Restart(&timer);
    ASSERT_EQ(App_Timer_UpdateAndGetState(&timer), TIMER_STATE_RUNNING);

    for (int t = 0; t < 500; t += 100)
    {
        ASSERT_EQ(App_Timer_UpdateAndGetState(&timer), TIMER_STATE_RUNNING);
        PassTime(100);
    }

    // Duration has elapsed, timer should be expired
    ASSERT_EQ(App_Timer_UpdateAndGetState(&timer), TIMER_STATE_EXPIRED);
    PassTime(100);
    ASSERT_EQ(App_Timer_UpdateAndGetState(&timer), TIMER_STATE_EXPIRED);

    // Stop timer, should go back to idle even though already expired
    App_Timer_Stop(&timer);

    PassTime(100);
    ASSERT_EQ(App_Timer_UpdateAndGetState(&timer), TIMER_STATE_IDLE);

    PassTime(100);
    ASSERT_EQ(App_Timer_UpdateAndGetState(&timer), TIMER_STATE_IDLE);
}

TEST_F(TimerTest, test_run_if_condition)
{
    // Init timer, should be in idle
    App_Timer_InitTimer(&timer, 5000); // 5s duration
    ASSERT_EQ(App_Timer_UpdateAndGetState(&timer), TIMER_STATE_IDLE);

    // Run w/ true condition, don't allow to complete
    for (int t = 0; t < 5000; t += 100)
    {
        ASSERT_EQ(App_Timer_RunIfCondition(&timer, true), TIMER_STATE_RUNNING);
        PassTime(100);
    }

    // Set false condition, timer stays stopped
    ASSERT_EQ(App_Timer_RunIfCondition(&timer, false), TIMER_STATE_IDLE);
    PassTime(100);

    ASSERT_EQ(App_Timer_RunIfCondition(&timer, false), TIMER_STATE_IDLE);
    PassTime(100);

    // Run w/ true condition again, but let complete
    for (int t = 0; t < 5000; t += 100)
    {
        ASSERT_EQ(App_Timer_RunIfCondition(&timer, true), TIMER_STATE_RUNNING);
        PassTime(100);
    }

    ASSERT_EQ(App_Timer_RunIfCondition(&timer, true), TIMER_STATE_EXPIRED);
    PassTime(100);

    // Confirm timer stays expired indefinitely
    for (int t = 0; t < 10000; t += 100)
    {
        ASSERT_EQ(App_Timer_RunIfCondition(&timer, true), TIMER_STATE_EXPIRED);
        PassTime(100);
    }

    // Finally, stop timer
    ASSERT_EQ(App_Timer_RunIfCondition(&timer, false), TIMER_STATE_IDLE);
}

TEST_F(TimerTest, test_get_elapsed_time)
{
    // Init timer, should be in idle
    App_Timer_InitTimer(&timer, 2000); // 2s duration
    ASSERT_EQ(App_Timer_GetElapsedTime(&timer), 0);
    ASSERT_EQ(App_Timer_UpdateAndGetState(&timer), TIMER_STATE_IDLE);

    // Check elapsed time w/ restart
    App_Timer_Restart(&timer);
    for (int t = 0; t < 1000; t += 100)
    {
        ASSERT_EQ(App_Timer_GetElapsedTime(&timer), t);
        ASSERT_EQ(App_Timer_UpdateAndGetState(&timer), TIMER_STATE_RUNNING);
        PassTime(100);
    }

    // Check restart actually restarts the elapsed time
    App_Timer_Restart(&timer);
    for (int t = 0; t < 1000; t += 100)
    {
        ASSERT_EQ(App_Timer_GetElapsedTime(&timer), t);
        ASSERT_EQ(App_Timer_UpdateAndGetState(&timer), TIMER_STATE_RUNNING);
        PassTime(100);
    }

    // Stop timer, elapsed time set back to 0
    App_Timer_Stop(&timer);
    ASSERT_EQ(App_Timer_GetElapsedTime(&timer), 0);
    ASSERT_EQ(App_Timer_UpdateAndGetState(&timer), TIMER_STATE_IDLE);

    // Check elapsed time w/ run if condition
    for (int t = 0; t < 1000; t += 100)
    {
        App_Timer_RunIfCondition(&timer, true);
        ASSERT_EQ(App_Timer_GetElapsedTime(&timer), t);
        ASSERT_EQ(App_Timer_UpdateAndGetState(&timer), TIMER_STATE_RUNNING);
        PassTime(100);
    }

    // Check that if timer expires, elapsed time is clamped to duration
    for (int t = 1000; t < 3000; t += 100)
    {
        App_Timer_RunIfCondition(&timer, true);
        ASSERT_EQ(
            App_Timer_GetElapsedTime(&timer),
            (t >= 2000) ? 2000 : t); // If t >= 2000, timer expired, so clamp elapsed time to duration
        ASSERT_EQ(App_Timer_UpdateAndGetState(&timer), (t >= 2000) ? TIMER_STATE_EXPIRED : TIMER_STATE_RUNNING);
        PassTime(100);
    }
}