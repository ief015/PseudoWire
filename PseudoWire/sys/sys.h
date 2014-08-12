#pragma once

//Just some reminder messages:
/*
#if !defined(IEF_ENABLE_SSE_MATH) && !defined(IEF_REMINDERS)
# pragma message("Reminders:")

# pragma message("Define ENABLE_SSE_MATH # if you want to enable SSE for the math library. (# being level of SEE -- 1:SSE1, 2:SSE2, 3:SSE3, 4:SSE4)")

# pragma message("End of reminders.")
# define IEF_REMINDERS
#endif
*/

//STD/STL Auto-Includes
#include <string>
#include <vector>
#include <iostream>

//Platforming Tools
#include "Platform.h"

//User Interface Tools
#include "Engine.h"
#include "State.h"
#include "Component.h"
#include "XComponent.h"

//Math and data types
#include "Math.h"
#include "Memory.h"
#include "Random.h"
#include "Color.h"
#include "Point.h"
#include "Vector.h"
#include "Rect.h"
#include "Ray.h"

//Tools
#include "RawArray.h"
#include "List.h"
#include "Stack.h"
#include "String.h"
#include "Stopwatch.h"
#include "HiresTimer.h"
#include "DispatchTimer.h"
#include "Gradient.h"

//Class interfaces
#include "Ref.h"
#include "Singleton.h"

//Helpers
#include "StringHelper.h"
#include "Dbg.h"
