	.text
	.file	"a.ll"
	.globl	main
	.align	16, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# BB#0:                                 # %entry
	subq	$24, %rsp
.Ltmp0:
	.cfi_def_cfa_offset 32
	movb	$114, 7(%rsp)
	movb	$101, 8(%rsp)
	movzbl	8(%rsp), %edi
	callq	writeInteger
	movl	$.Lnl, %edi
	callq	writeString
	movl	$97, %edi
	callq	writeInteger
	movl	$.Lnl, %edi
	callq	writeString
	movzbl	7(%rsp), %edi
	callq	writeInteger
	movl	$.Lnl, %edi
	callq	writeString
	movl	$1, %edi
	callq	hehe
	movzbl	%al, %edi
	callq	writeInteger
	movl	$.Lnl, %edi
	callq	writeString
	addq	$24, %rsp
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc

	.globl	hehe
	.align	16, 0x90
	.type	hehe,@function
hehe:                                   # @hehe
	.cfi_startproc
# BB#0:                                 # %entry
	movw	%di, -2(%rsp)
	movb	$104, %al
	retq
.Lfunc_end1:
	.size	hehe, .Lfunc_end1-hehe
	.cfi_endproc

	.type	.Lvars,@object          # @vars
	.local	.Lvars
	.comm	.Lvars,104,16
	.type	.Lnl,@object            # @nl
	.section	.rodata,"a",@progbits
.Lnl:
	.asciz	"\n"
	.size	.Lnl, 2


	.section	".note.GNU-stack","",@progbits
