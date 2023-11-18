#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

export TARGET := xulumenu
export TOPDIR := $(CURDIR)

include $(DEVKITARM)/ds_rules

.PHONY: data bootloader bootloaderalt clean

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
all: $(TARGET).nds

#---------------------------------------------------------------------------------
checkarm7:
	$(MAKE) -C arm7

#---------------------------------------------------------------------------------
checkarm9:
	$(MAKE) -C arm9

#---------------------------------------------------------------------------------
$(TARGET).nds : arm7/$(TARGET).elf arm9/$(TARGET).elf
	@ndstool	-c $@ -7 arm7/$(TARGET).elf -9 arm9/$(TARGET).elf \
	-h 0x200 -t nrio-banner.bin
	@dlditool nrio.dldi $@

data:
	@mkdir -p data

bootloader: data
	$(MAKE) -C bootloader LOADBIN=$(TOPDIR)/data/load.bin

bootloaderalt: data
	$(MAKE) -C bootloaderalt LOADBIN=$(TOPDIR)/data/loadAlt.bin

#---------------------------------------------------------------------------------
arm7/$(TARGET).elf:
	$(MAKE) -C arm7

#---------------------------------------------------------------------------------
arm9/$(TARGET).elf: bootloader bootloaderalt
	$(MAKE) -C arm9

#---------------------------------------------------------------------------------
clean:
	$(MAKE) -C arm9 clean
	$(MAKE) -C arm7 clean
	$(MAKE) -C bootloader clean
	$(MAKE) -C bootloaderalt clean
	rm -rf data
	rm -f $(TARGET).nds
