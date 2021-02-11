#ifndef SOURCE_DIR_H
#define SOURCE_DIR_H

#ifndef SOURCE_DIR
#error "SOURCE_DIR not set!"
#endif
#define STR1(x)  #x
#define STR(x)  STR1(x)

#include <string>

const std::string sourceDir = STR(SOURCE_DIR) "/";
#endif