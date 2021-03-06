#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include "CommCtrl.h"

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <array>

#include "utils.h"

#define SMALL_NUMBER  (1.e-8f)
#define KINDA_SMALL_NUMBER (1.e-4f)
#define BIG_NUMBER   (3.4e+38f)

#define RETURN_ON_ERROR(hres) if (FAILED(hres)) { return hres; }
#define FALSE_ON_ERROR(hres) if (FAILED(hres)) { return false; }

#define SAFE_RELEASE(punk) if ((punk) != NULL) { (punk)->Release(); (punk) = NULL; }

#define DEBUG_VALUE(value) { std::ostringstream s; s << (value) << std::endl; OutputDebugStringA(s.str().c_str()); }
