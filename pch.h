#pragma once

#include <Windows.h>
#include <iostream>
#include <algorithm>
#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include <cassert>
#include <type_traits>
#include <thread>
#include <tchar.h>
#include <comdef.h>
#include <sddl.h>

#include "magic_enum.hpp"
using magic_enum::enum_name;
#include "helper.h"

#include "CbsConf.h"
#include "CbsApi.h"
#include "CbsUtil.h"
#include "StackManager.h"
