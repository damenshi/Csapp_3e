# shell lab

+ [writeup](http://csapp.cs.cmu.edu/3e/shlab.pdf)

+ [readme](http://csapp.cs.cmu.edu/3e/README-shlab)

+ **系统编程的书籍**：推荐两本书：

  一本是《UNIX环境高级编程》，简称APUE，是已故知名技术作家Richard Stevens的传世之作。

  另外一本是《Linux编程接口》，简称TLPI，是针对Linux环境的编程完全手册。

1. eval

   ```c
   /* 
    * eval - Evaluate the command line that the user has just typed in
    * 
    * If the user has requested a built-in command (quit, jobs, bg or fg)
    * then execute it immediately. Otherwise, fork a child process and
    * run the job in the context of the child. If the job is running in
    * the foreground, wait for it to terminate and then return.  Note:
    * each child process must have a unique process group ID so that our
    * background children don't receive SIGINT (SIGTSTP) from the kernel
    * when we type ctrl-c (ctrl-z) at the keyboard.  
   */
   void eval(char *cmdline) 
   {
       char *argv[MAXARGS]; //用来保存所有命令行参数
       int bg;//说明是进程是前台运行还是后台运行
       pid_t pid;//进程id
   
       //分析输入的cmdline，将参数保存在argv，如果请求的是后台进程，则返回1
       bg = parseline(cmdline, argv);
       if(argv[0] == NULL)
           return;
      
       //如果不是内置命令
       if(!builtin_cmd(argv)){
           //参照书中8-40code
           sigset_t mask_all, mask_one, prev_one;
           sigfillset(&mask_all);//initializes set to full, including all signals.
           sigemptyset(&mask_one);//所有信号都被排除在该集合之外
           sigaddset(&mask_one, SIGCHLD);
   
           //在fork之前阻塞sigchild信号
           sigprocmask(SIG_BLOCK, &mask_one, &prev_one);//将sigchild添加到blocked位向量中，prev_one保存之前的blocked位向量
           if((pid = fork()) == 0){//子进程
               //setpgid(pid, pgid)将pid的进程组id设为参数pgid。如果参数pid为0, 则会用来设置目前进程的进程组id, 如果参数pgid为0, 则会以目前进程的pid作为进程组ID。
               //为了与父进程的pgid区分开，还可以使子进程的pid与pgid相同
               setpgid(0, 0);
   
               //在exev之前，必须解除屏蔽sigchild
               sigprocmask(SIG_SETMASK, &prev_one, NULL);
               //成功情况下execve不返回。
               if(execve(argv[0], argv, environ) < 0){
                   printf("%s: command not found\n", argv[0]);
                   exit(0);
               }
           }
           //父进程屏蔽所有信号
           sigprocmask(SIG_BLOCK, &mask_all, NULL);
           if(!bg){//如果是前台进程,等待子进程执行完毕。
               addjob(jobs, pid, FG, cmdline);
               waitfg(pid);
           }else{
               addjob(jobs, pid, BG, cmdline);
               printf("[%d] (%d) %s", pid2jid(pid), pid, cmdline);
           }
           //解除对sigchild的屏蔽
           sigprocmask(SIG_SETMASK, &prev_one, NULL);
       }
       return;
   }
   ```

   

2. builtin_cmd

   ```
   /* 
    * builtin_cmd - If the user has typed a built-in command then execute
    *    it immediately.  
    */
   int builtin_cmd(char **argv) 
   {   
       sigset_t mask_all, prev;
       sigfillset(&mask_all);
       sigprocmask(SIG_BLOCK, &mask_all, &prev);//block all signals
      
       //builtin_cmd quit jobs bg fg
       if(strcmp(argv[0], "quit") == 0){
           exit(0);
       }
       if(strcmp(argv[0], "jobs") == 0){
           listjobs(jobs);
           return 1;
       }
       if(strcmp(argv[0], "bg") == 0 || strcmp(argv[0], "fg") == 0){
           do_bgfg(argv);
           return 1;
       }
       sigprocmask(SIG_SETMASK, &prev, NULL);//unblock all signals
       return 0;     /* not a builtin command */
   }
   ```




3. do_bgfg

   ```c++
   /* 
    * do_bgfg - Execute the builtin bg and fg commands
    */
   void do_bgfg(char **argv) 
   {   
       struct job_t *job;
       
       if(argv[1] == NULL){
           printf("%s command requires PID or %%jobid argument\n", argv[0]);
           return;
       }
       if((argv[1][0] < '0' || argv[1][0] > '9') && argv[1][0] != '%') {
           printf("%s: argument must be a PID or %%jobid\n", argv[0]);
           return;
       }
       //if <job> argument is a JID
       if(argv[1][0] == '%'){//jobid
           int jobid = atoi(argv[1] + 1);
           //getjobjid  - Find a job (by JID) on the job list 
          
           job = getjobjid(jobs, jobid);
           
           if(job == NULL || job->pid == 0){
               printf("%s: No such job\n", argv[1]);
               return;
           }else{
               if(job->state == ST){
                   //send SIGCONT to pid
                   if(kill(job->pid, SIGCONT) < 0){
                       unix_error("builtin_cmd: fg: kill error\n");
                   }
               }       
           }
       }else{//if <job> argument is a PID
           int pid = atoi(argv[1]);
           //getjobpid  - Find a job (by PID) on the job list 
           job = getjobpid(jobs, pid);
           if(job == NULL){
               printf("(%s): No such process\n", argv[1]);
               return;
           }else{
               if(job->state == ST){
                   if(kill(job->pid, SIGCONT) < 0){
                       unix_error("builtin_cmd: fg: kill error\n");
                   }
               }       
           }
       }
   
       //如果是前台进程
       if(strcmp(argv[0], "fg") == 0 ){
           job->state = FG;
           waitfg(job->pid);
       }else{
           job->state = BG;
           printf("[%d] (%d) %s", job->jid, job->pid, job->cmdline);
       }
       return;
   }
   ```

  

4. waitfg

   ```c
   /* 
    * waitfg - Block until process pid is no longer the foreground process
    */
    void waitfg(pid_t pid)
   {
       sigset_t mask;
       sigemptyset(&mask);
       //fpgid: Return PID of current foreground job
       while(pid == fgpid(jobs)){
           sigsuspend(&mask);
       }
       return;
   }
   ```

   

5. sigchld_handler

   ```C++
   /* 
    * sigchld_handler - The kernel sends a SIGCHLD to the shell whenever
    *     a child job terminates (becomes a zombie), or stops because it
    *     received a SIGSTOP or SIGTSTP signal. The handler reaps all
    *     available zombie children, but doesn't wait for any other
    *     currently running children to terminate.  
    */
   void sigchld_handler(int sig) 
   {
       //参考书中8-40 handler 以及8.4.3回收子进程
       int olderrno = errno;//保存errno
       sigset_t mask_all, prev_all;
       pid_t pid;
       int status;
   
       sigfillset(&mask_all);
   
       //pid = -1,等待父进程所有的子进程
       while((pid = waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0){
           if(WIFEXITED(status)){//正常退出
               sigprocmask(SIG_BLOCK, &mask_all, &prev_all);//用来同步进程8-40
               deletejob(jobs, pid);
               sigprocmask(SIG_SETMASK, &prev_all, NULL);
           }else if(WIFSIGNALED(status)){//异常退出
               struct job_t *job = getjobpid(jobs, pid);
               printf("Job [%d] (%d) terminated by signal %d\n", job->jid, job->pid, WTERMSIG(status));
               sigprocmask(SIG_BLOCK, &mask_all, &prev_all);//用来同步进程8-40
               deletejob(jobs, pid);
               sigprocmask(SIG_SETMASK, &prev_all, NULL);
           }else if(WIFSTOPPED(status)){//子进程是停止的
               struct job_t *job = getjobpid(jobs, pid);
               printf("Job [%d] (%d) stoped by signal %d\n", job->jid, job->pid, WSTOPSIG(status));
               sigprocmask(SIG_BLOCK, &mask_all, &prev_all);//用来同步进程8-40
               job->state = ST;
               sigprocmask(SIG_SETMASK, &prev_all, NULL);
           }else{
               app_error("sigchld_handler: should never arrive here\n");
           }
       }
       errno = olderrno;
       return;
   }
   ```

   

6. sigint_handler

   ```C++
   /* 
    * sigint_handler - The kernel sends a SIGINT to the shell whenver the
    *    user types ctrl-c at the keyboard.  Catch it and send it along
    *    to the foreground job.  
    */
   void sigint_handler(int sig) 
   {   
       sigset_t mask_all, prev_all;
       sigfillset(&mask_all);
       sigprocmask(SIG_BLOCK, &mask_all, &prev_all);
   
       //gpid - Return PID of current foreground job, 0 if no such job 
       pid_t pid;
       if((pid = fgpid(jobs)) == 0)
           return;
   
       int olderrno = errno;
       if(kill(pid, SIGINT)){
           unix_error("sigint_handler: kill error");
       }
       errno = olderrno;
       sigprocmask(SIG_SETMASK, &prev_all, NULL);
       return;
   }
   ```

   

7. sigtstp_handler

   ```C++
   void sigtstp_handler(int sig) 
   {   
       sigset_t mask_all, prev_all;
       sigfillset(&mask_all);
       sigprocmask(SIG_BLOCK, &mask_all, &prev_all);
   
       //gpid - Return PID of current foreground job, 0 if no such job 
       pid_t pid;
       if((pid = fgpid(jobs)) == 0)
           return;
   
       int olderrno = errno;
       if(kill(pid, SIGSTOP)){
           unix_error("sigint_handler: kill error");
       }
       errno = olderrno;
       sigprocmask(SIG_SETMASK, &prev_all, NULL);
       return;
   }
   ```

   