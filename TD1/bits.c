/* 
 * INF559 Data Lab 
 * 
 * <Achille Nazaret achille.nazaret>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */

#endif

/* Copyright (C) 1991-2012 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* We do support the IEC 559 math functionality, real and complex.  */
/* wchar_t uses ISO/IEC 10646 (2nd ed., published 2011-03-15) /
   Unicode 6.0.  */
/* We do not support C11 <threads.h>.  */
/* bit manipulation */
/* 
 * bitAnd - x&y using only ~ and | 
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
int bitAnd(int x, int y) {
  return ~(~x|~y);
}
/* 
 * upperBits - pads n upper bits with 1's
 *  You may assume 0 <= n <= 32
 *  Example: upperBits(4) = 0xF0000000
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 10
 *  Rating: 1
 */
int upperBits(int n) {
  /* make use of the arithmetic shift */
  int potential_one = !!n;
  int oneleft = potential_one << 31;
  return oneleft >> (n + ~0);
}
/* 
 * anyEvenBit - return 1 if any even-numbered bit in word set to 1
 *   Examples anyEvenBit(0xA) = 0, anyEvenBit(0xE) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int anyEvenBit(int x) {
  int byte_mask = 85;
  byte_mask += byte_mask << 8;
  byte_mask += byte_mask << 16;
  return !!(byte_mask & x);
}
/* 
 * byteSwap - swaps the nth byte and the mth byte
 *  Examples: byteSwap(0x12345678, 1, 3) = 0x56341278
 *            byteSwap(0xDEADBEEF, 0, 2) = 0xDEEFBEAD
 *  You may assume that 0 <= n <= 3, 0 <= m <= 3
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 2
 */
int byteSwap(int x, int n, int m) {
/*
 55443322
 swap(2,0)
 55003300
	n*8 : n<<3
	m*8 : m<<3
*/

	//int bytemask = 0xFF;
	int n8 = n<<3;
	int m8 = m<<3;
	/*
	int maskn = bytemask << n8;
	int maskm = bytemask << m8;
	return ( (((x&maskn) >> n8)&0xFF) << m8 ) |
	( (((x&maskm) >> m8)&0xFF) << n8 ) | 
	( x&~(maskm|maskn));
	*/
	
	// From 15 op to 8 :) :) :) :)
	
	int xn8 = x >> n8;
	int xm8 = x >> m8;
	int combo = (xn8^xm8)&0xFF;
	return x^( combo << n8 )^(combo << m8);
	
	
	//Xor trick to avoid maskn and maskm ?  Done !!!!!
	

}
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
  int mask = !x + ~0;
  return (mask & y)|(~mask & z);
}
/* 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
  return !( ((x>>4)^3) | (!( (~x & 8) | !(x&6) )));
}
/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
int bitCount(int x) {
  // Sort of recursivity : on 2 bits : AB -> A+B;
  // on 4 bits : AB|CD , becomes A+B|C+D and then sum (A+B)+(C+D)
  // Add odd bits with even bits, 2bits odds with 2bits even ...
  // 0101 = 5
  // 0011 = 3
  // 1111 = F
  // 0000 0000 0000 0000 0000 0000 0000 0000
  
  int evenbits;
  int even2bits;
  int even4bits;
  int even8bits;
  int tmp;
  int signbit;
  int sign;
  
  //Drop x sign which will be annoying for right shifts
  signbit = 1<<31;
  sign = x&signbit;
  x = x&(~signbit);
  //4
  
  evenbits = 0x55;
  evenbits += evenbits << 8;
  evenbits += evenbits << 16;
  
  even2bits = 0x33;
  even2bits += even2bits << 8;
  even2bits += even2bits << 16;
  
  even4bits = 0x0F;
  even4bits += even4bits << 8;
  even4bits += even4bits << 16;
  
  even8bits = 0xFF;
  even8bits += even8bits << 16;
  //14 op
  
  tmp = x;
  tmp = (tmp&evenbits)  + ((tmp>>1)&evenbits); 
  tmp = (tmp&even2bits) + ((tmp>>2)&even2bits);
  tmp = (tmp&even4bits) + ((tmp>>4)&even4bits);
  tmp = (tmp&even8bits) + ((tmp>>8)&even8bits); 
  tmp = (tmp&0xFF) + (tmp >> 16);
  //19
  
  return tmp+!!sign;
  //3
  
  //XOR parité ? maxi 32 = bits ? to explore ...
  
}
/* 2's complement */
/* 
 * TMax - return maximum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmax(void) {
  return ~(1<<31);
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  return ~x + 1;
}
/* 
 * fitsBits - return 1 if x can be represented as an 
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) {
  int nminus = n + ~0;
  return (!(x>>nminus)) | (!(~x>>nminus));
}
/* 
 * addOK - Determine if can compute x+y without overflow
 *   Example: addOK(0x80000000,0x80000000) = 0,
 *            addOK(0x80000000,0x70000000) = 1, 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int addOK(int x, int y) {
  int signmask = 1<<31;
  int xsign = x&signmask;
  int ysign = y; //No need for another signmask because always used in pairs with &xsign
  
  // if xsign and ysign are differents : OK  xsign^ysign
  // if x,y < 0 (xsign&ysign) if (~x + ~y)&signmask --> NOT OK (
  // if x,y > 0 ~(xsign|ysign) if (x+y)&signmask --> NOT OK
  
  // Goal : 1 on the left if it is not ok
  // 1) RIEN
  // 2) (xsign&ysign)&( (~x + ~y + 1)&signmask  )    ( signmask useless )
  // 3) ~(xsign|ysign)&(x + y)&signmask
  
  return !(
  	( (xsign&ysign)&( (~x + ~y + 1)  ) ) | 
  	( ~(xsign|ysign)&(x + y)&signmask ) 
  );  
}
/* 
 * isGreater - if x > y  then return 1, else return 0 
 *   Example: isGreater(4,5) = 0, isGreater(5,4) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isGreater(int x, int y) {
 /*
  * x > y <=> y-x < 0
  *
  * 1) handle y-x overflow ... (can happen only if y and x are opposite sign
  * 
  * 2) if y-x negative : (y-x)&(1<<31) RAS
  */
  
  // 
  int signmask = 1<<31;
  int xsign = x&signmask;
  int ysign = y&signmask;
  
  // if x positive and y negative -> OK : ~xsign&ysign, OK
  // if x negative and y positive xsign&~ysign), NOT OK
  
  return !!((
  	((y + (~x + 1)) & (1<<31)) |
  	(~xsign&ysign)
  )&~(xsign&~ysign));
 
}
/* 
 * absVal - absolute value of x
 *   Example: absVal(-1) = 1.
 *   You may assume -TMax <= x <= TMax
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4
 */
int absVal(int x) {
  int sign = 1<<31;
  int xispos = !(sign&x); // 0 or 1
  int mask = xispos + ~0;
  return (x^mask)+(!xispos);
}
