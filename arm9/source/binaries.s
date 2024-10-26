	.arm
	.global udiskData, udiskData_end, stage2Data, stage2_end

udiskData:
	.incbin "../binaries/udisk.srl"
udiskData_end:

stage2Data:
	.incbin "../binaries/stage2.srl"
stage2_end:


