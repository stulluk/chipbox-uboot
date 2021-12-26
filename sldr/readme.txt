1. First, this simple loader will be loaded by the security boot code build in the CHIP's ROOM.
2. Then this simple loader will load the u-boot from offset 5KBytes in the Flash(NORFLASH/SPIFLASH) to ddr(0x02000000) and started,
the copy size will be 256KBytes.
3. The Display and Gfx Related Code is for Debug only, it can be removed for product application.