#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <memory>
#include <common/ZeroInitializedStruct.hpp>
#include "antlr4-common.h"
#include "antlr4-runtime.h"

using namespace antlr4;


    bool isEQStart();
    bool isInlineEqStart();
	bool isCodeStartBackticks();
    bool isCodeEndBackticks();
