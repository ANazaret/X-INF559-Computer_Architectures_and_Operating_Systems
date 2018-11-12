/* Achille Nazaret - achille.nazaret
 *
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

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */

#define BLOCKSIZE_32x32 8
#define BLOCKSIZE_64x64_I 8
#define BLOCKSIZE_64x64_J 8
#define BLOCKSIZE_61x67_ 8

char transpose_submit_desc[] = "Transpose submission";

void transpose_submit(int M, int N, int A[N][M], int B[M][N]) {
    int i, j, bi, bj;
    if (M == 32 && N == 32) {

        // 4 local var

        for (bi = 0; bi < 32; bi += BLOCKSIZE_32x32) {
            for (bj = 0; bj < 32; bj += BLOCKSIZE_32x32) {
                if (bi == bj)
                    continue;
                for (i = bi; i < bi + BLOCKSIZE_32x32; i++) {
                    for (j = bj; j < bj + BLOCKSIZE_32x32; j++) {
                        B[j][i] = A[i][j];
                    }
                }
            }

            for (i = bi; i < bi + BLOCKSIZE_32x32; i++) {
                for (j = bi; j < bi + BLOCKSIZE_32x32; j++) {
                    if (j != i)
                        B[j][i] = A[i][j];
                }
                B[i][i] = A[i][i];
            }
        }
    }


    if (M == 64 && N == 64) {

        int cache0, cache1, cache2, cache3, cache4, cache5, cache6, cache7;

        // 11 variables !

        for (bi = 0; bi < 64; bi += BLOCKSIZE_64x64_I) {
            for (bj = 0; bj < 64; bj += BLOCKSIZE_64x64_J) {
                for (i = bi; i < bi + 4; i++) {
                    cache0 = A[i][bj + 0];
                    cache1 = A[i][bj + 1];
                    cache2 = A[i][bj + 2];
                    cache3 = A[i][bj + 3];
                    cache4 = A[i][bj + 4];
                    cache5 = A[i][bj + 5];
                    cache6 = A[i][bj + 6];
                    cache7 = A[i][bj + 7];


                    B[bj + 0][i] = cache0;
                    B[bj + 1][i] = cache1;
                    B[bj + 2][i] = cache2;
                    B[bj + 3][i] = cache3;

                    // Don't want to evict the cache of B, let's store cacheX temporarily in B (it is free because in cache)
                    // Huge trick !! Roll by one to be able to alternate after!
                    B[bj + 0][i + 4] = cache5;
                    B[bj + 1][i + 4] = cache6;
                    B[bj + 2][i + 4] = cache7;
                    B[bj + 3][i + 4] = cache4;


                }

                for (i = 0; i < 4; i++) {
                    cache0 = A[bi + 4][bj + (i + 3) % 4];
                    cache1 = A[bi + 5][bj + (i + 3) % 4];
                    cache2 = A[bi + 6][bj + (i + 3) % 4];
                    cache3 = A[bi + 7][bj + (i + 3) % 4];

                    cache4 = A[bi + 4][bj + i + 4];
                    cache5 = A[bi + 5][bj + i + 4];
                    cache6 = A[bi + 6][bj + i + 4];
                    cache7 = A[bi + 7][bj + i + 4];

                    B[bj + i + 4][bi + 4] = cache4;
                    B[bj + i + 4][bi + 5] = cache5;
                    B[bj + i + 4][bi + 6] = cache6;
                    B[bj + i + 4][bi + 7] = cache7;

                    B[bj + i + 4][bi + 0] = B[bj + (i + 3) % 4][bi + 4];
                    B[bj + i + 4][bi + 1] = B[bj + (i + 3) % 4][bi + 5];
                    B[bj + i + 4][bi + 2] = B[bj + (i + 3) % 4][bi + 6];
                    B[bj + i + 4][bi + 3] = B[bj + (i + 3) % 4][bi + 7];

                    B[bj + (i + 3) % 4][bi + 4] = cache0;
                    B[bj + (i + 3) % 4][bi + 5] = cache1;
                    B[bj + (i + 3) % 4][bi + 6] = cache2;
                    B[bj + (i + 3) % 4][bi + 7] = cache3;

                }


            }
        }
    }


    if (M == 61 && N == 67) {
        int cache;
        // 4 local var



        for (bj = 0; bj < M; bj += BLOCKSIZE_61x67_) {
            for (bi = 0; bi < N; bi += BLOCKSIZE_61x67_) {
                if (bi == bj)
                    continue;
                for (i = bi; i < bi + BLOCKSIZE_61x67_ && i < 67; i++) {
                    for (j = bj; j < bj + BLOCKSIZE_61x67_ && j < 61; j++) {
                        B[j][i] = A[i][j];
                    }
                }
            }

            for (i = bj; i < bj + BLOCKSIZE_61x67_; i++) {
                for (j = bj; j < bj + BLOCKSIZE_61x67_; j++) {
                    if (j != i)
                        B[j][i] = A[i][j];
                    else {
                        cache = A[i][i];
                    }
                    //B[j][i] = A[i][j];
                }
                B[i][i] = cache;
            }
        }

    }


}


char transpose_init_submit_desc[] = "Clear code for 32x32";

void transpose_init_submit(int M, int N, int A[N][M], int B[M][N]) {
    int i, j, bi, bj;


    for (bi = 0; bi < 4; bi++) {
        for (bj = 0; bj < 4; bj++) {
            for (i = 0; i < 8; i++) {
                for (j = 0; j < 8; j++) {
                    if (j + bj * 8 != i + bi * 8)
                        B[j + bj * 8][i + bi * 8] = A[i + bi * 8][j + bj * 8];
                }
            }
        }
    }
    for (i = 0; i < 32; i++) {
        B[i][i] = A[i][i];
    }
}


char transpose_64_submit_desc[] = "Clear code for 64x64";

void transpose_64(int M, int N, int A[N][M], int B[M][N]) {
    int i, j, bi, bj;


    for (bi = 0; bi < 64; bi += BLOCKSIZE_64x64_I) {
        for (bj = 0; bj < 64; bj += BLOCKSIZE_64x64_J) {
            if (bi == bj)
                continue;
            for (j = bj; j < bj + BLOCKSIZE_64x64_J; j++) {
                for (i = bi; i < bi + BLOCKSIZE_64x64_I / 2; i++) {

                    B[j][i] = A[i][j];
                }
            }
            for (j = bj; j < bj + BLOCKSIZE_64x64_J; j++) {
                for (i = bi + BLOCKSIZE_64x64_I / 2; i < bi + BLOCKSIZE_64x64_I; i++) {

                    B[j][i] = A[i][j];
                }
            }
        }

        for (i = bi; i < bi + BLOCKSIZE_64x64_I; i++) {
            for (j = bi; j < bi + BLOCKSIZE_64x64_J / 2; j++) {
                if (j != i)
                    B[j][i] = A[i][j];
            }
        }
        for (i = bi; i < bi + BLOCKSIZE_64x64_I; i++) {
            for (j = bi + BLOCKSIZE_64x64_J / 2; j < bi + BLOCKSIZE_64x64_J; j++) {
                if (j != i)
                    B[j][i] = A[i][j];
            }
            B[i][i] = A[i][i];
        }
    }
}

char transpose_64_submit_desc2[] = "Researches for 64x64";

void transpose2_64(int M, int N, int A[N][M], int B[M][N]) {

    for (int bi1 = 0; bi1 < 64; bi1 += 32) {
        for (int bj1 = 0; bj1 < 64; bj1 += 32) {
            for (int bi2 = bi1; bi2 < bi1 + 32; bi2 += 16) {
                for (int bj2 = bj1; bj2 < bj1 + 32; bj2 += 16) {
                    for (int bi3 = bi2; bi3 < bi2 + 16; bi3 += 8) {
                        for (int bj3 = bj2; bj3 < bj2 + 16; bj3 += 8) {
                            for (int bi4 = bi3; bi4 < bi3 + 8; bi4 += 4) {
                                for (int bj4 = bj3; bj4 < bj3 + 8; bj4 += 4) {
                                    for (int j = bj4; j < bj4 + 4; j++) {
                                        for (int i = bi4; i < bi4 + 4; i++) {
                                            B[j][i] = A[i][j];
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

/*
 * void swap(int *a, int *b) {
    int c;
    c = *a;
    *a = *b;
    *b = c;

}*/

char transpose_64_store_in_B_desc[] = "Transpose the 64 x 64 matrix using cache in B";

void transpose_64_store_in_B(int M, int N, int A[N][M], int B[M][N]) {
    int bi, bj, i, cache0, cache1, cache2, cache3, cache4, cache5, cache6, cache7;

    // 11 variables !

    for (bi = 0; bi < 64; bi += BLOCKSIZE_64x64_I) {
        for (bj = 0; bj < 64; bj += BLOCKSIZE_64x64_I) {
            for (i = bi; i < bi + 4; i++) {
                cache0 = A[i][bj + 0];
                cache1 = A[i][bj + 1];
                cache2 = A[i][bj + 2];
                cache3 = A[i][bj + 3];
                cache4 = A[i][bj + 4];
                cache5 = A[i][bj + 5];
                cache6 = A[i][bj + 6];
                cache7 = A[i][bj + 7];


                B[bj + 0][i] = cache0;
                B[bj + 1][i] = cache1;
                B[bj + 2][i] = cache2;
                B[bj + 3][i] = cache3;

                // Don't want to evict the cache of B, let's store cacheX temporarily in B (it is free because in cache)
                // Huge trick !! Roll by one to be able to alternate after!
                B[bj + 0][i + 4] = cache5;
                B[bj + 1][i + 4] = cache6;
                B[bj + 2][i + 4] = cache7;
                B[bj + 3][i + 4] = cache4;


            }

/*
            cache0 = A[bi + 4][bj + 3];
            cache1 = A[bi + 5][bj + 3];
            cache2 = A[bi + 6][bj + 3];
            cache3 = A[bi + 7][bj + 3];

            cache4 = A[bi + 4][bj + 4];
            cache5 = A[bi + 5][bj + 4];
            cache6 = A[bi + 6][bj + 4];
            cache7 = A[bi + 7][bj + 4];

            B[bj + 4][bi + 4] = cache4;
            B[bj + 4][bi + 5] = cache5;
            B[bj + 4][bi + 6] = cache6;
            B[bj + 4][bi + 7] = cache7;

            B[bj + 4][bi + 0] = B[bj + 3][bi + 4];
            B[bj + 4][bi + 1] = B[bj + 3][bi + 5];
            B[bj + 4][bi + 2] = B[bj + 3][bi + 6];
            B[bj + 4][bi + 3] = B[bj + 3][bi + 7];

            B[bj + 3][bi + 4] = cache0;
            B[bj + 3][bi + 5] = cache1;
            B[bj + 3][bi + 6] = cache2;
            B[bj + 3][bi + 7] = cache3;*/


            for (i = 0; i < 4; i++) {
                cache0 = A[bi + 4][bj + (i + 3) % 4];
                cache1 = A[bi + 5][bj + (i + 3) % 4];
                cache2 = A[bi + 6][bj + (i + 3) % 4];
                cache3 = A[bi + 7][bj + (i + 3) % 4];

                cache4 = A[bi + 4][bj + i + 4];
                cache5 = A[bi + 5][bj + i + 4];
                cache6 = A[bi + 6][bj + i + 4];
                cache7 = A[bi + 7][bj + i + 4];

                B[bj + i + 4][bi + 4] = cache4;
                B[bj + i + 4][bi + 5] = cache5;
                B[bj + i + 4][bi + 6] = cache6;
                B[bj + i + 4][bi + 7] = cache7;

                B[bj + i + 4][bi + 0] = B[bj + (i + 3) % 4][bi + 4];
                B[bj + i + 4][bi + 1] = B[bj + (i + 3) % 4][bi + 5];
                B[bj + i + 4][bi + 2] = B[bj + (i + 3) % 4][bi + 6];
                B[bj + i + 4][bi + 3] = B[bj + (i + 3) % 4][bi + 7];

                B[bj + (i + 3) % 4][bi + 4] = cache0;
                B[bj + (i + 3) % 4][bi + 5] = cache1;
                B[bj + (i + 3) % 4][bi + 6] = cache2;
                B[bj + (i + 3) % 4][bi + 7] = cache3;

            }


        }
    }

}


/*
 *
 * DOESN'T WORK (YET ;) - see submission function)
 *
char transpose_64_store_in_B_desc[] = "Transpose the 64 x 64 matrix using cache in B";
void transpose_64_store_in_B(int M, int N, int A[N][M], int B[M][N]) {
    int bi, bj, i, cache0, cache1, cache2, cache3, cache4, cache5, cache6, cache7;

    // 11 variables + 1 in swap !

    for(bi = 0; bi < 64; bi += BLOCKSIZE_64x64_I){
        for(bj = 0; bj < 64; bj += BLOCKSIZE_64x64_I){
            //if (bi != bj)
            for(i=bi; i<bi+4; i++){
                cache0 = A[i][bj+0];
                cache1 = A[i][bj+1];
                cache2 = A[i][bj+2];
                cache3 = A[i][bj+3];
                cache4 = A[i][bj+4];
                cache5 = A[i][bj+5];
                cache6 = A[i][bj+6];
                cache7 = A[i][bj+7];


                B[bj+0][i] = cache0;
                B[bj+1][i] = cache1;
                B[bj+2][i] = cache2;
                B[bj+3][i] = cache3;

                // Don't want to evict the cache of B, let's store cacheX temporarily in B (it is free because in cache)
                // Huge trick !! Roll
                B[bj+0][i+4] = cache5;
                B[bj+1][i+4] = cache6;
                B[bj+2][i+4] = cache7;
                B[bj+3][i+4] = cache4;

            }


            cache0 = A[i+4][bj+0];
            cache1 = A[i+4][bj+1];
            cache2 = A[i+4][bj+2];
            cache3 = A[i+4][bj+3];
            cache4 = A[i][bj+4];
            cache5 = A[i][bj+5];
            cache6 = A[i][bj+6];
            cache7 = A[i][bj+7];



            cache0 = B[bj+3][bi+4];
            cache1 = B[bj+3][bi+5];
            cache2 = B[bj+3][bi+6];
            cache3 = B[bj+3][bi+7];

            for(i=0; i<4; i++){
                cache4 = B[bj+i+4][bi];
                cache5 = B[bj+i+4][bi+1];
                cache6 = B[bj+i+4][bi+2];
                cache7 = B[bj+i+4][bi+3];

                B[bj+i+4][bi] = cache0;
                B[bj+i+4][bi+1] = cache1;
                B[bj+i+4][bi+2]= cache2;
                B[bj+i+4][bi+3] = cache3;

                cache0 = B[bj+i][bi+4];
                cache1 = B[bj+i][bi+5];
                cache2 = B[bj+i][bi+6];
                cache3 = B[bj+i][bi+7];

                B[bj+i][bi+4] = cache4;
                B[bj+i][bi+5] = cache5;
                B[bj+i][bi+6] = cache6;
                B[bj+i][bi+7] = cache7;

            }





        }
    }

}*/

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";

void trans(int M, int N, int A[N][M], int B[M][N]) {
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
void registerFunctions() {
    /* Register your solution function */

    registerTransFunction(transpose_submit, transpose_submit_desc);

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc);
    registerTransFunction(transpose_init_submit, transpose_init_submit_desc);
    registerTransFunction(transpose_64, transpose_64_submit_desc);
    registerTransFunction(transpose2_64, transpose_64_submit_desc2);
    registerTransFunction(transpose_64_store_in_B, transpose_64_store_in_B_desc);
}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N]) {
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

