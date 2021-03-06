#ifndef _SRC_COMMON_OSTREAM_
#define _SRC_COMMON_OSTREAM_

#include <iostream>

#include "iris/iris.h"

std::ostream& operator<<(std::ostream& os, const ISTATUS& status);
std::ostream& operator<<(std::ostream& os, const POINT3& point);
std::ostream& operator<<(std::ostream& os, const VECTOR3& vector);

#endif  // _SRC_COMMON_OSTREAM