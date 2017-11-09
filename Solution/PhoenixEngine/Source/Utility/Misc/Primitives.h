#ifndef PHOENIX_PRIMITIVES_H
#define PHOENIX_PRIMITIVES_H

#include <cstddef>
#include <limits>
#include <utility>

namespace Phoenix
{
	typedef char FChar;

	typedef signed char Int8;
	typedef signed short Int16;
	typedef signed int Int32;
	typedef signed long long Int64;

	typedef unsigned char FByte;
	typedef unsigned char UInt8;
	typedef unsigned short UInt16;
	typedef unsigned int UInt32;
	typedef unsigned long long UInt64;

	typedef float Float32;
	typedef double Float64;

	typedef std::size_t SizeT;
	typedef std::nullptr_t NullptrT;
	
	enum class EInitParam
	{
		Null
	};

	template <class T>
	using TNumericLimits = std::numeric_limits<T>;

	template <class T1, class T2>
	using TPair = std::pair<T1, T2>;

	static_assert(sizeof(FChar) == 1, "FChar is expected to take up 1 byte.");

	static_assert(sizeof(Int8) == 1, "Int8 is expected to take up 1 byte.");
	static_assert(sizeof(Int16) == 2, "Int16 is expected to take up 2 bytes.");
	static_assert(sizeof(Int32) == 4, "Int32 is expected to take up 4 bytes.");
	static_assert(sizeof(Int64) == 8, "Int64 is expected to take up 8 bytes.");

	static_assert(sizeof(UInt8) == 1, "UInt8 is expected to take up 1 byte.");
	static_assert(sizeof(UInt16) == 2, "UInt16 is expected to take up 2 bytes.");
	static_assert(sizeof(UInt32) == 4, "UInt32 is expected to take up 4 bytes.");
	static_assert(sizeof(UInt64) == 8, "UInt64 is expected to take up 8 bytes.");

	static_assert(sizeof(Float32) == 4, "Float32 is expected to take up 4 bytes.");
	static_assert(sizeof(Float64) == 8, "Float64 is expected to take up 8 bytes.");

	static_assert(TNumericLimits<Float32>::is_iec559, "Float32 is expected to be in a IEEE 754 format.");
	static_assert(TNumericLimits<Float64>::is_iec559, "Float64 is expected to be in a IEEE 754 format.");
}

#endif
