#pragma once

#include <stdbool.h>
#include <stdint.h>

// Ignore the charger fault signal for the first 500 cycles (5 seconds)
#define CYCLES_TO_IGNORE_CHGR_FAULT (500U)

struct Charger;

// This library is modelled after the BRUSA NLG513 charger. It has the concept
// of a master switch (Pin 3: PON). In short, if the BRUSA NLG513 is powered on
// but the master switch is disabled, it can never start the charging sequence.
// For the charging sequence to start, the BRUSA NLG513 must be powered on and
// its master switch enabled.
//
// Read more: BMS-9, BMS-10, BMS-34, and BRUSA NLG513 datasheet

/**
 * Allocate and initialize a charger
 * @param enable_charger A function that can be called to enable the charger
 * @param disable_charger A function that can be called to disable the charger
 * @param is_charger_connected A function that returns whether the charger is
 *                             is connected
 * @param has_charger_faulted A function that returns whether the charger has a
 * fault
 * @return The created charger, whose ownership is given to the caller
 */
struct Charger *App_Charger_Create(
    void (*enable_charger)(void),
    void (*disable_charger)(void),
    bool (*is_charger_connected)(void),
    bool (*has_charger_faulted)(void));

/**
 * Deallocate the memory used by the given charger
 * @param charger The charger to deallocate
 */
void App_Charger_Destroy(struct Charger *charger);

/**
 * Enable the given charger
 * @param charger The charger to enable
 */
void App_Charger_Enable(struct Charger *charger);

/**
 * Disable the given charger
 * @param charger The charger to disable
 */
void App_Charger_Disable(struct Charger *charger);

/**
 * Check if the given charger is connected
 * @param charger The charger to check
 * @return true if the charger is connected, else false
 */
bool App_Charger_IsConnected(struct Charger *charger);

/**
 * Check if the charger is enabled
 * @param charger The charger to check
 * @return true if the charger is enabled, else false
 */
bool App_Charger_IsEnabled(const struct Charger *charger);

/**
 * Check if the charger has faulted (or is idle if the charger is not enabled)
 * @param charger The charger to check
 * @return true if the charger has faulted/idle, else false
 */
bool App_Charger_HasFaulted(const struct Charger *charger);

/**
 * Increment the timer used to ignore charger faults
 */
void App_Charger_IncrementFaultCounterVal(struct Charger *charger);

/**
 * Get current value of the timer used to ignore charger faults on charge state entry
 * @param charger The charger get timer value from
 */
uint16_t App_Charger_GetFaultCounterVal(const struct Charger *charger);

/**
 * Reset the timer used to ignore charger faults on charge state entry
 * @param charger The charger to reset timer for
 */
void App_Charger_ResetFaultCounterVal(struct Charger *charger);

/**
 * Increment the timer used to allow charger to shut down before exiting charge state
 */
void App_Charger_IncrementExitCounterVal(struct Charger *charger);

/**
 * Get current value of the timer used to allow charger to shut down before exiting charge state
 * @param charger The charger get timer value from
 */
uint16_t App_Charger_GetExitCounterVal(const struct Charger *charger);

/**
 * Reset the timer used to allow charger to shut down before exiting charge state
 * @param charger The charger to reset timer for
 */
void App_Charger_ResetExitCounterVal(struct Charger *charger);