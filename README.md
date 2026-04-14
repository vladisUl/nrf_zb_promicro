# Zigbee Temperature, Humidity, and Pressure Sensor on ProMicro nRF52840 + BME280

This project is a Zigbee device based on the popular **ProMicro nRF52840** board and the **BME280** sensor.

The device is intended to operate as a standalone Zigbee sensor and reports the following values to the network:

- temperature
- humidity
- pressure
- supply voltage and estimated battery level

The project is built on **nRF Connect SDK**, **Zephyr**, and **Zigbee R23**.

---

## Hardware Platform

The hardware platform used in this project is the **ProMicro nRF52840** board.

One important feature of this board is its built-in **Bootloader**, which allows firmware to be uploaded without using an SWD debugger.

### Entering Bootloader Mode

To enter bootloader mode:

- short **RST** to **GND** twice within **0.5 seconds**
- then connect the board to the computer over USB
- a storage device named **Nice! Nano** will appear
- copy the firmware file in **`.uf2`** format to this drive

### Board Schematic

<a href="docs/scheme.jpg">
  <img src="docs/scheme.jpg" alt="ProMicro nRF52840 board schematic" width="500">
</a>

---

## Project Features

### BME280 Sensor Support

The project reads data from the **BME280** using standard **Zephyr** drivers.  
The following values are measured:

- temperature
- humidity
- pressure

The sensor is initialized through Devicetree and accessed through the standard Zephyr sensor subsystem.

### Supply Voltage Measurement

Supply voltage is measured using the **ADC**.  
Based on the measured voltage, the firmware calculates an estimated battery level and publishes it through the Zigbee **Power Configuration cluster**.

### Poll Control Cluster

The project includes the **Poll Control cluster**.

It is used to wake the device with a short button press. This is useful when the device needs to be configured after installation and after joining a Zigbee network.

### Low Power Operation

The project is designed for battery-powered use and includes several power-saving mechanisms:

- Zigbee sleepy behavior
- placing the BME280 into suspend mode between measurements
- powering down unused RAM blocks
- infrequent wakeups for data transmission

---

## Development and Debugging

During development, the same board was used, but with the following signals exposed:

- SWD
- SCL
- GND
- VDD

This allowed:

- direct firmware flashing
- debugging
- erasing the bootloader when needed
- working with runtime logs

---

## Build Details for the Built-In Bootloader

Because this board uses a built-in bootloader, the final firmware must be built **not from address `0x0000`**, but from an offset compatible with the bootloader.

In this project, that is handled through **Partition Manager** and a static memory layout.

The following file is used for that purpose:

`/pm_static/pm_static.yml`

It must be copied to the project root under the name:

`pm_static.yml`

If this file is not present, the application will be built from the beginning of flash memory. That is convenient for SWD flashing, but it is not suitable for uploading through the board’s built-in bootloader.

---

## Preparing a Build for Bootloader Upload

To generate a firmware file suitable for uploading through the built-in bootloader, follow these steps.

### 1. Disable Logging

In `prj.conf`, disable logging or reduce it to a minimum.

### 2. Add the Static Memory Layout

Copy the file:

`/pm_static/pm_static.yml`

to the project root as:

`pm_static.yml`

### 3. Create a Separate Build Configuration

Use the following parameters:

**Extra CMake arguments:** `-DGEN_UF2=ON`  
**Build mode:** `no sysbuild`

### 4. Build the Project

After a successful build, the following file will be generated in:

`build/zephyr/`

File:

`zephyr.uf2`

---

## Uploading Firmware to the Board

1. Short **RST** to **GND** twice within **0.5 seconds**
2. Connect the board to the computer over USB
3. Wait until the **Nice! Nano** storage device appears
4. Copy `zephyr.uf2` to that drive

After the copy operation finishes, the board will reboot and start the new firmware.

---

## Typical Usage During Development

### Debug Build

Used for SWD flashing and debugging:

- without `pm_static.yml` in the project root
- the application is built for direct flashing into memory
- convenient for testing and debugging

### Build for the Built-In Bootloader

Used for normal operation without SWD access:

- with `pm_static.yml` in the project root
- with `uf2` generation enabled
- uploaded through the built-in bootloader

---

## Zigbee Functionality

The device uses a Zigbee endpoint with the following main clusters:

- Basic
- Identify
- Power Configuration
- Temperature Measurement
- Relative Humidity Measurement
- Pressure Measurement
- Poll Control

---

## Intended Use

This project can be used as a base for:

- a standalone Zigbee environmental sensor
- a battery-powered Zigbee device
- home automation projects
- custom Zigbee devices based on nRF52840 and Zephyr

---

## Note

This project is specifically designed for the **ProMicro nRF52840** board with its built-in bootloader.  
If you use a different board, a different bootloader, or a different flashing method, the memory layout and build settings may need to be adjusted.