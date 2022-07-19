# chap8 homework

8.9

| 进程对 | 并发？ |
| :----: | :----: |
|   AB   |   no   |
|   AC   |  Yes   |
|   AD   |  Yes   |
|   BC   |  Yes   |
|   BD   |  Yes   |
|   CD   |  Yes   |

8.10

A. fork

B. longjmp, execv

C. setjmp



8.11

```
                        c
                    +-------+
                    |    "hello"
                    |    
                c   |   p
            +-------+-------+
            |     fork   "hello"
            |
            |           c
            |       +-------+
            |       |    "hello"
            |       |    
            |   p   |   p    
     +------+-------+-------+
    main  fork    fork   "hello"

```

输出4个“hello”



8.12

```
                        c
                    +-------+-------+
                    |    "hello"   "hello" 
                    |    
                c   |   p
            +-------+-------+-------+
            |     fork   "hello"    "hello" 
            |
            |           c
            |       +-------+-------+
            |       |    "hello"    "hello" 
            |       |    
            |   p   |   p    
+----+------+-------+-------+-------+
main doit  fork    fork   "hello"   "hello" 

```

输出8个“hello”



8.13

```
                c   
            +-------+-------+
            |       4       3
            |
            |           
            |   p    
     +------+---------------+
    main  fork              2
```

一种可能的输出2、4、3.



8.14

```
                        c
                    +-------+
                    |    "hello"   
                    |    
                c   |   p
            +-------+-------+
            |     fork   "hello"    
            |         
            |           
            |   p          
+----+------+----------------+
main doit  fork           "hello" 
```

输出3个“hello”



8.15

```
                      c
                    +-------+--------+
                    |    "hello"     "hello"
                    |    
                c   |   p
            +-------+-------+--------+
            |     fork   "hello"     "hello"
            |         
            |           
            |   p          
+----+------+-------+----------------+
main doit  fork  return              "hello" 
```

输出5个“hello”



8.16

```
                                     
                c
            +---------------+
            |           counter = 0  
            |         
            |           
            |   p          
+-----------+---------------+--------+
main      fork             wait     counter = 2
```

输出 counter = 2.

> 父子进程的counter变量是相互独立的。



8.17

<img src="https://raw.githubusercontent.com/damenshi/myImage/main/img/image-20220704091815205.png" alt="image-20220704091815205" style="zoom:50%;" />

```
                     c
                     +---------+-------+
                     |      !pid = 1  Bye
                     |                 |
                     |           			 |	
                     |p                |           	
+----------+---------+---------+-------+-------+------+
main      "Hello"   Fork    !pid = 0  waitpid  2      Bye
```

所有可能的输出：

Hello, 1, Bye, 0, 2, Bye

Hello, 1,  0, Bye, 2, Bye

Hello, 0, 1, Bye, 2, Bye



8.18

```
                                c
                             +-------+-------+
                             |      "0"      exit "2"
                             |    
               c             |   p
            +-------+--------+-------+-------+
            |     atexit	  fork    "1"      exit "2" 
            |
            |       c
            |       +-------+------+
            |       |      "0"    exit
            |       |    
            |   p   |   p    
+-----------+-------+-------+------+
main      fork    fork     "1"     exit
```

可能的输出有：A、C、E



8.19

2^n



8.20

```c
#include<stdio.h>
#include "csapp.h"

int main(int argc, char *argv[], char *env[]){
    if(execve("/bin/ls", argv, env) == -1){
        fprintf(stderr, "execve error: %s\n", strerror(errno));
        exit(1);
    }
}   
```



8.21

```
              c
            +---------+-------+
            |        "a"    exit
            |                 |
            |           			| 	
            | p               |           	
+-----------+---------+-------+-------+------+
main       Fork       "b"  waitpid    "c"    exit
```

可能的输出有：a, b, c;  b, a, c



8.22

```c
//mysystem.c

#include<stdio.h>
#include "csapp.h"

int mysystem(char *command){
    pid_t pid;
    int status;

    if((pid = Fork()) == 0){//子进程
        char *argv[4] = {"", "-c", command, NULL};
        execve("/bin/sh", argv, environ);
    }

    printf("child pid: %d\n", pid);//打印子进程pid，方便kill。
    
    if(Waitpid(pid, &status, 0) > 0){
        //正常退出
        if (WIFEXITED(status))
            return WEXITSTATUS(status);

        //通过signal退出
        if (WIFSIGNALED(status))
            return WTERMSIG(status);
    }
}

int main(){
    int res;

    res = mysystem("./exit_code");
    printf("normally exit, res: %d\n", res); fflush(stdout);

    res = mysystem("./wait_sig");
    printf("signal exit, res: %d\n", res); fflush(stdout);
}
```

```c
//exit_code.c
#include "csapp.h"

int main(int argc, char* argv[]) {
  exit(10);
}
```

```c
//wait_sig.c
#include "csapp.h"

int main(int argc, char* argv[]) {
  while (1);
}
```

```
gcc -o wait_sig wait_sig.c 
gcc -o exit_code exit_code.c
gcc -o mysystem mysystem.c csapp.c -lpthread
```

运行结果：

![image-20220704110142313](https://raw.githubusercontent.com/damenshi/myImage/main/img/image-20220704110142313.png)

>  运行wait_sig ，另开一个终端窗口，输入kill -9 2795.



8.23

在任何时刻，一种类型至多只能有一个待处理信号，其他的会被丢弃。（书中528页解释）



8.24

```
void psignal(int sig, const char *s);
The  psignal()  function  displays a message on stderr consisting of the string s, a colon, a space, a string describing the signal number sig, and a trailing new‐line.  If the string s is NULL or empty, the colon and space are omitted.  If sig is invalid, the message displayed will indicate an unknown signal.
```

```c
#include "csapp.h"
#define N 2

int main(){
    int status, i;
    pid_t pid;

    for(i = 0; i < N; i++){
        if((pid = Fork()) == 0){
            int *p = NULL;
            *p = 0;//给地址0x0赋值会产生段错误。
        }
    }

    while((pid = waitpid(-1, &status, 0)) > 0){
        if(WIFEXITED(status)){
            printf("child %d terminate normally with exit status = %d\n", pid, WEXITSTATUS(status));
        }else if(WIFSIGNALED(status)){
            char buf[100];
            sprintf(buf, "child %d terminate by signal %d", pid, WTERMSIG(status));
            psignal(WTERMSIG(status), buf);
        }else{
            printf("child %d terminated abnormamally\n", pid);
        }
    }

    if(errno != ECHILD)
        unix_error("waitpid error");

    exit(0);
}
```

![image-20220704120413498](https://raw.githubusercontent.com/damenshi/myImage/main/img/image-20220704120413498.png)



8.25

使用了书中p546中8.6节非本地跳转技术。

> 我们在第一次使用`sigsetjmp(buf, 1)`时进入正常的读入（此时sigsetjmp返回值为0），但当信号出现（时间截止），信号处理函数就会`siglongjmp(buf，1)` （此时sigsetjmp返回值为1）,进入return NULL分支。

```c
#include<stdio.h>
#include "csapp.h"

#define LEN 100
sigjmp_buf buf;

void handler(int sig){
    siglongjmp(buf, 1);
}

char* tfgets(char *s, int size, FILE *stream){
    if(!sigsetjmp(buf, 1)){
        alarm(5);//alarm函数，其到指定时间后会raise一个SIGALRM信号
        if(signal(SIGALRM, handler) == SIG_ERR){
            unix_error("set alarm handler error");
        }
        return fgets(s, size, stream);
    }else{
        return NULL;
    }
}

int main(){
    char buf[LEN];
    char *input = tfgets(buf, LEN, stdin);
    if(input){
        printf("%s", input);
    }else{
        printf("NULL\n");
    }
    return 0;
}
```



8.26

本题对应shell lab实验，[shell lab]().