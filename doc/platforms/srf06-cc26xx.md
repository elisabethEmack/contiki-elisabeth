# cc26x0-cc13x0: TI cc26x0 and cc13x0 platforms

## Important Note

**Before you proceed with reading this guide, make sure you have read and understood this section.**

This platform/CPU is in the process of being renamed:

* Current name `srf06-cc26xx`: Used in releases v4.0, v4.1 and v4.2. Used in the pre-release `release-4.2` branch. Used in branch `develop` before 24 October 2018.
  * Platform code under `arch/platform/srf06-cc26xx`
  * CPU code under `arch/cpu/cc26xx-cc13xx`
  * Platform-specific examples under `examples/platform-specific/cc26xx`
* New name `cc26x0-cc13x0`: Used in branch `develop` as of 24 October 2018.
  * Platform code under `arch/platform/cc26x0-cc13x0`
  * CPU code under `arch/cpu/cc26x0-cc13x0`
  * Platform-specific examples under `examples/platform-specific/cc26x0-cc13x0`

While reading the guide below, you may have to use a different platform name depending on the version of Contiki-NG that you are using. The name `srf06-cc26xx` will be obsoleted in Release v4.3.

## Getting Started

This guide's aim is to help you start using Contiki for TI's CC13x0/CC26x0. The
platform supports multiple boards:

* SmartRF 06 Evaluation Board with a CC26x0 or CC13x0 Evaluation Module
  (relevant files and drivers are under `srf06/`)
* CC2650 SensorTag 2.0 (relevant drivers under `sensortag/cc2650`)
* CC1350 SensorTag 2.0 (relevant drivers under `sensortag/cc1350`)
* CC2650 LaunchPad (relevant drivers under `launchpad/cc2650`)
* CC1310 LaunchPad (relevant drivers under `launchpad/cc1310`)
* CC1350 LaunchPad (relevant drivers under `launchpad/cc1350`)

The CPU code, common for all boards, can be found under `$(CONTIKI)/cpu/cc26x0-cc13x0` (or `$(CONTIKI)/cpu/cc26xx-cc13xx` - see [important note at the start of this guide][#important-note].

The port was developed and tested with CC2650s, but the intention is for it to
work with the CC2630 as well. Thus, bug reports are welcome for both chips.
Bear in mind that the CC2630 does not have BLE capability. Similar rules apply
in terms of CC13x0 chips.

This port is only meant to work with 7x7mm chips. Contributions to add support for other chip types is always welcome.

This guide assumes that you have basic understanding of how to use the command
line and perform basic admin tasks on UNIX family OSs.

## Port Features

The platform has the following key features:

* Deep Sleep support with RAM retention for ultra-low energy consumption.
* Support for CC26x0 RF in IEEE as well as BLE mode (BLE/IPv6 support with BLEach is experimental).
* Support for CC13x0 prop mode: IEEE 802.15.4g-compliant sub GHz operation.

In terms of hardware support, the following drivers have been implemented:

* SmartRF06 EB peripherals
  * LEDs
  * Buttons
  * UART connectivity over the XDS100v3 backchannel
* SensorTag 2.0
  * LEDs
  * Buttons (One of the buttons can be used as a shutdown button)
  * Reed relay
  * Motion Processing Unit (MPU9250 - Accelerometer, Gyro)
  * BMP280 sensor
  * TMP007 sensor
  * HDC1000 sensor
  * OPT3001 sensor
  * Buzzer
  * External SPI flash
* LaunchPads
  * LEDs
  * Buttons
  * External SPI flash

## Requirements

To use the port you need:

* TI's CC26xxware sources. The correct version will be installed automatically
  as a submodule when you clone Contiki.
* TI's CC13xxware sources. The correct version will be installed automatically
  as a submodule when you clone Contiki.
* Contiki can automatically upload firmware to the nodes over serial with the
  included [cc2538-bsl script](https://github.com/JelmerT/cc2538-bsl).
  Note that uploading over serial doesn't work for the Sensortag, you can use
  TI's SmartRF Flash Programmer in this case.
* A toolchain to build firmware: If you use the docker image or the vagrant image, this will be pre-installed for you. Otherwise, depending on your system, please follow the respective installation instructions ([native Linux](/doc/getting-started/Toolchain-installation-on-Linux) / [native mac OS](/doc/getting-started/Toolchain-installation-on-macOS)).
* srecord (http://srecord.sourceforge.net/)
* You may also need other drivers so that the SmartRF can communicate with your
operating system and so that you can use the chip's UART for I/O. Please read
the section ["Drivers" in the CC2538DK readme](https://github.com/contiki-os/contiki/tree/master/platform/cc2538dk#drivers).

For additional help on how to set your system up, you may also find the guides below useful:
* [Native toolchain installation (Linux)](/doc/getting-started/Toolchain-installation-on-Linux)
* [Native toolchain installation (mac OS)](/doc/getting-started/Toolchain-installation-on-macOS)

## Examples

The port comes with three examples:
- A very basic example which demonstrates how to read sensors and how to use board peripherals. It also demonstrates how to send out BLE advertisements.
- A more advanced one (web demo) which includes a CoAP server, an MQTT client which connects and publishes to the IBM quickstart service, a net-based UART and lastly a web server that can be used to configure the rest of the example.
- An example demonstrating a very sleepy node.

More details about those three examples can be found in their respective READMEs.

### Build your First Example

It is recommended to start with the `cc26xx-demo` example under `examples/platform-specific/cc26xx/`. This is a very simple example which will help you get familiar with the hardware and the environment.

To build for this platform, you need to specify the correct value of the `TARGET` make variable. The value changes depending on your version of Contiki-NG - see [important note at the start of this guide][important-note]. Here we assume that you are using the current naming (`srf06/cc26xx`).

Strictly speaking, to build it you need to run `make TARGET=srf06-cc26xx BOARD=srf06/cc26xx`. However, the example directories contain a `Makefile.target` which is automatically included and specifies the correct `TARGET=` argument. The `BOARD=` environment variable defaults to `srf06/cc26xx` (which is the SmartRF06 EB + CC26X0EM). Thus, for examples under the `cc26xx` directory, and when using the SmartRF06 EB, you can simply run `make`.

Other options for the `BOARD` make variable are:

* Srf06+CC26x0EM: Set `BOARD=srf06/cc26xx`
* Srf06+CC13x0EM: Set `BOARD=srf06/cc13xx`
* CC2650 tag: Set `BOARD=sensortag/cc2650`
* CC1350 tag: Set `BOARD=sensortag/cc1350`
* CC2650 Launchpad: Set `BOARD=launchpad/cc2650`
* CC1310 Launchpad: Set `BOARD=launchpad/cc1310`
* CC1350 Launchpad: Set `BOARD=launchpad/cc1350`

If the `BOARD` variable is unspecified, an image for the Srf06 CC26x0EM will be built.

If you want to switch between building for one platform to the other, make certain to `make clean` before building for the new one, or you will get linker
errors.

For the `cc26xx-demo`, the included readme describes in detail what the example does.

To generate an assembly listing of the compiled firmware, run `make cc26xx-demo.lst`. This may be useful for debugging or optimizing your application code. To intersperse the C source code within the assembly listing, you must instruct the compiler to include debugging information by adding `DEBUG = 1` to the project Makefile and rebuild by running `make clean cc26xx-demo.lst`.

## How to Program your Device

In general terms, there are two possible ways to program your device:
* Over JTAG. This is always possible.
* Using the serial ROM bootloader. Some conditions need to be met before this is possible.

The build process will output firmware in multiple formats, a `.bin` file, a `.elf` file and an Intel HEX file (`.hex`). The correct file to upload to your device depends on the tool you use to do the programming. More information in the corresponding subsection.

### Over JTAG
This is always possible and you have two options in terms of what software to use:
* TI's [SmartRF Flash Programmer][smart-rf-flashprog] (FLASH-PROGRAMMER-2). Windows only. You can use the `.bin` as well as the `.hex` file.
* TI's [UniFlash](http://www.ti.com/tool/UNIFLASH). Linux, OS X and Windows. Use the `.bin`. It may also be possible to use the `.elf` or `.hex`, but at the time of writing this page this is not tested.

### Using the ROM bootloader
Under some circumstances, the device can also be programmed through its ROM bootloader, using the `cc2538-bsl` script under tools. This is commonly done using the `.upload` make target (e.g. `make cc26xx-demo.upload`), which automatically invokes `cc2538-bsl` with the correct arguments.

#### Device enumeration over USB in Linux/OS X
Firstly, you need to make sure your device enumerates properly. If you are using a SmartRF06 Eval Board, make sure the "Enable UART" jumper is set.

On Linux, the device will show up under `/dev` as `/dev/ttyUSB*` If not, on Linux, look for the device with `lsusb`:
```bash
$ lsusb
...
Bus 001 Device 002: ID 0403:a6d1 Future Technology Devices International, Ltd
```

If kernel version is above 3.12 use:
```bash
modprobe ftdi_sio
echo 0403 a6d1 > /sys/bus/usb-serial/drivers/ftdi_sio/new_id
```

On OS X, the device will show up as `/dev/tty.usbmodem<sequence of letters and numbers>` (e.g. `tty.usbmodemL1000191`)

#### Conditions to use the ROM bootloader.

On Linux and OS X, you can program your device via the chip's serial ROM boot loader. In order for this to work, the following conditions need to be met:

* The board supports the bootloader. This is the case for SmartRF06EB with CC26x0/CC13x0 EMs and it is also the case for LaunchPads. Note that uploading over serial does not (and will not) work for the Sensortag.
* The ROM bootloader is unlocked.

> **You will _not_ be able to use the ROM bootloader with a new out-of-the-box device programmed with factory firmware.**

For newly-bought hardware, you need to use the JTAG to first erase the device using either SmartRF Flash Programmer or UniFlash (see [the previous subsection](#over-jtag)).

After reset, the device will either execute the firmware on its flash, or will enter bootloader mode so it can be programmed. This is dictated by the following logic:

* If the flash is empty, the device will enter bootloader mode and can be programmed using the ROM bootloader
* If the flash contains a firmware image:
  * If the firmware is configured to lock the bootloader (which is the case e.g. for factory images), the device will execute the firmware and will _not_ enter ROM bootloader mode
  * If the firmware is configured to unlock the bootloader, and if a specific (configurable) DIO pin is high/low (also configurable), the device will enter bootloader mode and can be programmed using the ROM bootloader

To enable/unlock the bootloader backdoor in your image, define `ROM_BOOTLOADER_ENABLE` to 1 in `arch/cpu/cc26xx-cc13xx/cc13xx-cc26xx-conf.h`. The correct DIO and high/low state required to enter bootloader mode will be automatically configured for you, depending on your device.

With the above in mind, force your device into bootloader mode by keeping the correct user button pressed, and then pressing and releasing the reset button. On the SmartRF06, you enter the boot loader by resetting the EM (EM RESET button) while holding the `select` button. For the LaunchPad, you enter the bootloader by resetting the chip while holding `BTN_1`. It is possible to change the pin and its corresponding level (High/Low) that will trigger bootloader mode by changing `SET_CCFG_BL_CONFIG_BL_LEVEL` and `SET_CCFG_BL_CONFIG_BL_PIN_NUMBER` in `board.h`.

Remember that the device will always enter bootloader mode if you erase its flash contents. 

If your device has correctly entered bootloader mode, you can now program it using the `.upload` target:

```
make cc26xx-demo.upload
```

The serial uploader script will automatically pick the first available serial port. If this is not the port where your node is connected, you can force the script to use a specific port by defining the `PORT` argument eg:

```
make PORT=/dev/tty.usbmodemL1000191 cc26xx-demo.upload
```
If you get the error below, the most probable cause is that you have specified the wrong `PORT`, or the device has not entered bootloader mode:

```
Connecting to target...
ERROR: Timeout waiting for ACK/NACK after 'Synch (0x55 0x55)'
make: *** [cc26xx-demo.upload] Error 1
```

Some common causes why the device has not entered bootloader mode:

* The device's flash contains an image that was built with `ROM_BOOTLOADER_ENABLE` defined as 0. In this case, you will need to use SmartRF Flash Programmer of UniFlash to erase flash.
* You programmed the device with firmware meant for a different device (e.g. you programmed a launchpad with an image built for a sensortag). In this case, you will also need to use SmartRF Flash Programmer of UniFlash to erase flash.
* You reset the device without keeping the correct button pressed. Simply try again.

For more information on the serial bootloader, see its README under the `tools/cc2538-bsl` directory.

## Building deployment / production images
For deployment/production images, it is _strongly_ recommended to:

* Disable the ROM bootloader by defining `ROM_BOOTLOADER_ENABLE` as 0. In doing so, it is impossible to enter bootloader mode unless one first erases the device's flash.
* Disable the JTAG interface, by defining `CCFG_CONF_JTAG_INTERFACE_DISABLE` as 1. In doing so, the only JTAG operation available will be a device forced mass erase (using SmartRF Flash Programmer or UniFlash).

Both macros have default values set in `arch/cpu/cc26xx-cc13xx/cc13xx-cc26xx-conf.h`. You can change this file, or you can override the defaults in `contiki-conf.h` or `project-conf.h`.

If you do not follow these guidelines, an individual with physical access to your device may be able to read out its flash contents. This could give them access to your IP and it could also lead to a compromise of e.g. keys used for encryption.

## CC13x0/CC26x0 Border Router over UART

The platform code can be used as a border router (SLIP over UART) by using the example under `examples/rpl-border-router`. The example is expected to work off-the-shelf, without any modifications required.

## CC13x0/CC26x0 slip-radio with 6lbr

The platform can also operate as a slip-radio over UART, to be used with [6lbr].

Similar to the border router, the example is expected to work off-the-shelf, without any modifications required.

## Filename conflicts between Contiki and CC26xxware

There is a file called `timer.c` both in Contiki-NG as well as in CC26xxware. The
way things are configured now, we don't use the latter. However, if you need to
start using it at some point, you will need to rename it:

From `cpu/cc26xx/lib/cc26xxware/driverlib/timer.c` to `driverlib-timer.c`

## Some Sensortag-specific notes

### UART usage (with or without the Debugger Devpack)

There are two ways to get debugging (printf etc) output from the Sensortag.

* Purchase a Debugger Devpack and set `BOARD_CONF_DEBUGGER_DEVPACK` to 1 in
`contiki-conf.h` or `project-conf.h`. This will work off the shelf for revision
1.2.0 of the debugger devpack.
* If you have an older (rev 1.0.0) devpack, you will need to do the above and
then to modify `board.h` in order to cross the RX and TX DIO mappings. (TX to
`IOID_28`, RX to `IOID_29`).
* If you don't have/want a debugger devpack, you can use a SmartRF and modify
the jumper configuration on P408 as discussed in
[this thread](https://e2e.ti.com/support/wireless_connectivity/f/158/p/411992/1483824#1483824)
on E2E. For this to work, you need to set `BOARD_CONF_DEBUGGER_DEVPACK` to 0.

### External flash on the sensortag
If you are trying to use the cc26xx-web-demo on a sensortag and you are getting:
```
CC26XX Web Demo Process
CC26XX Web Server
CC26XX CoAP Server
CC26XX Net UART Process
Could not open flash to load config
```

Quote from: http://processors.wiki.ti.com/index.php/Debug_DevPack_User_Guide
> The CC2650 on the SensorTag uses the same I/O pin for SPI SCLK signal and JTAG TDI signal. The current firmware version of the XDS110 does not support the 2-pin c-JTAG standard, so it needs to use TDI and TDO in addition to TCK and TMS (full 4-pin JTAG). Due to this shared resource, it is not possible to access the external serial flash while debugging the SensorTag with the Debug DevPack.

## IEEE vs Sub-GHz operation

The platform supports both modes of operation, provided the chip also has the
respective capability. If you specify nothing, the platform will default to
Sub-GHz mode for CC13x0 devices, IEEE mode otherwise. To force IEEE mode, you
need to add this line to your `project-conf.h`.

    #define CC13XX_CONF_PROP_MODE 0

## Low Power Operation

The platform takes advantage of the CC26x0's power saving features. In a
nutshell, here is how things work:

* When the RF is TXing, the CPU will enter sleep mode and will resume after TX
  has completed.
* When there are no events in the Contiki-NG event queue, the chip will enter
  'some' low power mode (more below).

We do not use pre-defined power profiles (e.g. as mentioned in the TRM or as
we do for the CC2538 with LPM1, LPM2 etc). Each time we enter low power
operation, we either put the CM3 to sleep or to deep sleep. The latter case is
highly configurable: the LPM engine allows other code modules to register
themselves for notifications and to configure low power operation. With these
facilities, a module can e.g. prohibit deep sleep altogether, or it can request
that a power domain be kept powered. The LPM engine will turn off as many
CC26x0 components as it can while satisfying all restrictions set by registered
modules.

To determine which power mode to use, the following logic is followed:

* The deepest available low power mode can be hard-coded by using
  the `LPM_MODE_MAX_SUPPORTED` macro in the LPM driver (`lpm.[ch]`). Thus, it
  is possible to prohibit deep sleep altogether.
* Code modules which are affected by low power operation can 'register'
  themselves with the LPM driver.
* If the projected low-power duration is lower than `STANDBY_MIN_DURATION`,
  the chip will simply sleep.
* If the projected low power duration is sufficiently long, the LPM will visit
  all registered modules to query the maximum allowed power mode (maximum means
  sleep vs deep sleep in this context). It will then drop to this power mode.
  This is where a code module can forbid deep sleep if required.
* All registered modules will be notified when the chip is about to enter
  deep sleep, as well as after wake-up.

When the chip does enter deep sleep:

* The RF Core, VIMS, SYSBUS and CPU power domains are always turned off. Due to
  the way the RF driver works, the RFCORE PD should be off already.
* Peripheral clocks stop
* The Serial and Peripheral power domains are turned off, unless an LPM module
  requests them to stay operational. For example, the net-uart demo keeps the
  serial power domain powered on and the UART clocked under sleep and deep
  sleep in order to retain UART RX functionality.
* If both SERIAL and PERIPH PDs are turned off, we also switch power source to
  the uLDO for ultra low leakage under deep sleep.

The chip will come out of low power mode by one of the following events:

* Button press or, in the case of the SensorTag, a reed relay trigger.
* Software clock tick (timer) required to service a scheduled event timer.
* Rtimer (AON RTC) triggers, as part of a MAC layer's sleep/wake-up cycles.

[smart-rf-flashprog]: http://www.ti.com/tool/flash-programmer "SmartRF Flash Programmer"
[6lbr]: http://cetic.github.io/6lbr/
[important-note]: #important-note "Important note at the start of this page"
