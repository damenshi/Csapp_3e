---
tags: [操作系统/02-03]
title: 02-03-bits-ints
created: '2022-04-26T09:21:29.173Z'
modified: '2022-05-10T11:44:49.127Z'
---

# 02-03-bits-ints
[B站课程视频地址](https://www.bilibili.com/video/BV1iW411d7hd?p=2)

### 1.对于字长为w位的机器而言，虚拟地址范围为0～2^w - 1.
### 2.0x01234567：高位字节0x01，低位字节0x67
*大端存储* 
|0x100 |0x101 |0x102|0x103
|:------:|:------:|:-----:|:-----:|
|01|23|45|67
*小端存储* 
|0x100 |0x101 |0x102|0x103
|:------:|:------:|:-----:|:-----:|
|67|45|23|01
### 3. x^x = 0, x^0 = x, x^1 = ~x
### 4. 移动k>=w位时，实际位移量为k mod w
### 5. C语言数字默认是有符号的，无符号常量末尾加‘U'or’u'
### 6. 无符号数和有符号数执行运算，C语言会隐式的将有符号数转换为无符号数
### 7. 当short转换为unsigned时，先改变大小，再完成有符号到无符号的转换。
> short sx = -12345;
> unsigned uy = sx;

> printf("uy = %u:\t", uy);
> show_bytes((byte_pointer) &uy, sizeof(unsigned));

> 大端输出：uy = 4294954951： ff ff cf c7

### 8.除以2的幂，(x < 0 ? x + (1<<k) - 1 : x) >> k;//x小于0要加上偏移量，以使结果向0舍入。利用如下属性：x/y向上取整 = (x+y-1)/y向下取整。

### 9.根据exp（阶码）的值，浮点数被编码的值可以分为三种不同情况
> 情况1:规格化的值，exp既不全为0，也不全为1，E = exp - Bias，(单精度Bias为127，双精度为1023)，尾数为隐含的以1开头。
> 情况2:非规格化的值，exp全为0，E = 1 - Bias，尾数不包含隐含的开头1。
> 情况3:特殊值，exp全为1，尾数全为0，表示无穷。尾数不全为0，表示“”NaN“，即不是一个数。

### 10.IEEE默认舍入方式为向偶数舍入，也称为向最近值舍入。中间值的舍入效果，向偶数舍入采用的方法为将数字向上或向下舍入，使得结果最低有效数字为偶数。例如1.5和2.5都舍入为2.

### 练习题
#### 2.1
A：0011 1001 1010 0111 1111 1000
B：0xC97B
C: 1110 0101 1111 0100 1100
D: 0x26e7b5

#### 2.2
2^19 = 524288  0x80000
2^14 = 16384 0x4000
2^17 = 131072 0x20000
2^5 = 32 0x20
2^7 = 128 0x80

#### 2.3

|十进制 |二进制 |十六进制                
|:-----:|:-----------:|:---------:|
|0|0000 00000|0x00|
|167|1010 0111|0xA7|
|62|0011 1110|0x3e|
|188|1011 1100| 0xbc|
|..|..|..
|211|1110 0111|0xE7

#### 2.4
A.0x5044
B.0x4ffc
C.0x507c
D.0xae

#### 2.5
A.小端法：0x21  大端法：0x87
B.小端法：0x2143  大端法：0x8765
C.小端法：0x214365  大端法：0x876543

#### 2.6
A. 0x00359141 = 0000 0000 0011 0101 1001 0001 0100 0001
   0x4a564504 = 0100 1010 0101 0110 0100 0101 0000 0100
B. 21
C. 整数中除最高有效位，其余位都和浮点数中相匹配。

#### 2.7
61 62 63 64 65 66

#### 2.8
|运算|结果｜
|--|--------|
|~a|10010110|
|~b|10101010|
|a&b|01000001|
|a\|b|01111101|
|a^b|00111100|

#### 2.9
A. 
～黑色 = 白色、～蓝色 = 黄色、～绿色 = 红紫色
～蓝绿色 = 红色、～红色 = 蓝绿色、～红紫色 = 绿色
～黄色 = 蓝色、 ～白色 = 黑色

B.
蓝色｜绿色 = 蓝绿色
黄色 & 蓝绿色 = 绿色
红色 ^ 红紫色 = 蓝色

#### 2.10
1.a a^b
2.b a^b
3.b a

#### 2.11
A. fisrt = last = (2k + 1)/2
B. 因为a^a = 0
C. 循环判断条件改为 first < last

#### 2.12
A. x&0xff
B. x^~0xff
C. x | 0xff

#### 2.13
int result = bis(x, y);
int result = bis(bic(x, y), bic(y, x));

#### 2.14
x = 0x66 = 0110 0110 y = 0x39 = 0011 1001
|表达式|值|表达式|值|
|:-----:|:-:|:------:|:-:|
|x \& y|0x20|x && y|0x01
|x \| y|0x7f|x \|\| y|0x01
|~x \| -y|0xdf|!x \|\| !y|0x00
|x & !y|0x00|x && ~y|0x01

#### 2.15
    if(!(x^y)){
      retrun 1;
    }else{
      return 0;
    }

#### 2.16 略

#### 2.17 略

#### 2.18 略

#### 2.19
|x|T2U(x)|
|:-:|:-:|
|-8|8|
|-3|13
|-2|14
|-1|15|
|0|0|
|5|5|

#### 2.21
|表达式|类型|求值|
|:--:|:--:|:--:|:--:|
||无符号|1
||有符号|1
||无符号|0
||有符号|1
||无符号|1

#### 2.23
|w|func1(w)|func2(w)
|:--:|:--:|:--:|
|0x00000076|0x00000076|0x00000076|
|0x87654321|0x00000021|0x00000021|
|0x000000C9|0x000000c9|0xffffffc9|
|0xEDCBA987|0x00000087|0xffffff87|


#### 2.24 略

#### 2.25
0u - 1 = Umax，unsigned length 改为 int length

#### 2.26
改为 return strlen(s) > strlen(t);

#### 2.27
    int uadd_ok(unsigned x, unsigned y){
      unsigned res = x + y;
      if(res < x){
        return 0;//溢出
      }
      return 1;
    }

#### 2.28
|十六禁止|十进制|十进制|十六进制
|:--:|:--:|:--:|:--:|
|0|0|0|0|
|5|5|11|B|
|8|8|8|8|
|D|13|3|3|
|F|15|1|1|

#### 2.29 略

#### 2.30
    int tadd_ok(int x, int y){
      int flag1 = x >= 0 && y >= 0 && x + y < 0;
      int flag2 = x < 0 && y < 0 && x + y >= 0;
      return !flag1 && !flag2;
    }

#### 2.31
无论x+y是否溢出,(x+y)-x都等于y。

#### 2.32
当y = Tmin时，-y = Tmin，tadd_ok(x, -y),当x<0, y = Tmin时，会返回0(溢出).
而tsub_ok(x, y), x < 0, y = Tmin 应该返回1(未溢出)。

#### 2.33 
|十六禁止|十进制|十进制|十六进制
|:--:|:--:|:--:|:--:|
|0|0|0|0|
|5|5|-5|B|
|8|-8|-8|8|
|D|-3|3|3|
|F|-1|1|1|

#### 2.34 略

#### 2.35 略

#### 2.36
    int tmul_ok(int x, int y){
      int64_t res = x * y;
      return res == (int)res; 
    }

#### 2.37
A.没有改进，调用malloc函数，会将asize转换为32位，还是可能出现溢出。
B.
    uint64_t require = ele_cnt * (uint64_t) ele_size;
    size_t request = (size_t) require;
    if(request != require)//发生溢出
      return  null;
    void *result = malloc(request);
    if(result == null)
      return null; 

#### 2.38
b = 0 时2^k， b = a时，2^k + 1.因此可以计算a的1,2,3,4,5,8,9 

#### 2.39 略
#### 2.40
|K|移位|加法/减法|表达式|
|:--:|:--:|:--:|:--:|
|6|2|1|(x<<3) - (x<<1)|
|31|1|1|(x<<5) - x|
|-6|2|2|(x<<1) - (x<<3)|
|55|2|2|((x<<6)-x)-(x<<3)

#### 2.41
选择运算量小的方式。n = m,选择A，n = m+1选择A或B都可以，n > m + 1选择B。

#### 2.42
    int32_t div16(int32_t x){
        int32_t bias = (x>>31) & 0xf;//x >= 0 bias = 0, x < 0 bias = 15.
        return (x + bias) >> 4;
    }

#### 2.43
M = 31， N = 8.

#### 2.44
A.F x = Tmin ,x < 0 && x - 1 > 0
B.T
C.F x = 0xffff, x * x = 0xfffe0001 < 0
D.T
E.F x = Tmin, x < 0, -x < 0
F.T
G.T x*~y = x * (-y-1) // -y = ~y + 1, ~y = -y-1.
    x*-y + uy*ux = -x*y - x + uy*ux = -x

#### 2.45 略

#### 2.46
A.0.000000000000000000000001100[1100]...2
B.1/10 * 2 ^ -20 = 9.54 * 10^-8 //小数点右移20位，相当于除以2^20.
C.100h = 100 * 60 * 60 = 360000s
  360000 * 10 * 9.54 * 10^-8 = 0.343s
D.2000 * 0.343s = 687m

#### 2.47

|位|e|E|2^E|f|M|2\^E*M|V|十进制|
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|0 00 00|0|0|1|0/4|0/4|0/4|0|0.0
|0 00 01|0|0|1|1/4|1/4|1/4|1/4|0.25
|0 00 10|0|0|1|2/4|2/4|2/4|1/2|0.5
|0 00 11|0|0|1|3/4|3/4|3/4|3/4|0.75
|0 01 00|1|0|1|0/4|4/4|4/4|1|1.0
|0 01 01|1|0|1|1/4|5/4|5/4|5/4|1.25
|0 01 10|1|0|1|2/4|6/4|6/4|3/2|1.5
|0 01 11|1|0|1|3/4|7/4|7/4|7/4|1.75
|0 10 00|2|1|2|0/4|4/4|8/4|2|2.0
|0 10 01|2|1|2|1/4|5/4|10/4|5/2|2.5
|0 10 11|2|1|2|3/4|7/4|14/4|7/2|3.5
|0 11 00|-|-|-|-|-|-|无穷|-

#### 2.48
0x00359141 = 0000 0000 0011 0101 1001 0001 0100 0001
0x4A564504 = 0100 1010 0101 0110 0100 0101 0000 0100
符号位为0，阶码e = 1001 0100，E = e - 127 = 21，尾数为1.101 0110 0100 0101 0000 0100。

#### 2.49
A.2^n+1 + 1. 1后面跟n个0，再跟一个1.
B.2^24 + 1 = 16777217

#### 2.50
A.10.0 
B.10.1 
C.11.0
D.11.0

#### 2.51
A.x' = 0.0001 1001 1001 1001 1001 101
       0.1 = 0.0001 1001 1001 1001 1001 100...
B.x' - 0.1 = 0.0000 0000 0000 0000 0000 000[0011]...2
           = 1/10 * 2 ^ -22 = 2.38*10^-8
C.360000 * 10 * 2.38*10^-8 = 0.086s
D.2000 * 0.086 = 171m

#### 2.52 略

#### 2.53
#define POS_INFINITY 1e400
#define NEG_INFINITY (-POS_INFINITY)
#define NEG_ZERO (1.0/NEG_INFINITY)

#### 2.54
A.T
B.F x = Tmax,转换为float会舍入。float能准确描述的最大正数为2^25-1(24个1)
C.F d = 1e40
D.T
E.T
F.T
G.T
H.F f = 1e20, d = 1.0, f + d 舍入到1e20，左边为0，右边为1.0.




























