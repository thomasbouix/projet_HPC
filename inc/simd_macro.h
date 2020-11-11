/* -------------------- */
/* --- simd_macro.h --- */
/* -------------------- */


#ifndef __SIMD_MACRO_H__
#define __SIMD_MACRO_H__

#include "vnrutil.h"


// ACCES MEMOIRE
#define vec_load(addr) (_mm_load_si128((vuint8*)addr))
#define vec_store(addr,val) (_mm_store_si128((vuint8*) addr, val))

// A MODIFIER
#define vec_left1(a,b) (_mm_shuffle_ps(_mm_shuffle_ps(a, b, _MM_SHUFFLE(0, 0, 3, 3)), b, _MM_SHUFFLE(2, 1, 3, 0))) // sortie : b2 b1 b0 a3
#define vec_left2(a,b) (_mm_shuffle_ps(a, b, _MM_SHUFFLE(1, 0, 3, 2))) // sortie : b1 b0 a3 a2
#define vec_left3(v0,v1)  v0
#define vec_left4(v0,v1)  v0

#define vec_right1(b,c) (_mm_shuffle_ps(b, _mm_shuffle_ps(c, b, _MM_SHUFFLE(3, 3, 0, 0)), _MM_SHUFFLE(0, 3, 2, 1)))  // sortie : c0 b3 b2 b1
#define vec_right2(b,c) (_mm_shuffle_ps(b, c, _MM_SHUFFLE(1, 0, 3, 2))) // sortie : c1 c0 b3 b2
#define vec_right3(v1, v2) v1
#define vec_right4(v1, v2) v1


// DIVISIONS (A MODIFIER)
#define vec_div3(x) (_mm_mul_ps(x, _mm_set_ps((float)1/3, (float)1/3, (float)1/3, (float)1/3)))
#define vec_div5(x) (_mm_mul_ps(x, _mm_set_ps((float)1/5, (float)1/5, (float)1/5, (float)1/5)))
#define vec_div9(x) (_mm_mul_ps(x, _mm_set_ps((float)1/9, (float)1/9, (float)1/9, (float)1/9)))

// ADDITIONS
#define vec_add2(x0,x1) (_mm_add_epi8(x0, x1))

// SOUSTRACTIONS
#define vec_sub2(x0,x1) (_mm_sub_epi8(x0, x1))

// MULTIPLICATION EPI8 AVEC CONSTANTE (< 255) AVEC SATURATION
#define vec_muls_epi8_const(x0,N) (_mm_packus_epi16(_mm_mullo_epi16(_mm_unpacklo_epi8(x0, init_vuint8(0)), init_vuint16(N)), _mm_mullo_epi16(_mm_unpackhi_epi8(x0, init_vuint8(0)), init_vuint16(N))))

// COMPARAISONS
#define vec_cmpgt_vui8(x0,x1) (_mm_cmpgt_epi8(_mm_sub_epi8(x0, init_vuint8(128)), _mm_sub_epi8(x1, init_vuint8(128))))
#define vec_cmpeq_vui8(x0,x1) (_mm_cmpeq_epi8(x0, x1))
#define vec_cmpgte_vui8(x0,x1) (_mm_or_si128(vec_cmpgt_vui8(x0,x1), vec_cmpeq_vui8(x0,x1)))

// MAX et MIN
#define vec_MAX(x0,x1) (_mm_blendv_epi8(x1, x0, vec_cmpgt_vui8(x0,x1)))
#define vec_MIN(x0,x1) (_mm_blendv_epi8(x0, x1, vec_cmpgt_vui8(x0,x1)))

// MOYENNES A MODIFIER
#define vAVERAGE3(x0,x1,x2) (vec_div3(vec_add3(x0, x1, x2)))
#define vAVERAGE5(x0,x1,x2,x3,x4) (vec_div5(vec_add5(x0,x1,x2,x3,x4)))
#define vAVERAGE_2D_9(x0,x1,x2,x3,x4,x5,x6,x7,x8) (vec_div9(vec_add_2D_9(x0,x1,x2,x3,x4,x5,x6,x7,x8)))

// VAL ABS DIFF
#define vABS_DIFF(v0,v1) (_mm_or_si128(_mm_subs_epu8(v0,v1), _mm_subs_epu8(v1,v0)))

// COMPARAISON AVEC SEUIL (UNSIGNED)
#define vCMP_THRESHOLD(vec, threshold) (_mm_cmpgt_epi8(_mm_sub_epi8(vec, init_vuint8(128)), init_vsint8(threshold-128)))

// COMPARAISON PIXELS SD (Si t_moins_1 < t => t_moins_1 + 1, Si t_moins_1 > t => t_moins_1 - 1, Si t_moins_1 = t => t_moins_1)
#define vec_cmp_pixels_SD(t_moins_1,t) (_mm_blendv_epi8(_mm_blendv_epi8(vec_add2(t_moins_1,init_vuint8(1)), vec_sub2(t_moins_1,init_vuint8(1)), vec_cmpgt_vui8(t_moins_1, t)), t_moins_1,vec_cmpeq_vui8(t_moins_1,t)))

#endif // __SIMD_MACRO_H__
