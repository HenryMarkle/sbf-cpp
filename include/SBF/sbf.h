#pragma once

#include <cstddef>
#include <cstdint>

#ifdef _WIN32
	#ifdef SBF_EXPORTS
		#define SBF_API __declspec(dllexport)	
	#else
		#define SBF_API __declspec(dllimport)
	#endif
#else
	#define SBF_API 
#endif

typedef enum {
	// Do not use this.
	NodeType_None,

	NodeType_I32,  
	NodeType_I64,  
	NodeType_F32,  
	NodeType_F64,  
	NodeType_I8, 
	NodeType_U32,  
	NodeType_U64,              
	NodeType_U8,

	// Same as U8.
	NodeType_Char,

	NodeType_I32A, 
	NodeType_I64A, 
	NodeType_F32A, 
	NodeType_F64A, 
	NodeType_I8A, 
	NodeType_U32A, 
	NodeType_U64A,             
	NodeType_U8A,

	// Has the same underlying representation as U8A (not null-terminated).
	NodeType_String,
	
	NodeType_T

} NodeType;

typedef struct Node Node;

#ifdef __cplusplus
extern "C" {
#endif

SBF_API Node *SBF_CreateNode_I8(int8_t i8);
SBF_API Node *SBF_CreateNode_U8(uint8_t u8);
SBF_API Node *SBF_CreateNode_Char(char c);
SBF_API Node *SBF_CreateNode_I32(int32_t i32);
SBF_API Node *SBF_CreateNode_I64(int64_t i64);
SBF_API Node *SBF_CreateNode_U32(uint32_t u32);
SBF_API Node *SBF_CreateNode_U64(uint64_t u64);
SBF_API Node *SBF_CreateNode_F32(float f32);
SBF_API Node *SBF_CreateNode_F64(double f64);
/// Creates an array node based, taking ownership of the array buffer.
SBF_API Node *SBF_CreateNode_Array(NodeType type, void *array, size_t length); 
/// Creates a string node from a null-terminated string, taking ownership
/// of the string buffer.
SBF_API Node *SBF_CreateNode_String(char *str); 

/// Creates a table node, taking ownership of key/value buffers;
/// The key strings must be null-terminated.
SBF_API Node *SBF_CreateNode_Table(char **keys, Node **values, size_t length);

SBF_API void SBF_DestroyNode(Node *);

SBF_API NodeType SBF_GetNodeType(const Node *node);
SBF_API int8_t SBF_NodeGet_I8(Node *node);
SBF_API uint8_t SBF_NodeGet_U8(Node *node);
SBF_API char SBF_NodeGet_Char(Node *node);
SBF_API int32_t SBF_NodeGet_I32(Node *node);
SBF_API int64_t SBF_NodeGet_I64(Node *node);
SBF_API uint32_t SBF_NodeGet_U32(Node *node);
SBF_API uint64_t SBF_NodeGet_U64(Node *node);
SBF_API float SBF_NodeGet_F32(Node *node);
SBF_API double SBF_NodeGet_F64(Node *node);
SBF_API size_t SBF_NodeGet_ArrayLength(Node *node);
SBF_API int8_t *SBF_NodeGet_I8A(Node *node);
SBF_API uint8_t *SBF_NodeGet_U8A(Node *node);
SBF_API int32_t *SBF_NodeGet_I32A(Node *node);
SBF_API int64_t *SBF_NodeGet_I64A(Node *node);
SBF_API uint32_t *SBF_NodeGet_U32A(Node *node);
SBF_API uint64_t *SBF_NodeGet_U64A(Node *node);
SBF_API float *SBF_NodeGet_F32A(Node *node);
SBF_API double *SBF_NodeGet_F64A(Node *node);
/// Returns the length of the string NOT including the null terminator.
SBF_API size_t SBF_NodeGet_StringLength(const Node *node);
/// Returns a non-null-terminated string.
/// Reteive the string length with the dedicated function.
SBF_API char *SBF_NodeGet_String(Node *node);
SBF_API size_t SBF_NodeGet_TableLength(const Node *node);
SBF_API void SBF_NodeGet_Table(Node *node, char ***keys, Node ***values);

SBF_API Node *SBF_Deserialize(const uint8_t *bytes, size_t length, size_t *begin);

/// Writes the given node into bytes with length at cursor.
SBF_API void SBF_Serialize(const Node *node, uint8_t *bytes, size_t length, size_t *cursor);

/// Calculates the total size of the given node in bytes.
SBF_API size_t SBF_CalculateSize(const Node *node);

/// Serializes a node tree into a file.
/// If filepath does not exist, a file is creates (if path is valid; otherwise exception).
/// node pointer must not be null.
SBF_API void SBF_WriteFile(const char *filepath, const Node *node);

/// Deserializes file into a node tree, and retreives the format version.
/// filepath must exist, and version pointer is optional (can be null).
SBF_API Node *SBF_ReadFile(const char *filepath, uint8_t *version);

#ifdef SBF_STRIP_PREFIX
SBF_API inline Node *CreateNode_I8(int8_t i8) { return SBF_CreateNode_I8(i8); }
SBF_API inline Node *CreateNode_U8(uint8_t u8) { return SBF_CreateNode_U8(u8); }
SBF_API inline Node *CreateNode_Char(char c) { return SBF_CreateNode_Char(c); }
SBF_API inline Node *CreateNode_I32(int32_t i32) { return SBF_CreateNode_I32(i32); }
SBF_API inline Node *CreateNode_I64(int64_t i64) { return SBF_CreateNode_I64(i64); }
SBF_API inline Node *CreateNode_U32(uint32_t u32) { return SBF_CreateNode_U32(u32); }
SBF_API inline Node *CreateNode_U64(uint64_t u64) { return SBF_CreateNode_U64(u64); }
SBF_API inline Node *CreateNode_F32(float f32) { return SBF_CreateNode_F32(f32); }
SBF_API inline Node *CreateNode_F64(double f64) { return SBF_CreateNode_F64(f64); }

/// Creates an array node based, taking ownership of the array buffer.
SBF_API inline Node *CreateNode_Array(NodeType type, void *array, size_t length) { return SBF_CreateNode_Array(type, array, length); } 

/// Creates a string node from a null-terminated string, taking ownership
/// of the string buffer.
SBF_API inline Node *CreateNode_String(char *str) { return SBF_CreateNode_String(str); } 
SBF_API inline Node *CreateNode_Table(char **keys, Node **values, size_t length) { return SBF_CreateNode_Table(keys, values, length); }


SBF_API inline void DestroyNode(Node *node) { SBF_DestroyNode(node); }

SBF_API inline NodeType GetNodeType(const Node *node) { return SBF_GetNodeType(node); }
SBF_API inline int8_t NodeGet_I8(Node *node) { return SBF_NodeGet_I8(node); }
SBF_API inline uint8_t NodeGet_U8(Node *node) { return SBF_NodeGet_U8(node); }
SBF_API inline char NodeGet_Char(Node *node) { return SBF_NodeGet_Char(node); }
SBF_API inline int32_t NodeGet_I32(Node *node) { return SBF_NodeGet_I32(node); }
SBF_API inline int64_t NodeGet_I64(Node *node) { return SBF_NodeGet_I64(node); }
SBF_API inline uint32_t NodeGet_U32(Node *node) { return SBF_NodeGet_U32(node); }
SBF_API inline uint64_t NodeGet_U64(Node *node) { return SBF_NodeGet_U64(node); }
SBF_API inline float NodeGet_F32(Node *node) { return SBF_NodeGet_F32(node); }
SBF_API inline double NodeGet_F64(Node *node) { return SBF_NodeGet_F64(node); }
SBF_API inline size_t NodeGet_ArrayLength(Node *node) { return SBF_NodeGet_ArrayLength(node); }
SBF_API inline int8_t *NodeGet_I8A(Node *node) { return SBF_NodeGet_I8A(node); }
SBF_API inline uint8_t *NodeGet_U8A(Node *node) { return SBF_NodeGet_U8A(node); }
SBF_API inline int32_t *NodeGet_I32A(Node *node) { return SBF_NodeGet_I32A(node); }
SBF_API inline int64_t *NodeGet_I64A(Node *node) { return SBF_NodeGet_I64A(node); }
SBF_API inline uint32_t *NodeGet_U32A(Node *node) { return SBF_NodeGet_U32A(node); }
SBF_API inline uint64_t *NodeGet_U64A(Node *node) { return SBF_NodeGet_U64A(node); }
SBF_API inline float *NodeGet_F32A(Node *node) { return SBF_NodeGet_F32A(node); }
SBF_API inline double *NodeGet_F64A(Node *node) { return SBF_NodeGet_F64A(node); }
SBF_API inline size_t NodeGet_StringLength(const Node *node) { return SBF_NodeGet_StringLength(node); }

/// Returns a non-null-terminated string.
/// Reteive the string length with the dedicated function.
SBF_API inline char *NodeGet_String(Node *node) { return SBF_NodeGet_String(node); }
SBF_API inline size_t NodeGet_TableLength(const Node *node) { return SBF_NodeGet_TableLength(node); }
SBF_API inline void NodeGet_Table(Node *node, char ***keys, Node ***values) { return SBF_NodeGet_Table(node, keys, values); }

SBF_API inline Node *Deserialize(const uint8_t *bytes, size_t length, size_t *begin) {
	return SBF_Deserialize(bytes, length, begin);
}


/// Writes the given node into bytes with length at cursor.
SBF_API inline void Serialize(const Node *node, uint8_t *bytes, size_t length, size_t *cursor) { return SBF_Serialize(node, bytes, length, cursor); }

/// Calculates the total size of the given node in bytes.
SBF_API inline size_t CalculateSize(const Node *node) { return SBF_CalculateSize(node); }


/// Serializes a node tree into a file.
/// If filepath does not exist, a file is creates (if path is valid; otherwise exception).
/// node pointer must not be null.
SBF_API inline void WriteFile(const char *filepath, const Node *node) { SBF_WriteFile(filepath, node); }

/// Deserializes file into a node tree, and retreives the format version.
/// filepath must exist, and version pointer is optional (can be null).
SBF_API inline Node *ReadFile(const char *filepath, uint8_t *version) { return SBF_ReadFile(filepath, version); }
#endif


#ifdef __cplusplus
}
#endif
