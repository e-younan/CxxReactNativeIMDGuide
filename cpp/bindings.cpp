#include "bindings.h"

#include <iostream>
#include <string>

#include "metadata.h"  // <--- This is the header file we created in the previous step

using namespace facebook;

void imd::install(jsi::Runtime& rt) {
  auto getImageMetadata = jsi::Function::createFromHostFunction(rt, jsi::PropNameID::forAscii(rt, "getImageMetadata"), 1,
                                                                [](jsi::Runtime& rt, const jsi::Value& thisVal, const jsi::Value* args, size_t count) -> jsi::Object {
                                                                  if (count == 0) {
                                                                    throw jsi::JSError(rt,
                                                                                       "[react-native-image-metadata]["
                                                                                       "getImageMetadata] file path is required");
                                                                  }

                                                                  if (!args[0].isString()) {
                                                                    throw jsi::JSError(rt,
                                                                                       "[react-native-image-metadata][getImageMetadata] "
                                                                                       "file path must be a string");
                                                                  }

                                                                  std::string path = args[0].asString(rt).utf8(rt);

                                                                  return getPNGMetadata(rt, path);  // <--- This is the function we created in the previous step
                                                                });

  jsi::Object md = jsi::Object(rt);

  md.setProperty(rt, "getImageMetadata", std::move(getImageMetadata));

  rt.global().setProperty(rt, "__IMDProxy", std::move(md));
}