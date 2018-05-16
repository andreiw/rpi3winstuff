# `dwusb` UCX controller driver

The code in this repository implements an UCX-based controller driver for Windows 10 on ARM(64) for the **Synopsys® DesignWare® USB 2.0 HS OTG Controller**, specifically, the variant used in the **Raspberry Pi 3**.

The driver is fairly unfinished and unreliable, some inherently-wrong decisions were made during development, mainly with regards to channel allocation and request queue handling.

It's been tested with a single external USB 2.0 hub connected to one of the on-board RPi3 LAN controller hub ports, and connecting a generic simple (full-speed?) HP Modular Keyboard device to a port on this hub. Any more devices would run into reliability issues.

This driver is provided as-is without support, both to serve as a somewhat simplified example of using the UCX APIs, and perhaps as a base for people to build upon for device enablement on the RPi3.

## Attribution

Large portions of code were based on the USBXHCI driver sample included in the WDK, also the GPL'd [RaspberryPiPkg](https://github.com/andreiw/RaspberryPiPkg) USB driver, and the implementation in Das U-Boot the former was based upon. Therefore, the modified driver shall be considered as GPL-licensed as well, in the best case.