#ifndef VERSION_H
#define VERSION_H

extern const char Version[];
extern const char UART_Version[];
extern const char GitCommit[];
extern const char BuildDate[];

#ifdef ENABLE_CUSTOM_FIRMWARE_MODS
extern const char Edition[];
#endif

#endif // VERSION_H
