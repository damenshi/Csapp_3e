---
attachments: [Clipboard_2022-06-05-19-19-15.png, Clipboard_2022-06-05-19-26-01.png, Clipboard_2022-06-06-11-37-08.png, Clipboard_2022-06-07-16-37-02.png, Clipboard_2022-06-07-16-44-13.png, Clipboard_2022-06-07-16-51-26.png, Clipboard_2022-06-07-16-53-10.png, Clipboard_2022-06-07-17-16-06.png]
tags: [csapp/10]
title: chap5 note
created: '2022-06-05T10:00:46.457Z'
modified: '2022-06-09T01:20:56.986Z'
---

# chap5 note

[B站课程视频地址](https://www.bilibili.com/video/BV1iW411d7hd?p=10)

### 1.两个指针可能指向同一个内存位置的情况称为内存别名使用。在只执行安全的优化中，编译器必须假设不同的指针可能会指向内存中同一个位置。

### 2. 引入度量标准CPE（每元素的周期数）表示程序性能。
### 3. 优化方法：
  + 消除循环的低效率。如：代码移动 
  + 减少过程调用
  + 消除不必要的内存引用
  + 循环展开
  + 并行积累在多个值
  + 重新结合变换

### 4. 
  + 书中延迟界限的含义：任何必须按照严格顺序完成合并运算的函数所需的最小CPE值（CPE：每个OP的时钟周期数）.
  + 吞吐量界限：使用了流水线后，每个OP的时钟周期数。例如整数加法吞吐量界为0.5，说明使用了流水线每个时钟周期可以进行2个整数加法运算。

### 5. unxi提供了剖析程序GPROF，这个程序产生两种信息，首先确定程序中每个函数花费了多少cpu时间，其次计算每个函数被调用的次数。

### 5. 书中5.7节是重点，需要多看两遍。

### 练习题

#### 5.1
调用后*xp = 0, *yp = 0

#### 5.2
0=< n <=2时，版本1最快
3=< n <=7时，版本2最快
8=< n 时，版本3最快
由图可知
![](@attachment/Clipboard_2022-06-05-19-26-01.png)

#### 5.3
|code|min|max|incr|square|
|:-:|:-:|:-:|:-:|:-:|
|A|1|91|90|90|
|B|91|1|90|90|
|C|1|1|90|90|

### 5.4
A.
在未优化combine3中，%xmm0被用作临时值.
在优化过的combine3中，%xmm0用来存放累积乘积.
优化过的combine3的C语言表达：
    void combine3(vec_ptr v, data_t *dest){
      long i;
      long length = vec_length(v);
      data_t *data = get_vec_start(v);
      data_t acc = IDENT;

      for(int i = 0; i < length; i++){
        acc = acc OP data[i];
        *dest = acc;
      }
    }
B. 是的。
C. 即使使用内存别名，也产生相同的结果。因为优化过的程序在每轮循环都会将累积值保存到内存中。

### 5.5
A.n次加法运算，2*n次乘法运算
B.受浮点数乘法制约

### 5.6
A.n次加法，n次乘法
B.浮点数加法运算要在乘法运算结束之后才能进行，所以CPE = 5.00 + 3.00 = 8.00
C.虽然5.5有两个乘法运算，但只有一个出现在关键路径上。

### 5.7

     void combine5(vec_ptr v, data_t *dest){
      long i;
      long length = vec_length(v);
      long limit = length - 4;
      data_t *data = get_vec_start(v);
      data_t acc = IDENT;

      for(int i = 0; i < limit; i += 5){
        acc = acc OP data[i] OP data[i + 1] OP data[i + 2] OP data[i + 3] OP data[i + 4];
      }
      for( ; i < length; i++){
        acc = acc OP data[i];
      }
      *dest = acc;
    }

### 5.8
![](@attachment/Clipboard_2022-06-06-11-37-08.png)
由图中关键路径上的操作数可知：

A1: PCE = 5
A2: PCE = 5 * 2/3 = 3.33
A3: PCE = 5 * 1/3 = 1.67
A4: PCE = 5 * 1/3 = 1.67
A5: PCE = 5 * 2/3 = 3.33

### 5.9

void merge(long src1[], long src2[], long dest[], long n){
    long i1 = 0;
    long i2 = 0;
    long id = 0;
    while(i1 < n && i2 < n){
      long take = src1[i1] < src2[i2];
      dest[id++] = take ? src1[i1] : src2[i2];
      i1 += take;
      i2 += (1 - take);
    }

    while(i1 < n){
      dest[id++] = src1[i1++];
    }
    while(i2 < n){
      dest[id++] = src2[i2++];
    }
}

### 5.10
A.
a[]初始化为{0,1,2,...,999}
调用copy_arry(a+1, a, 999)，对于0=< i <= 998,a[i] = i + 1,a[]变为{1,2,...999,999}.

B.
调用copy_arry(a, a+1, 999)后，a[]变为{0,0,...0,0}.

C.
A调用数据无关
B调用每次加载都依赖前一次迭代的存储结果，有写/读相关
D.CPE为1.2,与A相同，数据的存储和后续加载不存在相关。

### 5.11
combine3:
![](@attachment/Clipboard_2022-06-07-16-51-26.png)
![](@attachment/Clipboard_2022-06-07-16-44-13.png)

psum1:
![](@attachment/Clipboard_2022-06-07-16-37-02.png)
![](@attachment/Clipboard_2022-06-07-16-53-10.png)
![](@attachment/Clipboard_2022-06-07-17-16-06.png)

> 函数连续迭代之间有写/读相关，一次迭代中的p[i]是下一次迭代的p[i-1].
  关键路径包括一次加载，一次浮点加法，一次存储，pce为9，与write_read一致(pce = 7,一次加载，一次整数加法，一次存储)。浮点加法3个时钟周期，整数加法1个时钟周期。

### 5.12
    void psum1(float a[], float p[], long n){
        long i;
        p[0] = a[0];
        float temp = a[0];
        for(int i = 1; i < n; i++){
            temp += a[i];
            p[i] = temp; 
        }
    }








