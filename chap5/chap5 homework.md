---
attachments: [Clipboard_2022-06-08-14-40-38.png, Clipboard_2022-06-08-15-15-45.png, Clipboard_2022-06-08-15-30-04.png, Clipboard_2022-06-08-17-07-23.png]
tags: [csapp/10]
title: chap5 homework
created: '2022-06-08T03:44:52.363Z'
modified: '2022-06-09T01:42:16.226Z'
---

# chap5 homework

### 5.13
![](@attachment/Clipboard_2022-06-08-14-40-38.png)
A. 
![](@attachment/Clipboard_2022-06-08-15-30-04.png)
> 只有在循环中改变数值的寄存器，才会存在于图中。
由于图中的 [load] 和 [mul] 不会改变图中寄存器中的值，所以没有放置在竖线当中。
由于浮点的加法更耗时，所以，右侧的竖线，成为关键路径。

B.
根据5-12，浮点数加法延迟为3,所以CPE下界为3.
C.
根据5-12，整数加法延迟为1，所以CPE下界为1.
D.
关键路径上只有浮点数加法。加载和乘法可以并行执行。

### 5.14

A.
void inner4(vec_ptr u, vec_ptr v, data_t *dest){
    long i;
    long length = vec_length(u) - 5;
    data_t *udata = get_vec_start(u);
    data_t *vdata = get_vec_start(v);
    data_t sum = (data_t) 0;
    for(i = 0; i < length; i += 6){
        sum = sum + udata[i] * vdata[i]
            + udata[i+1] * vdata[i+1]
            + udata[i+2] * vdata[i+2];
            + udata[i+3] * vdata[i+3];
            + udata[i+4] * vdata[i+4];
            + udata[i+5] * vdata[i+5];
    }
    for( ; i < length; i++){
      sum = sum + udata[i] + vdata[i];
    }
    *dest = sum;
}
![](@attachment/Clipboard_2022-06-08-17-07-23.png)

每个时钟周期只能加载两个值，即使流水线是满的，CPE最小只能为1.

B.关键路径上的加法操作没有减少。

### 5.15

    void inner4(vec_ptr u, vec_ptr v, data_t *dest){
        long i;
        long length = vec_length(u) - 5;
        data_t *udata = get_vec_start(u);
        data_t *vdata = get_vec_start(v);
        data_t sum = (data_t)0, sum1 = (data_t)0, sum2 = (data_t)0,sum3 = (data_t)0,sum4 = (data_t)0,sum5 = (data_t)0;
        for(i = 0; i < length; i += 6){
            sum = sum + udata[i] * vdata[i]
            sum1 = sum1 + udata[i+1] * vdata[i+1]
            sum2 = sum2 + udata[i+2] * vdata[i+2];
            sum3 = sum3 + udata[i+3] * vdata[i+3];
            sum4 = sum4 + udata[i+4] * vdata[i+4];
            sum5 = sum5 + udata[i+5] * vdata[i+5];
        }
        for( ; i < length; i++){
            sum = sum + udata[i] * vdata[i];
        }
        *dest = sum + sum1 + sum2 + sum3 + sum4 + sum5;
    }


> 注： 根据书中描述，浮点加法延迟L= 3， 容量C= 1， 循环展开 K >= 3时，接近吞吐量界限，CPE = 1. 目前K = 6，不能达到吞吐量界限可能的原因为：
> 1.处理器浮点数乘法单元数目限制，无法做到6个浮点数乘法并行计算。
> 2.寄存器数目不足

### 5.16

    void inner4(vec_ptr u, vec_ptr v, data_t *dest){
      long i;
      long length = vec_length(u) - 5;
      data_t *udata = get_vec_start(u);
      data_t *vdata = get_vec_start(v);
      data_t sum = (data_t) 0;
      for(i = 0; i < length; i += 6){
          sum = sum + (udata[i] * vdata[i]
              + udata[i+1] * vdata[i+1]
              + udata[i+2] * vdata[i+2];
              + udata[i+3] * vdata[i+3];
              + udata[i+4] * vdata[i+4];
              + udata[i+5] * vdata[i+5]);
      }
      for( ; i < length; i++){
          sum = sum + udata[i] * vdata[i];
      }
      *dest = sum;
    }

### 5.17

    #include<stdio.h>
    #include<assert.h>
    #include<string.h>
    #include<stdlib.h>

    void *basic_memset(void *s, int c, size_t n){
        size_t cnt = 0;
        unsigned char *schar = s;
        while (cnt < n)
        {
            *schar++ = (unsigned char) c;
            cnt++;
        }
        return s;
    }

    void *effective_memset(void *s, unsigned long lc, size_t n){
        size_t K = sizeof(unsigned long);

        //先用按字节填充，使地址向8对齐
        size_t cnt = 0;
        unsigned char *schar = s;
        while (cnt < n)
        {   
            if((size_t)schar % 8 == 0)
                break;
            *schar++ = (unsigned char) lc;
            cnt++;
        }

        unsigned long *slong = (unsigned long *)schar;
        size_t rest = n - cnt; //剩余需要填充的字节数
        size_t loop = rest / K; //每次填充8个字节，需要的循环数
        size_t tail = rest % 8; //剩余需要按字节填充的循环数。

        for(size_t i = 0; i < loop; i++){//每次填充8字节
            *slong++ = lc;
        }

        schar = (unsigned char*) slong;
        for(size_t i = 0; i < tail; i++){//每次填充1字节
            *schar++ = (unsigned char) lc; 
        }
        return s;
    }

    int main(){
        size_t space = sizeof(char) * 65537;

        void* basic_s = malloc(space);
        void* effective_s = malloc(space);

        int basic_fill = 0xff;
        unsigned long effect_fill = ~0;

        basic_memset(basic_s, basic_fill, space);
        effective_memset(effective_s, effect_fill, space);

        assert(memcmp(basic_s, effective_s,space) == 0);
        return 0;
    }

### 5.18

    /*
    根据书中描述，浮点加法延迟L= 5， 容量C= 2， 循环展开 K >= 10时，接近吞吐量界限，CPE = 1.
    */
    double poly10_10(double a[], double x, long degree){
        long i;
        
        double xpwr1 = x;
        double xpwr2 = xpwr1 * x;
        double xpwr3 = xpwr2 * x;
        double xpwr4 = xpwr3 * x; 
        double xpwr5 = xpwr4 * x;
        double xpwr6 = xpwr5 * x;
        double xpwr7 = xpwr6 * x;
        double xpwr8 = xpwr7 * x;
        double xpwr9 = xpwr8 * x;
        double xpwr10 = xpwr9 * x;
        double x10 = xpwr10;

        double result1 = a[0], result2 = 0, result3 = 0, result4 = 0, result5 = 0, result6 = 0, result7 = 0, result8 = 0, result9 = 0,result10 = 0;


        //循环展开、并行累积
        for(i = 1; i <= degree - 9; i += 10){
            result1 += a[i] * xpwr1;
            result2 += a[i + 1] * xpwr2;
            result3 += a[i + 2] * xpwr3;
            result4 += a[i + 3] * xpwr4;
            result5 += a[i + 4] * xpwr5;
            result6 += a[i + 5] * xpwr6;
            result7 += a[i + 6] * xpwr7;
            result8 += a[i + 7] * xpwr8;
            result9 += a[i + 8] * xpwr9;
            result10 += a[i + 9] * xpwr10;

            xpwr1 *= x10;
            xpwr2 *= x10;
            xpwr3 *= x10;
            xpwr4 *= x10;
            xpwr5 *= x10;
            xpwr6 *= x10;
            xpwr7 *= x10;
            xpwr8 *= x10;
            xpwr9 *= x10;
            xpwr10 *= x10;
        }

        for( ; i <= degree; i++){
            result1 += a[i] * xptr1;
            xpwr1 *= x;
        }
        result1 += (result2 + result3 + result4 + result5 + result6 + result7  + result8  + result9 + result10);
        return result1;

    }

    //参考2
    double poly6_3a(double a[], double x, long degree){
        long i;
        double result1 = a[0];
        double result2 = 0, result3 = 0;

        double xpwr1 = x;
        double xpwr2 = x * x * x;
        double xpwr3 = x * x * x * x * x;
        double x6 = xpwr3 * x;

        for(i = 1; i <= degree - 5; i +=6){
            result1 = result1 + (a[i] * xpwr1 + a[i + 1] * xpwr1 * x);
            result2 = result2 + (a[i + 2]) * xpwr2 + a[i + 3] * xpwr2 * x);
            result3 = result3 + (a[i + 4]) * xpwr3 + a[i + 5] * xpwr3 * x);

            xpwr1 *= x6;
            xpwr2 *= x6;
            xpwr3 *= x6;
        }

        for( ; i <= degree; i++){
            result1 += a[i] * xpwr1;
            xpwr1 *= x;
        }
        result1 += result2 + result3;
        return result1;
    }

### 5.19

+ 浮点数加法L = 3, C = 1, K >= 3;

    //循环展开，重新结合
    void psum4_1a(float a[], float p[], long n){
        long i;
        p[0] = a[0];
        float temp1 = a[0], temp2, temp3, temp4;
        for(int i = 1; i < n - 3; i += 4){
            temp1 += a[i];
            temp2 = temp1 + a[i + 1];
            temp3 = temp2 + a[i + 2];
            temp4 = temp3 + a[i + 3];
            
            p[i] = temp1;
            p[i + 1] = temp2;
            p[i + 2] = temp3;
            p[i + 3] = temp4;

            temp1 = temp4;
        }

        for( ; i < n; i++){
            temp1 += a[i];
            p[i] = temp1;
        }
    }


