#include "states/App_FaultState.h"
#include "states/App_AllStates.h"
#include "App_SharedMacros.h"

void App_FaultStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct FsmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    // TODO: JSONCAN ->
    // App_CanTx_SetPeriodicSignal_STATE(can_tx_interface, CANMSGS_FSM_STATE_MACHINE_STATE_FAULT_CHOICE);
}

void App_FaultStateRunOnTick1Hz(struct StateMachine *state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}
void App_FaultStateRunOnTick100Hz(struct StateMachine *state_machine)
{
    App_AllStatesRunOnTick100Hz(state_machine);

    struct FsmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    // TODO: JSONCAN ->
    // App_CanTx_SetPeriodicSignal_MAPPED_PEDAL_PERCENTAGE(can_tx, 0.0f);
}
void App_FaultStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetFaultState(void)
{
    static struct State fault_state = {
        .name              = "FAULT STATE",
        .run_on_entry      = App_FaultStateRunOnEntry,
        .run_on_tick_1Hz   = App_FaultStateRunOnTick1Hz,
        .run_on_tick_100Hz = App_FaultStateRunOnTick100Hz,
        .run_on_exit       = App_FaultStateRunOnExit,
    };
    return &fault_state;
}