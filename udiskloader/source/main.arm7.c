/*
 main.arm7.c

 By Michael Chisholm (Chishm)

 All resetMemory and startBinary functions are based
 on the MultiNDS loader by Darkain.
 Original source available at:
 http://cvs.sourceforge.net/viewcvs.py/ndslib/ndslib/examples/loader/boot/main.cpp

 License:
    NitroHax -- Cheat tool for the Nintendo DS
    Copyright (C) 2008  Michael "Chishm" Chisholm

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ARM7
# define ARM7
#endif
#include <nds/ndstypes.h>
#include <nds/memory.h>
#include <nds/system.h>
#include <nds/interrupts.h>
#include <nds/timers.h>
#include <nds/dma.h>
#include <nds/arm7/audio.h>
#include <nds/ipc.h>
#include <nds/card.h>

#ifndef NULL
#define NULL 0
#endif

#include "common.h"
#include "tonccpy.h"


#define TMP_DATA 0x02200000

extern void arm7_clearmem (void* loc, size_t len);
extern void arm7_reset (void);

#define NDS_HEADER 0x02FFFE00

#define FW_READ	 0x03
tNDSHeader* ndsHeader;

void arm7_readFirmware (uint32 address, uint8 * buffer, uint32 size) {
  uint32 index;

  // Read command
  while (REG_SPICNT & SPI_BUSY);
  REG_SPICNT = SPI_ENABLE | SPI_CONTINUOUS | SPI_DEVICE_NVRAM;
  REG_SPIDATA = FW_READ;
  while (REG_SPICNT & SPI_BUSY);

  // Set the address
  REG_SPIDATA =  (address>>16) & 0xFF;
  while (REG_SPICNT & SPI_BUSY);
  REG_SPIDATA =  (address>>8) & 0xFF;
  while (REG_SPICNT & SPI_BUSY);
  REG_SPIDATA =  (address) & 0xFF;
  while (REG_SPICNT & SPI_BUSY);

  for (index = 0; index < size; index++) {
    REG_SPIDATA = 0;
    while (REG_SPICNT & SPI_BUSY);
    buffer[index] = REG_SPIDATA & 0xFF;
  }
  REG_SPICNT = 0;
}

void arm7_resetMemory () {
	int i;
	u8 settings1, settings2;

	REG_IME = 0;

	for (i=0; i<16; i++) {
		SCHANNEL_CR(i) = 0;
		SCHANNEL_TIMER(i) = 0;
		SCHANNEL_SOURCE(i) = 0;
		SCHANNEL_LENGTH(i) = 0;
	}
	REG_SOUNDCNT = 0;

	// Clear out ARM7 DMA channels and timers
	for (i=0; i<4; i++) {
		DMA_CR(i) = 0;
		DMA_SRC(i) = 0;
		DMA_DEST(i) = 0;
		TIMER_CR(i) = 0;
		TIMER_DATA(i) = 0;
	}

	// Clear out FIFO
	REG_IPC_SYNC = 0;
	REG_IPC_FIFO_CR = IPC_FIFO_ENABLE | IPC_FIFO_SEND_CLEAR;
	REG_IPC_FIFO_CR = 0;

	// clear IWRAM - 037F:8000 to 0380:FFFF, total 96KiB
	arm7_clearmem ((void*)0x037F8000, 96*1024);

	// clear last part of EXRAM, skipping the ARM9's section
	arm7_clearmem ((void*)0x023FE000, 0x2000);
		
	// clear most of EXRAM - except after 0x023FD800, which has the ARM9 code
	// arm7_clearmem ((void*)0x02000000, 0x003FD800);
	arm7_clearmem ((void*)0x02000000, 0x00200000); // Skip temperary header/binary data
	
	REG_IE = 0;
	REG_IF = ~0;
	(*(vu32*)(0x04000000-4)) = 0;  //IRQ_HANDLER ARM7 version
	(*(vu32*)(0x04000000-8)) = ~0; //VBLANK_INTR_WAIT_FLAGS, ARM7 version
	REG_POWERCNT = 1;  //turn off power to stuffs

	// Reload DS Firmware settings
	arm7_readFirmware((u32)0x03FE70, &settings1, 0x1);
	arm7_readFirmware((u32)0x03FF70, &settings2, 0x1);

	if (settings1 > settings2) {
		arm7_readFirmware((u32)0x03FE00, (u8*)0x027FFC80, 0x70);
		arm7_readFirmware((u32)0x03FF00, (u8*)0x027FFD80, 0x70);
	} else {
		arm7_readFirmware((u32)0x03FF00, (u8*)0x027FFC80, 0x70);
		arm7_readFirmware((u32)0x03FE00, (u8*)0x027FFD80, 0x70);
	}

	// Load FW header
	arm7_readFirmware((u32)0x000000, (u8*)0x027FF830, 0x20);
	
}

void arm7_loadBinary() {
	tonccpy((void*)NDS_HEADER, (void*)TMP_DATA, 0x160);
	
	ndsHeader = (tNDSHeader*)NDS_HEADER;

	// Copy arm binaries and header to intended locations.
	tonccpy((void*)ndsHeader->arm9destination, ((void*)((u32)TMP_DATA + ndsHeader->arm9romOffset)), ndsHeader->arm9binarySize);
	tonccpy((void*)ndsHeader->arm7destination, ((void*)((u32)TMP_DATA + ndsHeader->arm7romOffset)), ndsHeader->arm7binarySize);
	
	// Clear temperary rom data now that they are copied
	// if (ndsHeader->arm9romOffset < ((u32)TMP_DATA - 0x02000000))arm7_clearmem ((void*)((u32)TMP_DATA), ndsHeader->romSize);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Main function
void arm7_main (void) {
	// Synchronise start
	while (ipcRecvState() != ARM9_START);
	ipcSendState(ARM7_START);

	// Wait until ARM9 is ready
	while (ipcRecvState() != ARM9_READY);

	ipcSendState(ARM7_MEMCLR);

	// Get ARM7 to clear RAM
	arm7_resetMemory();
	
	ipcSendState(ARM7_LOADBIN);
	
	arm7_loadBinary();
		
	ipcSendState(ARM7_BOOTBIN);
		
	arm7_reset();
}

