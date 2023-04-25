#ifdef __cplusplus
#import "react-native-image-metadata.h"
#endif

#ifdef RCT_NEW_ARCH_ENABLED
#import "RNImageMetadataSpec.h"

@interface ImageMetadata : NSObject <NativeImageMetadataSpec>
#else
#import <React/RCTBridgeModule.h>

@interface ImageMetadata : NSObject <RCTBridgeModule>
#endif

@end
