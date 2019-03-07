//
//  ExchangeMethod.h
//  Configuration
//
//  Created by 卡德 on 2019/3/6.
//  Copyright © 2019 卡德. All rights reserved.
//

#ifndef ExchangeMethod_h
#define ExchangeMethod_h

#import <objc/runtime.h>

static inline BOOL ExchangeMethod(Class fromClass, SEL origSelector,
                                  Class toClass, SEL newSelector) {
  if (!fromClass || !toClass) {
    return NO;
  }

  Method origMethod = class_getInstanceMethod(fromClass, origSelector);
  Method newMethod = class_getInstanceMethod(toClass, newSelector);

  if (!newSelector) {
    return NO;
  }

  Class metaClass = object_getClass(fromClass);
  BOOL isAddMethod = class_addMethod(metaClass, origSelector,
                                     method_getImplementation(newMethod),
                                     method_getTypeEncoding(newMethod));

  if (isAddMethod) {
    IMP origMethodIMP = method_getImplementation(origMethod)
                            ?: imp_implementationWithBlock(^(id selfObject){
                               });
    const char* origMethodTypeEncoding =
        method_getTypeEncoding(origMethod) ?: "v@:";
    class_replaceMethod(metaClass, newSelector, origMethodIMP,
                        origMethodTypeEncoding);
  } else {
    method_exchangeImplementations(origMethod, newMethod);
  }
  return YES;
}

#endif /* ExchangeMethod_h */
