#ifndef SYSINFO_H
#define SYSINFO_H

#include <stdbool.h>
#include "drivers/bsp/keyboard.h"

void SYSINFO_Init(void);
void SYSINFO_Render(void);
void SYSINFO_ProcessKeys(KEY_Code_t Key, bool bKeyPressed, bool bKeyHeld);

#endif // SYSINFO_H
