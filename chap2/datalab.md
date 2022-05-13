---
tags: [操作系统/02-03]
title: datalab
created: '2022-05-11T08:02:32.901Z'
modified: '2022-05-13T12:32:37.751Z'
---

# datalab

> Use the dlc compiler (./dlc) to automatically check your version of bits.c for compliance with the coding guidelines:

       unix> ./dlc bits.c

>To compile and run the btest program, type:

    unix> make btest
    unix> ./btest [optional cmd line args]

    unix> make clean
    unix> make btest

### 1.bitXor

>bitXor - x^y using only ~ and & 
 Example: bitXor(4, 5) = 1
 Legal ops: ~ &
 Max ops: 14
 Rating: 1

    int bitXor(int x, int y) {
      return ~(~x & ~y) & ~(x & y);
    }
>～(~x & ~y)   同时为0的位，变为0，不同的位变为1.
～(x & y) 同时为1的位，变为0，不同的位变为1.

### 2.tmin
 
 > tmin - return minimum two's complement integer 
   Legal ops: ! ~ & ^ | + << >>
   Max ops: 4
   Rating: 1
 
    int tmin(void) {
      return 1 << 31;
    }
### 3.isTmax

 > isTmax - returns 1 if x is the maximum, two's complement number,
      and 0 otherwise 
    Legal ops: ! ~ & ^ | +
    Max ops: 10
    Rating: 1

    int isTmax(int x) {
    //x = 0x7fffffff, x + 1 = ~x, 当x = 0xffffffff,也满足，需要排除
    return !(~x ^ (x + 1) | !(x + 1));
    }

### 4.allOddBits 
 
 > allOddBits - return 1 if all odd-numbered bits in word set to 1
    where bits are numbered from 0 (least significant) to 31 (most significant)
    Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
    Legal ops: ! ~ & ^ | + << >>
    Max ops: 12
    Rating: 2

    int allOddBits(int x) {
      int mask = (0xAA << 24) + (0xAA << 16) + (0xAA << 8) + 0xAA;
      //return !((x & 0xAAAAAAAA) ^ 0xAAAAAAAA)
      return !((x & mask) ^ mask);
    }

### 5.negate

 > negate - return -x 
    Example: negate(1) = -1.
    Legal ops: ! ~ & ^ | + << >>
    Max ops: 5
    Rating: 2
 
    int negate(int x) {
      return ~x + 1;
    }
    -x = ~x + 1

### 6.isAsciiDigit
 > isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
    Example: isAsciiDigit(0x35) = 1.
             isAsciiDigit(0x3a) = 0.
             isAsciiDigit(0x05) = 0.
    Legal ops: ! ~ & ^ | + << >>
    Max ops: 15
    Rating: 3
 
    int isAsciiDigit(int x) {
      int diff1 = ((x + ~0x30 + 1) >> 31) & 1;//x - 0x30 < 0, diff1 = 1, return 0.
      int diff2 = ((0x39 + ~x + 1) >> 31) & 1;//0x39 - x < 0, diff2 = 1, return 0.

      return !diff1 & !diff2;
    }

### 7.conditional

 > conditional - same as x ? y : z 
    Example: conditional(2,4,5) = 4
    Legal ops: ! ~ & ^ | + << >>
    Max ops: 16
    Rating: 3
 
    int conditional(int x, int y, int z) {
      int res;
      ((x && (res = y)) || (res = z));
      return res;
    }

### 8.isLessOrEqual

 > isLessOrEqual - if x <= y  then return 1, else return 0 
    Example: isLessOrEqual(4,5) = 1.
    Legal ops: ! ~ & ^ | + << >>
    Max ops: 24
    Rating: 3
 
    int isLessOrEqual(int x, int y) {
      int x_sign = (x >> 31) & 1;
      int y_sign = (y >> 31) & 1;

      //y + ~x + 1 = y - x
      int res = ((y + ~x + 1) >> 31) & 1;
      //if x < 0, y > 0, return 1, x_sign & ~y_sign = 1
      //if x > 0, y < 0, return 0, x_sign & ~y_sign = 0, !(x_sign ^ y_sign) = 0.
      //x, y同号, x_sign & ~y_sign = 0, !(x_sign ^ y_sign) = 1,  y - x >= 0, y >= x, res = 0, return 1.
      return (x_sign & ~y_sign) | (!(x_sign ^ y_sign) & !res);
    }

### 9.logicalNeg

 > logicalNeg - implement the ! operator, using all of 
               the legal operators except !
    Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
    Legal ops: ~ & ^ | + << >>
    Max ops: 12
    Rating: 4 

    int logicalNeg(int x) {
      int x_sign = (x >> 31) & 1;
      int max_int = ~(1 << 31);
      //x < 0, x_sign = 1, x_sign ^ 1 = 0, return 0. 
      //x > 0, x_sign = 0, x_sign ^ 1 = 1, ((max_int + x) >> 31) & 1 = 1, 1 ^ 1 = 0, return 1 & 0 = 0.
      // x = 0, x_sign = 0, x_sign ^ 1 = 1, ((max_int + x) >> 31) & 1 = 0, 0 ^ 1 = 1, return 1 & 1 = 1.
      return (x_sign ^ 1) & (((max_int + x) >> 31) & 1 ^ 1);
    }

### 10.howManyBits

>howManyBits - return the minimum number of bits required to represent x in
              two's complement
   Examples: howManyBits(12) = 5
             howManyBits(298) = 10
             howManyBits(-5) = 4
             howManyBits(0)  = 1
             howManyBits(-1) = 1
             howManyBits(0x80000000) = 32
   Legal ops: ! ~ & ^ | + << >>
   Max ops: 90
   Rating: 4
 
    //正数根据最高位的1进行判断， 负数根据最高位的0进行判断。如果是负数则取反，也通过最高位的1进行判断。
    // 12 = 0x0..01100, -5 = 0x1...101.
    int howManyBits(int x) {
      //如果x >= 0, 不变。如果x < 0, 按位取反。
      x = x ^ (x >> 31);

      int b16, b8, b4, b2, b1, b0;
      b16 = !!(x >> 16) << 4;//判断高16位是否有1，有的话x最少为16位。
      x >> b16;

      b8 = !!(x >> 8) << 3;//判断剩余的高8位是否有1， 有的话x最少为(16 + 8 = 24)位
      x >> b8;

      b4 = !!(x >> 4) << 2;//判断剩余的高4位是否有1， 有的话x最少为(16 + 8 + 4= 28)位
      x >> b4;

      b2 = !!(x >> 2) << 1;//判断剩余的高2位是否有1， 有的话x最少为(16 + 8 + 4 + 2= 30)位
      x >> b2;

      b1 = !!(x >> 1);//判断剩余的高1位是否有1， 有的话x最少为(16 + 8 + 4 + 2 + 1= 31)位
      x >> b1;

      b0 = x;
      return b0 + b1 + b2 + b4 + b8 + b16 + 1;//最高位1的位数 + 符号位。
    }

### 11.floatScale2(homework 2.94)
   
 > floatScale2 - Return bit-level equivalent of expression 2*f for
    floating point argument f.
    Both the argument and result are passed as unsigned int's, but
    they are to be interpreted as the bit-level representation of
    single-precision floating point values.
    When argument is NaN, return argument
    Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
    Max ops: 30
    Rating: 4

    unsigned floatScale2(unsigned uf) {
      unsigned sign = uf >> 31;
      unsigned exp = (uf >> 23) & 0xff;
      unsigned frac = uf & 0x7fffff;

      if(exp == 0xff)
        return uf;
      
      if(exp == 0){
        frac << 1;
      }else if(exp == 0xff - 1){
        exp = 0xff;
        frac = 0;
      }else{
        exp += 1;
      }
      return sign << 31 | exp << 23 | frac;
    }

### 12.floatFloat2Int(homework 2.96)
 
 > floatFloat2Int - Return bit-level equivalent of expression (int) f
    for floating point argument f.
    Argument is passed as unsigned int, but
    it is to be interpreted as the bit-level representation of a
    single-precision floating point value.
    Anything out of range (including NaN and infinity) should return
    0x80000000u.
    Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
    Max ops: 30
    Rating: 4
 
    int floatFloat2Int(unsigned uf) {
      unsigned sign = uf >> 31;
      unsigned exp = (uf >> 23) & 0xff;
      unsigned frac = uf & 0x7fffff;

      int bias = 0x7f;
      int E, M, res;
      if(exp >= 0 && exp < bias){//小于1的数
        return 0;
      }else if(exp >= bias + 31){//overflow
        return 0x80000000;
      }else{
        E = exp - bias;
        M = 0x800000 | frac;
        if(E >= 23){
          res = M << (E - 23);
        }else{
          res = M >> (23 - E);
        }
      }
      return sign ? -res : res;
    }

### 13.floatPower2

 > floatPower2 - Return bit-level equivalent of the expression 2.0^x
    (2.0 raised to the power x) for any 32-bit integer x.
    The unsigned value that is returned should have the identical bit
    representation as the single-precision floating-point number 2.0^x.
    If the result is too small to be represented as a denorm, return
    0. If too large, return +INF.
    Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while 
    Max ops: 30 
    Rating: 4
 

    unsigned floatPower2(int x) {
        unsigned sign = 0;
        unsigned exp;
        unsigned frac;

        int min_exp = -23 - 126;//正最小非规格化数 2^-23 * 2^-126;
        int max_exp = 127;//正最大规格化数 (2-w) * 2^127;
        int bias = 0x7f;

        if(x < min_exp)
          return 0;
        else if(x > max_exp){
          return 0x7f800000;
        }else if(x >= min_exp && x <= -126){//非规格化数
          exp = 0;
          frac = 0x800000 >> (-126 - x);
          frac &= 0x7fffffff;
        }else{
          exp = x + bias;
          frac = 0;
        }
        return sign << 31 | exp << 23 | frac;
    }

