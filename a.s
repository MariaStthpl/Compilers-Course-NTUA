	.text
	.file	"a.ll"
	.globl	writeByte               # -- Begin function writeByte
	.p2align	4, 0x90
	.type	writeByte,@function
writeByte:                              # @writeByte
	.cfi_startproc
# BB#0:
	pushq	%rax
.Lcfi0:
	.cfi_def_cfa_offset 16
	movsbl	%dil, %edi
	callq	writeInteger
	popq	%rax
	retq
.Lfunc_end0:
	.size	writeByte, .Lfunc_end0-writeByte
	.cfi_endproc
                                        # -- End function
	.globl	extend                  # -- Begin function extend
	.p2align	4, 0x90
	.type	extend,@function
extend:                                 # @extend
	.cfi_startproc
# BB#0:
	movsbl	%dil, %eax
                                        # kill: %AX<def> %AX<kill> %EAX<kill>
	retq
.Lfunc_end1:
	.size	extend, .Lfunc_end1-extend
	.cfi_endproc
                                        # -- End function
	.globl	shrink                  # -- Begin function shrink
	.p2align	4, 0x90
	.type	shrink,@function
shrink:                                 # @shrink
	.cfi_startproc
# BB#0:
	movl	%edi, %eax
	retq
.Lfunc_end2:
	.size	shrink, .Lfunc_end2-shrink
	.cfi_endproc
                                        # -- End function
	.globl	main                    # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# BB#0:                                 # %entry
	subq	$56, %rsp
.Lcfi1:
	.cfi_def_cfa_offset 64
	leaq	12(%rsp), %rdx
	leaq	16(%rsp), %rdi
	leaq	36(%rsp), %rsi
	movq	%rdi, %rcx
	movq	%rsi, %r8
	callq	fun
	movw	$1, 212(%rsp)
	movl	$1, %edi
	callq	writeInteger
	movl	$string_literal.2, %edi
	callq	writeString
	addq	$56, %rsp
	retq
.Lfunc_end3:
	.size	main, .Lfunc_end3-main
	.cfi_endproc
                                        # -- End function
	.globl	fun                     # -- Begin function fun
	.p2align	4, 0x90
	.type	fun,@function
fun:                                    # @fun
	.cfi_startproc
# BB#0:                                 # %entry
	subq	$40, %rsp
.Lcfi2:
	.cfi_def_cfa_offset 48
	movq	%rdi, 8(%rsp)
	movq	%rsi, (%rsp)
	movq	%rdx, 16(%rsp)
	movq	%rcx, 24(%rsp)
	movq	%r8, 32(%rsp)
	movw	$1, (%rdi)
	movq	8(%rsp), %rax
	movw	$1, 2(%rax)
	movq	(%rsp), %rax
	movw	$2, (%rax)
	movq	(%rsp), %rax
	movw	$2, 2(%rax)
	movq	(%rsp), %rax
	movw	$2, 4(%rax)
	movq	8(%rsp), %rax
	movzwl	(%rax), %edi
	callq	writeInteger
	movq	8(%rsp), %rax
	movzwl	2(%rax), %edi
	callq	writeInteger
	movl	$string_literal, %edi
	callq	writeString
	movq	(%rsp), %rax
	movzwl	(%rax), %edi
	callq	writeInteger
	movl	$string_literal.1, %edi
	callq	writeString
	addq	$40, %rsp
	retq
.Lfunc_end4:
	.size	fun, .Lfunc_end4-fun
	.cfi_endproc
                                        # -- End function
	.type	string_literal,@object  # @string_literal
	.data
string_literal:
	.asciz	"\n"
	.size	string_literal, 2

	.type	string_literal.1,@object # @string_literal.1
string_literal.1:
	.asciz	"\n"
	.size	string_literal.1, 2

	.type	string_literal.2,@object # @string_literal.2
string_literal.2:
	.asciz	"\n"
	.size	string_literal.2, 2


	.section	".note.GNU-stack","",@progbits
