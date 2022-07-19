# <CSAPP>Chapter2 homework

### 2.55

    #include <stdio.h>
    typedef unsigned char* byte_pointer;
    
    void show_bytes(byte_pointer start, size_t len) {
      size_t i;
      for (i = 0; i < len; i++) {
        printf(" %.2x", start[i]);
      }
      printf("\n");
    }
    
    void show_int(int x) {
      show_bytes((byte_pointer) &x, sizeof(int));
    }
    
    void show_float(float x) {
      show_bytes((byte_pointer) &x, sizeof(float));
    }
    
    void show_pointer(void* x) {
      show_bytes((byte_pointer) &x, sizeof(void*));
    }
    
    void test_show_bytes(int val) {
      int ival = val;
      float fval = (float) ival;
      int* pval = &ival;
    
      show_int(ival);
      show_float(fval);
      show_pointer(pval);
    }
    
    int main(int argc, char* argv[]) {
      int test_num = 1024;
    
      test_show_bytes(test_num);
      return 0;
    }

### 2.57
    #include <stdio.h>
    
    typedef unsigned char* byte_pointer;
    
    void show_bytes(byte_pointer start, size_t len) {
      size_t i;
      for (i = 0; i < len; i++) {
        printf(" %.2x", start[i]);
      }
      printf("\n");
    }
    
    void show_int(int x) {
      show_bytes((byte_pointer) &x, sizeof(int));
    }
    
    void show_float(float x) {
      show_bytes((byte_pointer) &x, sizeof(float));
    }
    
    void show_pointer(void* x) {
      show_bytes((byte_pointer) &x, sizeof(void*));
    }
    
    void show_short(short x){
        show_bytes((byte_pointer) &x, sizeof(short));
    }
    
    void show_long(long x){
        show_bytes((byte_pointer) &x, sizeof(long));
    }
    
    void show_double(double x){
        show_bytes((byte_pointer) &x, sizeof(double));
    }
    
    void test_show_bytes(int val) {
      int ival = val;
      float fval = (float) ival;
      int* pval = &ival;
      short sval = (short) ival;
      short lval = (long) ival;
      short dval = (double) ival;
    
      show_int(ival);
      show_float(fval);
      show_pointer(pval);
      show_short(sval);
      show_long(lval);
      show_double(dval);
    }
    
    int main(int argc, char* argv[]) {
      int test_num = 1024;
    
      test_show_bytes(test_num);
      return 0;
    }

### 2.58

    #include <stdio.h>
    #include <assert.h>
    typedef unsigned char* byte_pointer;
    
    int is_little_endian(){
        int test_num = 0xff;
        byte_pointer start = (byte_pointer) &test_num;
        if(start[0] == 0xff)
            return 1;
        return 0;
    }
    int main(){
        assert(is_little_endian());
        return 0;
    }

### 2.59
(x & 0xff) ^ (y & ~0xff);

    #include <stdio.h>
    int main(){
        int x = 0x89ABCDEF;
        int y = 0x76543210;
        int mask = 0xff;
        int res = (x & mask) ^ (y & ~mask);
        printf("%0X\n", res);
        return 0;
    }

### 2.60

    #include<stdio.h>
    #include<assert.h>
    typedef unsigned char * byte_pointer; 
    
    unsigned replace_byte(unsigned x, int i, unsigned char b){
        byte_pointer start = (byte_pointer) &x;
        start[i] = b;
        return x;
    }
    
    int main(){
        unsigned test_num = 0x12345678;
        unsigned char replace = 0xAB;
        unsigned rep0 = replace_byte(test_num, 2, replace);
        unsigned rep1 = replace_byte(test_num, 0, replace);
        assert(rep0 == 0x12AB5678);
        assert(rep1 == 0x123456AB);
        return 0;
    }

### 2.61
A.!~x
B.!x
C.!(~x & 0xff)
D.!((x>>(sizeof(int)-1 << 3)) & 0xff)

**test.c**

    #include <stdio.h>
    #include <assert.h>
    int A(int x){
        return !~x;
    }
    
    int B(int x){
        return !x;
    }
    
    int C(int x){
        return !(~x & 0xff);
    }
    
    int D(int x){
        return !((x >> ((sizeof(int) - 1) << 3)) & 0xff);
    }
    
    int main(){
        int all_one = ~0;
        int all_zero = 0;
    
        assert(A(all_one));
        assert(B(all_zero));
        assert(C(all_one));
        assert(D(all_zero));
    
        return 0;
    }

### 2.62

    #include <stdio.h>
    #include <assert.h>
    int int_shifts_are_arithmetic(){
        int num = -1;
        return !(num ^ (num >> 1));
    }
    
    int main(){
        assert(int_shifts_are_arithmetic());
        return 0;
    }

### 2.63

    #include <stdio.h>
    #include <assert.h>
    #include <limits.h>
    
    unsigned srl(unsigned x, int k){
        unsigned xrsa = (int) x >> k;
        int w = sizeof(int) << 3;
        int mask = (int)-1 << (w - k);
      
        return xrsa & ~mask;
    }
    
    //x>0,算数右移，左边填0. x < 0,算数右移，左边填1.
    //当x>0时，使mask前k位为0，当x < 0时， 使mask前k位为1.
    int sra(int x, int k){
        int xrsl = (unsigned) x >> k;
        
        int w = sizeof(int) << 3;
        int mask = (int)-1 << (w - k);
        int m = (int)1 << (w-1);
        mask &= !(m & x) - 1;
        //m&x,取x的符号位。符号位为0，!(m&x) - 1 = 1 - 1 = 0,mask & 0 = 0 
        //符号位为1，!(m&x) = 0, !(m&x) - 1 = 0 - 1 = -1,mask & -1 = mask
        //总结：当x>=0时，mask为0，当x<0时，mask保持不变。
        return mask | xrsl; 
    }
    
    int main(){
        unsigned test_unsigned = 0x12345678;
        int test_int = 0x12345678;
    
        assert(srl(test_unsigned, 4) == test_unsigned >> 4);
        assert(sra(test_int, 4) == test_int >> 4);
    
        test_unsigned = 0x87654321;
      test_int = 0x87654321;
    
      assert (srl (test_unsigned, 4) == test_unsigned >> 4);
      assert (sra (test_int, 4) == test_int >> 4);
    }

### 2.64
x任意奇数位为1，返回1，否则返回0

    #include <stdio.h>
    #include <assert.h>
    
    int any_odd_one(unsigned x){
        return !!(0xAAAAAAAA & x);
    }
    
    int main(){
        assert(any_odd_one(0x02));
        assert(!any_odd_one(0x05));
    }

### 2.65
[参考](https://stackoverflow.com/questions/9133279/bitparity-finding-odd-number-of-bits-in-an-integer/9133406#9133406)
#include <stdio.h>
#include <assert.h>

int odd_ones(unsigned x){
    x = (x & 0x0000FFFF) ^ (x >> 16);
    x = (x & 0x000000FF) ^ (x >> 8);
    x = (x & 0x0000000F) ^ (x >> 4);
    x = (x & 0x00000003) ^ (x >> 2);
    x = (x & 0x00000001) ^ (x >> 1);
    return x;
}

int main(){
    assert(odd_ones(0x0111));
    assert(!odd_ones(0x101));
    return 0;
}

### 2.66

    #include <stdio.h>
    #include <assert.h>
    
    int leftmost_one(unsigned x){
        //先将最左边的1右边的位全部变成1，0xff00->0xffff, 0x6600->0x7fff;
        x |= x >> 1;
        x |= x >> 2;
        x |= x >> 4;
        x |= x >> 8;
        x |= x >> 16;
        return (x >> 1) + (x & 1);//x = 0, return 0.
    }
    
    int main(){
        assert(leftmost_one(0xff00) == 0x8000);
        assert(leftmost_one(0x6600) == 0x4000);
    }

### 2.67

A. The compiler can shift to overflow, or shift it after modulating the length of the data type. Different compilers will get different results. SUN's machine prompts that the number of shifts exceeds the length of the type, and its int may not be 32 bits.

B.
    #include <stdio.h>
    #include <assert.h>

    int bad_int_size_is_32(){
        int set_msb = 1 << 31;
        int beyond_msb = set_msb << 1;
        return set_msb && !beyond_msb;
    }
    
    int main(){
        assert(bad_int_size_is_32());
        return 0;
    }
C.
    #include <stdio.h>
    #include <assert.h>

    int bad_int_size_is_32_16(){
        int set_msb = 1 << 15 << 15 << 1;//不会溢出，可以在int为16位的机器上运行。
        int beyond_msb = set_msb << 1;
    
        return set_msb && !beyond_msb;
    }
    
    int main(){
        assert(bad_int_size_is_32());
        assert(bad_int_size_is_32_16());//int为32位返回1，否则返回0.
        return 0;
    }

### 2.68

    #include <stdio.h>
    #include <assert.h>
    
    int lower_one_mask(int n){
        int w = sizeof(int) << 3;
        return (unsigned)-1 >> (w - n);
    }
    
    int main(){
        assert(lower_one_mask(6) == 0x3f);
        assert(lower_one_mask(17) == 0x1ffff);
    }

### 2.69

    #include <stdio.h>
    #include <assert.h>
    #include <limits.h>
    
    unsigned rotate_left(unsigned x, int n){
        int w = sizeof(int) << 3;
        return x << w | x >> (w - n - 1) >> 1;//防止n=0时，溢出
    }
    
    int main(){
        unsigned res = rotate_left(0x12345678, 0);
        printf("%x\n", res);
        return 0;
    }

### 2.70

    #include <stdio.h>
    #include <assert.h>
    int fit_bits(int x, int n){
        int w = sizeof(int) << 3;
        return x << (w - n) >> (w - n) == x;
    }
    
    int main(){
        assert(fit_bits(16, 6));
        return 0;
    }

### 2.71
A.unsigned 先扩展为32位，再转换为int。所以该函数无法提取负数。
B. 
    #include <stdio.h>

    typedef unsigned packed_t;
    
    int xbyte(packed_t word, int bytenum){
        return (int) word << ((3 - bytenum) << 3) >> (3 << 3);
    }
    
    int main(){
        int res = xbyte(0x112233ff, 0);
        printf("%x\n", res);
    }

### 2.72
A.maxbytes 类型位int，sizeof(val) 类型位unsigned, 进行减法操作会先将maxbytes转换为unsigned,unsigned类型的值>=0,所以if判断永远为真。
B.判断条件改为if(maxbytes - (int)sizeof(val) >= 0)
    #include <stdio.h>
    #include <assert.h>
    #include <stdlib.h>
    #include <string.h>

    void copy_int(int val, void* buf, int maxbytes){
        if(maxbytes - (int)sizeof(val) >= 0)
            memcpy(buf, (void*) &val, sizeof(val));
    }
    
    int main(){
        int maxbytes = sizeof(int) * 10;
        void* buf = malloc(maxbytes);
    
        int test_num = 0xffffffff;
        copy_int(test_num, buf, maxbytes);
        assert(*(int*)buf == test_num);
    
        test_num = 0x12345678;
        copy_int(test_num, buf, 0);
        assert(*(int*)buf != test_num);
    
        return 0;
    }

### 2.73

    #include <stdio.h>
    #include <limits.h>
    #include <assert.h>
    int saturating_add(int x, int y){
        int sum = x + y;
        int sign_mask = INT_MIN;
    
        // x > 0, y > 0, sum < 0, 正溢出
        // x < 0, y < 0, sum > 0, 负溢出
        int pos_over = !(x & sign_mask) && !(y & sign_mask) && (sum & sign_mask);
        int neg_over = (x & sign_mask) && (y & sign_mask) && !(sum & sign_mask);
    
        (pos_over && (sum = INT_MAX) || neg_over && (sum = INT_MIN));
    
        return sum;
    }
    
    int main(){
        int x = 0x7fffffff;
        int y = 0x7fffffff;
        int sum = saturating_add(x, y);
        printf("%x\n", sum);
        x = 0x80000000;
        y = -0x1234;
        sum = saturating_add(x, y);
        printf("%x\n", sum);
        return 0;
    }

### 2.74

    #include <stdio.h>
    #include <assert.h>
    #include <limits.h>
    
    int tsub_ok(int x, int y){
        int diff = x - y;
        int sign_mask = INT_MIN;
    
        // x > 0, y < 0, diff < 0, pos_over
        // x < 0, y > 0, diff > 0, neg_over
        int pos_over = !(x & sign_mask) && (y & sign_mask) && (diff & sign_mask);
        int neg_over = (x &sign_mask) && !(y & sign_mask) && !(diff & sign_mask);
    
        return !(pos_over || neg_over);
    }
    
    int main(){
        int x = 0xffff;
        int y = INT_MIN;
    
        assert(!tsub_ok(x, y));
}

### 2.75

![](@attachment/Clipboard_2022-05-08-11-20-03.png)
>x', y'表示有符号数x,y对应的无符号数。
>书中P68，公式2.18
题目要求 通过x*y的高32位，求出x'*y'的高32位。
[详细解释](https://www.zhihu.com/question/45395026)

    #include <stdio.h>
    #include <assert.h>
    #include <inttypes.h>
    int signed_high_prod(int x, int y){
        int64_t mul = (int64_t) x * y;
        return mul >> 32; 
    }
    
    unsigned unsigned_high_prod(unsigned x, unsigned y){  
        int x_sign = x >> 31;
        int y_sign = y >> 31;
        return signed_high_prod(x, y) + x * y_sign + y * x_sign;
    }
    
    unsigned another_unsigned_high_prod(unsigned x, unsigned y){
        u_int64_t mul = (u_int64_t) x * y;
        return mul >> 32;
    }//该函数用来验证unsigned_high_prod的正确性
    
    int main(){
        unsigned x = 0x12345678;
        unsigned y = 0xffffffff;
        assert(another_unsigned_high_prod(x, y) == unsigned_high_prod(x, y));
        return 0;
}

### 2.76

    #include <stdio.h>
    #include <assert.h>
    #include <string.h>
    #include <stdint.h>
    #include <stdlib.h>
    
    void* calloc(size_t nmemb, size_t size){
        if(nmemb == 0 || size == 0)
            return NULL;
        size_t n = nmemb * size;
        if(n / nmemb == size){//检查溢出
            void* buf = malloc(n);
            if(buf != NULL)
                memset(buf, 0, n);
            return buf;
        }
        return NULL;
    }
    
    int main(){
        void *p;
        p = calloc(0x1234, 1);
        assert(p != NULL);
        free(p);
    
        p = calloc(SIZE_MAX, 2);
        assert(p == NULL);
        free(p);
        return 0;
    }

### 2.77
    #include <stdio.h>
    
    int main(){
        int x = 10;
        //A k = 17.
        int res = (x << 4)+ x;
        printf("%d\n", res);
        //B k = -7
        res = x - (x << 3);
        printf("%d\n", res);
        //C k = 60
        res = (x << 6) - (x << 2);
        printf("%d\n", res);
        //D k = -112
        res = (x << 4) - (x << 7);
        printf("%d\n", res);
    }

### 2.78
**书中P73**
有符号数除2的幂，正数直接右移，负数需要加偏移，保证整数除法向0舍入。
除以2的幂，(x < 0 ? x + (1<<k) - 1 : x) >> k;//x小于0要加上偏移量，以使结果向0舍入。利用如下属性：x/y向上取整 = (x+y-1)/y向下取整。

    #include <stdio.h>
    #include <limits.h>
    
    //直接右移，负数会向下舍入,(-7 >> 1 = -4),而C语言整数除法向0舍入(-7/2 = -3)。
    //x/2^k
    int divide_power2(int x, int k){
        int sig_mask = INT_MIN;
        int x_sig = x & sig_mask;
        (x_sig && (x = x + (1 << k) - 1));
        return x >> k;
    }
    
    int main(){
        int x = -7;
        int res = divide_power2(x, 1);
        printf("%d\n", res);
    
        x = 7;
        res = divide_power2(x, 1);
        printf("%d\n", res);
    
        return 0;
    }


### 2.79

    #include <stdio.h>
    #include <limits.h>
    #include <assert.h>
    
    int mul3div4(int x){
        x = (x << 1)+ x;
        int sig_mask = INT_MIN;
        int x_sig = x & sig_mask;
        (x_sig && (x = x + (1 << 2) - 1));
        return x >> 2;
    }
    
    int main(){
        int x = 0x87654321;
        assert(mul3div4(x) == x*3/4);
        return 0;
    }

### 2.80

    #include <stdio.h>
    #include <assert.h>
    #include <limits.h>
    
    //3/4x,向0舍入，不会溢出。先计算除法，再计算乘法。但10*3/4 = 7, 10/4 * 3 = 6.
    //采用一个巧妙方法，将x进行拆分
    int threefourths(int x){
        int sig_mask = INT_MIN;
        int x_sig = x & sig_mask;
        int bias = (1 << 2) - 1;
        
        int f = x & ~0x3;//取x前30位
        int l = x & 0x3;//取x后两位
        //x = f + l
    
        int fd4 = f >> 2;
        int fd4m3 = (fd4 << 1) + fd4;//f没有舍入问题
    
        int lm3 = (l << 1) + l;
        (x_sig && (lm3 = lm3 + bias));
        int lm3d4 = lm3 >> 2;
    
        return fd4m3 + lm3d4;
    }
    
    int main(){
      assert(threefourths(8) == 6);
      assert(threefourths(9) == 6);
      assert(threefourths(10) == 7);
      assert(threefourths(11) == 8);
      assert(threefourths(12) == 9);
    
      assert(threefourths(-8) == -6);
      assert(threefourths(-9) == -6);
      assert(threefourths(-10) == -7);
      assert(threefourths(-11) == -8);
      assert(threefourths(-12) == -9);
      return 0;
}

### 2.81
A. -1 << k
B. ~(-1 << k) << j
    #include <stdio.h>

    int main(){
        
        int k = 10;
        //A
        int x = -1 << k;
        printf("%x\n", x);
    
        int j = 5;
        //B
        x = ~(-1 << k) << j;
        printf("%.8x\n", x);
        return 0;
    }

### 2.82

A.F. x = INT_MIN, -x = INT_MIN
B.T ((x + y) << 4) + y - x = x << 4 - x + y<<4+y = 15x+17y
C.T ~x + ~y + 1 = ~x + 1 + ~y + 1 - 1 = -x + -y -1 = -(x + y) -1 = ~(x + y) + 1 - 1 = ~(x + y)
D.T (ux - uy) = -(uy - ux) = -(y - x) = -(unsigned)(y - x)
E.T (x >> 2) << 2 = x & ~0x3 = x - num(00/01/10/11) <= x.

### 2.83

A. x = 0.yyyyyy...
   x << k = y.yyyyy... = Y + x //一个y是k位， 并且Y = B2U(y).
   x << k - x = Y
   x = Y/(2^k - 1).
B.
(a)101 x = 5/7
(b)0110 x = 6/15 = 2/5
(c)010011 x = 19/63

### 2.84

#include <stdio.h>
#include <assert.h>

    unsigned f2u(float x){
        return *(unsigned*)&x;
    }
    
    int float_le(float x, float y){
        unsigned ux = f2u(x);
        unsigned uy = f2u(y);
    
        unsigned sx = ux >> 31;
        unsigned sy = uy >> 31;
    
        return (ux << 1 == 0 && uy << 1 == 0) /*x = 0 && y = 0*/
        || (sx && !sy)  /*sx < 0, sy > 0*/
        || (!sx && !sy && ux <= uy) /* x >=0 , y > 0 or x > =0 , y >= 0*/
        || (sx && sy && ux >= uy);/*x <= 0, y < 0 or x < 0, y <= 0*/
    }
    
    int main(){
        
        assert(float_le(1.0, 2.0));
        assert(float_le(+0, -0));
        assert(float_le(-2.0, -1.0));
    
        return 0;
    }

### 2.85
**bias = 2^(k - 1) - 1**
**V = 2^E * M**

A.7.0 -> 111.0 ->1.110 * 2^2
 E = 2, e = E + bias, M = 1.11, f = 0.11, V = 7.0
 位表示 0 10...01 110...0
B.2 ^(n + 1) - 1;
E= n， e = E + bias, M = 1.11...1, f = 0.11...1 V = 2 ^(n + 1) - 1.
位表示 0 bias+n 11...1(n个1)
C.2^(bias - 1)
最小规格化数 E = 1 - bias, M = 1.00...0, f = 0.00...0, V = 2^E*M = 2^(1 - bias), 
倒数:V^-1 = 2^(bias - 1), E = e - bias -> e = E + bias = 2*bias - 1
位表示 0 11..101 0...0

### 2.86
bias = 2^(k - 1) - 1 = 2^14 - 1
|描述|值|十进制
|:--:|:--:|:--:|
|最小正非规格化数| 0 0...0 0...1|2^(1-bias-63)|
|最小正规格化数|0 0...1 0...0|2^(1-bias)|
|最大规格化数|0 1...0 1...1|2^bias*(2 - 2^-63)|

### 2.87

|描述|Hex|M|E|V|D|
|:--:|:--:|:--:|:--:|:--:|:--:|
|-0|0x8000|0|-14|-0|-0.0|
|最小的>2的值|0x4001|1025/1024|1|2050/1024|2.001953125|
|512|0x6000|1.0|9|512|512.0|
|最大的非规格化数|0x03ff|1023/1024|-14|1023/2^24|6.09755516e-5|
|负无穷|0xFC00|-|-|-|-
|- |0x3BB0|123/64|-1|123/128|0.9609375

### 2.88

|格式A||格式B||
|:--:|:--:|:--:|:--:|
|位|值|位|值
|1 01110 001|-9/16|1 0110 0010|-9/16
|0 10110 101|208|0 1110 1010|208
|1 00111 110|-7/1024|1 0000 0111|-7/1024
|0 00000 101|5/2^17|0 0000 0001|1/2^10
|1 11011 000|-4096|1 1110 1111|-248
|0 11000 100|768|0 1111 0000|+∞

### 2.89

A.T
B.F x = 0, y = INT_MIN
C.T
D.F
E.F dx != 0, dz = 0.

    #include <stdio.h>
    #include <assert.h>
    #include <stdlib.h>
    #include <limits.h>
    
    int A(int x, double dx){
        return (float)x == (float)dx;
    }
    
    int B(int x, int y, double dx, double dy){
        return dx - dy == (double)(x - y);
    }
    
    int C(double dx, double dy, double dz){
        return (dx + dy) + dz == dx + (dy + dz);
    }
    
    int D(double dx, double dy, double dz){
        return (dx * dy) * dz == dx * (dy * dz);
    }
    
    int E(double dx, double dz){
        return dx/dx == dz/dz;
    }
    
    int main(){
        int x = random();
        int y = random();
        int z = random();
    
        double dx = (double) x;
        double dy = (double) y;
        double dz = (double) z;
    
        //A
        assert(A(x, dx));
        //B
        assert(!B(0, INT_MIN, (double)0, (double)INT_MIN));
        //C
        assert(C(dx, dy, dz));
        //D
        assert(!D((double)(int)0x64e73387, (double)(int)0xd31cb264, (double)(int)0xd22f1fcd));
        //E
        assert(!E(dx, 0));
        
    }

### 2.90

    #include <stdio.h>
    #include <assert.h>
    #include <math.h>
    
    float u2f(unsigned u){
        return *(float*)&u;
    }
    
    float fpwr2(int x){
        unsigned exp, frac;
        unsigned u;
    
        if(x < -149){
            exp = 0;
            frac = 0;
        }else if(x < -126){
            exp = 0;
            frac = (unsigned)1 << (x + 149);
        }else if(x < 128){
            exp = x + 127;
            frac = 0;
        }else{
            exp = 0xff;
            frac = 0;
        }
    
        u = exp << 23 | frac;
        return u2f(u);
    }
    
    int main(){
        assert(fpwr2(-150) == powf(2, -150));
        assert(fpwr2(-127) == powf(2, -127));
        assert(fpwr2(127) == powf(2, 127));
        assert(fpwr2(128) == powf(2, 128));
    }

### 2.91

A.0x40490FDB = 0100 0000 0100 1001 0000 1111 1101 1011
E = e - bias = 128 - 127 = 1, M = 1.100 1001 0000 1111 1101 1011
V = 2^E*M = 2 * 1.100 1001 0000 1111 1101 1011 = 11.00 1001 0000 1111 1101 1011

B.(参考2.83) 22/7 = 11.001001(001)...
C. 第九位

### 2.92

    #include <stdio.h>
    #include <assert.h>
    typedef unsigned float_bits;
    
    float_bits float_negate(float_bits f){
        unsigned sign = f >> 31;
        unsigned exp = f >> 23 & 0xff;
        unsigned frac = f & 0x7fffff;
        
        if(exp == 0xff && frac != 0)
            return f;
        return (~sign << 31) | (exp << 23) | frac;
    }
    
    int main(){
        float_bits x = 0x87654321;
        float_bits y = float_negate(x);
        printf("%.8x\n", y);
        return 0;
    }

### 2.93

    #include <stdio.h>
    #include <assert.h>
    typedef unsigned float_bits;
    
    float_bits float_absval(float_bits f){
        unsigned sign = f >> 31;
        unsigned exp = f >> 23 & 0xff;
        unsigned frac = f & 0x7fffff;
        
        if(exp == 0xff && frac != 0)
            return f;
        return (0 << 31) | (exp << 23) | frac;
    }

### 2.94

    #include <stdio.h>
    #include <assert.h>
    typedef unsigned float_bits;
    
    float_bits float_twice(float_bits f){
        unsigned sign = f >> 31;
        unsigned exp = f >> 23 & 0xff;
        unsigned frac = f & 0x7fffff;
        
        if(exp == 0xff && frac != 0)
            return f;
        
        if(exp == 0){//非规格化
            frac <<= 1;
        }else if(exp == 0xff - 1){//twice is ∞。
            exp = 0xff;
            frac = 0;
        }else{
            exp = exp + 1;
        }
        
        return (sign << 31) | (exp << 23) | frac;
    }

### 2.95

    #include <stdio.h>
    #include <assert.h>
    typedef unsigned float_bits;
    
    float_bits float_half(float_bits f){
        unsigned sign = f >> 31;
        unsigned exp = f >> 23 & 0xff;
        unsigned frac = f & 0x7fffff;
        unsigned rest = f & 0x7fffffff;
    
        if(exp == 0xff && frac != 0)
            return f;
        /*
            向偶数舍入，只关心最后两位
            00 -> 0
            01 -> 0
            10 -> 1
            11 -> 2 (右移 + 1)
        */
        unsigned add = ((f & 0x3) == 0x3); //当后两位为11时，需要加1
    
        if(exp == 0){//非规格化
            frac >>= 1;
            frac += add;    
        }else if(exp == 1){//规格化转非规格化
            rest >>= 1;
            rest += add;
            exp = rest >> 23 & 0xff;
            frac = rest & 0x7fffff;
        }else{
            exp = exp - 1;
        }
        
        return (sign << 31) | (exp << 23) | frac;
    }

### 2.96

    #include <stdio.h>
    #include <assert.h>
    #include <math.h>
    typedef unsigned float_bits;
    
    int float_f2i(float_bits f){
        unsigned sign = f >> 31;
        unsigned exp = f >> 23 & 0xff;
        unsigned frac = f & 0x7fffff;
        unsigned bias = 0x7f;
    
        int num;
        unsigned E;
        unsigned M;
    
        if(exp >= 0 && exp < bias){//小于1的数,转换为int为0.
            num = 0;
        }else if(exp >= 31 + bias){//overflow，大于等于2^31 or 小于等于-2^31.
            num = 0x80000000;
        }else{//都是规格化数
            E = exp - bias;
            M = frac | 0x800000;//相当于尾数小数点已经右移23位
            if(E > 23){//向0舍入。
                num = M << (E - 23);
            }else{
                num = M >> (23 - E);
            }
        }
        return sign ? -num : num;
    }

### 2.97
**困难**

    #include <stdio.h>
    #include <assert.h>
    #include <math.h>
    #include <limits.h>
    
    typedef unsigned float_bits;
    
    //i > 0, 计算i的bit长度，例如 i = 7， bits_length(7) = 3. i = 0x80ff,  bits_length(i) = 16.
    int bits_length(int i){
        if((i & INT_MIN) != 0){//防止while陷入死循环。
            return 32;
        }
        int length = 0;
        unsigned u = (unsigned) i;
        while(u >= (1 << length)){ 
            length++;
        }
        return length;
    }
    
    /*
    3  => 0x00000007
    16 => 0x0000FFFF
    */
    unsigned bits_mask(int l){
        return (unsigned)-1 >> (32 - l);
    }
    
    float_bits float_i2f(int i){
        unsigned sign, exp, frac, bits, fbits, rest;
        unsigned bias = 0x7f;
    
        if(i == 0){
            sign = 0;
            exp = 0;
            frac = 0;
            return (sign << 31) | (exp << 23) || frac;
        }
        if(i == INT_MIN){
            sign = 1;
            exp = bias + 31;
            frac = 0;
            return (sign << 31) | (exp << 23) || frac;
        }
    
        sign = 0;
        if(i < 0){
            sign = 1;
            i = -i;
        }
    
        bits = bits_length(i);//i的二进制表示，最左边1到最右边的长度。
        fbits = bits - 1;//规格化数暗含1, E = fbits 
        exp = fbits + bias;// E = exp - bias
        rest = i & bits_mask(fbits);//取i的后fbits位
    
        if(fbits <= 23){//不需要舍入。
            frac = rest << (23 - fbits);//将rest中第一个1移到后23位中的第一个位置。
        }else{//需要向偶数舍入，offset为要舍去的位，当要舍去的部分为100..形式时进行判断。
            int offset = fbits - 23;
            int round_mid = 1 << (offset - 1);
            frac = rest >> offset;
    
            unsigned round_part = rest & bits_mask(offset);//该部分为要舍入的部分
            if(round_part == round_mid){
                if((frac & 0x1) == 1){//frac末位为1，则加1，为0，则不变。
                    frac += 1;
                }
            }else if(round_part > round_mid){
                frac += 1;
            }
        }
    
        return (sign << 31) | (exp << 23) || frac;
    }

**2022.05.10**
