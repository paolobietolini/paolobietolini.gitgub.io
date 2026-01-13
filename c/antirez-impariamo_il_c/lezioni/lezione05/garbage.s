	.file	"garbage.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"%02x(%c) "
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB23:
	.cfi_startproc
	endbr64
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	movl	$6, %ebp
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	leaq	.LC0(%rip), %rbx
	subq	$16, %rsp
	.cfi_def_cfa_offset 48
	leaq	11(%rsp), %r12
	movl	$1819043176, 11(%rsp)
	movq	%r12, %rdi
	movb	$111, 15(%rsp)
	call	puts@PLT
	movsbl	11(%rsp), %ecx
	movq	%rbx, %rsi
	xorl	%eax, %eax
	movl	$2, %edi
	movzbl	%cl, %edx
	call	__printf_chk@PLT
	movsbl	12(%rsp), %ecx
	movq	%rbx, %rsi
	xorl	%eax, %eax
	movl	$2, %edi
	movzbl	%cl, %edx
	call	__printf_chk@PLT
	movsbl	13(%rsp), %ecx
	movq	%rbx, %rsi
	xorl	%eax, %eax
	movl	$2, %edi
	movzbl	%cl, %edx
	call	__printf_chk@PLT
	movsbl	14(%rsp), %ecx
	movq	%rbx, %rsi
	xorl	%eax, %eax
	movl	$2, %edi
	movzbl	%cl, %edx
	call	__printf_chk@PLT
	movsbl	15(%rsp), %ecx
	movq	%rbx, %rsi
	xorl	%eax, %eax
	movl	$2, %edi
	movzbl	%cl, %edx
	call	__printf_chk@PLT
	movsbl	16(%rsp), %ecx
	movq	%rbx, %rsi
	xorl	%eax, %eax
	movl	$2, %edi
	movzbl	%cl, %edx
	call	__printf_chk@PLT
.L2:
	movsbl	(%r12,%rbp), %ecx
	movq	%rbx, %rsi
	movl	$2, %edi
	xorl	%eax, %eax
	addq	$1, %rbp
	movzbl	%cl, %edx
	call	__printf_chk@PLT
	cmpq	$40, %rbp
	jne	.L2
	movl	$10, %edi
	call	putchar@PLT
	addq	$16, %rsp
	.cfi_def_cfa_offset 32
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE23:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
