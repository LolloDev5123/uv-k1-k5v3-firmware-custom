
#include <string.h>
#include "apps/launcher/launcher.h"
#include "apps/spectrum/spectrum.h"
#include "apps/fm/fm.h"
#include "apps/scanner/scanner.h"
#include "ui/ui.h"
#include "ui/helper.h"
#include "ui/menu.h"
#include "drivers/bsp/st7565.h"
#include "features/action.h"
#include "features/app.h"
#include "audio.h"
#include "apps/settings/settings.h"
#include "apps/aircopy/aircopy.h"
#include "apps/boot/boot.h"

#ifdef ENABLE_APP_BREAKOUT_GAME
#include "features/breakout.h"
#endif

#include "apps/scanner/scanner_ui.h"
#include "apps/fm/fm_ui.h"
#include "apps/aircopy/aircopy_ui.h"
#include "apps/boot/welcome.h"

// Reference implementation: https://github.com/qshosfw/matoz/blob/main/ui/appmenu.c
// Renamed to LAUNCHER_... for consistency

uint16_t LAUNCHER_cursor = 0;

typedef void (*LauncherAction_t)(void);

typedef struct {
    const char* name;
    LauncherAction_t action;
} LauncherItem_t;

// Actions
static void Action_Settings(void) {
    gRequestDisplayScreen = DISPLAY_MENU;
}

static void Action_EditScanlist(void) {
     gEeprom.SCAN_LIST_DEFAULT = (gEeprom.SCAN_LIST_DEFAULT + 1) % 6;
    #ifdef ENABLE_BOOT_RESUME_STATE
        SETTINGS_WriteCurrentState();
    #endif
    gRequestDisplayScreen = DISPLAY_MAIN;
}

static void Action_Spectrum(void) {
    #ifdef ENABLE_SPECTRUM_EXTENSIONS
    APP_RunSpectrum();
    #endif
    gRequestDisplayScreen = DISPLAY_MAIN;
}

static void Action_FM(void) {
    #ifdef ENABLE_FMRADIO
    if (!gFmRadioMode) {
        ACTION_FM();
    } else {
        gRequestDisplayScreen = DISPLAY_FM;
    }
    #endif
}

static void Action_Scanner(void) {
     gBackup_CROSS_BAND_RX_TX = gEeprom.CROSS_BAND_RX_TX;
     gEeprom.CROSS_BAND_RX_TX = CROSS_BAND_OFF;
     gUpdateStatus = true;
     SCANNER_Start(false);
     gRequestDisplayScreen = DISPLAY_SCANNER;
}

static void Action_AirCopy(void) {
    #ifdef ENABLE_AIRCOPY
    gRequestDisplayScreen = DISPLAY_AIRCOPY;
    #endif
}

static void Action_Info(void) {
    UI_DisplayWelcome();
}

// Dynamic List Construction
static const LauncherItem_t* CurrentList = NULL;
static uint8_t CurrentListCount = 0;

static LauncherItem_t Items[] = {
    {"Settings", Action_Settings},
    {"Edit Scanlist", Action_EditScanlist},
    {"Spectrum", Action_Spectrum},
    #ifdef ENABLE_FMRADIO
    {"FM Radio", Action_FM},
    #endif
    {"Scanner", Action_Scanner},
    #ifdef ENABLE_AIRCOPY
    {"Air Copy", Action_AirCopy},
    #endif
    {"Info", Action_Info}
};

void LAUNCHER_Init() {
    CurrentList = Items;
    CurrentListCount = sizeof(Items) / sizeof(LauncherItem_t);
}

void LAUNCHER_move(bool down) {
    if (CurrentList == NULL) LAUNCHER_Init();
    
    if (down) {
        if (LAUNCHER_cursor == CurrentListCount - 1) {
            LAUNCHER_cursor = 0;
        } else {
            LAUNCHER_cursor++;
        }
    } else {
        if (LAUNCHER_cursor == 0) {
            LAUNCHER_cursor = CurrentListCount - 1;
        } else {
            LAUNCHER_cursor--;
        }
    }
    gUpdateDisplay = true;
}

void LAUNCHER_close(void) {
    gRequestDisplayScreen = DISPLAY_MAIN;
}

void UI_DisplayLauncher(void) {
    if (CurrentList == NULL) LAUNCHER_Init();

    char String[32];
    
    UI_DisplayClear();

    // NOTE: Status bar is handled globally by UI_DisplayStatus if called?
    // User requested to remove redundant status bar code.
    // However, if we don't call anything to draw status bar, will it be drawn?
    // UI_DisplayMain calls UI_DisplayStatus. UI_DisplayLauncher typically just draws screen.
    // If the main loop clears screen then calls this, we might need to verify if status bar persists.
    // But assuming the global status bar system works as requested:
    // We should ensure we don't draw ON TOP of it.
    
    const uint8_t perScreen = 7; // Fits 7 items in remaining 7 pages (Screen Pages 1-7)
    
    // Clamp offset calculation
    int offset_val = LAUNCHER_cursor - 2;
    if (offset_val < 0) offset_val = 0;
    if (offset_val > CurrentListCount - perScreen) offset_val = CurrentListCount - perScreen;
    if (offset_val < 0) offset_val = 0; 
    
    const uint8_t offset = (uint8_t)offset_val;

    for (uint8_t i = 0; i < perScreen; ++i) {
        uint8_t itemIndex = i + offset;
        if (itemIndex >= CurrentListCount) break;

        // Draw at y=0 relative to content buffer (gFrameBuffer[0]).
        // BlitFullScreen shifts this to Screen Page 1 (y=8), immediately after status bar.
        uint8_t y = i * 8; 
        uint8_t *pLine = gFrameBuffer[i]; // Buffer Page i maps to Screen Page i+1

        bool isCurrent = LAUNCHER_cursor == itemIndex;

        if (isCurrent) {
            memset(pLine, 127, 128); 
        }

        sprintf(String, "%s", CurrentList[itemIndex].name);
        UI_PrintStringSmallest(String, 1, y, false, !isCurrent);
    }

    ST7565_BlitFullScreen();
    gUpdateDisplay = true;
}

void LAUNCHER_ProcessKeys(KEY_Code_t Key, bool bKeyPressed, bool bKeyHeld) {
    if (!bKeyPressed) return;
    if (CurrentList == NULL) LAUNCHER_Init();

    switch (Key) {
        case KEY_EXIT:
            LAUNCHER_close();
            break;
        case KEY_UP:
            LAUNCHER_move(false);
            break;
        case KEY_DOWN:
            LAUNCHER_move(true);
            break;
        case KEY_PTT:
        case KEY_MENU:
            if (CurrentList[LAUNCHER_cursor].action) {
                CurrentList[LAUNCHER_cursor].action();
            }
            break;
        default:
            break;
    }
}
