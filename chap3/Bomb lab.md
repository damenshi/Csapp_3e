---
attachments: [Clipboard_2022-06-01-00-07-05.png]
tags: [csapp/05-09]
title: Bomb lab
created: '2022-05-27T15:04:33.557Z'
modified: '2022-06-03T15:36:46.046Z'
---

# Bomb lab

+ [实验说明](https://www.bilibili.com/video/BV1a54y1k7YE?p=11)

+ objdump -t
  This will print out the bomb’s symbol table. The symbol table includes the names of all functions and global variables in the bomb, the names of all the functions the bomb calls, and their addresses. You may learn something by looking at the function names!
+ objdump -d
  Use this to disassemble all of the code in the bomb. You can also just look at individual functions.
  Reading the assembler code can tell you how the bomb works.

+ strings bomb | less

+ gdb
  [gdb用法](http://csapp.cs.cmu.edu/3e/docs/gdbnotes-x86-64.pdf)
  > gdb bomb
  > help breakpoints
  > help break
  > break explode_bomb //在explode_bomb 前加断点
  > run
  > ctrl + c //离开gdb

  > 例子：phase1
    break phase_1
    disas
    help disas
    info registers //查看全部寄存器信息
    print $rsp  // 查看单个寄存器信息
    stepi       //按步执行指令
    x $rsp      //十六进制显示($rsp)
    nexti
    stack


1. phase_1

    gdb bomb
    break explode_bomb
    break phase_1 
    run
    disas

        phase_1()
          0x0000000000400ee0 <+0>:     sub    $0x8,%rsp  // %rsp -= 8
          0x0000000000400ee4 <+4>:     mov    $0x402400,%esi // %esi = $0x402400
          0x0000000000400ee9 <+9>:     callq  0x401338 <strings_not_equal> 
          0x0000000000400eee <+14>:    test   %eax,%eax 
          0x0000000000400ef0 <+16>:    je     0x400ef7 <phase_1+23> // if %eax = 0， jump
          0x0000000000400ef2 <+18>:    callq  0x40143a <explode_bomb>
          0x0000000000400ef7 <+23>:    add    $0x8,%rsp
          0x0000000000400efb <+27>:    retq  

   stepi

        strings_not_equal:
          0x0000000000401338 <+0>:     push   %r12 //store %12
          0x000000000040133a <+2>:     push   %rbp 
          0x000000000040133b <+3>:     push   %rbx
          0x000000000040133c <+4>:     mov    %rdi,%rbx //%rbx = %rdi,此处%rdi 存放phase的第一个参数，即input
          0x000000000040133f <+7>:     mov    %rsi,%rbp //%rbp = %rsi，%rsi中为设定的字符串的地址
          0x0000000000401342 <+10>:    callq  0x40131b <string_length>
          0x0000000000401347 <+15>:    mov    %eax,%r12d // %r12d = %eax
          0x000000000040134a <+18>:    mov    %rbp,%rdi // %rdi = %rbp
          0x000000000040134d <+21>:    callq  0x40131b <string_length>
          0x0000000000401352 <+26>:    mov    $0x1,%edx //%edx = 1
          0x0000000000401357 <+31>:    cmp    %eax,%r12d //cmp %eax %r12d 两个字符串长度不同，则不等
          0x000000000040135a <+34>:    jne    0x40139b <strings_not_equal+99>

        strings_length:
          0x000000000040131b <+0>:     cmpb   $0x0,(%rdi)
          0x000000000040131e <+3>:     je     0x401332 <string_length+23> if M[%rdi] = 0, jump
          0x0000000000401320 <+5>:     mov    %rdi,%rdx
          0x0000000000401323 <+8>:     add    $0x1,%rdx  //%rdx += 1
          0x0000000000401327 <+12>:    mov    %edx,%eax  //%eax = %edx
          0x0000000000401329 <+14>:    sub    %edi,%eax  //%eax -= %edi
          0x000000000040132b <+16>:    cmpb   $0x0,(%rdx)
          0x000000000040132e <+19>:    jne    0x401323 <string_length+8>
          0x0000000000401330 <+21>:    repz retq 
          0x0000000000401332 <+23>:    mov    $0x0,%eax
          0x0000000000401337 <+28>:    retq 
          //从%rdi的值指向的内存地址开始，一直往后走到遇到‘\0',返回长度为‘\0'地址-初始地址。
          stepi
          print $rbx
          $1 = 6305664
          print $rbp
          $2 = (void *) 0x402400
          (gdb) print (char*) 0x402400
          ru$3 = 0x402400 "Border relations with Canada have never been better."
          + result:Border relations with Canada have never been better.

2. phase_2

        phase_2():
          0x0000000000400efc <+0>:     push   %rbp
          0x0000000000400efd <+1>:     push   %rbx
          0x0000000000400efe <+2>:     sub    $0x28,%rsp
          0x0000000000400f02 <+6>:     mov    %rsp,%rsi //栈指针作为第二个参数
          0x0000000000400f05 <+9>:     callq  0x40145c <read_six_numbers>
          0x0000000000400f0a <+14>:    cmpl   $0x1,(%rsp) // 比较栈指针的内存值是否为1 确定第一个数为1
          0x0000000000400f0e <+18>:    je     0x400f30 <phase_2+52> //是，跳转
          0x0000000000400f10 <+20>:    callq  0x40143a <explode_bomb> //否，爆炸
          0x0000000000400f15 <+25>:    jmp    0x400f30 <phase_2+52>
          0x0000000000400f17 <+27>:    mov    -0x4(%rbx),%eax //begin loop %eax = %rbx - 0x4
          0x0000000000400f1a <+30>:    add    %eax,%eax
          0x0000000000400f1c <+32>:    cmp    %eax,(%rbx) //比较%eax和（%rbx）
          //确定每个数字是前一个数字的2倍
          0x0000000000400f1e <+34>:    je     0x400f25 <phase_2+41> //相等跳转
          0x0000000000400f20 <+36>:    callq  0x40143a <explode_bomb> //不等爆炸
          0x0000000000400f25 <+41>:    add    $0x4,%rbx //%rbx指向下一个数
          0x0000000000400f29 <+45>:    cmp    %rbp,%rbx //cmp %rbx, %rbp 
          0x0000000000400f2c <+48>:    jne    0x400f17 <phase_2+27> //没比完，继续比较
          0x0000000000400f2e <+50>:    jmp    0x400f3c <phase_2+64>//比较完所有数字，则循环结束
          0x0000000000400f30 <+52>:    lea    0x4(%rsp),%rbx //%rbx = %rsp + 0x4
          0x0000000000400f35 <+57>:    lea    0x18(%rsp),%rbp //%rbp = %rsp + 0x18 指向最后一个数
          0x0000000000400f3a <+62>:    jmp    0x400f17 <phase_2+27>
          0x0000000000400f3c <+64>:    add    $0x28,%rsp
          0x0000000000400f40 <+68>:    pop    %rbx
          0x0000000000400f41 <+69>:    pop    %rbp
          0x0000000000400f42 <+70>:    retq   

        read_six_numbers():
          0x000000000040145c <+0>:     sub    $0x18,%rsp
          0x0000000000401460 <+4>:     mov    %rsi,%rdx// 第二个参数存放的调用前%rsp
          0x0000000000401463 <+7>:     lea    0x4(%rsi),%rcx //%rcx = %rsi + 0x4
          0x0000000000401467 <+11>:    lea    0x14(%rsi),%rax //%rax = %rsi + 0x14
          0x000000000040146b <+15>:    mov    %rax,0x8(%rsp)  //M[%rsp + 0x8] = %rax
          0x0000000000401470 <+20>:    lea    0x10(%rsi),%rax //%rax = %rsi + 0x10
          0x0000000000401474 <+24>:    mov    %rax,(%rsp)     //M[%rsp] = %rax
          0x0000000000401478 <+28>:    lea    0xc(%rsi),%r9   //%r9 = %rsi + 0xc
          0x000000000040147c <+32>:    lea    0x8(%rsi),%r8   //%r8 = %rsi + 0x8
          0x0000000000401480 <+36>:    mov    $0x4025c3,%esi  //%esi = 0x4025c3
          0x0000000000401485 <+41>:    mov    $0x0,%eax       //%eax = 0
          0x000000000040148a <+46>:    callq  0x400bf0 <__isoc99_sscanf@plt> // 成功则返回读入数据的个数
          0x000000000040148f <+51>:    cmp    $0x5,%eax //cmp %eax , 0x5 
          0x0000000000401492 <+54>:    jg     0x401499 <read_six_numbers+61> //大于5
          0x0000000000401494 <+56>:    callq  0x40143a <explode_bomb>
          0x0000000000401499 <+61>:    add    $0x18,%rsp
          0x000000000040149d <+65>:    retq   
      
      > 从栈顶，每隔4字节，把地址放在寄存器中，共放了4个，还有两个把内存地址存在栈中，为什么呢，因为sscanf这个函数，有8个参数，第一个是input，第二个是输入的模式串，之后6个是读取的6个值存的地址，因为最多只能传6个寄存器参数，所以最后两个把内存地址存在栈中 。六个地址分别存在%rdx, %rcx, %r8, %r9, (%rsp), 8(%rsp)处，其中的值为0x0(%rsi), 0x4(%rsi), 0x8(%rsi), 0xc(%rsi), 0x10(%rsi), 0x14(%rsi)处（即从调用函数栈顶向上依次存放6个数，共24字节）。

      先确定第一个数字是1，每个数字是前一个数字的2倍
      result: 1 2 4 8 16 32

3. phase_3
  
        phase_3():
        0x0000000000400f43 <+0>:     sub    $0x18,%rsp
        0x0000000000400f47 <+4>:     lea    0xc(%rsp),%rcx //第4个参数
        0x0000000000400f4c <+9>:     lea    0x8(%rsp),%rdx //第3个参数
        0x0000000000400f51 <+14>:    mov    $0x4025cf,%esi //第2个参数 format
        0x0000000000400f56 <+19>:    mov    $0x0,%eax  
        0x0000000000400f5b <+24>:    callq  0x400bf0 <__isoc99_sscanf@plt>//返回参数个数
        0x0000000000400f60 <+29>:    cmp    $0x1,%eax //比较返回值和1
        0x0000000000400f63 <+32>:    jg     0x400f6a <phase_3+39>//大于则jump
        0x0000000000400f65 <+34>:    callq  0x40143a <explode_bomb>//否则爆炸
        0x0000000000400f6a <+39>:    cmpl   $0x7,0x8(%rsp) //比较输入的第一个值和7
        0x0000000000400f6f <+44>:    ja     0x400fad <phase_3+106> //大于7爆炸
        0x0000000000400f71 <+46>:    mov    0x8(%rsp),%eax //%eax中放入第一个输入值
        0x0000000000400f75 <+50>:    jmpq   *0x402470(,%rax,8) // 跳转到M(8*%rax + 0x402470)
        0x0000000000400f7c <+57>:    mov    $0xcf,%eax//%eax = 0xcf
        0x0000000000400f81 <+62>:    jmp    0x400fbe <phase_3+123>
        0x0000000000400f83 <+64>:    mov    $0x2c3,%eax
        0x0000000000400f88 <+69>:    jmp    0x400fbe <phase_3+123>
        0x0000000000400f8a <+71>:    mov    $0x100,%eax
        0x0000000000400f8f <+76>:    jmp    0x400fbe <phase_3+123>
        0x0000000000400f91 <+78>:    mov    $0x185,%eax
        0x0000000000400f96 <+83>:    jmp    0x400fbe <phase_3+123>
        0x0000000000400f98 <+85>:    mov    $0xce,%eax
        0x0000000000400f9d <+90>:    jmp    0x400fbe <phase_3+123>
        0x0000000000400f9f <+92>:    mov    $0x2aa,%eax
        0x0000000000400fa4 <+97>:    jmp    0x400fbe <phase_3+123>
        0x0000000000400fa6 <+99>:    mov    $0x147,%eax
        0x0000000000400fab <+104>:   jmp    0x400fbe <phase_3+123>
        0x0000000000400fad <+106>:   callq  0x40143a <explode_bomb>//爆炸
        0x0000000000400fb2 <+111>:   mov    $0x0,%eax
        0x0000000000400fb7 <+116>:   jmp    0x400fbe <phase_3+123>
        0x0000000000400fb9 <+118>:   mov    $0x137,%eax
        0x0000000000400fbe <+123>:   cmp    0xc(%rsp),%eax // 比较第二个输入值 和 %eax
        0x0000000000400fc2 <+127>:   je     0x400fc9 <phase_3+134>//相等则结束
        0x0000000000400fc4 <+129>:   callq  0x40143a <explode_bomb>//不等则爆炸
        0x0000000000400fc9 <+134>:   add    $0x18,%rsp
        0x0000000000400fcd <+138>:   retq   

        依次查看地址0x402470 + 8*0 ～ 0x402470 + 8*7 的内存值。
          (gdb) x/w 0x402470              //0
          0x402470:       0x00400f7c      //0xcf
          (gdb) x/w 0x402478              //1
          0x402478:       0x00400fb9      //0x137
          (gdb) x/w 0x402480              //2
          0x402480:       0x00400f83      //0x2c3
          (gdb) x/w 0x402488              //3
          0x402488:       0x00400f8a      //0x100
          (gdb) x/w 0x402490              //4
          0x402490:       0x00400f91      //0x185
          (gdb) x/w 0x402498              //5
          0x402498:       0x00400f98      //0xce
          (gdb) x/w 0x4024A0              //6
          0x4024a0:       0x00400f9f      //0x2aa
          (gdb) x/w 0x4024A8              //7
          0x4024a8:       0x00400fa6      //0x147
          任选一组答案。

          (gdb) x/8ga 0x402470
            0x402470:       0x400f7c <phase_3+57>   0x400fb9 <phase_3+118>
            0x402480:       0x400f83 <phase_3+64>   0x400f8a <phase_3+71>
            0x402490:       0x400f91 <phase_3+78>   0x400f98 <phase_3+85>
            0x4024a0:       0x400f9f <phase_3+92>   0x400fa6 <phase_3+99>

4. phase_4

        phase_4():
        0x000000000040100c <+0>:     sub    $0x18,%rsp  
        0x0000000000401010 <+4>:     lea    0xc(%rsp),%rcx //第四个参数
        0x0000000000401015 <+9>:     lea    0x8(%rsp),%rdx //第三个参数
        0x000000000040101a <+14>:    mov    $0x4025cf,%esi
        0x000000000040101f <+19>:    mov    $0x0,%eax
        0x0000000000401024 <+24>:    callq  0x400bf0 <__isoc99_sscanf@plt>
        0x0000000000401029 <+29>:    cmp    $0x2,%eax //比较返回值和0x2
        0x000000000040102c <+32>:    jne    0x401035 <phase_4+41>// 不等于则爆炸
        0x000000000040102e <+34>:    cmpl   $0xe,0x8(%rsp)// 比较第一个输入值和0xe
        0x0000000000401033 <+39>:    jbe    0x40103a <phase_4+46>// <= 则跳转
        0x0000000000401035 <+41>:    callq  0x40143a <explode_bomb>//爆炸
        0x000000000040103a <+46>:    mov    $0xe,%edx //第3个参数 0xe
        0x000000000040103f <+51>:    mov    $0x0,%esi //第2个参数 0x0
        0x0000000000401044 <+56>:    mov    0x8(%rsp),%edi //第1个参数设为第1个输入值
        0x0000000000401048 <+60>:    callq  0x400fce <func4>// call func4
        0x000000000040104d <+65>:    test   %eax,%eax //
        0x000000000040104f <+67>:    jne    0x401058 <phase_4+76>// %eax!=0 则爆炸
        0x0000000000401051 <+69>:    cmpl   $0x0,0xc(%rsp)//比较0x0和第二个参数
        0x0000000000401056 <+74>:    je     0x40105d <phase_4+81>//相等则return
        0x0000000000401058 <+76>:    callq  0x40143a <explode_bomb>
        0x000000000040105d <+81>:    add    $0x18,%rsp
        0x0000000000401061 <+85>:    retq   

    > (gdb) print (char*) 0x4025cf
    $19 = 0x4025cf "%d %d"
    已知第二个输入值为0，第一个输入值范围为0～14.

        func4():
        0x0000000000400fce <+0>:     sub    $0x8,%rsp
        0x0000000000400fd2 <+4>:     mov    %edx,%eax//%eax = e
        0x0000000000400fd4 <+6>:     sub    %esi,%eax//%eax = e - 0
        0x0000000000400fd6 <+8>:     mov    %eax,%ecx//%ecx = e
        0x0000000000400fd8 <+10>:    shr    $0x1f,%ecx// %ecx = e >> 0x1f(31) = 0
        0x0000000000400fdb <+13>:    add    %ecx,%eax //%eax = e
        0x0000000000400fdd <+15>:    sar    %eax // %eax = 0x7 隐含右移1位
        0x0000000000400fdf <+17>:    lea    (%rax,%rsi,1),%ecx //%ecx = %rsi + %rax=0x7
        0x0000000000400fe2 <+20>:    cmp    %edi,%ecx //cmp %ecx, %edi 第一个参数和7比较
        0x0000000000400fe4 <+22>:    jle    0x400ff2 <func4+36> // <=7  则jump
        0x0000000000400fe6 <+24>:    lea    -0x1(%rcx),%edx //%edx = %ecx - 1 = 6
        0x0000000000400fe9 <+27>:    callq  0x400fce <func4>
        0x0000000000400fee <+32>:    add    %eax,%eax
        0x0000000000400ff0 <+34>:    jmp    0x401007 <func4+57>
        0x0000000000400ff2 <+36>:    mov    $0x0,%eax //%eax = 0
        0x0000000000400ff7 <+41>:    cmp    %edi,%ecx // 第一个参数和7比较
        0x0000000000400ff9 <+43>:    jge    0x401007 <func4+57> >=7 则返回
        0x0000000000400ffb <+45>:    lea    0x1(%rcx),%esi //%esi = %rcx + 1
        0x0000000000400ffe <+48>:    callq  0x400fce <func4>
        0x0000000000401003 <+53>:    lea    0x1(%rax,%rax,1),%eax //%eax = %rax + %rax + 0x1
        0x0000000000401007 <+57>:    add    $0x8,%rsp
        0x000000000040100b <+61>:    retq   

    > 当第一个参数为7时，返回值为0.满足要求。
    result: 7 0

        C语言版：
        #include<stdio.h>

        //a in %rdi, b in %rsi, c in %rdx
        int func4(int a, int b, int c){
            int result = c - b; // result in %rax
            int t = ((unsigned)result) >> 31;// in %rcx
            result = (t + result)>> 1;
            t = b + result;
            if(t - a > 0){
                c = t - 1;
                int r = func4(a, b, c);
                return 2*r;
            }else{
                result = 0;
                if(t - a >= 0){
                    return result;
                }else{
                    b = t + 1;
                    int r = func4(a, b, c);
                    return 2*r + 1;
                }
            }
        }

        int main(){
            for(int i = 0; i <= 14; i++){
                if(func4(i, 0, 14) == 0){
                    printf("%d\n", i);//0 1 3 7
                }
            }
            return 0;
        }
        result: 0 0; 1 0; 3 0; 7 0; 


5. phase_5:

        phase_5():
        0x0000000000401062 <+0>:     push   %rbx
        0x0000000000401063 <+1>:     sub    $0x20,%rsp
        0x0000000000401067 <+5>:     mov    %rdi,%rbx  //%rbx = %rdi
        0x000000000040106a <+8>:     mov    %fs:0x28,%rax // 随机值，用来检测栈是否溢出
        0x0000000000401073 <+17>:    mov    %rax,0x18(%rsp)// M[%rsp + 0x18] = %rax
        0x0000000000401078 <+22>:    xor    %eax,%eax //%eax = 0
        0x000000000040107a <+24>:    callq  0x40131b <string_length>
        0x000000000040107f <+29>:    cmp    $0x6,%eax // 比较输入字符串长度 和 6
        0x0000000000401082 <+32>:    je     0x4010d2 <phase_5+112> // =6
        0x0000000000401084 <+34>:    callq  0x40143a <explode_bomb>//不等则爆炸
        0x0000000000401089 <+39>:    jmp    0x4010d2 <phase_5+112>
        0x000000000040108b <+41>:    movzbl (%rbx,%rax,1),%ecx //%ecx = M[%rax+%rbx] 输入的字符串每个字符
        0x000000000040108f <+45>:    mov    %cl,(%rsp) //M[%rsp] = %cl
        0x0000000000401092 <+48>:    mov    (%rsp),%rdx //%rdx = M[%rsp] = %cl
        0x0000000000401096 <+52>:    and    $0xf,%edx //%edx &= 0xf 每个字符的后四位
        0x0000000000401099 <+55>:    movzbl 0x4024b0(%rdx),%edx //%edx = M[0x4024b0 + %rdx]
        0x00000000004010a0 <+62>:    mov    %dl,0x10(%rsp,%rax,1)// M[%rax + %rsp + 0x10] = %dl
        0x00000000004010a4 <+66>:    add    $0x1,%rax //%rax += 1
        0x00000000004010a8 <+70>:    cmp    $0x6,%rax //
        0x00000000004010ac <+74>:    jne    0x40108b <phase_5+41> //!= 6，循环
        //循环等价于
        //for(int i = 0; i < 6; i++>){
        //    narr[i] = array[input[i]&0xf]
        //}
        0x00000000004010ae <+76>:    movb   $0x0,0x16(%rsp)
        0x00000000004010b3 <+81>:    mov    $0x40245e,%esi  //内存中的字符串
        0x00000000004010b8 <+86>:    lea    0x10(%rsp),%rdi // 根据输入取到的字符串narr
        0x00000000004010bd <+91>:    callq  0x401338 <strings_not_equal>
        0x00000000004010c2 <+96>:    test   %eax,%eax
        0x00000000004010c4 <+98>:    je     0x4010d9 <phase_5+119>// == 0 jump
        0x00000000004010c6 <+100>:   callq  0x40143a <explode_bomb>
        0x00000000004010cb <+105>:   nopl   0x0(%rax,%rax,1)
        0x00000000004010d0 <+110>:   jmp    0x4010d9 <phase_5+119>
        0x00000000004010d2 <+112>:   mov    $0x0,%eax //%eax = 0
        0x00000000004010d7 <+117>:   jmp    0x40108b <phase_5+41>
        0x00000000004010d9 <+119>:   mov    0x18(%rsp),%rax //
        0x00000000004010de <+124>:   xor    %fs:0x28,%rax
        0x00000000004010e7 <+133>:   je     0x4010ee <phase_5+140> //相等，表示未溢出，返回
        0x00000000004010e9 <+135>:   callq  0x400b30 <__stack_chk_fail@plt>
        0x00000000004010ee <+140>:   add    $0x20,%rsp
        0x00000000004010f2 <+144>:   pop    %rbx
        0x00000000004010f3 <+145>:   retq 

    > (gdb) x/16c 0x4024b0
    0x4024b0 <array.3449>:  109 'm' 97 'a'  100 'd' 117 'u' 105 'i' 101 'e' 114 'r' 115 's'
    0x4024b8 <array.3449+8>:        110 'n' 102 'f' 111 'o' 116 't' 118 'v' 98 'b'  121 'y' 108 'l'

    > (gdb) x/s 0x40245e
    0x40245e:       "flyers"  
    对应0x4024b0中的位置为9 15 14 5 6 7
    输入字符的后四位为1001 1111 1110 0101 0110 0111 对应字符为'i''o''n''e''f''g'

    ![](@attachment/Clipboard_2022-06-01-00-07-05.png)
    图中任意满足flyers的组合都满足题意。

6. phase_6

        phase_6():
        0x00000000004010f4 <+0>:     push   %r14
        0x00000000004010f6 <+2>:     push   %r13
        0x00000000004010f8 <+4>:     push   %r12
        0x00000000004010fa <+6>:     push   %rbp
        0x00000000004010fb <+7>:     push   %rbx
        0x00000000004010fc <+8>:     sub    $0x50,%rsp
        0x0000000000401100 <+12>:    mov    %rsp,%r13
        0x0000000000401103 <+15>:    mov    %rsp,%rsi
        0x0000000000401106 <+18>:    callq  0x40145c <read_six_numbers> //读入6个数字
        //(gdb) x/6wd $rsp
        //0x7fffffffdd80: 1       2       3       4
        //0x7fffffffdd90: 5       6   
        //输入数字存放位置
        0x000000000040110b <+23>:    mov    %rsp,%r14
        0x000000000040110e <+26>:    mov    $0x0,%r12d //%r12 = 0， 计数器
        0x0000000000401114 <+32>:    mov    %r13,%rbp
        0x0000000000401117 <+35>:    mov    0x0(%r13),%eax //%eax = M[%r13]，遍历input
        0x000000000040111b <+39>:    sub    $0x1,%eax // 第一个数 -  1 <= 5
        0x000000000040111e <+42>:    cmp    $0x5,%eax //和0x5比较
        0x0000000000401121 <+45>:    jbe    0x401128 <phase_6+52> //<=5时继续
        //input - 1 <= 5, 1 <= input <= 6
        0x0000000000401123 <+47>:    callq  0x40143a <explode_bomb>//否则爆炸
        0x0000000000401128 <+52>:    add    $0x1,%r12d  //%r12d += 1
        0x000000000040112c <+56>:    cmp    $0x6,%r12d // 和6比较
        0x0000000000401130 <+60>:    je     0x401153 <phase_6+95>//等于6则跳转
        0x0000000000401132 <+62>:    mov    %r12d,%ebx //%ebx = %r12d, 计数器
        0x0000000000401135 <+65>:    movslq %ebx,%rax //%rax = %ebx
        0x0000000000401138 <+68>:    mov    (%rsp,%rax,4),%eax //%eax = M[4*%rax + %rsp] 遍历input,取下一个数
        0x000000000040113b <+71>:    cmp    %eax,0x0(%rbp) //这个数和上一个数比较
        0x000000000040113e <+74>:    jne    0x401145 <phase_6+81> // !=时继续
        0x0000000000401140 <+76>:    callq  0x40143a <explode_bomb>// 否则爆炸
        0x0000000000401145 <+81>:    add    $0x1,%ebx //%ebx += 1
        0x0000000000401148 <+84>:    cmp    $0x5,%ebx // cmp %ebx 0x5
        0x000000000040114b <+87>:    jle    0x401135 <phase_6+65> // <= jump
        0x000000000040114d <+89>:    add    $0x4,%r13 //%r13 += 4 取下一个数
        0x0000000000401151 <+93>:    jmp    0x401114 <phase_6+32>
        //双层循环遍历，input[0]~ input[5] 互不相等，且 <= 6,不能为0， 0 - 1会溢出，所以输入为1～6。
        //for(int i = 0; i < 6; i++){
        //   if(arr[i] - 1 > 5)
        //     bomb();
        //   for(int j = i + 1; j <= 5; j++){
        //     if(arr[i] == arr[j])
        //       bomb();
        //   }
        //}
        0x0000000000401153 <+95>:    lea    0x18(%rsp),%rsi //第6个数的下一个地址放在%rsi
        0x0000000000401158 <+100>:   mov    %r14,%rax //第1个数地址放在%rax
        0x000000000040115b <+103>:   mov    $0x7,%ecx //%ecx = 0x7
        0x0000000000401160 <+108>:   mov    %ecx,%edx //%edx = 0x7
        0x0000000000401162 <+110>:   sub    (%rax),%edx // %edx = 0x7 - input[0]
        0x0000000000401164 <+112>:   mov    %edx,(%rax) //  input[0] = 7 - input[0]
        0x0000000000401166 <+114>:   add    $0x4,%rax //%rax += 4,下一个数
        0x000000000040116a <+118>:   cmp    %rsi,%rax // 比较是否取到最后一个数
        0x000000000040116d <+121>:   jne    0x401160 <phase_6+108> != jump
        //遍历input，将值换为7 - input

        //往下为重点部分
        假设从 rsp+0x20 开始处的内存地址存放的6个元素（每个元素占8个字节）为 bi(1<=i<=6)
        for(int i=1; i<=6; i++)
        {
            if(arr[i] <= 1)
            {
                b[i] = 0x6032d0;
            }
            else
            {
                int j = 1;
                b[i] = 0x6032d0;
                while(j != arr[i])
                {
                  b[i] = *(b[i] + 0x8);
                  ++j;
                }
            }
        }
        0x000000000040116f <+123>:   mov    $0x0,%esi //%esi = 0
        0x0000000000401174 <+128>:   jmp    0x401197 <phase_6+163>

        0x0000000000401176 <+130>:   mov    0x8(%rdx),%rdx //%rdx = M[%rdx + 0x8]
        0x000000000040117a <+134>:   add    $0x1,%eax//%eax为计数器
        0x000000000040117d <+137>:   cmp    %ecx,%eax //j != array[i]
        0x000000000040117f <+139>:   jne    0x401176 <phase_6+130>
        0x0000000000401181 <+141>:   jmp    0x401188 <phase_6+148>

        0x0000000000401183 <+143>:   mov    $0x6032d0,%edx //%edx = 0x6032d0
        0x0000000000401188 <+148>:   mov    %rdx,0x20(%rsp,%rsi,2) //M[2*%rsi + %rsp + 0x20] = %rdx
        0x000000000040118d <+153>:   add    $0x4,%rsi //循环
        0x0000000000401191 <+157>:   cmp    $0x18,%rsi//遍历每个input
        0x0000000000401195 <+161>:   je     0x4011ab <phase_6+183> // == jump
        0x0000000000401197 <+163>:   mov    (%rsp,%rsi,1),%ecx //%ecx = M[%rsi + %rsp]，依次获得6个数
        0x000000000040119a <+166>:   cmp    $0x1,%ecx // 比较input和1
        0x000000000040119d <+169>:   jle    0x401183 <phase_6+143> // <=1 往上跳
        //遍历input0～input5 <= 1, 循环  找到input中等于1的，M[%rsp + 0x20 + 2*%rsi] = 0x6032d0

        0x000000000040119f <+171>:   mov    $0x1,%eax //否则， %eax = 0x1
        0x00000000004011a4 <+176>:   mov    $0x6032d0,%edx //%edx = 0x6032d0
        0x00000000004011a9 <+181>:   jmp    0x401176 <phase_6+130>
        //
        (gdb) x/x 0x6032d0
        0x6032d0 <node1>:       0x0000014c
        结合 move 0x8(%rdx), %rdx; add = *(add + 8)，add + 8对应struct中的next

        输出结构体信息
        (gdb) x/24w 0x6032d0
        0x6032d0 <node1>:       0x0000014c      0x00000001      0x006032e0      0x00000000
        0x6032e0 <node2>:       0x000000a8      0x00000002      0x006032f0      0x00000000
        0x6032f0 <node3>:       0x0000039c      0x00000003      0x00603300      0x00000000
        0x603300 <node4>:       0x000002b3      0x00000004      0x00603310      0x00000000
        0x603310 <node5>:       0x000001dd      0x00000005      0x00603320      0x00000000
        0x603320 <node6>:       0x000001bb      0x00000006      0x00000000      0x00000000
        输入的值为 1 2 3 4 5 6，每个node第二个4字节和我们的输入相同。第三个4字节是下一个node的地址
        是一个链表。
        struct{
          int sth//第一个4字节
          int id //节点编号
          node* next;//下一个node地址
        }node
        该部分将链表的结点地址按7-input的顺序存在%rsp+0x20 ～ %rsp+0x42。
        例如 输入 1 2 3 4 5 6， 就把节点按照6 5 4 3 2 1的顺序放在%rsp+0x20 ～ %rsp+0x42。
        

          for(int i=1; i<=5; i++)
        {
            *(b[i] + 0x8) = b[i+1];
        }

        0x00000000004011ab <+183>:   mov    0x20(%rsp),%rbx //%rbx = head
        0x00000000004011b0 <+188>:   lea    0x28(%rsp),%rax //%rax = %rsp + 28
        0x00000000004011b5 <+193>:   lea    0x50(%rsp),%rsi //%rsi = %rsp + 50
        0x00000000004011ba <+198>:   mov    %rbx,%rcx //%rcx = head
        0x00000000004011bd <+201>:   mov    (%rax),%rdx //%rdx = head.next
        0x00000000004011c0 <+204>:   mov    %rdx,0x8(%rcx) //head.next = head.next
        0x00000000004011c4 <+208>:   add    $0x8,%rax //%rax = %rsp + 28 + 8
        0x00000000004011c8 <+212>:   cmp    %rsi,%rax // cmp %rsi, %rax
        0x00000000004011cb <+215>:   je     0x4011d2 <phase_6+222> //== 循环结束 
        0x00000000004011cd <+217>:   mov    %rdx,%rcx //head.value = head.next.value
        0x00000000004011d0 <+220>:   jmp    0x4011bd <phase_6+201>
        0x00000000004011d2 <+222>:   movq   $0x0,0x8(%rdx) // M[%rdx + 0x8] = 0
        //
          将6个节点按照在栈中顺序重组，node6变为head
          (gdb) x/24w 0x6032d0
          0x6032d0 <node1>:       0x0000014c      0x00000001      0x00000000      0x00000000
          0x6032e0 <node2>:       0x000000a8      0x00000002      0x006032d0      0x00000000
          0x6032f0 <node3>:       0x0000039c      0x00000003      0x006032e0      0x00000000
          0x603300 <node4>:       0x000002b3      0x00000004      0x006032f0      0x00000000
          0x603310 <node5>:       0x000001dd      0x00000005      0x00603300      0x00000000
          0x603320 <node6>:       0x000001bb      0x00000006      0x00603310      0x00000000
          
          (gdb) p/a $rbx
          $22 = 0x603320 <node6>
          $rbx放的就是head
          输入1 2 3 4 5 6
          7 - input之后: 6 5 4 3 2 1，也就是当前链表节点的顺序
        //
        0x00000000004011da <+230>:   mov    $0x5,%ebp //%ebp = 0x5
        0x00000000004011df <+235>:   mov    0x8(%rbx),%rax //%rax 保存%rbx下一个节点的指针
        0x00000000004011e3 <+239>:   mov    (%rax),%eax //%eax = 结构体中sth
        0x00000000004011e5 <+241>:   cmp    %eax,(%rbx) //比较两个node的sth
        0x00000000004011e7 <+243>:   jge    0x4011ee <phase_6+250> // >= 前一个节点的sth>后一个节点的sth jump
        0x00000000004011e9 <+245>:   callq  0x40143a <explode_bomb> // 否则爆炸
        0x00000000004011ee <+250>:   mov    0x8(%rbx),%rbx //移动指针
        0x00000000004011f2 <+254>:   sub    $0x1,%ebp //遍历链表
        0x00000000004011f5 <+257>:   jne    0x4011df <phase_6+235>
        0x00000000004011f7 <+259>:   add    $0x50,%rsp
        0x00000000004011fb <+263>:   pop    %rbx
        0x00000000004011fc <+264>:   pop    %rbp
        0x00000000004011fd <+265>:   pop    %r12
        0x00000000004011ff <+267>:   pop    %r13
        0x0000000000401201 <+269>:   pop    %r14
        0x0000000000401203 <+271>:   retq   
        (gdb) print 0x14c
        $33 = 332
        (gdb) print 0x0a8
        $34 = 168
        (gdb) print 0x39c
        $35 = 924
        (gdb) print 0x2b3
        $36 = 691
        (gdb) print 0x1dd
        $37 = 477
        (gdb) print 0x1bb
        $38 = 443
        //链表需要按sth的值降序排列
        //链表节点在栈中顺序应为 3 4 5 6 1 2
        //所以输入顺序为 4 3 2 1 6 5


    > 总结一下：1.输入6个数字1<=input<=6,且互不相同，确定输入值为1～6.
              2.input = 7 - input
              3.内存中有一个链表，共6个节点，编号为1～6，将该链表按照input的顺序依次放入栈中%rep+20~%rsp+50
              4.将链表按照在栈中的顺序重新链接。
              5.遍历链表，链表需要满足其中存储值按降序排列的条件。
    
