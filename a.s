	.text
	.file	"a.ll"
	.globl	main
	.align	16, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# BB#0:                                 # %entry
	pushq	%rax
.Ltmp0:
	.cfi_def_cfa_offset 16
	movabsq	$31063898063135076, %rax # imm = 0x6E5C7361646164
	movq	%rax, (%rsp)
	leaq	(%rsp), %rdi
	callq	writeString
	movl	$.Lnl, %edi
	callq	writeString
	popq	%rax
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
