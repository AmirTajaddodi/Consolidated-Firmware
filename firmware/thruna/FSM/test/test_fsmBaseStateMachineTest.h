#include <gtest/gtest.h>
#include "Test_Utils.h"
#include "Test_BaseStateMachineTest.h"

#include "fake_io_time.hpp"
#include "fake_io_apps.hpp"
#include "fake_io_brake.hpp"
#include "fake_io_coolant.hpp"
#include "fake_io_steering.hpp"
#include "fake_io_wheels.hpp"

extern "C"
{
#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_CanAlerts.h"
#include "App_SharedHeartbeatMonitor.h"
#include "App_SharedStateMachine.h"
#include "App_CanUtils.h"
#include "App_SharedMacros.h"
#include "states/app_driveState.h"
#include "configs/App_HeartbeatMonitorConfig.h"
#include "app_globals.h"
#include "app_apps.h"
#include "app_brake.h"
#include "app_coolant.h"
#include "app_steering.h"
#include "app_wheels.h"
}

// Test fixture definition for any test requiring the state machine. Can also be used for non-state machine related
// tests.

class FsmBaseStateMachineTest : public BaseStateMachineTest
{
  protected:
    void SetUp() override
    {
        BaseStateMachineTest::SetUp();

        App_CanTx_Init();
        App_CanRx_Init();

        app_apps_init();
        app_brake_init();
        app_coolant_init();
        app_steering_init();
        app_wheels_init();

        heartbeat_monitor = App_SharedHeartbeatMonitor_Create(
            io_time_getCurrentMs, HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS, HEARTBEAT_MONITOR_BOARDS_TO_CHECK);
        state_machine = App_SharedStateMachine_Create(NULL, app_driveState_get());

        globals->heartbeat_monitor = heartbeat_monitor;
    }

    void TearDown() override
    {
        TearDownObject(state_machine, App_SharedStateMachine_Destroy);
        TearDownObject(heartbeat_monitor, App_SharedHeartbeatMonitor_Destroy);

        // Reset fakes.
        fake_io_time_getCurrentMs_reset();
        fake_io_apps_getPrimary_reset();
        fake_io_apps_getSecondary_reset();
        fake_io_apps_isPrimaryOCSC_reset();
        fake_io_apps_isSecondaryOCSC_reset();
        fake_io_brake_isActuated_reset();
        fake_io_brake_getPedalPercentTravel_reset();
        fake_io_brake_pedalSensorOCSC_reset();
        fake_io_coolant_getFlowRate_reset();
        fake_io_steering_sensorOCSC_reset();
    }

    struct StateMachine *    state_machine;
    struct HeartbeatMonitor *heartbeat_monitor;
};