---
tags: [csapp/05-09]
title: chap3 note
created: '2022-05-15T11:18:45.345Z'
modified: '2022-06-03T15:36:26.014Z'
---

# chap3 note
[B站课程视频地址](https://www.bilibili.com/video/BV1iW411d7hd?p=5)

### 1.程序计数器（通常称为PC，在x86-64中用%rip表示）给出将要执行的下一条指令在内存中的地址。

### 2.x86-64的虚拟地址是由64位的字来表示的，在目前实现中，这些地址的高16位必须设置为0，所以一个地址实际能够指定的是2^48范围内的一个字节。

### 3.一个x86-64的cpu包含一组16个存储64位值的通用目的寄存器，这些寄存器用来存储整数数据和指针。

### 4. Imm(rb, ri, s)表示最常用的寻址模式， Imm为一个立即数偏移，rb为基址寄存器，ri为变址寄存器，s为比例因子（必须是1、2、4、或8），有效地址被计算为Imm + R[rb] + R[ri] * s。

### 5.栈向低地址方向增长，压栈是减下栈指针（寄存器%rsp）的值，并将数据存放到内存中，出栈是从内存中读取数据，并增加栈指针的值。

### 6.leaq指令不改变任何条件码。cmp和test只设置条件码

### 7. 参数位置：对应寄存器
       1 : %rdi, 2 : %rsi, 3 : %rdx, 4 : %rcx, 5 : %r8, 6 : %r9
       超过六个参数，通过栈进行传递。

### 8. C语言允许对指针进行运算，计算出来的值会根据该指针引用的数据类型的大小进行伸缩。

### 9.union提供了一种方式，能够规避C语言的类型系统，允许以多种类型来引用一个对象。

### 10.数据对齐：对齐原则是任何K字节的基本对象的地址必须是K的倍数。



### 练习题

#### 3.1 略

#### 3.2
*寄存器部分的大小必须与指令最后一个字符(b, w, l, q)指定的大小匹配*
1. movl
2. movw
3. movb
4. movb
5. movq %rax是4个字的，所以应该是movq
6. movw

#### 3.3
1. 内存引用总是由四字长寄存器给出，改为 movb $0xF, (%rbx)
2. movl与%rax不匹配，改为 movl %eax, (%rsp) or movq %rax, (%rsp)
3. 传送指令的两个操作数不能同时指向内存。
4. 没有%sl这个寄存器
5. 立即数不能作为目的操作数
6. 目的操作数大小不对，改为 movl %eax,%edx
7. movb 和 %si 大小不匹配。改为movw %si,8(%rbp) 或movb %sil,8(%rbp)

#### 3.4
|src_t|dest_t|指令|
|:-:|:-:|:-:|
|long|long|movq (%rdi),%rax. movq %rax,(%rsi)|
|char|int|movsbl (%rdi),%eax. movl %eax,(%rsi)|
|char|unsigned|movsbl (%rdi),%eax. movl %eax,(%rsi)|
|unsigned char|long|movzbl (%rdi),%eax. movq %rax,(%rsi)|
|int|char|movl (%rdi),%eax. movb %al,(%rsi)|
|unsigned|unsigned char|movl (%rdi),%eax. movb %al,(%rsi)|
|char|short|movsbw (%rdi),%ax. movw %ax,(%rsi)|

+ 从小到大，无符号数先进行零扩展，有符号数先进行符号扩展。
+ unsigned -> char  第一条指令为什么不是movzbq (%rdi),%rax?
网上解释说是优化cpu效率，使用movzbl (%rdi),%eax.隐含的将%rdi高位4字节设置为0.

+ int->char 从大到小，先直接全部读，然后再按照目标类型大小截断即可

#### 3.5

    void decode1(long *xp, long *yp, long *zp){
        long x = *xp;
        long y = *yp;
        long z = *zp;

        *yp = x;
        *zp = y;
        *xp = z;
    }

#### 3.6

|表达式|结果|
|:-:|:-:|
|leaq 6(%rax),%rdx|x + 6|
|leaq (%rax,%rcx),%rdx|x+y|
|leaq (%rax,%rcx,4),%rdx|4*y + x|
|leaq 7(%rax,%rax,8),%rdx|9*x + 7|
|leaq 0xA(,%rcx,4),%rdx|4*y + 10|
|leaq 9(%rax,%rcx,2),%rdx|2*y + x + 9|

#### 3.7略
#### 3.8略

#### 3.9

shift_left4_rightn:
  movq %rdi, %rax    get x
  salq $4, %rax      x <<= 4
  movl %esi, %ecx    get n(4 bytes) //只有最低字节才指示位移量
  sarq %cl, %rax     x >>= n 

+ 位移量可以是立即数，或者放在单字节寄存器%cl中。 

#### 3.10 略

#### 3.11
A. 将%rdx设置为0. x^x = 0
B. move $0,%rdx.

#### 3.12

    uremdiv:
      movq %rdx, %r8    copy qp
      movq %rdi, %rax
      movl $0,%edx      %rdx设置为0
      divq %rsi
      movq %rax, (%r8)
      movq %rdx, (%rcx)
      ret

#### 3.13

A. cmpl %esi,%edi 
   setl %al
   data_t(int),COMP(<)

B.cmpw %si,%di 
  setge %al
  data_t(short), COMP(>=)

C.cmpb %sil,%dil
  setbe %al
  data_t(unsigned char), COMP(<=)

D.cmpq %rsi, %rdi
  setne %a
  data_t(long), COMP(!=)
  data_t(unsigned long), COMP(!=)
  data_t(某种形式指针)，COMP(!=)
#### 3.14

A. data_t(long) TEST(>=)

B. data_t(short) TEST(==)
   data_t(unsigned short) TEST(==)

C. data_t(unsigned char) TEST(>)

D. data_t(int) TEST(!=)
   data_t(unsigned int) TEST(!=)

#### 3.15
 
A. 4003fe
B. 400425
C. ja:400543 pop: 400545
D. 400560 (0x4005ed-141 = 0x400560)

#### 3.16
A. 
    void gotocond(long a, long *p){
      if(p & p == 0){
        goto done;
      }
      if(*p >= a){
        goto done;
      }
      *p = a;
      done:
        return;
    }
B. if语句包含两个条件，如果对p非空测试失败，会跳过对 a > *p的测试。

#### 3.17

A.
    long gotodiff_se(long x, long y){
        long result;
        if(x < y)
          goto x_l_y;
        ge_cnt++;
        result = x - y;
        return result;
      x_l_y:
        lt_cnt++;
        result = y - x;
        return result;
    }

B. 原来的规则在没有else分支时，实现更容易。

#### 3.18

long test(long x, long y, long z){
  long val = x + y + z;
  if(x < -3){
    if(y < z)
      val = x * y;
    else
      val = y * z;
  }else if(x > 2)
    val = x * z;
  return val;
}

#### 3.19
A. 设错误处罚为x.
  0.5 * 16 + 0.5 * (x + 16) = 31 -> x = 30
B. 错误时需要 16 + 30 = 46个时钟周期。

#### 3.20
A. #define OP /
   通过右移3位实现除8，当x为负数时，要加上7的偏移量，保证向0舍入。
B.
  arith:
    leaq 7(%rdi), %rax  // temp = 7 + x
    testq %rdi, %rdi    //test x
    cmovns %rdi, %rax   //if x >= 0, temp = x
    sarq $3, %rax       // result = temp >> 3
  
#### 3.21
    long test(long x, long y){
      long val = 8x;
      if(y > 0){
        if(x >= y)
          val = y & x;
        else
          val = y - x;
      }else if(y <= -2){
        val = x + y;
      }
      return val;
    }

#### 3.22 
A. 12 12! = 479 001 600
B. 19 

#### 3.23
A. x : %rax, y : %rcx, n : %rdx
B. leaq 1(%rcx, %rax), %rdx  //x = x + y + 1
C.
  dw_loop:
    movq %rdi, %rax  //copy x to %rax
    movq %rdi, %rcx  //copy x to %rcx
    imulq %rdi, %rcx // y = x * x
    leaq (%rdi,%rdi), %rdx // n = 2 * x

  .L2:
    leaq 1(%rcx, %rax), %rax // x = x + y + 1
    subq $1, %rdx  // n--
    testq %rdx, %rdx //test n
    jg .L2  // n > 0 goto loop
    rep; ret //return

#### 3.24

    long loop_while(long a, long b){
        long result = 1;
        while(a < b){
            result = result * (a + b);
            a = a + 1;
        }
        return result;
    }

#### 3.25

    long loop_while2(long a, long b){
        long result = b;
        while(b > 0){
            result = result * b;
            b = b - a;
        }
        return result;
    }

#### 3.26

A. jump to middle
B.
    long fun_a(unsigned long x){
        long val = 0;
        while(x != 0){
          val ^= x;
          x >>= 1;
        }
        val &= 1;
        return val;
    }
C. 有奇数个1，返回1. 有偶数个1，返回0。

#### 3.27

    long fact_for_gd_goto(long n){
      long i = 2;
      long result = 1;
      if(i <= 1)
        goto done;
      loop:
        result *= i;
        i++;
        if(i <= n)
          goto loop;
      done:
        return result;
    }


#### 3.28

A.
    long func_b(unsigned long x){
        long val = 0;
        long i;
        for(i = 64; i != 0; i--){
            //x &= 1;
            //val <<= 1;
            //val |= x;
            val = (val << 1) | (x & 0x1);
            x >>= 1;
        }
        return val;
    }
B. i = 64, 编译器知道i ！= 0，所以不需要做初始测试。
C. 反转x中的位。

#### 3.29
A. 
    long sum = 0;
    long i = 0;
    while(i < 10){
      if(i & 1)
        continue;
      sum += i;
      i++;
    }
    当i = 1时，会陷入死循环。continue 会跳过i++.
B.

    long sum = 0;
    long i = 0;
    while(i < 10){
      if(i & 1)
        goto update;
      sum += i;
      update:
        i++;
    }

#### 3.30
A.
  跳转表
  1 .L4:
  2    .quad     .L9    //case -1
  3    .quad     .L5    //case 0
  4    .quad     .L6    //case 1
  5    .quad     .L7    //case 2
  6    .quad     .L2    //case 3 default
  7    .quad     .L7    //case 4
  8    .quad     .L8    //case 5
  9    .quad     .L2    //case 6 default
  10   .quad     .L5    //case 7

  由第2行，编译器先将x+1，说明源程序中标号最小为-1，
  第3行，x+1 和 8比较，大于8则跳到默认分支，所以源程序中最大标号为7.
  由第4行，ja .L2可知 .L2为默认分支。
  
  跳转表第6行，直接跳转.L2,说明该源程序中没有该标号(3).
  标号分别为 -1，0， 1， 2， 4， 5， 7.
B. 标号为0， 7, 都跳转到.L5.
   标号为2， 4, 都跳转到.L7.

#### 3.31

跳转表
  1 .L4:
  2    .quad     .L3    //case 0
  3    .quad     .L2    //case 1 default
  4    .quad     .L5    //case 2
  5    .quad     .L2    //case 3 default
  6    .quad     .L6    //case 4 
  7    .quad     .L7    //case 5
  8    .quad     .L2    //case 6 default
  9    .quad     .L5    //case 7 
 
根据汇编代码第2行， cmpq  $7, %rdi 可知源代码标号为 0 ～ 7
根据汇编代码第3行， ja  .L2 可知 .L2为默认分支。
标号1， 3， 6 都跳到default，说明源码没有这三个标号。

    void switcher(long a, long b, long c, long *dest){
        long val;
        switch(a){
        case 5: //对应汇编代码.L7分支，标号为5
            c = b ^ 0xf;
        case 0: //对应汇编代码.L3分支，标号为0
            val =  c + 112;
            break; 
        case 2:
        case 7://对应汇编代码.L5分支，标号为2和7.
            val = (c + b) << 2;
            break;
        case 4://对应汇编代码.L6,标号为4.
            val = a；
            break；
        default://对应汇编代码.L2分支
            val = b;
        }
        *dest = val; //对应汇编代码.L6
    }

#### 3.32 略

#### 3.33
  movslq %edi, %rdi 可知 第一个参数为int类型（32位），转换为long。
  add %rdi, (%rdx) 对应 *u += a, 可知 int a 为第一个参数， 第三个参数为long* u。
  addb %sil, (%rcx) 对应 *v += b， 可知第二个参数为b，将低字节加到 *v，第四个参数为char* v。
  movl $6, %eax 将6保存到%rax, 说明 sizeof(a) + sizeof(b) = 6,所以b类型为short。

  int procprob(int a, short b , long* u, char* v).


#### 3.34

    1 P:
    2   pushq %r15      save %r15
    3   pushq %r14      save %r14
    4   pushq %r13      save %r13
    5   pushq %r12      save %r12
    6   pushq %rbp      save %rbp
    7   pushq %rbx      save %rbx
    8   subq  $24, %rsp   分配栈帧
    9   movq  %rdi, %rbx  save first argument
    10  leaq  1(%rdi), %r15  save 1 + x
    11  leaq  2(%rdi), %r14  save 2 + x
    12  leaq  3(%rdi), %r13  save 3 + x
    13  leaq  4(%rdi), %r12  save 4 + x
    14  leaq  5(%rdi), %rbp  save 5 + x
    15  leaq  6(%rdi), %rax  save 6 + x
    16  movq  %rax, (%rsp)   save 6 + x on stack
    17  leaq  7(%rdi), %rdx  save 7 + x
    18  movq  %rdx, 8(%rsp)  save 7 + x on stack
    19  movl  $0, %eax       set %rax = 0
    10  call  Q

A. a0 ~ a5 这些局部值保存在被调用者保存寄存器中。
B. a6, a7存在栈上 
C. 因为被调用者保存寄存器只有6个，为%rbx， %rbp， %r12~%r15,用完后剩下两个局部变量保存在栈上。

#### 3.35
    long rfun(unsigned long x){
      if(x == 0)
        return 0;
      unsigned long nx = x >> 2;
      long rv = rfun(nx);
      return x + rv;
    }

    1 rfun
    2   pushq   %rbx          保存被调用者保存寄存器
    3   movq    %rdi, %rbx    save first argument
    4   movl    $0, %eax      set %rax = 0
    5   testq   %rdi, %rdi    test x
    6   je      .L2
    7   shrq    $2, %rdi      x >>= 2
    8   call    rfun
    9   addq    %rbx, %rax    x + yfun()
    10  .L2
    11  popq    %rbx          restore %rbx
    12  ret

A. 存储的x的值。

#### 3.36 略 //简单题，画表麻烦。。。

#### 3.37

|表达式|类型|值|汇编代码
|:-:|:-:|:-:|:-:|
|s + 1|short*|Xs + 1*2| leaq 2(%rdx), %rax|
|s[3]|short|M[Xs + 6]| movw 6(%rdx), %ax|
|&s[i]|short*|Xs + 2*i| leaq (%rdx, %rcx,2), %rax|
|s[4*i + 1]|short|M[Xs + 2*(4*i + 1)]| movw 2(%rdx, %rcx, 8), %ax|
|s + i - 5|short*|Xs + 2*(i-5)| leaq -10(%rdx, %rcx, 2), %rax|

#### 3.38

    i in %rdi, j in %rsi
    1 sum_element:
    2   leaq  0(,%rdi,8), %rdx    // 8i -> %rdx
    3   subq  %rdi, %rdx          // 8i - i -> %rdx
    4   addq  %rsi, %rdx          // 7i + j -> %rdx
    5   leaq  (%rsi,%rsi,4), %rax // 5j -> %rax 
    6   addq  %rax, %rdi          // i + 5j -> %rdi
    7   movq  Q(,%rdi,8), %rax    // 8(i + 5j) + Q-> %rax
    8   addq  P(,%rdx,8), %rax    // 8(7i + j) + P + %rax -> %rax
    9   ret

由Q[N][M], Q[j][i] = 8(5j + i) + Q, 可知 M = 5.
由P[M][N], P[i][j] = 8(7i + j) + P, 可知 N = 7.

#### 3.39
&D[i][j] = Xd + L(C*i + j) (3.1)

由代码知，C = 16, L = 4
int *Aptr = &A[i][0] = Xa + 4(16i + 0) = Xa + 64i.
int *Bptr = &B[0][k] = Xb + 4k.
int *Bend = &B[N][k] = Xb + 4(16*16 + k) = Xb + 1024 + 4k.

#### 3.40

    1 fix_set_diag
    2   movl  $0, %eax    // 0 -> %eax
    3   .L3:
    4     movl %esi, (%rdi, %rax)   // vai -> M[A + 0]
    5     addq $68, %rax            // 68 + %rax -> %rax
    6     cmpq $1088, %rax          // cmp 
    7     jne  .L13                 // !=
    8     rep; ret

    void fix_set_diag_opt(fix_matrix A, int val){
        int *Abase = &A[0][0];
        long i = 0;
        long iend = (N + 1) * N;
        do{
          Abase[i] = val;
          i += (N + 1);
        }while(i != iend);
    }

#### 3.41
A. p: 0, s.x: 8, s.y: 12, next: 16
B. 8 + 4 + 4 + 8 = 24byte
C.
    sp in %rdi
    1 sp_init:
    2   movl  12(%rdi), %eax    //M[12 + sp] -> %eax
    3   movl  %eax, 8(%rdi)     //M[12 + sp] -> M[sp + 8]
    4   leaq  8(%rdi), %rax     // sp + 8 -> %rax
    5   movq  %rax, (%rdi)      // sp + 8 -> M[sp]
    6   movq  %rdi, 16(%rdi)    // sp -> M[sp + 16]
    7   ret.

    void sp_init(struct prob *sp){
        sp->s.x = sp->s.y; // 对应汇编2、 3行
        sp->p = &(sp->s.x); // 4、5
        sp->next = sp;    // 6
    }

#### 3.42
    struct ELE{
      long v;
      struct ELE *p;
    };
    ptr in %rdi
    A.
    1   fun:
    2     movl $0, %eax   // 0 -> %eax
    3     jmp   .L2
    4    .L3:
    5      addq (%rdi), %rax   // M[ptr] + %rax -> %rax
    6      movq 8(%rdi), %rdi   // ptr + 8 -> %rdi
    7    .L2:
    8      testq %rdi, %rdi    // test ptr
    9      jne  .L3            // !=
    10     rep; ret

    long func(struct ELE *ptr){
        long result = 0;
        while(ptr){
            result += ptr->v;
            ptr = ptr->next;
        }
        return result;
    }

B.实现的链表，执行的遍历链表求和操作。

#### 3.43
|expr|type|代码|
|:-:|:-:|:-:|
|up->t1.u|long|movq (%rdi),%rax; movq %rax,(%rsi)|
|up->t1.v|short|movw 8(%rdi), %ax; movw %ax,(%rsi)|
|&up->t1.w|char*|addq $10, %rdi, movq %rdi, (%rsi)|
|up->t2.a|int*|movq %rdi, (%rsi)|
|up->t2.a[up->t1.u]|int|movq (%rdi),%rax; movl (%rdi, 4, %rax),%eax; movl %eax, (%rsi)|
|*up->t2.p|char|movq 8(%rdi),%rax; movb (%rax),%al; movb %al,(%rsi)|

+ 注：答案有误，movq 两个操作数不能都是内存。

#### 3.44
> x86(Linux默认#pragma pack(4)， Window默认#pragma pack(8))。Linux最大支持4字节对齐。
 方法：
 1 取pack(n)的值 (n = 1, 2, 4，8……)，取结构体中类型最大值为m。两者取小即为外对齐大小 Y = (m < n ? m: n)；
 2 将每一个结构体的成员大小与Y比较取小者为X，作为内对齐的大小；
 3 所谓按X对齐，即为地址(设起始地址为0)能被X整除的地方开始存放数据；
 4 外部对齐原则是依据Y的值(Y的最小整数倍)，进行补空操作；

A.偏移量：i:0, c:4, j:8, d:12, 总大小：16 对齐：4
B.偏移量：i:0, c:4, j:5, d:8, 总大小：16  对齐：8
C.偏移量：w:0, c:6  总大小：10            对齐：2
D.偏移量：w:0, c:16 总大小：40            对齐：8
D.偏移量：a:0, t:24   总大小：40          对齐：8

#### 3.45
    struct{
      char    *a;
      short   b;
      double  c;
      char    d;
      float   e;
      char    f;
      long    g;
      int     h;
    }rec;
A. a: 0, b: 8, c: 16, d: 24, e: 28, f: 32, g: 40, h: 48. 对齐:8

B. 56byte （大于51的8的最小倍数）
C.
    struct{
      char    *a;
      double  c;
      long    g;
      float   e;
      int     h;
      short   b; 
      char    d;  
      char    f;
    }rec;

偏移量：a:0, c: 8, g: 16, e: 24, h: 28, b:32, d:34, f:35.
总大小：40byte。（大于35的8的最小倍数）

#### 3.46
A.
2 push %rbx
3 sub  $0x16, %rsp
看保存%rbx，再将栈指针减小16.
|00 00 00 00 00 40 00 76|<-返回地址
|-|
|01 23 45 67 89 AB CD EF|保存%rbx，小端存储
||
||<-%rsp
||

B.
输入字符是由低地址向高地址。
|00 00 00 00 00 40 00 34|<-返回地址
|-|
|33 32 31 30 39 38 37 36|保存%rbx
|35 34 33 32 31 30 39 38|
|37 36 35 34 33 32 31 30|<-%rsp
||

C.00 00 00 00 00 40 00 34.(后两个字节被00 34覆盖)。
D.%rbx被破坏
E. 
result = (char*)malloc(strlen(buf) + 1);
代码应该检查返回值是否为null。

#### 3.47
A.0xffffd754 - 0xffffb754 = 0x00002000 = 2^13.
地址大概范围为2^13.
B.2^13/128 = 2^6 = 64次。

#### 3.48
A.
a.不带保护者 buf：%rsp ~ %rsp + 23,共24byte， v: %rsp + 24
b.带保护者 金丝雀值:%rsp + 40, buf:%rsp + 16 ~ %rsp + 39,共24byte
          v:%rsp + 8.
B.v比buf更靠近栈顶，buf溢出不会破坏v的值。

#### 3.49

    1 vframe:
    5   leaq  22(,%rdi,8), %rax     //8n + 22 -> %rax
    6   andq  $-16, %rax            //(8n + 22) & 0xfffffff0 ->%rax
    7   subq  %rax, %rsp            //%rsp - %rax ->%rsp
    8   leaq  7(%rsp), %rax         // 7 + %rsp -> %rax
    9   shrq  $3, %rax              // (7 + %rsp) >> 3 -> %rax
    10  leaq  0(,%rax,8), %r8       // ((7 + %rsp) >> 3)*8 -> %r8
    11  movq  %r8, %rcx             // %r8 -> %rcx

A.(8*n + 22) & 0xfffffff0 向下舍入到最接近16的倍数
n是奇数时，为8n+8，为偶数时，为8n+16.
s2 = s1 - ((8*n + 22) & 0xfffffff0)

B.p = ((s2 + 7) >> 3) * 8;
将s2向上舍入到最接近8的倍数。

C.
|n|s1|s2|p|e1|e2|
|-|-|-|-|-|-|
|5|2065|2017|2024|1|7|
|6|2064|2000|2000|16|0|
D.
保证s2-s1 为8*n最接近的16的倍数
p以8的倍数对齐，正是8字节数组对齐方式。

#### 3.50

    1 fcvt2:
    2   movl        (%rdi), %eax        //i = *ip
    3   vmovss      (%rsi), %xmm0       //f = *fp
    4   vcvttsd2si  (%rdx), %r8d        //d = *dp and convert to int
    5   movl        %r8d, (%rdi)        //*ip = (int)d
    6   vcvtsi2ss   %eax, %xmm1, %xmm1  //convert i to float
    7   vmovss      %xmm1, (%rsi)       //*fp = (float)i
    8   vcvtsi2sdq  %rcx, %xmm1, %xmm1  //convert l to double
    9   vmovsd      %xmm1, (%rdx)       //*dp = (double)l
    10  vunpcklps   %xmm0, %xmm0, %xmm0
    11  vcvtps3pd   %xmm0, %xmm0        //convert float to double
    12  ret


val1:d, val2:i, val3:l, val4:f.

#### 3.51

|Tx|Ty|指令|
|:-:|:-:|:-:|
|long|double|vcvtsi2sdq %rdi, %xmm0, %xmm0|
|double|int|vcvttsd2si %xmm0, %eax|
|double|float|vmoddup %xmm0, %xmm0; vcvtpd2psx %xmm0, %xmm0|
|long|float|vcvtsi2ssq %rdi, %xmm0, %xmm0|
|float|long|vcvttss2siq %xmm0, %rax|

#### 3.52略

#### 3.53

    double funct1(arg1_t p(%edi), arg2_t q, arg3_t r, arg4_t s(%xmm1)){
        return p/(q + r) - s;
    }

    1   funct1:
    2     vcvtsi2ssq  %rsi, %xmm2, %xmm2    // qword convert to float
    3     vaddss      %xmm0, %xmm2, %xmm0   // q + r
    4     vcvtsi2ss   %edi, %xmm2, %xmm2    // int convert to float
    5     vdivss      %xmm0, %xmm0, %xmm0   // p / (q + r)
    6     vunpcklps   %xmm0, %xmm0, %xmm0   //
    7     vcvtps2pd   %xmm0, %xmm0          // float convert to double
    8     vsubsd      %xmm1, %xmm0, %xmm0   // p/(q + r) - s
    9     ret       

arg1_t : int, arg2_t : float, arg3_t: long, arg4_t: double
or
arg1_t : int, arg2_t : long, arg3_t: float, arg4_t: double


#### 3.54

    w in %xmm0, x in %edi, y in %xmm1, z in %rsi
    1   funct2:
    2     vcvtsi2ss   %edi, %xmm2, %xmm2 // int convert to float
    3     vmulss      %xmm1, %xmm2, %xmm1// x*y
    4     vunpcklps   %xmm1, %xmm1, %xmm1//
    5     vcvtps2pd   %xmm1, %xmm2       // float convert to double
    6     vcvtsi2sdq  %rsi, %xmm1, %xmm1 // convert long to double
    7     vdivsd      %xmm1, %xmm0, %xmm0// w / z
    8     vsubsd      %xmm0, %xmm2, %xmm0//x*y - w / z 
    9     ret

    double funct2(double w, int x, float y, long z){
        return x * y - w / z;
    }

#### 3.55
.L3:
  .long 0           (0x00000000) 低4字节
  .long 1077936128  (0x40400000) 高4字节
符号位(0), 指数字段为：0x404(1028),减去偏移1023得到指数为5.
小数字段为0000000000000，加上隐含的1为1.0，1.0*2^5 = 32.0.

#### 3.56

double simplefun(double x){
    return EXPR(x);
}

A.
1   vmovsd  .LC1(%rip), %xmm1 
2   vandpd   %xmm1, %xmm0, %xmm0
3   .LC1:
4     .long 4294967295  //(0xffffffff)
5     .long 2147483647  //(0x7fffffff)
6     .long 0
7     .long 0

低8字节，除了最高位均为1，x & (0x7fffffffffffffff),将x的符号位变为0，相当于取绝对值。EXPR(x) 为 fabs(x).

B.
1   vxorpd  %xmm0, %xmm0, %xmm0 // x ^ x = 0.
EXPR(x) 为 x^x

C. 
1  vmovsd   .LC2(%rip), %xmm1
2  vxorpd   %xmm1, %xmm0, %xmm0
3  .LC2:
4     .long 0
5     .long -2147483648  // 0x80000000
6     .long 0
7     .long 0

x ^ (0x8000000000000000),将x符号位取反
EXPR(x) 为-x.

#### 3.57

1   funct3:
2     vmovss      (%rdx), %xmm1       //d = *dp -> %xmm1
3     vcvtsi2sd   (%rdi), %xmm2, %xmm2 //a = *ap,  convert a to double 
4     vucomisd    %xmm2, %xmm0 // cmp b a
5     jbe   .L8                 // b <= a,jump .L8
6     vcvtsi2ssq  %rsi, %xmm0, %xmm0  // convert c to float
7     vmulss      %xmm1, %xmm0, %xmm1 // c * d->%xmm1
8     vunpcklps   %xmm1, %xmm1, %xmm1 
9     vcvtps2pd   %xmm1, %xmm0        //convert float to double
10    ret
11  .L8:
12      vaddss %xmm1, %xmm1, %xmm1   // 2 * d
13      vcvtsi2ssq  %rsi, %xmm0, %xmm0 // convert c to float
14      vaddss  %xmm1, %xmm0, %xmm0   // c + 2*d ->%xmm0
15      vunpcklps %xmm0, %xmm0, %xmm0 
16      vcvtps2pd %xmm0, %xmm0  // convert float to double
17      ret

double funct3(int *ap, double b, long c, float *dp){
    int a = *ap;
    float d = *dp;
    if(b > a){
        return c * d;
    }else{
        return c + 2*d;
    }
    
}












