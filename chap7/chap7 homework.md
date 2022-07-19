# chap7 homework

7.6

| 符号  | swap.o .symtab条目？ | 符号类型 | 定义符号的模块 |  节   |
| :---: | :------------------: | :------: | :------------: | :---: |
|  buf  |         Yes          |  extern  |      m.o       | .data |
| bufp0 |         Yes          |  global  |     swap.o     | .data |
| bufp1 |         Yes          |  local   |     swap.o     | .bss  |
| swap  |         yes          |  global  |     swap.o     | .text |
| temp  |          no          |          |                |       |
| Incr  |         Yes          |  local   |     swap.o     | .text |
| count |         Yes          |  local   |     swap.o     | .bss  |

<img src="https://raw.githubusercontent.com/damenshi/myImage/main/img/image-20220627103749617.png" alt="image-20220627103749617" style="zoom:50%;" />

7.7

将bar5.c中double x 修改为 static double x.



7.8

A. (a)REF(main.1) -> DEF(main.1)	 (b)REF(main.2) -> DEF(main.2)

B.  (a)未知 	(b)未知

C.  (a)错误 	(b)错误



7.9

![image-20220627110218641](https://raw.githubusercontent.com/damenshi/myImage/main/img/image-20220627110218641.png)



```
objdump -d main

000000000000064a <main>:                                                                       

 64a:   55                      push   %rbp                                                    

 64b:   48 89 e5                mov    %rsp,%rbp                                               

 64e:   e8 07 00 00 00          callq  65a                                                 

 653:   b8 00 00 00 00          mov    $0x0,%eax                                               

 658:   5d                      pop    %rbp                                                    

 659:   c3                      retq  
```

> Bar6.c中的main是一个弱全局符号，在链接后，被重定位到foo6.c的main的地址。
> 所以，在 p2() 中使用 main 变量的时候，找到了 main() 函数的起点位置的字节。
>
> 从反汇编代码中可以看出 main() 的内存地址为 0x64a，这个字节中保存的内容正是 0x55

> + 疑惑？我的代码地址为什么不是0x400000开始？[解释](https://stackoverflow.com/questions/66361767/why-does-not-entry-point-address-start-at-0x400000)



7.10

A. gcc p.o libx.a

B. gcc p.o libx.a liby.a libx.a

C.gcc p.o libx.a liby.a libx.a libz.a

+ 注意链接器看到.o文件会直接更新E，U，D（书上有定义），后面即使有依赖也不用包含。



7.11

可执行文件中的 .bss 节，并没有占用空间。但是在导入内存后，.bss 节中的变量需要被初始化为 0。从而占用了空间。



7.12

A.

ADDR(s) = ADDR(.text) = 0x4004e0

ADDR(r.symbol) = ADDR(swap) = 0x4004f8

refaddr = ADDR(s) + r.offset = 0x4004e0 + 0xa = 0x4004ea

*refptr = (unsigned)(ADDR(r.symbol) + r.addend - refaddr)

​			 = (unsigned)(0x4004f8 + (-4) - 0x4004ea)

​			 = (unsigned)(0xa)

可执行目标文件中，对swap的重定位引用值为 0xa。

B.

ADDR(s) = ADDR(.text) = 0x4004d0

ADDR(r.symbol) = ADDR(swap) = 0x400500

refaddr = ADDR(s) + r.offset = 0x4004d0 + 0xa = 0x4004da

*refptr = (unsigned)(ADDR(r.symbol) + r.addend - refaddr)

​			 = (unsigned)(0x400500 + (-4) - 0x4004da)

​			 = (unsigned)(0x22)

可执行目标文件中，对swap的重定位引用值为 0x22。



7.13

A.

libc.a

![image-20220627121918315](https://raw.githubusercontent.com/damenshi/myImage/main/img/image-20220627121918315.png)

libm.a

![image-20220627122044935](https://raw.githubusercontent.com/damenshi/myImage/main/img/image-20220627122044935.png)

[解释](https://blog.csdn.net/aurora0_0/article/details/84547376)

![image-20220627122756695](https://raw.githubusercontent.com/damenshi/myImage/main/img/image-20220627122756695.png)

B.

```
/*
 * little.c
 */
int main() {
  return 0;
}
```

```
gcc -Og little.c -o Og-little
gcc -Og -g  little.c -o Ogg-little
```

![image-20220627125519040](https://raw.githubusercontent.com/damenshi/myImage/main/img/image-20220627125519040.png)

不同

C.

![image-20220627124039601](https://raw.githubusercontent.com/damenshi/myImage/main/img/image-20220627124039601.png)