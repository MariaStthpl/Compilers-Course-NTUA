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
	movb	$97, 23(%rsp)
	movb	$98, 22(%rsp)
	movabsq	$2324210631224608321, %rax # imm = 0x2041414141544641
	movq	%rax, 4(%rsp)
	movabsq	$9369253208541509, %rax # imm = 0x214949414E4945
	movq	%rax, 12(%rsp)
	leaq	4(%rsp), %rdi
	callq	writeString
	movl	$.Lnl, %edi
	callq	writeString
	movl	$2, %edi
	callq	writeInteger
	movl	$.Lnl, %edi
	callq	writeString
	movw	$2, 20(%rsp)
	movzbl	22(%rsp), %edi
	callq	writeChar
	movl	$.Lnl, %edi
	callq	writeString
	movl	$97, %edi
	callq	writeInteger
	movl	$.Lnl, %edi
	callq	writeString
	movl	$65535, %edi            # imm = 0xFFFF
	callq	writeInteger
	movl	$.Lnl, %edi
	callq	writeString
	addq	$24, %rsp
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
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
