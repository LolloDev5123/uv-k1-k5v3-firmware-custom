# deltafw

Custom firmware for the UV-K1 and UV-K5 V3 radios (PY32F071 MCU).

![Build Status](https://github.com/qshosfw/deltafw/actions/workflows/main.yml/badge.svg)

## About

deltafw is a custom firmware by qshosfw, based on the excellent work of:
- [DualTachyon](https://github.com/DualTachyon) - Original UV-K5 open-source firmware
- [Egzumer](https://github.com/egzumer) - Custom firmware modifications
- [F4HWN/Armel](https://github.com/armel) - UV-K1/K5 V3 port
- [Fagci](https://github.com/fagci) - Spectrum analyzer and graphics

> [!WARNING]
> Use this firmware at your own risk. There is no guarantee it will work on your radio.
> It may even brick your device. Always backup your calibration data before flashing.

## Features

- Enhanced UI with AG_Graphics system
- Memories app with T9 text input
- System Info menu (version, commit, build date, battery)
- Settings menu with categorized options
- Spectrum analyzer support

## Building

### Prerequisites
- Docker installed on your system
- Bash environment (Linux, macOS, WSL)

### Build Commands

```bash
# Build Custom edition
./compile-with-docker.sh Custom

# Build Fusion edition
./compile-with-docker.sh Fusion

# Build all editions
./compile-with-docker.sh All
```

### Build Output

Firmware files are generated in `build/<Preset>/`:
- `.bin` - Binary firmware file
- `.hex` - Intel HEX format
- `.elf` - ELF executable

## Flashing

Use [UVTools2](https://armel.github.io/uvtools2/) to flash the firmware:

1. Put your radio in DFU mode
2. Connect via USB
3. Select the `.bin` file
4. Click "Flash Firmware"

## System Info

The firmware includes build information:
- **Version**: Firmware version string
- **Commit**: Git commit hash at build time
- **Built**: Build date
- **Battery**: Current battery status
- **License**: GPL v3

Access via: Menu → Info

## License

This project is licensed under the **GNU General Public License v3.0**.

Based on prior work licensed under Apache 2.0 by DualTachyon, Egzumer, F4HWN, and others.
See [LICENSE](LICENSE) for full details and attribution.

## Credits

- [DualTachyon](https://github.com/DualTachyon) - Original firmware foundation
- [Egzumer](https://github.com/egzumer) - Custom firmware base
- [F4HWN/Armel](https://github.com/armel) - PY32F071 port
- [Muzkr](https://github.com/muzkr) - PY32F071 port collaboration
- [Fagci](https://github.com/fagci) - Graphics and spectrum analyzer
