# chap8 note

+ 第一次看视频教程没太看懂，可以先看书，再看视频。

1. 异常可以分为四类：中断（interrupt）、陷阱（trap）、故障（fault）、终止（abort）。

   ![image-20220630094202113](https://raw.githubusercontent.com/damenshi/myImage/main/img/image-20220630094202113.png)

2. 陷阱（trap）最重要的用途是在用户程序和内核之间提供一个像过程一样的接口，叫做系统调用。

3. 故障：一个典型的故障示例是缺页异常。

   ![image-20220630095909595](https://raw.githubusercontent.com/damenshi/myImage/main/img/image-20220630095909595.png)

4. 进程的经典定义就是一个执行中程序的实例。系统中的每个程序都运行在某个进程的上下文中。上下文是由程序正确运行所需要的状态组成的。这个状态包括存放在内存中的程序的代码和数据，它的栈、通用目的寄存器的内容、程序计数器、环境变量以及打开文件描述符的集合。
5. <img src="https://raw.githubusercontent.com/damenshi/myImage/main/img/image-20220630102314835.png" alt="image-20220630102314835" style="zoom:50%;" />

6. 内核为每个进程维持一个上下文。上下文就是内核重新启动一个被抢占的进程所需的状态。
7. fork函数被调用一次，却会返回两次。在父进程中，fork返回子进程的PID。在子进程中，fork返回0。
8. 当一个进程由于某种原因终止时，内核并不是立即把它从系统中清除。相反，进程被保持在一种已终止的状态中，直到被它的父进程收回。一个终止了但还未被回收的进程称为僵尸进程。
9. 一个父进程终止了，内核会安排init进程成为它的孤儿进程的养父。
10. 一个进程可以通过调用waitpid函数来等待他的子进程终止或者停止。
11. execve函数在当前进程上下文中加载并运行一个新程序。

12. Linux信号，它允许进程和内核中断其他进程。一个信号就是一条小消息，它通知进程，系统中发生了一个某种类型的事件。

13. ```
    pause() returns only when a signal was caught and the signal-
           catching function returned.  In this case, pause() returns -1,
           and errno is set to EINTR.
    ```





**练习题**

8.1

| 进程对 | 并发的？ |
| :----: | :------: |
|   AB   |   yes    |
|   AC   |    no    |
|   BC   |   yes    |

8.2

A. p1: x=2 p2: x=1

B. p2: x=0

8.3

acbc	abcc  bacc

![image-20220630114642729](https://raw.githubusercontent.com/damenshi/myImage/main/img/image-20220630114642729.png)

8.4

A. 6行

B.

Hello, 1, Bye, 0, 2, Bye

![image-20220630120417541](https://raw.githubusercontent.com/damenshi/myImage/main/img/image-20220630120417541.png)

8.5

```c
unsigned int snooze(unsigned int secs){
		unsigned int ret = sleep(secs);//返回还剩下要休眠的秒数。
		printf("Slept for %d of %d secs.\n", secs-ret, secs);
		return ret;
}
```

8.6

```c
//myecho

int main(int argc, char *argv[], char *envp[]){
		printf("Command-line arguments:\n");
		for(int i = 0; argv[i] != NULL ; i++){
			printf("	argv[%2d]: %s\n", i, argv[i]);
		}
		printf("Environment variables:\n");
		for(int i = 0; envp[i] != NULL ; i++){
			printf("	envp[%2d]: %s\n", i, envp[i]);
		}
		exit(0);
}
```

8.7

```c
#include "csapp.h"

unsigned int snooze(unsigned int secs){
		unsigned int ret = sleep(secs);//返回还剩下要休眠的秒数。
		printf("Slept for %d of %d secs.\n", secs-ret, secs);
		return ret;
}

void handler(int sig){
  	return;
}

int main(int argc, char *argv[]){
  	if(argc != 2){
      fprintf("stderr", "usage: %s <secs>\n");
      exit(0);
    }
  	if(signal(SIGINT, handler) == SIG_ERR)
      unix_error("signal error");
  	snooze(atoi(argv[1])); 
  	exit(0);
}
```

8.8

213.

父进程打印2，之后创建子进程，子进程进入无限循环，父进程向子进程发送一个信号，然后等待回收子进程。子进程捕获这个信号，中断无限循环，将计数器从初始值2进行减1，输出1，然后终止。父进程回收子进程之后，将计数器从初始值2进行加1，输出3.

