# chap10 homework

10.6

fd2 = 4

10.7

```c
#include "csapp.h"

int main(){
	int n;
	rio_t rio;
	char buf[MAXBUF];
	rio_readinitb(&rio, STDIN_FILENO);
	while(n = Rio_readn(&rio, buf, MAXBUF) != 0){
			Rio_writen(STDOUT_FILENO, buf, n);
	}
	return 0;
}
```

10.8

```c
#include "csapp.h"

int main(int argc, char* argv[]){
	struct stat stat;
	char *type, char *readok;
	
  //修改部分
	int fd;
	if(argc <= 1)
		fd = 0;
	else
		fd = atoi(argv[1]);
	fstat(fd, &stat);
	//
  
	if (S_ISREG(stat.st_mode))
    type = "regular";
  else if (S_ISDIR(stat.st_mode))
    type = "dir";
  else
    type = "other";
  if ((stat.st_mode & S_IRUSR))
    readok = "yes";
  else
    readok = "no";

  printf("type: %s, read: %s\n", type, readok);

  return 0;
}
```

10.9

```
if(Fork() == 0){
	dup2(0, 3);//将描述符3重定向到标准输入。
	Execve("fstatcheck", argv, envp);
}
```

10.10

```c
#include "csapp.h"

int main(int argc, char *argv[]){
	int n;
	rio_t rio;
	char buf[MAXLINE];
  if(argc == 2){
    int fd = Open(argv[1], O_RDONLY, 0);
    rio_readinitb(&rio, fd);
  }else
		rio_readinitb(&rio, STDIN_FILENO);
  
	while(n = Rio_readlineb(&rio, buf, MAXLINE) != 0){
			Rio_writen(STDOUT_FILENO, buf, n);
	}
	return 0;
}
```

