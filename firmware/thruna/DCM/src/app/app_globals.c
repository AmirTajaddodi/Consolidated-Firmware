#include "app_globals.h"

Globals        dcm_globals;
Globals *const globals = &dcm_globals;

void app_globals_init(const GlobalsConfig *config)
{
    globals->config = config;

    globals->torque_vectoring_switch_is_on = false;
    app_timer_init(&globals->buzzer_timer, BUZZER_ON_DURATION_MS);
}