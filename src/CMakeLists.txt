
add_library(App INTERFACE)
target_include_directories(App INTERFACE .)
target_link_libraries(App INTERFACE PY32F071_Driver)
target_compile_definitions(App INTERFACE PRINTF_INCLUDE_CONFIG_H)

# ----------------------------------------------------------------------------
# Subdirectories
# ----------------------------------------------------------------------------
add_subdirectory(core)
add_subdirectory(drivers)
add_subdirectory(middlewares)



# ----------------------------------------------------------------------------
# Source Files
# ----------------------------------------------------------------------------
target_sources(App INTERFACE
    # Main Entry
    main.c
    
    # Root source files (formerly App/*)
    board.c
    misc.c
    scheduler.c
    "version.c"
    audio.c
    radio.c
    frequencies.c
    dcs.c
    settings.c
    functions.c
    bitmaps.c
    font.c

    # Features (formerly App/app)
    features/action.c
    features/app.c
    features/chFrScanner.c
    features/common.c
    features/dtmf.c
    features/generic.c
    features/main_screen.c
    features/menu.c
    features/scanner.c

    # Drivers BSP (formerly App/driver)
    drivers/bsp/backlight.c
    drivers/bsp/bk4829.c
    drivers/bsp/py25q16.c
    drivers/bsp/gpio.c
    drivers/bsp/i2c.c
    drivers/bsp/keyboard.c
    drivers/bsp/st7565.c
    drivers/bsp/system.c
    drivers/bsp/systick.c

    # Helpers
    helper/battery.c
    helper/boot.c

    # UI
    ui/battery.c
    ui/helper.c
    ui/inputbox.c
    ui/main.c
    ui/menu.c
    ui/scanner.c
    ui/status.c
    ui/ui.c
    ui/welcome.c

    # Third Party
    external/printf/printf.c
)

# ----------------------------------------------------------------------------
# Feature Macros
# ----------------------------------------------------------------------------
macro(enable_feature feature)
    if(${feature})
        target_compile_definitions(App INTERFACE ${feature})
        if(${ARGC} GREATER 1)
            target_sources(App INTERFACE ${ARGN})
        endif()
    endif()
endmacro()

# ----------------------------------------------------------------------------
# Configuration & Definitions
# ----------------------------------------------------------------------------
if(ENABLE_FEAT_F4HWN)
    target_compile_definitions(App INTERFACE
        "AUTHOR_STRING_1=\"${AUTHOR_STRING_1}\""
        "AUTHOR_STRING_2=\"${AUTHOR_STRING_2}\""
        "VERSION_STRING_1=\"${VERSION_STRING_1}\""
        "VERSION_STRING_2=\"${VERSION_STRING_2}\""
        "EDITION_STRING=\"${EDITION_STRING}\""
        ALERT_TOT=10
    )
endif()

target_compile_definitions(App INTERFACE
    "AUTHOR_STRING=\"${AUTHOR_STRING}\""
    "VERSION_STRING=\"${VERSION_STRING}\""
)

if(NOT SQL_TONE)
    set(SQL_TONE 550)
endif()
target_compile_definitions(App INTERFACE SQL_TONE=${SQL_TONE})


# ----------------------------------------------------------------------------
# Communication Interfaces (UART / USB / AirCopy)
# ----------------------------------------------------------------------------
if(ENABLE_AIRCOPY OR ENABLE_UART OR ENABLE_USB)
    target_sources(App INTERFACE
        drivers/bsp/crc.c
        drivers/bsp/eeprom_compat.c
    )
endif()

enable_feature(ENABLE_UART
    drivers/bsp/uart.c
    features/uart.c
    # drivers/bsp/aes.c
)

if(ENABLE_USB)
    target_link_libraries(App INTERFACE CherryUSB)
    target_compile_definitions(App INTERFACE ENABLE_USB)
    target_include_directories(App INTERFACE usb)
    target_sources(App INTERFACE
        drivers/bsp/vcp.c
        usb/usbd_cdc_if.c
    )
endif()

if(ENABLE_UART OR ENABLE_USB)
    target_sources(App INTERFACE
        features/uart.c
    )
endif()


# ----------------------------------------------------------------------------
# Stock Quansheng Features
# ----------------------------------------------------------------------------
enable_feature(ENABLE_FMRADIO           drivers/bsp/bk1080.c features/fm.c ui/fmradio.c)
enable_feature(ENABLE_AIRCOPY           features/aircopy.c ui/aircopy.c)
enable_feature(ENABLE_VOICE             drivers/bsp/voice.c)
enable_feature(ENABLE_PWRON_PASSWORD    ui/lock.c)
enable_feature(ENABLE_FLASHLIGHT        features/flashlight.c)

enable_feature(ENABLE_NOAA)
enable_feature(ENABLE_VOX)
enable_feature(ENABLE_ALARM)
enable_feature(ENABLE_TX1750)
enable_feature(ENABLE_DTMF_CALLING)


# ----------------------------------------------------------------------------
# Custom Mods
# ----------------------------------------------------------------------------
enable_feature(ENABLE_SPECTRUM          features/spectrum.c)
enable_feature(ENABLE_AM_FIX            am_fix.c)

enable_feature(ENABLE_BIG_FREQ)
enable_feature(ENABLE_SMALL_BOLD)
enable_feature(ENABLE_CUSTOM_MENU_LAYOUT)
enable_feature(ENABLE_KEEP_MEM_NAME)
enable_feature(ENABLE_WIDE_RX)
enable_feature(ENABLE_TX_WHEN_AM)
enable_feature(ENABLE_F_CAL_MENU)
enable_feature(ENABLE_CTCSS_TAIL_PHASE_SHIFT)
enable_feature(ENABLE_BOOT_BEEPS)
enable_feature(ENABLE_SHOW_CHARGE_LEVEL)
enable_feature(ENABLE_REVERSE_BAT_SYMBOL)
enable_feature(ENABLE_NO_CODE_SCAN_TIMEOUT)
enable_feature(ENABLE_SQUELCH_MORE_SENSITIVE)
enable_feature(ENABLE_FASTER_CHANNEL_SCAN)
enable_feature(ENABLE_RSSI_BAR)
enable_feature(ENABLE_AUDIO_BAR)
enable_feature(ENABLE_COPY_CHAN_TO_VFO)
enable_feature(ENABLE_REDUCE_LOW_MID_TX_POWER)
enable_feature(ENABLE_BYP_RAW_DEMODULATORS)
enable_feature(ENABLE_BLMIN_TMP_OFF)
enable_feature(ENABLE_SCAN_RANGES)
enable_feature(ENABLE_NAVIG_LEFT_RIGHT)


# ----------------------------------------------------------------------------
# Contrib Mods
# ----------------------------------------------------------------------------
enable_feature(ENABLE_REGA              features/rega.c) # @markusb
enable_feature(ENABLE_EXTRA_UART_CMD)               # @reppad


# ----------------------------------------------------------------------------
# F4HWN Mods
# ----------------------------------------------------------------------------
enable_feature(ENABLE_FEAT_F4HWN)
enable_feature(ENABLE_FEAT_F4HWN_GAME           features/breakout.c)
enable_feature(ENABLE_FEAT_F4HWN_SCREENSHOT     screenshot.c)

enable_feature(ENABLE_FEAT_F4HWN_SPECTRUM)
enable_feature(ENABLE_FEAT_F4HWN_RX_TX_TIMER)
enable_feature(ENABLE_FEAT_F4HWN_CHARGING_C)
enable_feature(ENABLE_FEAT_F4HWN_SLEEP)
enable_feature(ENABLE_FEAT_F4HWN_RESUME_STATE)
enable_feature(ENABLE_FEAT_F4HWN_NARROWER)
enable_feature(ENABLE_FEAT_F4HWN_INV)
enable_feature(ENABLE_FEAT_F4HWN_CTR)
enable_feature(ENABLE_FEAT_F4HWN_RESCUE_OPS)
enable_feature(ENABLE_FEAT_F4HWN_VOL)
enable_feature(ENABLE_FEAT_F4HWN_RESET_CHANNEL)
enable_feature(ENABLE_FEAT_F4HWN_PMR)
enable_feature(ENABLE_FEAT_F4HWN_GMRS_FRS_MURS)
enable_feature(ENABLE_FEAT_F4HWN_CA)
enable_feature(ENABLE_FEAT_F4HWN_DEBUG)


# ----------------------------------------------------------------------------
# Debugging
# ----------------------------------------------------------------------------
enable_feature(ENABLE_AM_FIX_SHOW_DATA)
enable_feature(ENABLE_AGC_SHOW_DATA)
enable_feature(ENABLE_UART_RW_BK_REGS)
enable_feature(ENABLE_SWD)
