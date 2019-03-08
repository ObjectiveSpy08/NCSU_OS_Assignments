# declare zfunction as global so that it is accessible outside
.globl zfunction
zfunction:
	# push old ebp to stack
	push %ebp
	# move from esp to ebp
	mov %esp, %ebp
	# move first parameter into eax from the stack
	# long is 4 bytes. and we need to skip old return address.
	# https://eli.thegreenplace.net/2011/02/04/where-the-top-of-the-stack-is-on-x86/
	mov 8(%ebp), %eax
	# clear 10th to 18th bit. Count strats with 1 from left
	and $0xFF803FFF, %eax
	# shift left by 4
	shl $0x4, %eax
	# pop return address and return
	pop %ebp
	ret
