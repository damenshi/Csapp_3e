# Cache lab

+ [实验视频说明](https://scs.hosted.panopto.com/Panopto/Pages/Viewer.aspx?id=17464223-21aa-485b-9de7-749dd2562616)

+ [实验简介](http://csapp.cs.cmu.edu/3e/README-cachelab)

实验文件说明

  ```
  Makefile # 指导make指令进行构建的文件
  README # 实验说明
  cachelab.c # PartB 的代码框架
  cachelab.h # PartB 的代码框架
  csim-ref # 二进制文件，PartA的教师实现版，用于对比订正自己的问题
  csim.c # PartA你需要填充的代码文件
  driver.py # 代码判分文件 python2
  test-csim # 
  test-trans.c
  tracegen.c
  traces # 一些样例文件
  trans.c # PartB你需要填充的代码文件
  ```

  

+ Part A

  > 写一个程序来模拟高速缓存的行为。根据样例文件中的数据加载信息，计算cpu完成这些命令后，一个缓存的miss、hit 以及 eviction次数。
	 ```
	数据加载信息样例：
	I 0400d7d4,8
   M 0421c7f0,4
   L 04f6b868,8
   S 7ff0005c8,8
  
  每行格式是[空格]操作指令 地址,size。对于操作指令：
  
  I，表示指令load，我们的cache可以忽略指令加载相关的cache开销。前没有空格
  L，表示数据load，如果不在Cache中就miss，加载时可能存在eviction；前有空格
  S，表示数据Store写入，如果不在Cache中就miss，加载时可能存在eviction；前有空格
  M，表示数据修改，等同于执行了Load+Store，前有空格
   ```
  > 压缩文件中包含了一个csim-ref的可执行文件，我们写的程序csim功能上需要与这个csim-ref基本类似，至少对于相同内存读写时命中、不命中和驱逐次数三个数值相同.
	```
	根据压缩包里提供的帮助文档，可以知道在控制台输入的参数格式如下:
	Usage: ./csim-ref [-hv] -s <s> -E <E> -b <b> -t <tracefile>
	• -h: Optional help flag that prints usage info
	• -v: Optional verbose flag that displays trace info
	• -s <s>: Number of set index bits (S = 2^s is the number of sets)
	• -E <E>: Associativity (number of lines per set)
	• -b <b>: Number of block bits (B = 2^b is the block size)
	• -t <tracefile>: Name of the valgrind trace to replay
	```
	> 其中加上参数v后，会把每一条指令是什么类型（L, M, S)，读写内存大小和是否命中是否驱逐的信息逐行逐指令一一显示。而如果不加参数v只会显示做完所有指令后统计的命中、不命中和驱逐次数的和。
	
  
  > 代码中要用到的一些技术
	>
	> [getopt](https://www.cnblogs.com/qingergege/p/5914218.html):man 3 getopt
	>
	> 完整代码：从main函数入手，逐步理清思路。

  ```C
  #include "cachelab.h"
  #include <getopt.h>
  #include <stdio.h>
  #include <stdlib.h>
	
	/*
	1. 定义cache结构体, 根据书中定义，一个cache包含S个Set，每个Set有E个Line。每个line由valid，tag，block位组成（由于程序只模仿miss，hit等行为，所以省略block）。LRUcount用以实现LRU替换，该数值越大，替换优先级越高。
	*/
  //line
  typedef struct{
      int valid;
      int tag;
      int LRUcount;//越大，替换优先级越高
  }Line;
  
  //set
  typedef struct{
      Line* lines;
  }Set;
  
  typedef struct{
      Set* sets;
      int setnum;//S
      int linenum;//E
  }Cache;
  
  int hitcount = 0;
  int misscount = 0;
  int evictioncount = 0;
  
  void printHelp(){//和帮助文档中csim-ref程序相同
      printf("Usage: ./csim [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n");
      printf("Options:\n");
      printf("  -h         Print this help message.\n");
      printf("  -v         Optional verbose flag.\n");
      printf("  -s <num>   Number of set index bits.\n");
      printf("  -E <num>   Number of lines per set.\n");
      printf("  -b <num>   Number of block offset bits.\n");
      printf("  -t <file>  Trace file.\n\n");
      printf("Examples:\n");
      printf("  linux>  ./csim -s 4 -E 1 -b 4 -t traces/yi.trace\n");
      printf("  linux>  ./csim -v -s 8 -E 2 -b 4 -t traces/yi.trace\n\n");
  }
  
  //初始化cache，分配空间，初始化valid，LRUcount.
  void init_cache(int s, int E, int b, Cache* sim_cache){
      if(s <= 0 || E <= 0)
          exit(0);
  
      sim_cache->setnum = 1 << s;//组数
      sim_cache->linenum = E;//每组行数
      sim_cache->sets = (Set*)malloc(sizeof(Set) * sim_cache->setnum);
      if(!sim_cache->sets){
          exit(0);
      }
      for(int i = 0; i < sim_cache->setnum; i++){
          sim_cache->sets[i].lines = (Line*)malloc(sizeof(Line) * E);
          for(int j = 0; j < E; j++){
              sim_cache->sets[i].lines[j].valid = 0;
              sim_cache->sets[i].lines[j].LRUcount = 0;
          }
      }
  }
  
  //将命中行的LRUcount置为0，其余行+1.最近最久未访问的行LRUcount最大
  void updateLRU(Cache *sim_cache, int setid, int lineid){
      int ln = sim_cache->linenum;
      for(int i = 0; i < ln; i++){
          sim_cache->sets[setid].lines[i].LRUcount++;
      }
      sim_cache->sets[setid].lines[lineid].LRUcount = 0;
  }
  
  //判断是否命中,命中return 1， 未命中return 0
  int judgeHit(Cache *sim_cache, int setid, int cur_tag){  
      int ln = sim_cache->linenum;
      for(int i = 0; i < ln; i++){
          if(sim_cache->sets[setid].lines[i].valid == 1 && sim_cache->sets[setid].lines[i].tag == cur_tag){
              //命中，更新LRUcount
              updateLRU(sim_cache, setid, i);
              return 1;
          }
      }
      return 0;
  }
  
  void visitCache(Cache *sim_cache, int setid, int cur_tag, int verbose){
			
      if(judgeHit(sim_cache, setid, cur_tag)){
        //命中
          hitcount++;
          if(verbose == 1)
              printf("hit ");
      }else{
          //未命中,分两种情况，1.目标组行没满，直接写入cache，2.目标组行已满，需要替换
          misscount++;
          if(verbose == 1)
              printf("miss ");
  
          //行没满,直接写入
          int ln = sim_cache->linenum;
          for(int i = 0; i < ln; i++){
            	//遍历该组所有行，如果有valid = 0说明该行为空。
              if(sim_cache->sets[setid].lines[i].valid == 0){
                  sim_cache->sets[setid].lines[i].valid = 1;
                  sim_cache->sets[setid].lines[i].tag = cur_tag;
                  updateLRU(sim_cache, setid, i);
                  return; 
              }
          }
  
          //行已满，寻找替换行
          int max_LRu = -1;
          int replace_line = 0;
        	//遍历该组所有行，找到LRUcount最大的行，即为LRU策略要替换的行。
          for(int i = 0; i < ln; i++){
              if(sim_cache->sets[setid].lines[i].LRUcount > max_LRu){
                  max_LRu = sim_cache->sets[setid].lines[i].LRUcount;
                  replace_line = i; 
              }
          }
          sim_cache->sets[setid].lines[replace_line].valid = 1;
          sim_cache->sets[setid].lines[replace_line].tag = cur_tag;
          updateLRU(sim_cache, setid, replace_line); 
          evictioncount++;
          if(verbose == 1)
              printf("eviction ");
          }   
  }
  int main(int argc, char** argv)
  {
      int s = -1, E = -1, b = -1, verbose = 0;//当输入命中有-v时，vervose = 1,显示具体信息
      FILE* tracefile = NULL;
      Cache sim_cache;
      char ch;
  		
	  	//将终端输入转换为对应参数
      while((ch = getopt(argc, argv, "hvs:E:b:t:")) != -1){// man 3 getopt
          switch(ch){
              case 'h' : 
                  printHelp();//打印和帮助文档中csim-ref程序相同的提示。
                  break;
              case 'v':
                  verbose = 1;
                  break;
              case 's':
                  s = atoi(optarg);
                  break;
              case 'E':
                  E = atoi(optarg);
                  break;
              case 'b':
                  b = atoi(optarg);
                  break;
              case 't':
                  tracefile = fopen(optarg, "r");
                  break;
              default:
                  printHelp();
                  break; 
          }
      }
    
      if (s == -1 || E == -1 || b == -1 || tracefile == NULL)
    {
      printf("%s: Missing required command line argument\n", argv[0]);
      printHelp();
      //exit(1);
    }
    
      init_cache(s, E, b, &sim_cache);//根据输入参数初始化cache
  /*
  Cache初始化之后，读取trace文件中的操作指令，并根据其中地址部分判断cache是命中还是未命中，以及是否覆盖。
  例如：L 10，1 addr = 10， 后b位为块偏移，中间s位为组索引，剩余部分为tag。
  */
      {
          //trcae中数据信息格式 option addr size
          char option;
          unsigned long long addr;
          int size;
          while(fscanf(tracefile, "%c %llx,%d", &option, &addr, &size) > 0){//按行读入trace文件
              if(option == 'I')//忽略指令load
                  continue;
              int cur_tag = addr >> (s + b);//根据地址解析tag
              int setid = (addr >> b) & ((1 << s) - 1);//根据地址解析行号
              if(option == 'L' || option == 'S'){
                  if(verbose == 1){//有-v命令
                      printf("%c %llx,%d ", option, addr, size);
                  }
                  visitCache(&sim_cache, setid, cur_tag, verbose);//访问cache进行判断
              }
              if(option == 'M'){//相当于同时执行load+store，所以两次访问Cache。
                  if(verbose == 1){
                      printf("%c %llx,%d ", option, addr, size);
                  }
                  visitCache(&sim_cache, setid, cur_tag, verbose);
                  visitCache(&sim_cache, setid, cur_tag, verbose);
              }
              if(verbose == 1)
                  printf("\n");
          }
      }
  		//输出结果
	    printSummary(hitcount, misscount, evictioncount);
      return 0;
  }
  ```
  
+ 运行结果：

![image-20220619200927549](https://raw.githubusercontent.com/damenshi/myImage/main/img/image-20220619200927549.png)



+ Part B

> 利用缓存的特性优化矩阵转置程序，使miss次数尽可能小。主要使用blocking策略。
>
> 关于分块技术[原文参考](http://csapp.cs.cmu.edu/public/waside/waside-blocking.pdf)，[知乎参考](https://zhuanlan.zhihu.com/p/129392257).
>
> 输入是三个不同的固定大小的矩阵，矩阵分别为：
>
> - 32 x 32，miss 次数小于 300 满分。
> - 64 x 64，miss 次数小于 1300 满分。
> - 61 x 67， miss 次数小于 2000 满分。
>
> 可以根据矩阵的大小做特定的优化。实验还有如下限制：
>
> - 最多 12 个 `int` 类型的本地变量，不能用其他整数类型来储存 `int` 类型的值。
> - 不能修改 `A` 矩阵的内容，`B` 矩阵可随意。
> - 不能用 `malloc` 类函数。
>
> 用来测试 miss 的是 Part A 的缓存模拟器，缓存的参数为 `s=5, E=1, b=5`， 即共有 32 个 set， 每个 set 有一个 cache line( 直接映射 ), 每个块的大小为 32 bytes。

1. 为什么要分块

   [参考](https://yangtau.me/computer-system/csapp-cache.html)

```
简单转置实现
void trans(int M, int N, int A[N][M], int B[M][N]) {
    int i, j, tmp;
    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }
}
```

> 我们知道在C语言中多维矩阵在内存中是按行排列的，在该程序中，A矩阵按行读取，读取A[0]/[0],除了将A[0]/[0]加载到缓存中，A[0]/[1],A[0]/[2]...也可能被加入到缓存中（缓存个数取决于块大小）。
>
> 但是B矩阵是按列写入，在列上相邻的元素很可能不在一个内存块上，这样第一列写入时都不命中，在第一列写完，由于缓存大小有限，在返回写第一行第二列时，原来缓存的内容可能已经被替换，又需要重新从内存读取。最坏情况下，每次写都不能命中。

```
矩阵 B 按列写的过程， * 表示内容加载到了缓存中。
假设每个缓存块的大小为 4 个 int 的大小，B 矩阵每相隔四行的行地址在缓存中的组索引会重复一次。
也就是说第一行的前四个元素和第五行的前四个元素会占据相同的缓存块。
   |                          |                           |
   V                          V                           V
  +-+-+-+-+-+-+-+-+          +-+-+-+-+-+-+-+-+         +-+-+-+-+-+-+-+-+
  |*|*|*|*| | | | |          | | | | | | | | |         |*|*|*|*| | | | |
  +-+-+-+-+-+-+-+-+          +-+-+-+-+-+-+-+-+         +-+-+-+-+-+-+-+-+
  |*|*|*|*| | | | |          | | | | | | | | |         |*|*|*|*| | | | |
  +-+-+-+-+-+-+-+-+          +-+-+-+-+-+-+-+-+         +-+-+-+-+-+-+-+-+
  |*|*|*|*| | | | |          | | | | | | | | |         |*|*|*|*| | | | |
  +-+-+-+-+-+-+-+-+          +-+-+-+-+-+-+-+-+         +-+-+-+-+-+-+-+-+
  |*|*|*|*| | | | |   =>     | | | | | | | | |   =>    |*|*|*|*| | | | |
  +-+-+-+-+-+-+-+-+          +-+-+-+-+-+-+-+-+         +-+-+-+-+-+-+-+-+
  | | | | | | | | |          |*|*|*|*| | | | |         | | | | | | | | |
  +-+-+-+-+-+-+-+-+          +-+-+-+-+-+-+-+-+         +-+-+-+-+-+-+-+-+
  | | | | | | | | |          |*|*|*|*| | | | |         | | | | | | | | |
  +-+-+-+-+-+-+-+-+          +-+-+-+-+-+-+-+-+         +-+-+-+-+-+-+-+-+
  | | | | | | | | |          |*|*|*|*| | | | |         | | | | | | | | |
  +-+-+-+-+-+-+-+-+          +-+-+-+-+-+-+-+-+         +-+-+-+-+-+-+-+-+
  | | | | | | | | |          |*|*|*|*| | | | |         | | | | | | | | |
  +-+-+-+-+-+-+-+-+          +-+-+-+-+-+-+-+-+         +-+-+-+-+-+-+-+-+
        (1)                        (2)                         (3)

(1) 写入第一列的前四行
(2) 写入第一列后四行时，前四行缓存的内容被替换
(3) 写入第二列的时候，又需要重新从内存中读前四行的内容
```

> 分块策略充分利用B矩阵中缓存部分。例如将上面矩阵进行4 * 4分块。

```
   |                          |
   V                          V
  +-+-+-+-+-+-+-+-+        +-+-+-+-+-+-+-+-+
  |*|*|*|*| | | | |        |*|*|*|*| | | | |
  +-+-+-+-+-+-+-+-+        +-+-+-+-+-+-+-+-+
  |*|*|*|*| | | | |        |*|*|*|*| | | | |
  +-+-+-+-+-+-+-+-+        +-+-+-+-+-+-+-+-+
  |*|*|*|*| | | | |        |*|*|*|*| | | | |
  +-+-+-+-+-+-+-+-+        +-+-+-+-+-+-+-+-+
  |*|*|*|*| | | | |   =>   |*|*|*|*| | | | |
  +-+-+-+-+-+-+-+-+        +-+-+-+-+-+-+-+-+
  | | | | | | | | |        | | | | | | | | |
  +-+-+-+-+-+-+-+-+        +-+-+-+-+-+-+-+-+
  | | | | | | | | |        | | | | | | | | |
  +-+-+-+-+-+-+-+-+        +-+-+-+-+-+-+-+-+
  | | | | | | | | |        | | | | | | | | |
  +-+-+-+-+-+-+-+-+        +-+-+-+-+-+-+-+-+
  | | | | | | | | |        | | | | | | | | |
  +-+-+-+-+-+-+-+-+        +-+-+-+-+-+-+-+-+
        (1)                      (2)

(1) 先写入第一列的前四行，前四列的前四行都被加载到缓存中
(2) 再写入第二列的前四行，不需要再从内存中重新加载
```

2. 32 * 32
>因为测试使用参数 `s=5, E=1, b=5`， 即共有 32 个 set， 每个 set 有一个 cache line( 直接映射 ), 每个块的大小为 32 bytes。
>
>即每个块可以缓存32byte，一个int类型为4byte，一个块可以缓存8个int型。共32个set，可以缓存256个int，也就是说每隔8行，就会映射到cache中相同的set。如图中所示，A[0]/[0]~A[0]/[8]映射到第0组，A[8]/[0]~A[8]/[8]也映射到第0组。如果不分块按列写入，已写入的缓存就会被后面的行覆盖。

```
 矩阵中缓存块 `index` 的分布：
 		0  8  16 24	32
    +--+--+--+--+
 0  | 0| 1| 2| 3|
    +--+--+--+--+
 1  | 4| 5| 6| 7|
    +--+--+--+--+
 2  | 8| 9|10|11|
    +--+--+--+--+
 3  |12|13|14|15|
    +--+--+--+--+
 4  |16|17|18|19|
    +--+--+--+--+
 5  |20|21|22|23|
    +--+--+--+--+
 6  |24|25|26|27|
    +--+--+--+--+
 7  |28|29|30|31|
    +--+--+--+--+
 8  | 0| 1| 2| 3|
    +--+--+--+--+
    ...

每一个小格子表示一个缓存块，格子中的数字是缓存块的 index。
可以看到第 0 行和第 8 行缓存块的 index 是重复的。
```

> 因此32*32矩阵可按8 * 8进行分块

```C
直接按照8*8分块实现矩阵转置
void transpose_8_8(int M, int N, int A[N][M], int B[M][N])
{   
    for(int i = 0; i < N; i += 8){
        for(int j = 0; j < M; j += 8){
            for(int k = i; k < min(i+8, N); k++){
                for(int s = j; s < min(j+8, M); s++){
                    B[s][k] = A[k][s];
                }
            }
        }
    }
}
```

运行结果：

![image-20220620160516377](https://raw.githubusercontent.com/damenshi/myImage/main/img/image-20220620160516377.png)

理论结果：

> 每块miss次数为8，32*32矩阵共分为16个8\*8的块，miss数为16\*8=128.共两个矩阵miss总数 = 256.
>
> 实际结果大于理论值，没有达到满分要求。因为A、B矩阵之间发生了冲突。[参考](https://zhuanlan.zhihu.com/p/138881600)
>
> 因为矩阵A、B是连续分布的
>
> ```
> // tracegen.c
> static int A[256][256];
> static int B[256][256];
> ```
>
> A、B矩阵大小都是256*256.
>
> ![image-20220620164053219](https://raw.githubusercontent.com/damenshi/myImage/main/img/image-20220620164053219.png)
>
> 灰色为组索引部分，所以可知A[0]与B[0]映射到同一个set。进一步A[m]与B[m]均映射到同一组。
>
> 例如：在读A[0]\[0]后，A[0]\[0...7]映射到cache第0组，之后写入B[0]\[0],B[0]\[0...7]会将cache第0组覆盖，再读A[0]\[1]时，又会miss。等之后写B[0]\[1]时，也会miss。
>
> 形如B[m]\[m] = A[m]\[m]的语句都会发生这样的冲突，导致不命中次数比理论数多。

```
具体过程
缓存中的内容 :
+-----------------------+-------------------+
| opt                   |  cache            |
+-----------------------+-------------------+
|before B[0][0]=tmp     | A[0]              |---+
+-----------------------+-------------------+   |
|after B[0][0]=tmp      | B[0]              |   |
+-----------------------+-------------------+   |    A 的第一行复制到 B 的第一列 .
|after tmp=A[0][1]      | A[0]              |   |    最终缓存中剩下 A[0], B[1]...B[7].
+-----------------------+-------------------+   +--> A[0]被两次加载进入内存 ,
|after B[1][0]=tmp      | A[0] B[1]         |   |    总的 miss 次数是 10.
+-----------------------+-------------------+   |
|...                    |                   |   |
+-----------------------+-------------------+   |
|after B[7][0]=tmp      | A[0] B[1..7]      |---+
+-----------------------+-------------------+
|after B[0][1]=tmp      | A[1] B[0] B[2..7] |---+
+-----------------------+-------------------+   |    A 的第二行复制到 B 的的二列 .
|after B[1][1]=tmp      | B[0..7]           |   |    其中发生的 miss 有 :
+-----------------------+-------------------+   +--> A[0], B[0], A[1]与 B[1]的相互取代 .
|...                    |                   |   |    总的 miss 次数为 4.
+-----------------------+-------------------+   |
|after B[7][1]=tmp      | A[1] B[0] B[2..]  |---+
+-----------------------+-------------------+        之后的三至七行与第二行类似 ,
|...                    |                   |------> miss 的次数都是 4.
+-----------------------+-------------------+
|after tmp=A[7][7]      | A[7] B[0..6]      |---+    最后一行 A[7]被 B[7]取代后 ,
+-----------------------+-------------------+   +--> 不需要重新加载 .
|after B[7][7]=tmp      | B[0..7]           |---+    总的 miss 数为 3.
+-----------------------+-------------------+

所以对角块上的总的 miss 次数是 10+4*6+3=37.
额外Miss= 37 - 16 = 21
```



> 使用局部变量来缓解矩阵间的冲突。


> 使用本地变量先存下A的一行后，再复制给B。

```
void transpose_32_32(int A[32][32], int B[32][32])
{   
    int a0, a1, a2, a3, a4, a5, a6, a7;
    for(int i = 0; i < 32; i += 8){
        for(int j = 0; j < 32; j += 8){
            for(int k = i; k < i + 8; k++){
                a0 = A[k][j];
                a1 = A[k][j + 1];
                a2 = A[k][j + 2]; 
                a3 = A[k][j + 3];
                a4 = A[k][j + 4];
                a5 = A[k][j + 5];
                a6 = A[k][j + 6];
                a7 = A[k][j + 7];
                B[j][k] = a0;
                B[j + 1][k] = a1;
                B[j + 2][k] = a2;
                B[j + 3][k] = a3;
                B[j + 4][k] = a4;
                B[j + 5][k] = a5;
                B[j + 6][k] = a6;
                B[j + 7][k] = a7;
            }
        }
    }
}
```

```
具体过程
缓存中的内容 :
+-----------------------+-------------------+
| opt                   |  cache            |
+-----------------------+-------------------+
|after tmp=A[0][0]      | A[0]              |
+-----------------------+-------------------+
|after tmp=A[0][1]      | A[0]              |
+-----------------------+-------------------+
|after tmp=A[0][2]      | A[0]              |
+-----------------------+-------------------+
|...                    |                   |
+-----------------------+-------------------+
|after tmp=A[0][7]      | A[0]              |
+-----------------------+-------------------|
|after B[0][0]=tmp      | B[0]              | ---> 缓存替换
+-----------------------+-------------------+
|after B[1][0]=tmp      | B[0..1]           |
+-----------------------+-------------------+
|after B[2][0]=tmp      | B[0..2]           |
+-----------------------+-------------------+
|...                    |                   |
+-----------------------+-------------------+
|after B[7][0]=tmp      | B[0..7]           |
+-----------------------+-------------------+
|after tmp=A[1][0]      | A[1] B[0] B[2..7] | ---> 缓存替换
+-----------------------+-------------------+
|...                    |                   |
+-----------------------+-------------------+
|after tmp=A[1][7]      | A[1] B[0] B[2..7] |
+-----------------------+-------------------+
|after B[0][1]=tmp      | A[1] B[0] B[2..7] |
+-----------------------+-------------------+
|after B[1][1]=tmp      | B[0..7]           | ---> 缓存替换（额外的）
+-----------------------+-------------------+
|after B[2][1]=tmp      | B[0..7]           |
+-----------------------+-------------------+
|...                    |                   |
+-----------------------+-------------------+

额外的 miss 次数为 7 次
对角块上的总的 miss 次数是 16 + 7 = 23次.
减少了矩阵A、B间的冲突次数，但还没有完全消除。
```

![image-20220620173034532](https://raw.githubu sercontent.com/damenshi/myImage/main/img/image-20220620173034532.png)

> 使用先复制后转置完全消除矩阵间对角线冲突
>
> 前两种方式无法消除对角线上的冲突的原因是A、B矩阵的交替访问时，B矩阵某一时刻全部会全部加载到缓存中，而下一时刻对A矩阵的访问会导致B矩阵部分缓存的换出，而被换出的B矩阵部分需要再重新加载回来。这一点导致了前两种方案无法消除冲突。
>
> 而先复制后转置方案，则将A中`8*8`的所有块复制到对应B中后，再将B中的`8*8`的块进行转置。

```
void transpose_copy(int M, int N, int A[N][M], int B[M][N])
{   
    int a0, a1, a2, a3, a4, a5, a6, a7;
    int i, j, k, s;
    for(i = 0; i < N; i += 8){
        for(j = 0; j < M; j += 8){
            //先复制
            for(k = i, s = j; k < i + 8; k++, s++){
                a0 = A[k][j];
                a1 = A[k][j + 1];
                a2 = A[k][j + 2]; 
                a3 = A[k][j + 3];
                a4 = A[k][j + 4];
                a5 = A[k][j + 5];
                a6 = A[k][j + 6];
                a7 = A[k][j + 7];
                B[s][i] = a0;
                B[s][i + 1] = a1;
                B[s][i + 2] = a2;
                B[s][i + 3] = a3;
                B[s][i + 4] = a4;
                B[s][i + 5] = a5;
                B[s][i + 6] = a6;
                B[s][i + 7] = a7;
            }
            再转置
            for(k = 0; k < 8; k++){
                for(s = k + 1; s < 8; s++){
                    a0 = B[k + j][s + i];
                    B[k + j][s + i] = B[s + i][k + j];
                    B[s + i][k + j] = a0;
                }
            }
        }
    }
}
```
> 代码中矩阵转置方法如下图所示，假设一个4\*4的矩阵，按2\*2分块。

![image-20220621090004943](https://raw.githubusercontent.com/damenshi/myImage/main/img/image-20220621090004943.png)

运行结果miss次数为259，比理论值256多3，是由函数调用导致的。

![image-20220621090536563](https://raw.githubusercontent.com/damenshi/myImage/main/img/image-20220621090536563.png)



3. 64 * 64

>64 * 64的矩阵每行需要8个缓存块，每四行缓存index会重复一次

```
 矩阵中缓存 `index` 分布
    0  8 16 24 32 40 48 56 64
    +--+--+--+--+--+--+--+--+
 0  | 0| 1| 2| 3| 4| 5| 6| 7|
    +--+--+--+--+--+--+--+--+
 1  | 8| 9|10|11|12|13|14|15|
    +--+--+--+--+--+--+--+--+
 2  |16|17|18|19|20|21|22|23|
    +--+--+--+--+--+--+--+--+
 3  |24|25|26|27|28|29|30|31|
    +--+--+--+--+--+--+--+--+
 4  | 0| 1| 2| 3| 4| 5| 6| 7|
    +--+--+--+--+--+--+--+--+
 5  | 8| 9|10|11|12|13|14|15|
    +--+--+--+--+--+--+--+--+
 6  |16|17|18|19|20|21|22|23|
    +--+--+--+--+--+--+--+--+
    ...
```

> 按照8 * 8分块，同一块内会发生缓存冲突，如果按照4 * 4分块，则没有充分利用加载进入缓存中的部分(每个line缓存8个int)。
>
> 依然利用上面的方法，使用局部变量，先复制再转置。
>
> 思路：保持分块为 8 * 8， 在大的分块下再分成4个 4 * 4的小分块。[参考](https://yangtau.me/computer-system/csapp-cache.html):有点复杂，不太容易理解
>
> 下图左边为矩阵A，右边为矩阵B。
>
> 将A的前4行按照图1的方式复制到B，此时B中左上4*4在最终正确位置，右上的4\*4最终应该放到B的左下。
>
> 利用局部变量，将B的右上内容复制到左下。

<img src="https://raw.githubusercontent.com/damenshi/myImage/main/img/image-20220621094336958.png" alt="image-20220621094336958" style="zoom:50%;" />

<img src="https://raw.githubusercontent.com/damenshi/myImage/main/img/image-20220621094402235.png" alt="image-20220621094402235" style="zoom:50%;" />

> 1. 先将 `A` 的前四行按照 (1) 复制到 `B` 中。
>
> 2. 按照 (2) 将 `A` 中对应位置的元素存到本地变量中。
>
> 3. `buf1` 的四个元素与 `B` 右上角的第一行交换，将 `buf2` 中的值存到 `B` 右下角的对应位置。此时缓存中 `B[4]` 替换 `B[0]`。
>
> 4. 将 `buf1` 中的元素存放到 `B` 左下角对应位置。
>
> 5. 改变位置，重复 (2)，(3)，(4)，直到所有元素到达正确位置。
>
>    注：步骤2～4蓝色和紫色部分的替换，同时也在转置原先白色部分的数据，当B中右上复制到左下以后，整个8*8块的转置工作也同时完成。

```
void transpose_64_64(int N, int M, int A[N][M], int B[M][N])
{   
    int i = 0, j = 0, k = 0;
    int temp;
    int a0, a1, a2, a3, a4, a5, a6, a7;
    const int block_size = 8;

    for(i = 0; i < N; i += block_size){
        for(j = 0; j < M; j += block_size){
            for(k = 0; k < block_size / 2; k++){
                //A top left
                a0 = A[k + i][j];
                a1 = A[k + i][j + 1];
                a2 = A[k + i][j + 2];
                a3 = A[k + i][j + 3]; 

                //A top right
                a4 = A[k + i][j + 4];
                a5 = A[k + i][j + 5];
                a6 = A[k + i][j + 6];
                a7 = A[k + i][j + 7];

                //B top left
                B[j][k + i] = a0;
                B[j + 1][k + i] = a1;
                B[j + 2][k + i] = a2;
                B[j + 3][k + i] = a3;

                //B top right
                B[j][k + 4 + i] = a4;
                B[j + 1][k + 4 + i] = a5;
                B[j + 2][k + 4 + i] = a6;
                B[j + 3][k + 4 + i] = a7; 
            }
            for(k = 0; k < block_size / 2; k++){
                //step2 蓝色部分
                a0 = A[i + 4][j + k];
                a1 = A[i + 5][j + k];
                a2 = A[i + 6][j + k];
                a3 = A[i + 7][j + k]; 
                //step2 紫色部分
                a4 = A[i + 4][j + k + 4];
                a5 = A[i + 5][j + k + 4];
                a6 = A[i + 6][j + k + 4];
                a7 = A[i + 7][j + k + 4];

                //step3 交换蓝色部分和B右上
                temp = B[j + k][i + 4], B[j + k][i + 4] = a0, a0 = temp;
                temp = B[j + k][i + 5], B[j + k][i + 5] = a1, a1 = temp;
                temp = B[j + k][i + 6], B[j + k][i + 6] = a2, a2 = temp;
                temp = B[j + k][i + 7], B[j + k][i + 7] = a3, a3 = temp;

                //step4 将蓝色部分和紫色部分放入对应部分（此时蓝色部分中是交换过的B中右上的内容）
                B[j + k + 4][i + 0] = a0;
                B[j + k + 4][i + 1] = a1;
                B[j + k + 4][i + 2] = a2;
                B[j + k + 4][i + 3] = a3;
                B[j + k + 4][i + 4] = a4;
                B[j + k + 4][i + 5] = a5;
                B[j + k + 4][i + 6] = a6;
                B[j + k + 4][i + 7] = a7;
            }

        }
    }
}
```

运行结果：miss次数为1107.

![image-20220621105005642](https://raw.githubusercontent.com/damenshi/myImage/main/img/image-20220621105005642.png)

3. 61 * 67
> 直接按8 * 8分块，结果miss次数为2118，距离2000有差距。

>参考大佬按照8 * 23进行分块。

```
void transpose_61_67(int M, int N, int A[N][M], int B[M][N])
{   
    int i = 0 , j = 0, k = 0, s = 0;
    int a0, a1, a2, a3, a4, a5, a6, a7;
    for(i = 0; i < N; i += 8){
        for(j = 0; j < M; j += 23){
            if(i + 8 <= N && j + 23 <= M){
                for(s = j; s < j + 23; s++){
                    a0 = A[i][s];
                    a1 = A[i + 1][s];
                    a2 = A[i + 2][s]; 
                    a3 = A[i + 3][s];
                    a4 = A[i + 4][s];
                    a5 = A[i + 5][s];
                    a6 = A[i + 6][s];
                    a7 = A[i + 7][s];

                    B[s][i] = a0;
                    B[s][i + 1] = a1;
                    B[s][i + 2] = a2;
                    B[s][i + 3] = a3;
                    B[s][i + 4] = a4;
                    B[s][i + 5] = a5;
                    B[s][i + 6] = a6;
                    B[s][i + 7] = a7;     
                }
            }else{
                for(k = i; k < min(i + 8, N); k++){
                    for(s = j; s < min(j + 23, M); s++){
                        B[s][k] = A[k][s];
                    }
                }
            }
        }
    }
}
```

运行结果：miss次数为1863

![image-20220621113150960](https://raw.githubusercontent.com/damenshi/myImage/main/img/image-20220621113150960.png)



+ 总结

  Cache实验总体难度还是比较大，参考了不少资料，尤其矩阵分块部分，不太好理解。
