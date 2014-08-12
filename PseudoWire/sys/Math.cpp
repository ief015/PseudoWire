#include "Math.h"
#include "Platform.h"
#include "Memory.h"

using namespace sys;

// wtf
const f32 Math::INFINITY_32 =		sys::Memory::Reinterpret<f32, s32>(0x7F800000);
const f64 Math::INFINITY =			sys::Memory::Reinterpret<f64, s64>(0x7FF0000000000000);
const f32 Math::INFINITY_NEG32 =	sys::Memory::Reinterpret<f32, s32>(0xFF800000);
const f64 Math::INFINITY_NEG =		sys::Memory::Reinterpret<f64, s64>(0xFFF0000000000000);
const f32 Math::NAN_32 =			sys::Memory::Reinterpret<f32, s32>(0x7FFFFFFF);
const f64 Math::NAN =				sys::Memory::Reinterpret<f64, s64>(0x7FFFFFFFFFFFFFFF);
const f64 Math::PI = 3.141592653589793238462643383279502884;
const f64 Math::PI2 = 1.5707963267948966192313216916398;
const f32 Math::PI_32 = 3.141592653589793238462643383279502884f;
const f32 Math::PI2_32 = 1.5707963267948966192313216916398f;
const f64 Math::E = 2.71828182845904523536028747135266249775724709369995;
const f32 Math::E_32 = 2.71828182845904523536028747135266249775724709369995f;

const u8 Math::MAX_BYTE = 0xFF;
const s8 Math::MAX_CHAR = 0x7F;
const u16 Math::MAX_USHORT = 0xFFFF;
const s16 Math::MAX_SHORT = 0x7FFF;
const u32 Math::MAX_UINT = 0xFFFFFFFF;
const s32 Math::MAX_INT = 0x7FFFFFFF;
const u64 Math::MAX_ULONG = 0xFFFFFFFFFFFFFFFF;
const s64 Math::MAX_LONG = 0x7FFFFFFFFFFFFFFF;
const f32 Math::MAX_FLOAT = 3.4e38f;
const f64 Math::MAX_DOUBLE = 1.7e308;
