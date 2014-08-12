#pragma once
#include "../Platform.h"

#ifdef IEF_PLATFORM_WIN32

# include "../externs/glew17/includes/glew.h"
# include "../externs/glew17/includes/wglew.h"

# include <windows.h>
# include <GL/gl.h>
# include <GL/glu.h>

#elif IEF_PLATFORM_LINUX || IEF_PLATFORM_FREEBSD

# include <GL/gl.h>
# include <GL/glu.h>

#elif IEF_PLATFORM_MACOSX

# include <OpenGL/gl.h>
# include <OpenGL/glu.h>

#endif
