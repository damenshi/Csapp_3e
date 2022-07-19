# chap7 note

+ 推荐书籍：程序员的自我修养 -- 链接, 装载与库
+ [Linux环境下：程序的链接, 装载和库](https://www.bilibili.com/video/BV1hv411s7ew/)   （比较详细，探讨了很多细节）

1. 在现代操作系统中，链接是由叫做链接器的程序自动执行的

2. <img src="https://raw.githubusercontent.com/damenshi/myImage/main/img/image-20220624121330896.png" alt="image-20220624121330896" style="zoom:50%;" />

>  cpp: C预处理器，cc1:C编译器，as:汇编器
>
> ./prog shell滴哦啊用操作系统中一个叫做加载器（loader）的函数，它将可执行文件prog中的代码和数据复制到内存，然后将控制转移到这个程序的开头。

3. 目标文件有三种形式：可重定位目标文件、可执行目标文件、共享目标文件。

4. 在目标文件中，未初始化变量不需要占据任何实际的磁盘空间。运行时，在内存中分配这些变量，初始值为0。

5. 动态链接共享库：基本思路是当创建可执行文件时，静态执行一些链接，然后在程序加载时，动态完成链接过程。

   <img src="https://raw.githubusercontent.com/damenshi/myImage/main/img/image-20220626194224882.png" alt="image-20220626194224882" style="zoom:50%;" />

   6. Linux系统为动态链接器提供了一个简单的接口，允许应用程序在运行时加载和链接共享库。

**练习题**

7.1 

| 符号 | .symtab条目 | 符号类型 | 在哪个模块中定义 | 节 |
| :--: | :---------: | :----: | :----: | :----: |
| buf | Yes | 外部 | m.o | .data |
| bufp0 | Yes | Global | swap.o | .data |
| bufp1 | Yes | Global | swap.o | Common |
| swap | Yes | Global | swap.o | .text |
| Temp | No | - | - | - |

7.2 

A. (a)REF(main.1) -> DEF(main.1)	(b)REF(main.2) -> DEF(main.1)

B. (a)error (b)error

C. (a)REF(x.1) -> DEF(x.2)	(b)REF(x.2) -> DEF(x.2)



7.3

A. gcc p.o libx.a

B. gcc p.o libx.a liby.a

C. gcc p.o libx.a liby.a libx.a

+ 注意链接器看到.o文件会直接更新E，U，D（书上有定义），后面即使有依赖也不用包含。

7.4

A. 0x4004df

B. 0x05



7.5

ADDR(s) = ADDR(.text) = 0x4004d0

ADDR(r.symbol) = ADDR(swap) = 0x4004e8

refaddr = ADDR(s) + r.offset = 0x4004d0 + 0xa = 0x4004da

*refptr = (unsigned)(ADDR(r.symbol) + r.addend - refaddr)

​			 = (unsigned)(0x4004e8 + (-4) - 0x4004da)

​			 = (unsigned)(0xa)

可执行目标文件中，对swap的PC相对引用值为 0xa。

0x4004d9: e8 0a 00 00 00  callq 4004e8<swap>

