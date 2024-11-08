Ensure new update bin is multiple of 0x20000 (prior to marker insertion). Pad file to fill the final 0x20000 block near end of file when needed.
After marker insertion, remove any additional zero data the marker tool might have added then make sure last byte is changed to 0x00 instead of 0xFF.

Any markers present in 0xFF padding should be 0xFF filled as well.

Note that in arm9 binaries of this version of udisk Updator defines the max allowed update size at offset 0x794 in the bin file.

This is a multiple of 0x20000. It is recommended to increase/decrease the max block count to match the block count your update bin takes up.

For example a file just over 1MB tends to require 9 blocks so change the value to 09.

As for NitroFS data structure, file name for update can be anything as long as it has an extension (like NDS or BIN for example).
Though filenames of excessive length may bug out UI. A file with no extension is reserved for UI to use as a "version" string.

Only two files should be packed in NitroFS.
It should contain only update binary and an empty file with filename intended to be displayed in UI as the version number/string.

Do not give this file an extension or add any data.

The included cmd file is setup to allow packing of bin files/NitroFS back into the original SRL (rom file) container for the uDisk updator.
This cmd file relies on twlTool. Ensure it's in your PATH or it will not operate as intended.