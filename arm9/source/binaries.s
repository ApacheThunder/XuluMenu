	.arm
	.global udiskData, stage2Data, nriousbData

.align 16
udiskData:
	.incbin "../binaries/udisk.srl"

.align 16
stage2Data:
	.incbin "../binaries/stage2.srl"

.align 16
nriousbData:
	.incbin "../binaries/nriousb.srl"

