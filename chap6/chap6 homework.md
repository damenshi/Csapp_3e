# chap6 homework

### 6.22
设每条磁道bit数 = x * r * K
设磁道总数 = (r - x * r) * M = r * (1 - x) * M
M、K 为常数。
磁盘容量 = M * K * r^2 * x * (1 - x)
当x = 1/2时，容量最大。

### 6.23
T = 4ms + 1/2 * 60s / 15000 * 1000ms/s + 60s / 15000 * 1000ms/s * 1 / 800扇区/磁道 = 4ms + 2ms + 0.005ms = 6.005ms

### 6.24
寻道时间 = 4ms，旋转时间 = 1/2 * 60s / 15000 * 1000ms/s = 2ms
A.
传送时间 = 60s / 15000 * 1000ms/s * 1 / 1000扇区/磁道 = 0.004ms
2MB = 2 * 2^20byte, 共 2^21 / 2^9 = 2^12个扇区
顺序读取最优时间T = 4ms + 2ms + 0.004ms * 2^12 = 22.384ms.
B.
随机读取时间T = （4ms + 2ms + 0.002ms） * 2^12 / 1000ms/s = 24.584s

### 6.25
|高速缓存|m|C|B|E|S|t|s|b|
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|1.|32|1024|4|4|64|24|6|2|
|2.|32|1024|4|256|1|30|0|2|
|3.|32|1024|8|1|128|22|7|3|
|4.|32|1024|8|128|1|29|0|3|
|5.|32|1024|32|1|32|22|5|5|
|6.|32|1024|32|4|8|24|3|5|

### 6.26
|高速缓存|m|C|B|E|S|t|s|b|
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|1.|32|2048|8|1|256|21|8|3|
|2.|32|2048|4|4|128|23|7|2|
|3.|32|1024|2|8|64|25|6|1|
|4.|32|1024|32|2|16|23|4|5|

### 6.27

![](@attachment/Clipboard_2022-06-17-09-38-26.png)
A.
所有会在组1中命中的16进制内存地址
0100010100100 ~ 0100010100111, 0x08A4, 0x08A5, 0x08A6, 0x08A7
0011100000100 ~ 0011100000100, 0x0704, 0x0705, 0x0706, 0x0707

B.
所有会在组6中命中的16进制内存地址
1001000111000 ~ 1001000111011, 0x1238, 0x1239, 0x123A, 0x123B

### 6.28
A.所有会在组2中命中的16进制内存地址
无
B.所有会在组4中命中的16进制内存地址
1100011110000 ~ 1100011110011, 0x18F0, 0x18F1, 0x18F2, 0x18F3
0000010110000 ~ 0000010110011, 0x00B0, 0x00B1, 0x00B2, 0x00B3
C.所有会在组5中命中的16进制内存地址
0111000110100 ~ 0111000110111, 0x0E34, 0x0E35, 0x0E36, 0x0E37
D.所有会在组7中命中的16进制内存地址
1101111011100 ~ 1101111011111, 0x1BDC, 0x1BDD, 0x1BDE, 0x1BDF

### 6.29
A.

|11|10|9|8|7|7|5|4|3|2|1|0|
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|CT|CT|CT|CT|CT|CT|CT|CT|CI|CI|CO|CO|

B.
|操作|地址|命中？|读出的值(未知)|
|:-:|:-:|:-:|:-:|
|读|0x834|未命中||
|写|0x836|未命中||
|读|0xFFD|命中|0xC0|

### 6.30
A.
高速缓存大小C = S * E * B = 8 * 4 * 4 = 128byte
B.
|12|11|10|9|8|7|7|5|4|3|2|1|0|
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|CT|CT|CT|CT|CT|CT|CT|CT|CI|CI|CI|CO|CO|

### 6.31
A.
|12|11|10|9|8|7|7|5|4|3|2|1|0|
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|0|0|1|1|1|0|0|0|1|1|0|1|0|
B.
|参数|值|
|:-:|:-:|
|高速缓存块偏移（CO）|0x2|
|CI|0x6|
|CT|0x38|
|是否命中|yes|
|返回值|0xEB|

### 6.32
A.
|12|11|10|9|8|7|7|5|4|3|2|1|0|
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|1|0|1|1|0|1|1|1|0|1|0|0|0|
B.
|参数|值|
|:-:|:-:|
|高速缓存块偏移（CO）|0x0|
|CI|0x2|
|CT|0xB7|
|是否命中|no|
|返回值||

### 6.33
1011110001000 ~ 1011110001011, 0x1788 ~ 0x178B
1011011001000 ~ 1011011001011, 0x16C8 ~ 0x16CB

### 6.34
src地址从0开始
src第0行地址 000 0 0000 ～ 000 0 1100
src第1行地址 000 1 0000 ～ 000 1 1100
src第2行地址 001 0 0000 ～ 001 0 1100
src第3行地址 001 1 0000 ～ 001 1 1100

dst地址从64开始
dst第0行地址 010 0 0000 ～ 010 0 1100
dst第1行地址 010 1 0000 ～ 010 1 1100
dst第2行地址 011 0 0000 ～ 011 0 1100
dst第3行地址 011 1 0000 ～ 011 1 1100

src数组
|列0|列1|列2|列3|
|:-:|:-:|:-:|:-:|
|行0|m|m|h|m|
|行1|m|h|m|h|
|行2|m|m|h|m|
|行3|m|h|m|h|

dst数组
|列0|列1|列2|列3|
|:-:|:-:|:-:|:-:|
|行0|m|m|m|m|
|行1|m|m|m|m|
|行2|m|m|m|m|
|行3|m|m|m|m|

> 注：L1缓存共32byte，块大小16字节，直接映射，说明共2组。
读src[0]/[0],未命中，会把src[0]/[0]~src[0]/[3]写入缓存第0组。
然后写dst[0]/[0]，未命中，会把dst[0]/[0]~src[0]/[3]写入缓存第0组。
然后读src[0]/[1],因为第0组被dst覆盖，依然未命中。依此类推。

### 6.35
> L1缓存共128byte，块大小16byte，共8组。可以放下两个数组，不会覆盖。

src地址从0开始
src第0行地址 0 000 0000 ～ 0 000 1100
src第1行地址 0 001 0000 ～ 0 001 1100
src第2行地址 0 010 0000 ～ 0 010 1100
src第3行地址 0 011 0000 ～ 0 011 1100

dst地址从64开始
dst第0行地址 0 100 0000 ～ 0 100 1100
dst第1行地址 0 101 0000 ～ 0 101 1100
dst第2行地址 0 110 0000 ～ 0 110 1100
dst第3行地址 0 111 0000 ～ 0 111 1100

src数组
|列0|列1|列2|列3|
|:-:|:-:|:-:|:-:|
|行0|m|h|h|h|
|行1|m|h|h|h|
|行2|m|h|h|h|
|行3|m|h|h|h|

dst数组
|列0|列1|列2|列3|
|:-:|:-:|:-:|:-:|
|行0|m|h|h|h|
|行1|m|h|h|h|
|行2|m|h|h|h|
|行3|m|h|h|h|

### 6.36
A. 
高速缓存C = 512byte，块B = 16byte，直接映射E = 1，S = 32组。
int x[0]/[0] ~ x[0]/[127]的地址为 00000 0000 ～ 11111 1100
int x[1]/[0] ~ x[1]/[127]的地址为 1 00000 0000 ～ 1 11111 1100
x[0]/[i] 和 x[1]/[i]映射到同一组。
不命中率为100%。
B.
高速缓存1024byte，块16byte，直接映射，共64组。
不命中率为25%。
C.
高速缓存512byte，块16byte，二路组相联E = 2，S= 16组。
int x[0]/[0] ~ x[0]/[127]的地址为 0 0000 0000 ～ 1 1111 1100
int x[1]/[0] ~ x[1]/[127]的地址为 10 0000 0000 ～ 11 1111 1100

    for (i = 0; i < 64; i++)
      sum += x[0][i] * x[1][i];
前一半元素，都能被缓存，不命中率为25%。

    for (i = 0; i < 64; i++)
      sum += x[0][i] * x[1][i];
后一半元素，根据LRU策略，x[0]/[i]替换x[0]/[i-64],x[1]/[i]替换x[1]/[i-64],不命中率也为25%。
D.
不能，块大小为16，不命中率最低为1/4.
E.
能.
如果块大小B = 32byte。不命中率为1/8.

### 6.37
[参考](https://github.com/aQuaYi/CSAPP3E/blob/master/homework/06/6.37.md)
C = 4KB， B = 16byte， E = 1， S = 256.

|函数|N=64|N=60|
|:-:|:-:|:-:|
|sumA|1/4|1/4|
|sumB|1|1/4|
|sumC|1/2|1/4|

```
   0    4    8    12
 0 +----+----+----+
   | 0  | 1  | 2  |
   +----+----+----+

上图的含义是，
a[0][0:4] 会被缓存到 组0
a[0][4:8] 会被缓存到 组1
a[0][8:12] 会被缓存到 组2
```


+ N = 64，64 * 64 * 4byte = 16KB = 4 * C

```
   0    4    8    12   16   20   24   28   32   36   40   44   48   52   56   60   64
 0 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 0  | 1  | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | 10 | 11 | 12 | 13 | 14 | 15 |
 1 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 16 | 17 | 18 | 19 | 20 | 21 | 22 | 23 | 24 | 25 | 26 | 27 | 28 | 29 | 30 | 31 |
 2 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 32 | 33 | 34 | 35 | 36 | 37 | 38 | 39 | 40 | 41 | 42 | 43 | 44 | 45 | 46 | 47 |
 3 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 48 | 49 | 50 | 51 | 52 | 53 | 54 | 55 | 56 | 57 | 58 | 59 | 60 | 61 | 62 | 63 |
 4 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 64 | 65 | 66 | 67 | 68 | 69 | 70 | 71 | 72 | 73 | 74 | 75 | 76 | 77 | 78 | 79 |
 5 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 80 | 81 | 82 | 83 | 84 | 85 | 86 | 87 | 88 | 89 | 90 | 91 | 92 | 93 | 94 | 95 |
 6 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 96 | 97 | 98 | 99 | 100| 101| 102| 103| 104| 105| 106| 107| 108| 109| 110| 111|
 7 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 112| 113| 114| 115| 116| 117| 118| 119| 120| 121| 122| 123| 124| 125| 126| 127|
 8 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 128| 129| 130| 131| 132| 133| 134| 135| 136| 137| 138| 139| 140| 141| 142| 143|
 9 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 144| 145| 146| 147| 148| 149| 150| 151| 152| 153| 154| 155| 156| 157| 158| 159|
10 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 160| 161| 162| 163| 164| 165| 166| 167| 168| 169| 170| 171| 172| 173| 174| 175|
11 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 176| 177| 178| 179| 180| 181| 182| 183| 184| 185| 186| 187| 188| 189| 190| 191|
12 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 192| 193| 194| 195| 196| 197| 198| 199| 200| 201| 202| 203| 204| 205| 206| 207|
13 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 208| 209| 210| 211| 212| 213| 214| 215| 216| 217| 218| 219| 220| 221| 222| 223|
14 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 224| 225| 226| 227| 228| 229| 230| 231| 232| 233| 234| 235| 236| 237| 238| 239|
15 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 240| 241| 242| 243| 244| 245| 246| 247| 248| 249| 250| 251| 252| 253| 254| 255|
16 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 0  | 1  | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | 10 | 11 | 12 | 13 | 14 | 15 |
17 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 16 | 17 | 18 | 19 | 20 | 21 | 22 | 23 | 24 | 25 | 26 | 27 | 28 | 29 | 30 | 31 |
18 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 32 | 33 | 34 | 35 | 36 | 37 | 38 | 39 | 40 | 41 | 42 | 43 | 44 | 45 | 46 | 47 |
19 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 48 | 49 | 50 | 51 | 52 | 53 | 54 | 55 | 56 | 57 | 58 | 59 | 60 | 61 | 62 | 63 |
20 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 64 | 65 | 66 | 67 | 68 | 69 | 70 | 71 | 72 | 73 | 74 | 75 | 76 | 77 | 78 | 79 |
21 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 80 | 81 | 82 | 83 | 84 | 85 | 86 | 87 | 88 | 89 | 90 | 91 | 92 | 93 | 94 | 95 |
22 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 96 | 97 | 98 | 99 | 100| 101| 102| 103| 104| 105| 106| 107| 108| 109| 110| 111|
23 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 112| 113| 114| 115| 116| 117| 118| 119| 120| 121| 122| 123| 124| 125| 126| 127|
24 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 128| 129| 130| 131| 132| 133| 134| 135| 136| 137| 138| 139| 140| 141| 142| 143|
25 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 144| 145| 146| 147| 148| 149| 150| 151| 152| 153| 154| 155| 156| 157| 158| 159|
26 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 160| 161| 162| 163| 164| 165| 166| 167| 168| 169| 170| 171| 172| 173| 174| 175|
27 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 176| 177| 178| 179| 180| 181| 182| 183| 184| 185| 186| 187| 188| 189| 190| 191|
28 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 192| 193| 194| 195| 196| 197| 198| 199| 200| 201| 202| 203| 204| 205| 206| 207|
29 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 208| 209| 210| 211| 212| 213| 214| 215| 216| 217| 218| 219| 220| 221| 222| 223|
30 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 224| 225| 226| 227| 228| 229| 230| 231| 232| 233| 234| 235| 236| 237| 238| 239|
31 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 240| 241| 242| 243| 244| 245| 246| 247| 248| 249| 250| 251| 252| 253| 254| 255|
32 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 0  | 1  | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | 10 | 11 | 12 | 13 | 14 | 15 |
33 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 16 | 17 | 18 | 19 | 20 | 21 | 22 | 23 | 24 | 25 | 26 | 27 | 28 | 29 | 30 | 31 |
34 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 32 | 33 | 34 | 35 | 36 | 37 | 38 | 39 | 40 | 41 | 42 | 43 | 44 | 45 | 46 | 47 |
35 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 48 | 49 | 50 | 51 | 52 | 53 | 54 | 55 | 56 | 57 | 58 | 59 | 60 | 61 | 62 | 63 |
36 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 64 | 65 | 66 | 67 | 68 | 69 | 70 | 71 | 72 | 73 | 74 | 75 | 76 | 77 | 78 | 79 |
37 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 80 | 81 | 82 | 83 | 84 | 85 | 86 | 87 | 88 | 89 | 90 | 91 | 92 | 93 | 94 | 95 |
38 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 96 | 97 | 98 | 99 | 100| 101| 102| 103| 104| 105| 106| 107| 108| 109| 110| 111|
39 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 112| 113| 114| 115| 116| 117| 118| 119| 120| 121| 122| 123| 124| 125| 126| 127|
40 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 128| 129| 130| 131| 132| 133| 134| 135| 136| 137| 138| 139| 140| 141| 142| 143|
41 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 144| 145| 146| 147| 148| 149| 150| 151| 152| 153| 154| 155| 156| 157| 158| 159|
42 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 160| 161| 162| 163| 164| 165| 166| 167| 168| 169| 170| 171| 172| 173| 174| 175|
43 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 176| 177| 178| 179| 180| 181| 182| 183| 184| 185| 186| 187| 188| 189| 190| 191|
44 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 192| 193| 194| 195| 196| 197| 198| 199| 200| 201| 202| 203| 204| 205| 206| 207|
45 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 208| 209| 210| 211| 212| 213| 214| 215| 216| 217| 218| 219| 220| 221| 222| 223|
46 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 224| 225| 226| 227| 228| 229| 230| 231| 232| 233| 234| 235| 236| 237| 238| 239|
47 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 240| 241| 242| 243| 244| 245| 246| 247| 248| 249| 250| 251| 252| 253| 254| 255|
48 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 0  | 1  | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | 10 | 11 | 12 | 13 | 14 | 15 |
49 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 16 | 17 | 18 | 19 | 20 | 21 | 22 | 23 | 24 | 25 | 26 | 27 | 28 | 29 | 30 | 31 |
50 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 32 | 33 | 34 | 35 | 36 | 37 | 38 | 39 | 40 | 41 | 42 | 43 | 44 | 45 | 46 | 47 |
51 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 48 | 49 | 50 | 51 | 52 | 53 | 54 | 55 | 56 | 57 | 58 | 59 | 60 | 61 | 62 | 63 |
52 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 64 | 65 | 66 | 67 | 68 | 69 | 70 | 71 | 72 | 73 | 74 | 75 | 76 | 77 | 78 | 79 |
53 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 80 | 81 | 82 | 83 | 84 | 85 | 86 | 87 | 88 | 89 | 90 | 91 | 92 | 93 | 94 | 95 |
54 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 96 | 97 | 98 | 99 | 100| 101| 102| 103| 104| 105| 106| 107| 108| 109| 110| 111|
55 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 112| 113| 114| 115| 116| 117| 118| 119| 120| 121| 122| 123| 124| 125| 126| 127|
56 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 128| 129| 130| 131| 132| 133| 134| 135| 136| 137| 138| 139| 140| 141| 142| 143|
57 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 144| 145| 146| 147| 148| 149| 150| 151| 152| 153| 154| 155| 156| 157| 158| 159|
58 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 160| 161| 162| 163| 164| 165| 166| 167| 168| 169| 170| 171| 172| 173| 174| 175|
59 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 176| 177| 178| 179| 180| 181| 182| 183| 184| 185| 186| 187| 188| 189| 190| 191|
60 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 192| 193| 194| 195| 196| 197| 198| 199| 200| 201| 202| 203| 204| 205| 206| 207|
61 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 208| 209| 210| 211| 212| 213| 214| 215| 216| 217| 218| 219| 220| 221| 222| 223|
62 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 224| 225| 226| 227| 228| 229| 230| 231| 232| 233| 234| 235| 236| 237| 238| 239|
63 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   | 240| 241| 242| 243| 244| 245| 246| 247| 248| 249| 250| 251| 252| 253| 254| 255|
64 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   0    4    8    12   16   20   24   28   32   36   40   44   48   52   56   60   64

方框中的数字代表 a 中的元素会缓存到的组数

```

> sumA 访问顺序为a[0]/[0]...a[0]/[63], a[1]/[0]...a[1]/[63]...,由图可知，只有冷不命中，每四个元素有一个未命中,未命中率为25%.
>
> sumB 访问顺序为a[0]/[0]...a[63]/[0],a[0]/[1]...a[63]/[1]...,由图可知，在第一轮循环中a[0]/[0], a[16]/[0],a[32]/[0],a[48]/[0]均映射到第0组，所以第二轮循环访问a[0]/[1]时，第0组早已经被覆盖，所以未命中率为100%。
>
> sumC 访问顺序为a[0]/[0], a[1]/[0], a[0]/[1], a[1]/[1], a[2]/[0], a[3]/[0], a[2]/[1], a[3]/[1]...,从上到下，每次访问四格。命中率为50%。
>
> ```
>     j     j+1   j+2   j+3
>      +-----+-----+-----+-----+
>  i  |  m  |  h  |  m  |  h  |
>      +-----+-----+-----+-----+
>  i+1 |  m  |  h  |  m  |  h  |
>      +-----+-----+-----+-----+
> 在访问完 a[i][j] 后，接着会访问到 a[i+16][j], a[i+32][j],a[i+48][j] 等元素，才会访问 a[j][i+2] ，这些元素都会缓存到同一组中，组中的元素早就被替换了。
> ```
>
> 

+ N = 60

```
    0    4    8    12   16   20   24   28   32   36   40   44   48   52   56   60
  0 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 0  | 1  | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | 10 | 11 | 12 | 13 | 14 |
  1 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 15 | 16 | 17 | 18 | 19 | 20 | 21 | 22 | 23 | 24 | 25 | 26 | 27 | 28 | 29 |
  2 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 30 | 31 | 32 | 33 | 34 | 35 | 36 | 37 | 38 | 39 | 40 | 41 | 42 | 43 | 44 |
  3 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 45 | 46 | 47 | 48 | 49 | 50 | 51 | 52 | 53 | 54 | 55 | 56 | 57 | 58 | 59 |
  4 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 60 | 61 | 62 | 63 | 64 | 65 | 66 | 67 | 68 | 69 | 70 | 71 | 72 | 73 | 74 |
  5 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 75 | 76 | 77 | 78 | 79 | 80 | 81 | 82 | 83 | 84 | 85 | 86 | 87 | 88 | 89 |
  6 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 90 | 91 | 92 | 93 | 94 | 95 | 96 | 97 | 98 | 99 | 100| 101| 102| 103| 104|
  7 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 105| 106| 107| 108| 109| 110| 111| 112| 113| 114| 115| 116| 117| 118| 119|
  8 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 120| 121| 122| 123| 124| 125| 126| 127| 128| 129| 130| 131| 132| 133| 134|
  9 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 135| 136| 137| 138| 139| 140| 141| 142| 143| 144| 145| 146| 147| 148| 149|
10 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 150| 151| 152| 153| 154| 155| 156| 157| 158| 159| 160| 161| 162| 163| 164|
11 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 165| 166| 167| 168| 169| 170| 171| 172| 173| 174| 175| 176| 177| 178| 179|
12 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 180| 181| 182| 183| 184| 185| 186| 187| 188| 189| 190| 191| 192| 193| 194|
13 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 195| 196| 197| 198| 199| 200| 201| 202| 203| 204| 205| 206| 207| 208| 209|
14 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 210| 211| 212| 213| 214| 215| 216| 217| 218| 219| 220| 221| 222| 223| 224|
15 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 225| 226| 227| 228| 229| 230| 231| 232| 233| 234| 235| 236| 237| 238| 239|
16 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 240| 241| 242| 243| 244| 245| 246| 247| 248| 249| 250| 251| 252| 253| 254|
17 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 255| 0  | 1  | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | 10 | 11 | 12 | 13 |
18 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 14 | 15 | 16 | 17 | 18 | 19 | 20 | 21 | 22 | 23 | 24 | 25 | 26 | 27 | 28 |
19 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 29 | 30 | 31 | 32 | 33 | 34 | 35 | 36 | 37 | 38 | 39 | 40 | 41 | 42 | 43 |
20 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 44 | 45 | 46 | 47 | 48 | 49 | 50 | 51 | 52 | 53 | 54 | 55 | 56 | 57 | 58 |
21 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 59 | 60 | 61 | 62 | 63 | 64 | 65 | 66 | 67 | 68 | 69 | 70 | 71 | 72 | 73 |
22 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 74 | 75 | 76 | 77 | 78 | 79 | 80 | 81 | 82 | 83 | 84 | 85 | 86 | 87 | 88 |
23 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 89 | 90 | 91 | 92 | 93 | 94 | 95 | 96 | 97 | 98 | 99 | 100| 101| 102| 103|
24 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 104| 105| 106| 107| 108| 109| 110| 111| 112| 113| 114| 115| 116| 117| 118|
25 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 119| 120| 121| 122| 123| 124| 125| 126| 127| 128| 129| 130| 131| 132| 133|
26 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 134| 135| 136| 137| 138| 139| 140| 141| 142| 143| 144| 145| 146| 147| 148|
27 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 149| 150| 151| 152| 153| 154| 155| 156| 157| 158| 159| 160| 161| 162| 163|
28 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 164| 165| 166| 167| 168| 169| 170| 171| 172| 173| 174| 175| 176| 177| 178|
29 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 179| 180| 181| 182| 183| 184| 185| 186| 187| 188| 189| 190| 191| 192| 193|
30 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 194| 195| 196| 197| 198| 199| 200| 201| 202| 203| 204| 205| 206| 207| 208|
31 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 209| 210| 211| 212| 213| 214| 215| 216| 217| 218| 219| 220| 221| 222| 223|
32 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 224| 225| 226| 227| 228| 229| 230| 231| 232| 233| 234| 235| 236| 237| 238|
33 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 239| 240| 241| 242| 243| 244| 245| 246| 247| 248| 249| 250| 251| 252| 253|
34 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 254| 255| 0  | 1  | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | 10 | 11 | 12 |
35 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 13 | 14 | 15 | 16 | 17 | 18 | 19 | 20 | 21 | 22 | 23 | 24 | 25 | 26 | 27 |
36 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 28 | 29 | 30 | 31 | 32 | 33 | 34 | 35 | 36 | 37 | 38 | 39 | 40 | 41 | 42 |
37 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 43 | 44 | 45 | 46 | 47 | 48 | 49 | 50 | 51 | 52 | 53 | 54 | 55 | 56 | 57 |
38 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 58 | 59 | 60 | 61 | 62 | 63 | 64 | 65 | 66 | 67 | 68 | 69 | 70 | 71 | 72 |
39 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 73 | 74 | 75 | 76 | 77 | 78 | 79 | 80 | 81 | 82 | 83 | 84 | 85 | 86 | 87 |
40 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 88 | 89 | 90 | 91 | 92 | 93 | 94 | 95 | 96 | 97 | 98 | 99 | 100| 101| 102|
41 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 103| 104| 105| 106| 107| 108| 109| 110| 111| 112| 113| 114| 115| 116| 117|
42 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 118| 119| 120| 121| 122| 123| 124| 125| 126| 127| 128| 129| 130| 131| 132|
43 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 133| 134| 135| 136| 137| 138| 139| 140| 141| 142| 143| 144| 145| 146| 147|
44 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 148| 149| 150| 151| 152| 153| 154| 155| 156| 157| 158| 159| 160| 161| 162|
45 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 163| 164| 165| 166| 167| 168| 169| 170| 171| 172| 173| 174| 175| 176| 177|
46 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 178| 179| 180| 181| 182| 183| 184| 185| 186| 187| 188| 189| 190| 191| 192|
47 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 193| 194| 195| 196| 197| 198| 199| 200| 201| 202| 203| 204| 205| 206| 207|
48 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 208| 209| 210| 211| 212| 213| 214| 215| 216| 217| 218| 219| 220| 221| 222|
49 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 223| 224| 225| 226| 227| 228| 229| 230| 231| 232| 233| 234| 235| 236| 237|
50 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 238| 239| 240| 241| 242| 243| 244| 245| 246| 247| 248| 249| 250| 251| 252|
51 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 253| 254| 255| 0  | 1  | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | 10 | 11 |
52 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 12 | 13 | 14 | 15 | 16 | 17 | 18 | 19 | 20 | 21 | 22 | 23 | 24 | 25 | 26 |
53 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 27 | 28 | 29 | 30 | 31 | 32 | 33 | 34 | 35 | 36 | 37 | 38 | 39 | 40 | 41 |
54 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 42 | 43 | 44 | 45 | 46 | 47 | 48 | 49 | 50 | 51 | 52 | 53 | 54 | 55 | 56 |
55 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 57 | 58 | 59 | 60 | 61 | 62 | 63 | 64 | 65 | 66 | 67 | 68 | 69 | 70 | 71 |
56 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 72 | 73 | 74 | 75 | 76 | 77 | 78 | 79 | 80 | 81 | 82 | 83 | 84 | 85 | 86 |
57 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 87 | 88 | 89 | 90 | 91 | 92 | 93 | 94 | 95 | 96 | 97 | 98 | 99 | 100| 101|
58 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 102| 103| 104| 105| 106| 107| 108| 109| 110| 111| 112| 113| 114| 115| 116|
59 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    | 117| 118| 119| 120| 121| 122| 123| 124| 125| 126| 127| 128| 129| 130| 131|
60 +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    0    4    8    12   16   20   24   28   32   36   40   44   48   52   56   60

方框中的数字代表 a 中的元素会缓存到的组数。
```

  > sumA 访问顺序为a[0]/[0]...a[0]/[59], a[1]/[0]...a[1]/[59]...,由图可知，只有冷不命中，每四个元素有一个未命中,未命中率为25%.
  >
  > sumB 访问顺序为a[0]/[0]...a[59]/[0],a[0]/[1]...a[59]/[1]...,由图可知，在第一轮循环中只有a[0]/[0]映射到第0组，所以第二轮循环访问a[0]/[1]时，第0组未被覆盖，可以命中，所以未命中率为25%。
  >
  > sumC 访问顺序为a[0]/[0], a[1]/[0], a[0]/[1], a[1]/[1], a[2]/[0], a[3]/[0], a[2]/[1], a[3]/[1]...,从上到下，每次访问四格。命中率为25%。
  >
  > ```
  >     j     j+1   j+2   j+3
  >      +-----+-----+-----+-----+
  >   i  |  m  |  h  |  h  |  h  |
  >      +-----+-----+-----+-----+
  >  i+1 |  m  |  h  |  h  |  h  |
  >      +-----+-----+-----+-----+
  > ```
  >
  > 

### 6.38

A. 16 * 16 * 4 = 1024
B.

```
   0    2    4    6    8    10   12   14   16
 0 +----+----+----+----+----+----+----+----+
   | 0  | 1  | 2  | 3  | 4  | 5  | 6  | 7  |
 1 +----+----+----+----+----+----+----+----+
   | 8  | 9  | 10 | 11 | 12 | 13 | 14 | 15 |
 2 +----+----+----+----+----+----+----+----+
   | 16 | 17 | 18 | 19 | 20 | 21 | 22 | 23 |
 3 +----+----+----+----+----+----+----+----+
   | 24 | 25 | 26 | 27 | 28 | 29 | 30 | 31 |
 4 +----+----+----+----+----+----+----+----+
   | 32 | 33 | 34 | 35 | 36 | 37 | 38 | 39 |
 5 +----+----+----+----+----+----+----+----+
   | 40 | 41 | 42 | 43 | 44 | 45 | 46 | 47 |
 6 +----+----+----+----+----+----+----+----+
   | 48 | 49 | 50 | 51 | 52 | 53 | 54 | 55 |
 7 +----+----+----+----+----+----+----+----+
   | 56 | 57 | 58 | 59 | 60 | 61 | 62 | 63 |
 8 +----+----+----+----+----+----+----+----+
   | 0  | 1  | 2  | 3  | 4  | 5  | 6  | 7  |
 9 +----+----+----+----+----+----+----+----+
   | 8  | 9  | 10 | 11 | 12 | 13 | 14 | 15 |
10 +----+----+----+----+----+----+----+----+
   | 16 | 17 | 18 | 19 | 20 | 21 | 22 | 23 |
11 +----+----+----+----+----+----+----+----+
   | 24 | 25 | 26 | 27 | 28 | 29 | 30 | 31 |
12 +----+----+----+----+----+----+----+----+
   | 32 | 33 | 34 | 35 | 36 | 37 | 38 | 39 |
13 +----+----+----+----+----+----+----+----+
   | 40 | 41 | 42 | 43 | 44 | 45 | 46 | 47 |
14 +----+----+----+----+----+----+----+----+
   | 48 | 49 | 50 | 51 | 52 | 53 | 54 | 55 |
15 +----+----+----+----+----+----+----+----+
   | 56 | 57 | 58 | 59 | 60 | 61 | 62 | 63 |
16 +----+----+----+----+----+----+----+----+
   0    2    4    6    8    10   12   14   16
```

C = 2048byte, E = 1, B = 32byte, S = 64.
当j%2 == 0时，未命中，未命中次数为16*8 = 128
C.未命中率为= 128/1024 = 1/8.

### 6.39
A. 16 * 16 * 4 = 1024
B. 
square[j+1]/[i] - square[j]/[i] = 256
square[j+8]/[i] - square[j]/[i] = 2048
square[j+8]/[i] 覆盖 square[j]/[i]
未命中次数为16 * 16 = 256
C.未命中率 = 256 / 1024 = 1/4.

### 6.40
A. 16*16 + 16*16*3 = 1024
B.
第1段2重循环未命中总数 = 16*16/2 = 128
第2段2重循环未命中总数 = 16*16*3/6 = 128
总不命中数 = 256
C.
未命中率 = 256 / 1024 = 1/4

### 6.41
C = 64KB， E = 1， B = 4byte， S = 2^14
写总数 = 640 * 480 * 4，
每次读取buffer[i]/[j].r不命中
不命中率为25%.

### 6.42
同上，不命中率为25%.

### 6.43
每行只能缓存一个int型数值，所以每次都不命中。
不命中率100%

### 6.44
[下载地址](http://csapp.cs.cmu.edu/3e/mountain.tar)

```
Clock frequency is approx. 2500.0 MHz
Memory mountain (MB/sec)
        s1      s2      s3      s4      s5      s6      s7      s8      s9      s10     s11   s12      s13     s14     s15
128m    10480   5702    4003    3070    2484    2094    1827    1624    1481    1382    1296  1250     1190    1148    1139
64m     10283   5869    4044    3247    2490    2145    1855    1767    1473    1393    1296    1204    1195    1151    1213
32m     11722   6912    5891    4594    3716    3064    2854    1934    2024    2087    2488    1982    2326    2102    2586
16m     20980   11046   7562    5837    4687    4002    3572    3141    3017    2961    2881    2837    2799    2803    2783
8m      22583   12521   8432    6355    5088    4238    3633    3173    3058    2950    2894    2800    2766    2730    2736
4m      22974   12647   8443    6349    5090    4255    3646    3187    3071    2967    2897    2802    2768    2740    2731
2m      22886   12737   8510    6392    5116    4253    3661    3205    3130    3133    3210    3394    3751    4225    4760
1024k   25917   18415   15028   12561   10487   8933    7818    6827    7756    8940    10082   11026   12178   12274   14060
512k    28516   23779   23126   22860   21679   20175   18304   16718   16341   16290   16141   16220   16029   16048   15934
256k    28479   24108   23688   23083   21243   19156   17059   15170   14646   14760   14531   14634   14646   14263   13905
128k    28673   24388   23890   23856   23439   21880   20388   20358   18277   17282   17460   17526   17359   17976   16778
64k     28484   24294   23807   23676   23239   22716   21200   22960   17336   17692   17079   17195   17651   16862   22382
32k     32405   31411   30474   28524   29254   28208   28967   26667   26765   27483   28201   27300   26695   25885   25278
16k     32926   31220   30072   28927   29043   27083   27083   28132   25278   24375   23266   23373   20724   24788   22377
```

### 6.45
[参考1](https://dreamanddead.github.io/CSAPP-3e-Solutions/chapter6/6.45/)：分块方法
[参考2](https://github.com/DesmondoRay/CSAPP-3e/blob/master/chapter_6/6.45.c)： 循环展开
