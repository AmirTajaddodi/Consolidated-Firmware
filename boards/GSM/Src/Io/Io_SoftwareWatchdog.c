#include <stm32f3xx.h>
#include <string.h>
#include "App_CanTx.h"
#include "Io_SoftwareWatchdog.h"
#include "Io_SharedMacros.h"

extern IWDG_HandleTypeDef hiwdg;

void Io_HardwareWatchdog_Refresh(void)
{
    HAL_IWDG_Refresh(&hiwdg);
}

void Io_SoftwareWatchdog_TimeoutCallback(SoftwareWatchdogHandle_t watchdog)
{
    BREAK_IF_DEBUGGER_CONNECTED();

    // JSONCAN -> App_CanTx_SetPeriodicSignal_WATCHDOG_TIMEOUT(_can_tx, true);

    // struct CanMsgs_gsm_watchdog_timeout_t payload;
    // memcpy(&payload.task_name, Io_SharedSoftwareWatchdog_GetName(watchdog), sizeof(payload.task_name));

    // App_CanTx_SendNonPeriodicMsg_GSM_WATCHDOG_TIMEOUT(_can_tx, &payload);
}