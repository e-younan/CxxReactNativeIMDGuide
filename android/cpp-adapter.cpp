#include <jni.h>
#include "react-native-image-metadata.h"

extern "C"
JNIEXPORT jint JNICALL
Java_com_imagemetadata_ImageMetadataModule_nativeMultiply(JNIEnv *env, jclass type, jdouble a, jdouble b) {
    return imagemetadata::multiply(a, b);
}
