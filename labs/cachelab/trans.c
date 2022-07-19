/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

void transpose_32_32(int A[32][32], int B[32][32]);
void transpose_8_8_block(int M, int N, int A[N][M], int B[M][N]);
void transpose_copy(int M, int N, int A[N][M], int B[M][N]);
void transpose_4_4_block(int A[64][64], int B[64][64]);
void transpose_64_64(int M, int N, int A[N][M], int B[M][N]);
void transpose_61_67(int M, int N, int A[N][M], int B[M][N]);

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
#define BLOCK 16
int min(int a, int b){
    return a < b ? a : b;
}

int max(int a, int b){
    return a > b ? a : b;
}

char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{   
    if(M == 32 && N == 32){
        return transpose_copy(M, N, A, B);
    }else if(M == 64 && N == 64){
        return transpose_64_64(M, N, A, B);
    }else if(M == 61 && N == 67){
        return transpose_61_67(M, N, A, B);
    }
    
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

// 直接分块实现转置
char transpose_8_8_desc[] = "Transpose 8*8 block";
void transpose_8_8_block(int M, int N, int A[N][M], int B[M][N])
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

//引入局部变量实现32*32转置
char transpose_32_32_desc[] = "Transpose 32 * 32";
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
//先复制再转置
char transpose_copy_trans[] = "copy then trans";
void transpose_copy(int M, int N, int A[N][M], int B[M][N])
{   
    int a0, a1, a2, a3, a4, a5, a6, a7;
    int i = 0, j = 0, k = 0, s = 0;
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
            //再转置
            for(k = 0; k < 8; k++){
                for(s = k + 1; s < 8; s++){
                    a0 = B[k + j][s + i];
                    B[k + j][s + i] = B[s + j][k + i];
                    B[s + j][k + i] = a0;
                }
            }
        }
    }
}

// 4 * 4 block
char transpose_4_4_desc[] = "Transpose 4 * 4 block";
void transpose_4_4_block(int A[64][64], int B[64][64])
{   
    int a0, a1, a2, a3;
    for(int i = 0; i < 64; i += 4){
        for(int j = 0; j < 64; j += 4){
            for(int k = i; k < i + 4; k++){
                a0 = A[k][j];
                a1 = A[k][j + 1];
                a2 = A[k][j + 2]; 
                a3 = A[k][j + 3];
            
                B[j][k] = a0;
                B[j + 1][k] = a1;
                B[j + 2][k] = a2;
                B[j + 3][k] = a3;
            }
        }
    }
}

// 64 * 64
char transpose_64_64_desc[] = "Transpose 64 * 64";
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

// 61 * 67
char transpose_61_67_desc[] = "Transpose 61 * 67";
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
/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(transpose_8_8_block, transpose_8_8_desc); 
    //registerTransFunction(transpose_32_32, transpose_32_32_desc);  
    //registerTransFunction(transpose_copy, transpose_copy_trans); 
    registerTransFunction(trans, trans_desc); 
}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

