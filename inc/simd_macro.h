/* -------------------- */
/* --- simd_macro.h --- */
/* -------------------- */


#ifndef __SIMD_MACRO_H__
#define __SIMD_MACRO_H__

// a remplir

#define vec_load(addr) (_mm_load_ps((float32*)addr))
#define vec_store(addr,val) (_mm_store_ps((float*) addr, val))

#define vec_left1(a,b) (_mm_shuffle_ps(_mm_shuffle_ps(a, b, _MM_SHUFFLE(0, 0, 3, 3)), b, _MM_SHUFFLE(2, 1, 3, 0))) // sortie : b2 b1 b0 a3
#define vec_left2(a,b) (_mm_shuffle_ps(a, b, _MM_SHUFFLE(1, 0, 3, 2))) // sortie : b1 b0 a3 a2
#define vec_left3(v0,v1)  v0
#define vec_left4(v0,v1)  v0

#define vec_right1(b,c) (_mm_shuffle_ps(b, _mm_shuffle_ps(c, b, _MM_SHUFFLE(3, 3, 0, 0)), _MM_SHUFFLE(0, 3, 2, 1)))  // sortie : c0 b3 b2 b1
#define vec_right2(b,c) (_mm_shuffle_ps(b, c, _MM_SHUFFLE(1, 0, 3, 2))) // sortie : c1 c0 b3 b2
#define vec_right3(v1, v2) v1
#define vec_right4(v1, v2) v1


// calculs
#define vec_div3(x) (_mm_mul_ps(x, _mm_set_ps((float)1/3, (float)1/3, (float)1/3, (float)1/3)))
#define vec_div5(x) (_mm_mul_ps(x, _mm_set_ps((float)1/5, (float)1/5, (float)1/5, (float)1/5)))
#define vec_div9(x) (_mm_mul_ps(x, _mm_set_ps((float)1/9, (float)1/9, (float)1/9, (float)1/9)))

#define vec_add3(x0,x1,x2) (_mm_add_ps(x2, _mm_add_ps(x0, x1)))
#define vec_add5(x0,x1,x2,x3,x4) (_mm_add_ps(x4, _mm_add_ps(x3, _mm_add_ps(x2, _mm_add_ps(x0,x1)))))
#define vec_add_2D_9(x0,x1,x2,x3,x4,x5,x6,x7,x8) (_mm_add_ps(x8, _mm_add_ps(x7, _mm_add_ps(x6, _mm_add_ps(x5, _mm_add_ps(x4, _mm_add_ps(x3, _mm_add_ps(x2, _mm_add_ps(x0,x1)))))))))


#define vAVERAGE3(x0,x1,x2) (vec_div3(vec_add3(x0, x1, x2)))
#define vAVERAGE5(x0,x1,x2,x3,x4) (vec_div5(vec_add5(x0,x1,x2,x3,x4)))
#define vAVERAGE_2D_9(x0,x1,x2,x3,x4,x5,x6,x7,x8) (vec_div9(vec_add_2D_9(x0,x1,x2,x3,x4,x5,x6,x7,x8)))

#endif // __SIMD_MACRO_H__
