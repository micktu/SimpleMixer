// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <sstream>

// TODO: reference additional headers your program requires here

#define RETURN_ON_ERROR(hres) if (FAILED(hres)) { return hres; }
#define FALSE_ON_ERROR(hres) if (FAILED(hres)) { return false; }

#define SAFE_RELEASE(punk) if ((punk) != NULL) { (punk)->Release(); (punk) = NULL; }

#define DEBUG_VALUE(value) { std::ostringstream s; s << (value) << std::endl; OutputDebugStringA(s.str().c_str()); }
