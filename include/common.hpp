#ifndef COMMON_HPP
#define COMMON_HPP

// Type aliases
using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using f32  = float;
using f64  = double;
using f128 = long double;

using isize = ssize_t;
using usize = size_t;

using str = std::string;
using cstr = const char*;

// Common constants
constexpr f32 PI = 3.14159265359f;
constexpr f32 TAU = 2.0f * PI;

#endif