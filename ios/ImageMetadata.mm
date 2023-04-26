#import "ImageMetadata.h"

#import <React/RCTBridge+Private.h>

#import <React/RCTUtils.h>
#import <ReactCommon/RCTTurboModule.h>
#import <jsi/jsi.h>

#import "../cpp/bindings.h"

@implementation ImageMetadata

RCT_EXPORT_MODULE(IMD)

RCT_EXPORT_BLOCKING_SYNCHRONOUS_METHOD(install) {
  NSLog(@"Installing ImageMetadata module...");

  RCTBridge *bridge = [RCTBridge currentBridge];
  RCTCxxBridge *cxxBridge = (RCTCxxBridge *)bridge;
  if (cxxBridge == nil) {
    return @false;
  }

  using namespace facebook;

  auto jsiRuntime = (jsi::Runtime *)cxxBridge.runtime;
  if (jsiRuntime == nil) {
    return @false;
  }
  auto &runtime = *jsiRuntime;

  imd::install(runtime);

  return @true;
}

@end