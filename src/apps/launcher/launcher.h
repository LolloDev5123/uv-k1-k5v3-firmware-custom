
#ifndef APPS_LAUNCHER_H
#define APPS_LAUNCHER_H

#include <stdbool.h>
#include <stdint.h>

#include "drivers/bsp/keyboard.h"

void LAUNCHER_ProcessKeys(KEY_Code_t Key, bool bKeyPressed, bool bKeyHeld);
void UI_DisplayLauncher(void);

#endif
