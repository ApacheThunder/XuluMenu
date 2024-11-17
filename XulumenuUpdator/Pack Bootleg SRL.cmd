@Echo off
ndstool -c autorun1_bootleg.nds -9 arm9_bootleg.bin -7 arm7.bin -t banner_bootleg.bin -h header.bin -d NitroFS_Bootleg
ndstool -c autorun2_bootleg.nds -9 arm9_bootleg.bin -7 arm7.bin -t banner_bootleg.bin -h header.bin -d NitroFS_Bootleg
ndstool -c autorun3_bootleg.nds -9 arm9_bootleg.bin -7 arm7.bin -t banner_bootleg.bin -h header.bin -d NitroFS_Bootleg
pause