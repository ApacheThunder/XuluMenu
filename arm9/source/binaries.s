	.arm
	.global udiskData, udiskData_end, stage2Data, stage2Data_end, nriousbData, nriousbData_end

udiskData:
	.incbin "../binaries/udisk.srl"
udiskData_end:

stage2Data:
	.incbin "../binaries/stage2.srl"
stage2Data_end:

nriousbData:
	.incbin "../binaries/nriousb.srl"
nriousbData_end:


