# chap10 note

1. Linux shell 创建的每个进程开始时都有三个打开的文件：标准输入（描述符为0）、标准输出（描述符为1）、标准错误（描述符为2）。
2. 给定一个大小为m字节的文件，k>=m时（k是文件当前位置）执行读操作会触发一个称为EOF的条件，应用程序能检测到这个条件。在文件结尾处并没有明确的“EOF符号”。
3. 套接字是用来和另一个进程进行跨网络通信的文件

4. 共享文件

   <img src="https://raw.githubusercontent.com/damenshi/myImage/main/img/image-20220713102449276.png" alt="image-20220713102449276" style="zoom:50%;" />

   <img src="https://raw.githubusercontent.com/damenshi/myImage/main/img/image-20220713102844313.png" alt="image-20220713102844313" style="zoom:50%;" />

   <img src="https://raw.githubusercontent.com/damenshi/myImage/main/img/image-20220713102908266.png" alt="image-20220713102908266" style="zoom:50%;" />



**练习题**

10.1

Linux shell 创建的每个进程开始时都有三个打开的文件：标准输入（描述符为0）、标准输出（描述符为1）、标准错误（描述符为2）。

open函数总是返回当前没有打开的最小描述符。第一次调用会返回描述符3，close会释放描述符3.

所以程序输出为fd2 = 3。



10.2

c = f

fd1和fd2都有各自的打开文件表表项，所以每个描述符对于foobar.txt都有自己的文件位置。对应图10-13.



10.3

c = o

描述符fd在父子进程都指向同一个打开文件表表项，当子进程读取文件第一个字节时，文件位置加1.所以父进程会读第二个字节。



10.4

dup2(5, 0);



10.5

c = o

dup2(fd2, fd1); 将fd1重定向到fd2。



