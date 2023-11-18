# XuluMenu

N-Card (and clones) Universal Loader

This is a custom udisk replacement for the N-Card and it's clones. This version requires flashing to the cart via usb.

This will simply launch `BOOT.NDS` homebrew on the root of the NAND. A fall back file browsing UI is added if boot.nds is missing or if B button is held on boot.

### Flashing instructions

1. Ensure your cart had previously been updated to udisk 1.45 and is formatted to be work with udisk 1.45 and the 1.45 matching DLDI driver.
2. Find and connect a DS/DS Lite with GBA slot2 writer device for your N-card/clone to PC.
3. Boot up DS and boot the game while holding [L + R + A + B + DPad Up] at the same time until you see "USB SAFE BLOCK" text on bottom screen.
4. Run the included UPDATE_ALL.cmd file to have USB exe flash the new SRL to the cart.
5. After the process is complete, reboot and see your changes live! File browser will come up if boot.nds is not present or if cart is booted with B button held.

### License

Major portions of this code are licensed under GPL-2.0-or-later (particularly, nds-bootloader and nds_loader_arm9.)
```
	Copyright (C) 2005 - 2010
		Michael "Chishm" Chisholm
		Dave "WinterMute" Murphy

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
```

All other source code files are licensed under the 0BSD license.
