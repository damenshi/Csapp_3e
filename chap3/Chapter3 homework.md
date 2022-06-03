---
attachments: [Clipboard_2022-05-24-23-26-34.png, Clipboard_2022-05-24-23-30-03.png, Clipboard_2022-05-24-23-30-25.png, Clipboard_2022-05-24-23-30-34.png, Clipboard_2022-05-24-23-30-59.png, Clipboard_2022-05-26-10-23-18.png, Clipboard_2022-05-26-10-23-46.png, Clipboard_2022-05-26-10-56-13.png, Clipboard_2022-05-26-17-32-02.png]
tags: [csapp/05-09]
title: Chapter3 homework
created: '2022-05-24T14:01:26.349Z'
modified: '2022-06-03T15:36:36.117Z'
---

# Chapter3 homework

### 3.58
x in %rdi, y in %rsi, z in %rdx

    1  decode2:
    2     subq   %rdx, %rsi   // y -= z
    3     imulq  %rsi, %rdi   // x *= y
    4     movq   %rsi, %rax   // y -> %rax
    5     salq   $63,  %rax   // y <<= 63
    6     sarq   $63,  %rax   // y >>= 63
    7     xorq   %rdi, %rax   // y ^= x
    8     ret

    long decode2(long x, long y, long z){
        y -= z;
        x *= y;
        y <<= 63;
        y >>= 63;
        y ^= x;
        return y;
    }

    参考：
    long decode2(long x, long y, long z){
        y -= z;
        x *= y;
        return (y % 2 == 0 ? x : ~x);//y为偶数返回x，奇数返回～x。
    }

### 3.59
assume
![](@attachment/Clipboard_2022-05-24-23-26-34.png)
![](@attachment/Clipboard_2022-05-24-23-30-03.png)
multiple
![](@attachment/Clipboard_2022-05-24-23-30-25.png)
![](@attachment/Clipboard_2022-05-24-23-30-34.png)
2^128 overflows, so 
![](@attachment/Clipboard_2022-05-24-23-30-59.png)

    void store_prod(int128_t, *dest, int64_t x, int64_t y){
        *dest = x * (int128_t) y;
    }
    *dest in %rdi, x in %rsi, y in %rdx

    x63 表示x的第63位， y63 表示x的第63位，
    1  store_prod:
    2     movq    %rdx, %rax    // %rax = y
    3     cqto                  //cqto指令不需要操作数，它隐含读出%rax的符号位，并将它复制到%rdx的所有位 (int128_t)y, %rdx = -1(y63)        
    4     movq    %rsi, %rcx    //%rcx = x
    5     sarq    $63,  %rcx    // %rcx = -1(x63)
    6     imulq   %rax, %rcx    // %rcx = -1(x63)*y
    7     imulq   %rsi, %rdx    // %rdx = -1(y63)*x
    8     addq    %rdx, %rcx    // %rdx = (-x63) * y + x * (-y63)
    9     mulq    %rsi          // mulq 一个操作数 为无符号全乘法 ux * uy
    R[%rdx]:R[%rax] <-%rsi * R[%rax]
    10    addq    %rcx, %rdx    // %rdx = ux * uy(high 64bits)-(x63*y + y63*x) 
    11    movq    %rax, (%rdi)  // R[%rax] -> M[%rdi] ux * uy 和 x * y低64位相同，第二章.
    12    movq    %rdx, 8(%rdi) // R[%rdx] -> M[%rdi + 8]
    13    ret

### 3.60

    x in %rdi, n in %esi
    1   loop:
    2     movl  %esi, %ecx    // %ecx = n
    3     movl  $1, %edx      // %edx = 1
    4     movl  $0, %eax      // %eax = 0
    5     jmp   .L2
    6   .L3:
    7     movq  %rdi, %r8     // %r8 = x
    8     andq  %rdx, %r8     // %r8 = x & %rdx
    9     orq   %r8,  %rax    // %rax = %rax | %r8
    10    salq  %cl,  %rdx    // %rdx = %rdx << %cl
    11  .L2:
    12    testq %rdx, %rdx    // test %rdx
    13    jne   .L3           // != 0
    14    rep; ret


    long loop(long x, int n){
        long result = 0;
        long mask;
        for(mask = 1; mask != 0; mask <<= n){
            result |= (x & mask);
        }
        return result;
    }

A. x in %rdi, n in %esi, result in %rax, mask in %rdx
B. result 初始值为0， mask初始值为1.
C. mask != 0
D. mask <<= n
E. result |= (x & mask)

+ 书中p131 x86-64中，移位操作对w位长的数据值进行操作，移位量是由%cl寄存器的低m位决定的，这里2m=w。高位会被忽略。所以salb移位量由%cl低3位、salw移位量由%cl低4位、sall移位量由%cl低5位、salq移位量由%cl低6位决定。mask <<= (n&0x3f),不显式表达也可以，编译器会进行处理。

### 3.61

    long cread_alt(long *xp){
        return (!xp ? 0 : *xp);
    }

    cread_alt:
        movl  $0, %eax
        testq %rdi, %rdi
        cmovne (%rdi), %rax //条件传送指令
        ret

### 3.62
    typedef enum {MODE_A, MODE_B, MODE_C, MODE_D, MODE_E} mode_t;

    long switch3(long *p1, long *p2, mode_t action){
        long result = 0;
        switch(action){
            case MODE_A:
              result = *p2;
              *p2 = *p1;
              break;
            case MODE_B:
              result = *p1 + *p2;
              *p1 = result;
              break;
            case MODE_C:
              *p1 = 59;
              result = *p2;
              break;
            case MODE_D:
              *p1 = *p2;
              result = 27;
              break;
            case MODE_E:
              result = 27;
              break;
            default:
              result = 12;
              break;
        }
        return result;
    }

    1   .L8: MODE_E
    2     movl  $27, %eax       //%eax = 27
    3     ret
    4   .L3: MODE_A
    5     movq  (%rsi), %rax    //%rax = *p2
    6     movq  (%rdi), %rdx    //%rdx = *p1
    7     movq  %rdx, (%rsi)    //*p2 = *p1
    8     ret
    9   .L5: MODE_B
    10    movq  (%rdi), %rax    //%rax = *p1
    11    addq  (%rsi), %rax    //%rax = *p1 + *p2
    12    movq  %rax, (%rdi)    //*p1 = *p1 + *p2
    13    ret
    14  .L6: MODE_C
    15    movq  $59, (%rdi)     //*p1 = 59
    16    movq  (%rsi), %rax    //%rax = *p2
    17    ret
    18  .L7: MODE_D
    19    movq  (%rsi), %rax    //%rax = *p2
    20    movq  %rax, (%rdi)    //*p1 = *p2;
    21    movl  $27, %eax       //%eax = 27
    22    ret
    23  .L9: default
    24    movl  $12, %eax      // %eax = 12
    25    ret

### 3.63
    long switch_prob(long x, long n){
        long result = x;
        switch(n){
          case 60:
          case 62:
            result = 8*x;
            break;
          case 63:
            result = x >> 3;
            break;
          case 64:
            x = x << 4 - x;
          case 65:
            x *= x;
          default:
            result = x + 75;
            break;
        }
        return result;
    }

    1  switch_prob
    2   sub   $0x3c, %rsi               //%rsi = n - 60  
    3   cmp   $0x5, %rsi                //cmp n-60 5  
    4   ja    4005c3<switch_prob + 0x33> // n > 65 jump to 17
    5   jmpq  *0x4006f8(, %rsi,8)       // 跳转表
    6   lea   0x0(,%rdi,8), %rax        //%rax = 8*x
    7
    8   retq
    9   mov   %rdi, %rax                //%rax = x
    10  sar   $0x3, %rax                //%rax = x >> 3
    11  retq
    12  mov   %rdi, %rax                //%rax = x
    13  shl   $0x4, %rax                //%rax = x << 4
    14  sub   %rdi, %rax                //%rax = x << 4 - x
    15  mov   %rax, %rdi                //%rdi = x << 4 - x
    16  imul  %rdi, %rdi                //%rdi = x * x
    17  lea   0x4b(%rdi), %rax   // %rax = x + 75
    18  retq

+ 根据1 2 可知case为60 ～ 65，根据跳转表地址可分析出跳转位置。

### 3.64
> T D[R][C]
> 它的数组元素D[i][j]的内存地址为： &D[i][j] = Xd + L(C*i + j)，L是数据类型T以字节为单位的大小。（3.1）

A. 
long A[R][S][T]
&A[i][j][k] = Xa + L*(i*S*T + j*T + k)

B.
long store_ele(long i, long j, long k, long *dest){
    *dest = A[i][j][k];
    return sizeof(A);
}
1  store_ele:
2     leaq  (%rsi, %rsi, 2), %rax  // %rax = 3*j
3     leaq  (%rsi, %rax, 4), %rax   //%rax = 4*3*j + j = 13*j
4     movq  %rdi, %rsi              //%rsi = i
5     salq  $6, %rsi                //%rsi = i << 6
6     addq  %rsi, %rdi              //%rdi = i + i << 6 = 65*i
7     addq  %rax, %rdi              //%rdi = 65*i + 13*j
8     addq  %rdi, %rdx              //%rdx = k + 65i + 13j
9     movq  A(,%rdx,8), %rax        //%rax = M[A + 8(65i + 13j + k)]
10    movq  %rax, (%rcx)            //*dest = A[i][j][k]
11    movl  $3640, %eax             //%eax = 3640
12    ret 

由汇编代码可知，T= 13， S= 5. sizeof(A) = 8 * R * S * T = 3640,可知R= 3640/520 = 7.

### 3.65

    1 .L6:
    2   movq  (%rdx), %rcx      //%rcx = M[%rdx]
    3   movq  (%rax), %rsi      //%rsi = M[%rax]
    4   movq   %rsi, (%rdx)     //M[%rdx] = %rsi
    5   movq   %rcx, (%rax)     //M[%rax] = %rcx
    6   addq   $8, %rdx         //%rdx += 8 每次地址偏移为8 A[i][j]->A[i][j+1]
    7   addq   $120, %rax       //%rax += 120 A[j][i]->A[j+1][i]
    8   cmpq   %rdi, %rax       
    9   jne    .L6  

由6，7 可知，
A. A[i][j]保存在%rdx，
B. A[j][i]保存在%rax。
C. M = 120 / 8 = 15.

### 3.66

    1  sum_col:
    2     leaq  1(,%rdi,4), %r8         // %r8 = 4n + 1
    3     leaq  (%rdi, %rdi, 2), %rax   // %rax = 3n
    4     movq  %rax, %rdi              // %rdi = 3n
    5     testq %rax，%rax              // 
    6     jle   .L4                     // if 3n <= 0, jump L4
    7     salq  $3, %r8                 // %r8 = (4n + 1) * 8
    8     leaq  (%rsi, %rdx, 8), %rcx   // %rcx = 8j + A
    9     movl  $0, %eax                // %eax = 0;
    10    movl  $0, %edx                // %edx = 0;
    11  .L3:
    12    addq  (%rcx), %rax            // %rax = M[8j + A]
    13    addq  $1, %rdx                // %rdx += 1; (i++)
    14    addq  %r8, %rcx               // %rcx = 8j + A + (4n + 1)*8
    15    cmpq  %rdi, %rdx              // cmp %rdi & %rdx
    16    jne   .L3                     // %rdi != %rdx (i != 3n)
    17    rep;ret
    18  .L4:
    19    movl  $0, %eax                //%eax = 0
    10    ret

    NC(n) = 4*n + 1
    NR(n) = 3*n


### 3.67

    `strB process(strA s)
    1   procesee:
    2     movq  %rdi, %rax      //%rax = %rsp + 64
    3     movq  24(%rsp), %rdx  //%rdx = M[%rsp + 24] = z
    4     movq  (%rdx), %rdx    //%rdx = M[%rdx]
    5     movq  16(%rsp), %rcx  //%rcx = M[%rsp + 16] = &z
    6     movq  %rcx, (%rdi)    //M[%rdi] = &z
    7     movq  8(%rsp), %rcx   //%rcx = M[%rsp + 8]=y
    8     movq  %rcx, 8(%rdi)   //M[%rdi + 8] = y
    9     movq  %rdx, 16(%rdi)  //M[%rdi + 16] = 
    10    ret


    1   eval:
    2     subq  $104, %rsp      //%rsp -= 104
    3     movq  %rdx, 24(%rsp)  //M[%rsp + 24] = z
    4     leaq  24(%rsp), %rax  //%rax = %rsp + 24
    5     movq  %rdi, (%rsp)    //M[%rsp] = x
    6     movq  %rsi, 8(%rsp)   //M[%rsp + 8] = y
    7     movq  %rax, 16(%rsp)  //M[%rsp + 16] = %rsp + 24
    8     leaq  64(%rsp), %rdi  //%rdi = %rsp + 64
    9     call  process
    10    movq  72(%rsp), %rax  //%rax = M[%rsp + 72]
    11    addq  64(%rsp), %rax  //%rax = M[%rsp + 72] + M[%rsp + 64]
    12    addq  80(%rsp), %rax  //%rax = M[%rsp + 72] + M[%rsp + 64] + M[%rsp + 80]
    13    addq  $104, %rsp //%rsp += 104
    14    ret

A.
![](@attachment/Clipboard_2022-05-26-10-23-46.png)
B.传递了%rsp + 64
C.通过%rsp + offset访问s的元素
D.eval 传入%rsp + 64，procee从该位置起向栈中存入r的字段
E.
![](@attachment/Clipboard_2022-05-26-10-56-13.png)
+ 过程调用要先将返回地址保存在栈中
F.调用函数分配栈空间，并将该空间地址传给被调用函数。被调用在该空间内存储数据，并返回该地址。

### 3.68

    1 setVal:
    2   movslq  8(%rsi), %rax  //%rax = M[8 + q]
    3   addq    32(%rsi), %rax  //%rax = M[8 + q] + M[32 + q]
    4   movq    %rax, 184(%rdi) //M[p + 184] =  M[8 + q] + M[32 + q] 
    5   ret

+ 应用数据对齐原理。
> 根据汇编代码，q->t的地址为 q + 8, char array[B]的地址空间为q～q + 8，最多8个(如果B小于4，q->t的地址应为q + 4， int向4对齐)， 可知 4<= B <= 8.。
q->u的地址为q + 32，short s[A]的地址空间为q + 12 ～ q + 32，最多为10个（如果A小于6，那么q->u的地址应该为q + 24，long类型向8对齐）， 可知 6 <= A <= 10。

> 根据汇编代码，p->y的地址为 p + 184， int x[A][B]的地址空间为p ～ p + 184.
因为long向8对齐，所以int x[A][B]最少占176字节，否则p->y的地址应为p + 176.
所以， 176 <= A\*B\*4 <= 184.

> 44<= A*B << 46
6 <= A <= 10
4 <= B <= 8
可求得， A = 9， B = 5 满足条件。  

### 3.69

    void test(long i, b_struct *bp)

    1   test:
    2     mov   0x120(%rsi), %ecx         //%ecx = *(bp + 288)
    //bp->last = *(bp + 288)
    3     add   (%rsi), %ecx              //%ecx = *(bp + 0x120) + *bp
    //n = bp->first + bp->last
    4     lea   (%rdi, %rdi, 4), %rax     //%rax = 5*i
    5     lea   (%rsi, %rax, 8), %rax     //%rax = 40i + bp
    6     mov   0x8(%rax), %rdx           // %rdx = *(bp + 0x8 + 40i) ap->idx
    //*ap = &bp->a[i], bp + 0x8为a的首地址，加0x8是为了跳过first.因为idx是a_struct的第一个元素，所以%rdx放的是idx的值。
    7     movslq  %ecx, %rcx              // %rcx = n
    8     mov     %rcx, 0x10(%rax, %rdx, 8) //M[%rax + 0x10 + 8*idx] = %rcx
    //ap->x[ap->idx] = n, %rax + 0x10 = bp + 0x8 + 40i + 0x8 ，跳过了first和idx，即为a_struct中数组x的首地址，8*idx说明x中元素为long型（题中说是有符号数）。
    9     retq

A.
bp->last的地址为bp + 288， bp->first的地址为bp，所以a_struct a[CNT]的地址空间为bp + 8 ～ bp + 288，空4byte用于数据对齐，由汇编代码5可知，sizeof(a_struct) = 40
280 / 40 = 7.所以CNT = 7.

B.

    typedef struct{
        long idx;
        long x[4];
    } a_struct;

因为sizeof(a_struct) = 40，所以x有4个元素。

### 3.70
A. 
e1.p  0
e1.y  8
e2.x  0
e2.next 8
B. 16byte
C.
1   proc:
2     movq  8(%rdi), %rax   
      //%rax = *(up + 8) is y or next.由2知，%rax是指针，所以%rax = up->e2.next
3     movq  (%rax), %rdx    
      //由4知，%rdx是指针，所以%rdx = *(up->e2.next).e1.p
4     movq  (%rdx), %rdx    //%rdx = *(*(up->e2.next).e1.p)    
5     subq  8(%rax), %rdx   
      //%rdx is long, 所以8(%rax)也是long，8(%rax) = *(up->e2.next).e1.y
6     movq  %rdx, (%rdi)    //(%rdi) = up->e2.x
7     ret

void proc(union ele *up){
    up->e2.x = *(*(up->e2.next).e1.p) - *(up->e2.next).e1.y
}

### 3.71
    #include <stdio.h>
    #define BUF_SIZE 12

    void good_echo(){
        char buf[BUF_SIZE];
        while(1){
            char *p = fgets(buf, BUF_SIZE, stdin);
            if(p == NULL)
                break;
            printf("%s", p);
        }
        return;
    }

    int main(){
        good_echo();
        return 0;
    }

### 3.72

+ [参考](https://github.com/aQuaYi/CSAPP3E/blob/master/homework/03/3.72.md):详细推导过程
+ 栈帧结构
![](@attachment/Clipboard_2022-05-26-17-32-02.png)

    1   aframe
    2     pushq   %rbp                  //store %rbp
    3     movq    %rsp, %rbp            //%rbp = %rsp 帧指针
    4     subq    $16, %rsp             //%rsp = %rsp - 16 s1
    5     leaq    30(, %rdi, 8), %rax   //%rax = 8*n + 30
    6     andq    $-16, %rax            //%rax = %rax &(0xf...f0)
    7     subq    %rax, %rsp            //%rsp -= %rax  s2
    8     leaq    15(%rsp), %r8         //%r8 = s2 + 15
    9     andq    $-16, %r8             //%r8 &= 0xf...f0

A. 
s2 = s1 -（(8*n + 30) & 0xfffffff0）
当n为奇数时，n = 2*k-1, s2 = s1 - (8*n + 24) (1)
当n为偶数时，n = 2*k, s2 = s1 - (8*n + 16) (2).
k = 1, 2...
由 s2 + e2 + 8*n + e1 = s1,得
n = 2*k - 1, e1 + e2 = s1 - s2 - 8*n = 24 (3)
n = 2*k,     e1 + e2 = s1 - s2 - 8*n = 16 (4)

B.
p = (s2 + 15) & 0xfffffff0
比s2大的最小的16的倍数。

    p = (s2 + 15) - (s2 + 15)%16  (5)
    e2 = p - s2 = 15 - (s2 + 15)%16 (6)

C. 
当 n = 2*k - 1,由公式3

    e1 = 24 - e2 (代入6)
      = 24 - 15 + (s2 + 15)%16
      = 9 + (s2 + 15)%16 (代入1)
      = 9 + (s1 - 8*n - 24 + 15)%16
      = 9 + (s1 - 8*n - 9)%16 (代入n = 2*k - 1)
      = 9 + (s1 - 8*(2*k - 1) - 9)%16
      = 9 + (s1 - 16k -1)%16
      = 9 + (s1 - 1)%16
当 (s1 - 1)%16 = 0时， 即s1%16 = 1, e1取最小值，e1 = 9.
当(s1 - 1)%16 = 15时，即s1%16 = 0,e1取最大值，e1 = 24.


当n = 2*k,由公式4

    e1 = 16 - e2
      = 16 - 15 + (s2 + 15)%16
      = 1 + (s2 + 15)%16
      = 1 + (s1 - 8*n -16 + 15)%16
      = 1 + (s1 - 8*n - 1)%16
      = 1 + (s1 - 16*k - 1)%16
      = 1 + (s1 - 1)%16
当(s1 - 1)%16 = 0时，即s1%16 = 1,e1取最小值，e1 = 1.
当(s1 - 1)%16 = 15时,即s1%16 = 0时，e1取最大值，e1 = 16. 

综上
n = 2*k, s1%16 = 1时， e1取最小值，e1 = 1.
n = 2*k + 1, s1%16 = 0时，e1取最大值，e1 = 24.

D.

由p = (s2 + 15) - (s2 + 15)%16  (5)可知，p向16对齐。
由s1 - s2 = 16*(k + 1),s2和s1总是相差16的倍数。

### 3.73

[内联汇编](https://www.jianshu.com/p/1782e14a0766)
+ 基本內联汇编

>例如:
asm("movl %ecx, %eax"); /* 把 ecx 内容移动到 eax */ __asm__("movb %bh , (%eax)"); /* 把bh中一个字节的内容移动到eax指向的内存 */

>你可能注意到了这里使用了两个不同的关键字 asm 和 __asm__。这两个关键字都可以使用。不过当遇到asm关键字与程序其他变量有冲突的时候就必须用__asm__了。如果内联汇编有多条指令，则每行要加上双引号，并且该行要以\n\t结尾。这是因为GCC会将每行指令作为一个字符串传给as(GAS)，使用换行和TAB可以将正确且格式良好的代码行传递给汇编器。

    __asm__ ( "movl %eax, %ebx\n\t"
                    "movl $56, %esi\n\t"
                    "movl %ecx, $label(%edx,%ebx,$4)\n\t"
                    "movb %ah, (%ebx)");



> PF(奇偶标志:反映运算结果低8位中“1”的个数。“1”的个数为偶数，则PF置1，否则置0)
如果PF为1则JP跳转指令可以跳转到目标位置 JNP相反..
+ 以下代码在linux环境运行
    #include <stdio.h>
    #include <assert.h>

    typedef enum {NEG, ZERO, POS, OTHER} range_t;

    range_t find_range(float x){
      __asm__(
          "vxorps %xmm1, %xmm1, %xmm1\n\t" // %xmm1 = 0
          "vucomiss %xmm1, %xmm0\n\t" //cmp x  0
          // 当 x 为 NaN 时
          // vucomiss 会把 CF，ZF 和 PF 全部设置为 1
          // 所以，需要最先运行 jp 命令
          "jp .P\n\t"
          "jb .B\n\t"
          "je .E\n\t"
          "ja .A\n\t"
          ".B:\n\t" // x < 0
          "movl $0, %eax\n\t"
          "jmp .done\n\t"
          ".E:\n\t" // x == 0
          "movl $1, %eax\n\t"
          "jmp .done\n\t"
          ".A:\n\t" // x > 0
          "movl $2, %eax\n\t"
          "jmp .done\n\t"
          "movl $3, %eax\n\t"// x == Nan
          ".done:\n\t"
      );  
    }

    int main(){
      range_t n = NEG, z = ZERO, p = POS, o = OTHER;
      assert(find_range(0.0/0.0) == o);
      assert(find_range(-2.3) == n);
      assert(find_range(0.0) == z);
      assert(find_range(2.3) == p);
      return 0;
    }

### 3.74

    #include <stdio.h>
    #include <assert.h>

    typedef enum {NEG, ZERO, POS, OTHER} range_t;

    range_t find_range(float x){
        __asm__(
            "vxorps %xmm1, %xmm1, %xmm1\n\t" // %xmm1 = 0
            "movq $1, %rax\n\t" // x == 0
            "movq $0, %r8\n\t"
            "movq $2, %r9\n\t"
            "movq $3, %r10\n\t"

            "vucomiss %xmm1, %xmm0\n\t" //cmp x  0
            
            "cmovpq %r10, %rax\n\t" // x == Nan
            "cmovbq %r8, %rax\n\t" // x < 0
            "cmovaq %r9, %rax\n\t" // x > 0
        );  
    }

    int main(){
        range_t n = NEG, z = ZERO, p = POS, o = OTHER;
        assert(find_range(0.0/0.0) == o);
        assert(find_range(-2.3) == n);
        assert(find_range(0.0) == z);
        assert(find_range(2.3) == p);
        return 0;
    }

### 3.75
A.
|参数位置|实部|虚部|
|:-:|:-:|:-:|
|1|%xmm0|%xmm1|
|2|%xmm2|%xmm3|
|3|%xmm4|%xmm5|
|...|...|...|

B.
%xmm0保存实部，%xmm1保存虚部。

22.05.27








