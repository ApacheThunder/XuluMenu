@Echo off
ndstool -c autorun1.nds -9 arm9.bin -7 arm7.bin -t banner.bin -h header.bin -d NitroFS
ndstool -c autorun2.nds -9 arm9.bin -7 arm7.bin -t banner.bin -h header.bin -d NitroFS
ndstool -c autorun3.nds -9 arm9.bin -7 arm7.bin -t banner.bin -h header.bin -d NitroFS
pause