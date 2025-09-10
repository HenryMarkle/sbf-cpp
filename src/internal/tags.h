#pragma once

#include <cstdint>

namespace SBF {

enum class TagType : uint8_t {

	Open_I32 = 1, 		// 32-bit integer
	Open_I64 = 2, 		// 64-bit integer
	Open_F32 = 3, 		// 32-bit float
	Open_F64 = 4, 		// 64-bit float
	Open_I8 = 5, 		// 8-bit integer
	Open_U32 = 6, 		// unsigned 32-bit integer
	Open_U64 = 7, 		// unsigned 64-bit integer
	Open_U8 = 8, 		// unsgined 8-bit integer
	Open_Char = 9,
	Open_I32_Array = 10,	// 32-bit integer array
	Open_I64_Array = 11,	// 64-bit integer array
	Open_F32_Array = 12,	// 32-bit float array
	Open_F64_Array = 13,	// 64-bit float array
	Open_I8_Array = 14,	// 8-bit integer array
	Open_U32_Array = 15,	// unsigned 32-bit integer array
	Open_U64_Array = 16,	// unsigned 64-bit integer array
	Open_U8_Array = 17,	// unsigned 8-bit integer array
	Open_String = 18,
	Open_Table = 19,	// table

	
	Close_I32 = (uint8_t)-1,
	Close_I64 = (uint8_t)-2,
	Close_F32 = (uint8_t)-3,
	Close_F64 = (uint8_t)-4,
	Close_I8 = (uint8_t)-5,
	Close_U32 = (uint8_t)-6,
	Close_U64 = (uint8_t)-7,
	Close_U8 = (uint8_t)-8,
	Close_Char = (uint8_t)-9,
	Close_I32_Array = (uint8_t)-10,
	Close_I64_Array = (uint8_t)-11,
	Close_F32_Array = (uint8_t)-12,
	Close_F64_Array = (uint8_t)-13,
	Close_I8_Array = (uint8_t)-14,
	Close_U32_Array = (uint8_t)-15,
	Close_U64_Array = (uint8_t)-16,
	Close_U8_Array = (uint8_t)-17,
	Close_String = (uint8_t)-18,
	Close_Table = (uint8_t)-19,

};

};
