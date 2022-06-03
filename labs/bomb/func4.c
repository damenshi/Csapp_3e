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
            printf("%d\n", i);
        }
    }
    return 0;
}