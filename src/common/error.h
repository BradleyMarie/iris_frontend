#ifndef _SRC_COMMON_ERROR_
#define _SRC_COMMON_ERROR_

#include "iris/iris.h"

namespace iris {

void ReportOOM[[noreturn]]();
void SuccessOrOOM(ISTATUS status);

}  // namespace iris

#endif  // _SRC_COMMON_ERROR_