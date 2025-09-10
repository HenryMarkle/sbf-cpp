#pragma once

#include <filesystem>
#include <concepts>
#include <cstdint>
#include <cstring>
#include <vector>

namespace SBF {

std::vector<uint8_t> ReadFileAsBytes(const std::filesystem::path &filepath);


inline int32_t ReadI32LE(const uint8_t bytes[4]) { return *reinterpret_cast<const int32_t *>(bytes); }
inline int64_t ReadI64LE(const uint8_t bytes[8]) { return *reinterpret_cast<const int64_t *>(bytes); }
inline float ReadF32LE(const uint8_t bytes[4]) { return *reinterpret_cast<const float *>(bytes); }
inline double ReadF64LE(const uint8_t bytes[8]) { return *reinterpret_cast<const double *>(bytes); }
inline uint32_t ReadU32LE(const uint8_t bytes[4]) { return *reinterpret_cast<const uint32_t *>(bytes); }
inline uint64_t ReadU64LE(const uint8_t bytes[8]) { return *reinterpret_cast<const uint64_t *>(bytes); }


inline int32_t ReadI32BE(const uint8_t bytes[4]) { 
	return 
		(static_cast<int32_t>(bytes[0]) << 24) |
		(static_cast<int32_t>(bytes[1]) << 16) |
		(static_cast<int32_t>(bytes[2]) << 8) | 
		static_cast<int32_t>(bytes[3]); 
}
inline int64_t ReadI64BE(const uint8_t bytes[8]) { 
	return 
		(static_cast<int64_t>(bytes[0]) << 56) |
		(static_cast<int64_t>(bytes[1]) << 48) |
		(static_cast<int64_t>(bytes[2]) << 40) |
		(static_cast<int64_t>(bytes[3]) << 32) |
		(static_cast<int64_t>(bytes[4]) << 24) |
		(static_cast<int64_t>(bytes[5]) << 16) |
		(static_cast<int64_t>(bytes[6]) << 8) |
		static_cast<int64_t>(bytes[7]); 
}
inline uint32_t ReadU32BE(const uint8_t bytes[4]) { 
	return 
		(static_cast<uint32_t>(bytes[0]) << 24) |
		(static_cast<uint32_t>(bytes[1]) << 16) |
		(static_cast<uint32_t>(bytes[2]) << 8) | 
		static_cast<uint32_t>(bytes[3]); 
}
inline uint64_t ReadU64BE(const uint8_t bytes[8]) { 
	return 
		(static_cast<uint64_t>(bytes[0]) << 56) |
		(static_cast<uint64_t>(bytes[1]) << 48) |
		(static_cast<uint64_t>(bytes[2]) << 40) |
		(static_cast<uint64_t>(bytes[3]) << 32) |
		(static_cast<uint64_t>(bytes[4]) << 24) |
		(static_cast<uint64_t>(bytes[5]) << 16) |
		(static_cast<uint64_t>(bytes[6]) << 8) |
		static_cast<uint64_t>(bytes[7]); 
}
inline float ReadF32BE(const uint8_t bytes[4]) { 
	auto i = ReadU32BE(bytes);
	float res;
	std::memcpy(&res, &i, sizeof(float));
	return res;
}
inline double ReadF64BE(const uint8_t bytes[8]) { 
	auto f = ReadU32BE(bytes);
	double res;
	std::memcpy(&res, &f, sizeof(double));
	return res;
}

template<typename T>
concept Primitive = std::integral<T> || std::floating_point<T>;

template<Primitive T>
inline T ReadLE(const uint8_t *bytes) { return *reinterpret_cast<const T *>(bytes); }

template<Primitive T>
inline T ReadBE(const uint8_t *bytes) {
	uint8_t reveresed[sizeof(T)];

	for (auto l = 0; l < sizeof(T); l++) {
		reveresed[l] = bytes[sizeof(T) - 1 - l];		
	}

	return *reinterpret_cast<T *>(reveresed);
}

//

inline void WriteI32LE(uint8_t bytes[4], int32_t i32) { *reinterpret_cast<int32_t *>(bytes) = i32; }
inline void WriteI64LE(uint8_t bytes[8], int64_t i64) { *reinterpret_cast<int64_t *>(bytes) = i64; }
inline void WriteF32LE(uint8_t bytes[4], float f32) { *reinterpret_cast<float *>(bytes) = f32; }
inline void WriteF64LE(uint8_t bytes[8], double f64) { *reinterpret_cast<double *>(bytes) = f64; }
inline void WriteU32LE(uint8_t bytes[4], uint32_t u32) { *reinterpret_cast<uint32_t *>(bytes) = u32; }
inline void WriteU64LE(uint8_t bytes[8], uint64_t u64) { *reinterpret_cast<uint64_t *>(bytes) = u64; }


inline void WriteI32BE(uint8_t bytes[4], int32_t i32) { 
	bytes[0] = static_cast<uint8_t>(i32 << 24);
	bytes[1] = static_cast<uint8_t>(i32 << 16);
	bytes[2] = static_cast<uint8_t>(i32 << 8); 
	bytes[3] = static_cast<uint8_t>(i32); 
}
inline void WriteI64BE(uint8_t bytes[8], int64_t i64) { 
	bytes[0] = static_cast<uint8_t>(i64 >> 56);
	bytes[1] = static_cast<uint8_t>(i64 >> 48);
	bytes[2] = static_cast<uint8_t>(i64 >> 40);
	bytes[3] = static_cast<uint8_t>(i64 >> 32);
	bytes[4] = static_cast<uint8_t>(i64 >> 24);
	bytes[5] = static_cast<uint8_t>(i64 >> 16);
	bytes[6] = static_cast<uint8_t>(i64 >> 8);
	bytes[7] = static_cast<uint8_t>(i64); 
}
inline void WriteU32BE(uint8_t bytes[4], uint32_t u32) { 
	bytes[0] = static_cast<uint8_t>(u32 << 24);
	bytes[1] = static_cast<uint8_t>(u32 << 16);
	bytes[2] = static_cast<uint8_t>(u32 << 8); 
	bytes[3] = static_cast<uint8_t>(u32); 
}
inline void WriteU64BE(uint8_t bytes[8], uint64_t u64) { 
	bytes[0] = static_cast<uint8_t>(u64 >> 56);
	bytes[1] = static_cast<uint8_t>(u64 >> 48);
	bytes[2] = static_cast<uint8_t>(u64 >> 40);
	bytes[3] = static_cast<uint8_t>(u64 >> 32);
	bytes[4] = static_cast<uint8_t>(u64 >> 24);
	bytes[5] = static_cast<uint8_t>(u64 >> 16);
	bytes[6] = static_cast<uint8_t>(u64 >> 8);
	bytes[7] = static_cast<uint8_t>(u64); 
}
inline void WriteF32BE(uint8_t bytes[4], float f) { 
	std::memcpy(bytes, &f, sizeof(float));
}
inline void WriteF64BE(uint8_t bytes[8], double d) { 
	std::memcpy(bytes, &d, sizeof(double));
}

template<Primitive T>
inline void WriteLE(uint8_t *bytes, const T &value) { std::memcpy(bytes, &value, sizeof(T)); }

template<Primitive T>
inline void WriteBE(uint8_t *bytes, const T &value) {
	for (auto l = 0; l < sizeof(T); l++) {
		bytes[sizeof(T) - 1 - l] = reinterpret_cast<const uint8_t *>(&value)[l];
	}
}

#if defined(__BYTE_ORDER__)

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
inline int32_t ReadI32(const uint8_t bytes[4]) { return ReadI32LE(bytes); }
inline int64_t ReadI64(const uint8_t bytes[8]) { return ReadI64LE(bytes); }
inline float ReadF32(const uint8_t bytes[4]) { return ReadF32LE(bytes); }
inline double ReadF64(const uint8_t bytes[8]) { return ReadF64LE(bytes); }
inline uint32_t ReadU32(const uint8_t bytes[4]) { return ReadU32LE(bytes); }
inline uint64_t ReadU64(const uint8_t bytes[8]) { return ReadU64LE(bytes); }

template<Primitive T>
inline T Read(const uint8_t *bytes) { return ReadLE<T>(bytes); }

inline void WriteI32(uint8_t bytes[4], int32_t i32) { WriteI32LE(bytes, i32); }
inline void WriteI64(uint8_t bytes[8], int64_t i64) { WriteI64LE(bytes, i64); }
inline void WriteF32(uint8_t bytes[4], float f32) { WriteF32LE(bytes, f32); }
inline void WriteF64(uint8_t bytes[8], double f64) { WriteF64LE(bytes, f64); }
inline void WriteU32(uint8_t bytes[4], uint32_t u32) { WriteU32LE(bytes, u32); }
inline void WriteU64(uint8_t bytes[8], uint64_t u64) { WriteU64LE(bytes, u64); }

template<Primitive T>
inline void Write(uint8_t *bytes, const T &value) { WriteLE<T>(bytes, value); }
#else
inline int32_t ReadI32(uint8_t bytes[4]) { return ReadI32BE(bytes); }
inline int64_t ReadI64(uint8_t bytes[8]) { return ReadI64BE(bytes); }
inline float ReadF32(uint8_t bytes[4]) { return ReadF32BE(bytes); }
inline double ReadF64(uint8_t bytes[8]) { return ReadF64BE(bytes); }
inline uint32_t ReadU32(uint8_t bytes[4]) { return ReadU32BE(bytes); }
inline uint64_t ReadU64(uint8_t bytes[8]) { return ReadU64BE(bytes); }

template<Primitive T>
inline T Read(const uint8_t *bytes) { return ReadBE<T>(bytes); }

inline void WriteI32(uint8_t bytes[4], int32_t i32) { WriteI32BE(bytes, i32); }
inline void WriteI64(uint8_t bytes[8], int64_t i64) { WriteI64BE(bytes, i64); }
inline void WriteF32(uint8_t bytes[4], float f32) { WriteF32BE(bytes, f32); }
inline void WriteF64(uint8_t bytes[8], double f64) { WriteF64BE(bytes, f64); }
inline void WriteU32(uint8_t bytes[4], uint32_t u32) { WriteU32BE(bytes, u32); }
inline void WriteU64(uint8_t bytes[8], uint64_t u64) { WriteU64BE(bytes, u64); }

template<Primitive T>
inline T Write(const uint8_t *bytes, const T &value) { WriteBE<T>(bytes, value); }
#endif

#elif defined(_WIN32)
inline int32_t ReadI32(uint8_t bytes[4]) { return ReadI32LE(bytes); }
inline int64_t ReadI64(uint8_t bytes[8]) { return ReadI64LE(bytes); }
inline float ReadF32(uint8_t bytes[4]) { return ReadF32LE(bytes); }
inline double ReadF64(uint8_t bytes[8]) { return ReadF64LE(bytes); }
inline uint32_t ReadU32(uint8_t bytes[4]) { return ReadU32LE(bytes); }
inline uint64_t ReadU64(uint8_t bytes[8]) { return ReadU64LE(bytes); }

template<Primitive T>
inline T Read(const uint8_t *bytes) { return ReadLE<T>(bytes); }

inline void WriteI32(uint8_t bytes[4], int32_t i32) { WriteI32LE(bytes, i32); }
inline void WriteI64(uint8_t bytes[8], int64_t i64) { WriteI64LE(bytes, i64); }
inline void WriteF32(uint8_t bytes[4], float f32) { WriteF32LE(bytes, f32); }
inline void WriteF64(uint8_t bytes[8], double f64) { WriteF64LE(bytes, f64); }
inline void WriteU32(uint8_t bytes[4], uint32_t u32) { WriteU32LE(bytes, u32); }
inline void WriteU64(uint8_t bytes[8], uint64_t u64) { WriteU64LE(bytes, u64); }

template<Primitive T>
inline void Write(uint8_t *bytes, const T &value) { WriteLE<T>(bytes, value); }
#endif

};
