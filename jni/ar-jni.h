/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class ch_bfh_bachelor_ar_ArLib */

#ifndef _Included_ch_bfh_bachelor_ar_ArLib
#define _Included_ch_bfh_bachelor_ar_ArLib
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     ch_bfh_bachelor_ar_ArLib
 * Method:    initArLib
 * Signature: (IIIIF[F[S[F[FFFF)V
 */
JNIEXPORT void JNICALL Java_ch_bfh_bachelor_ar_ArLib_initArLib
  (JNIEnv *, jclass, jint, jint, jint, jint, jfloat);

/*
 * Class:     ch_bfh_bachelor_ar_ArLib
 * Method:    precessImage
 * Signature: ([BFFF[F)V
 */
JNIEXPORT void JNICALL Java_ch_bfh_bachelor_ar_ArLib_precessImage
  (JNIEnv *, jclass, jbyteArray, jfloat, jfloat, jfloat, jfloatArray);

/*
 * Class:     ch_bfh_bachelor_ar_ArLib
 * Method:    addModel
 * Signature: ([F[S[F[FFFF)V
 */
JNIEXPORT void JNICALL Java_ch_bfh_bachelor_ar_ArLib_addModel
  (JNIEnv *, jclass, jint, jfloatArray, jshortArray, jfloatArray, jfloatArray, jfloat, jfloat, jfloat, jobjectArray, jintArray);

#ifdef __cplusplus
}
#endif
#endif
