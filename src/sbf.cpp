#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <stdlib.h>
#include <stdio.h>
#include <cstdint>
#include <cstring>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "SBF/sbf.h"

#include "exceptions.h"
#include "tags.h"
#include "io.h"

// Memory inefficient. Needs better implementation in the future.
typedef struct Node {
	NodeType type;
	
	union {

		int8_t i8;
		uint8_t u8;
		int32_t i32;
		int64_t i64;
		uint32_t u32;
		uint64_t u64;
		float f32;
		double f64;
		char c;

		struct {
			/// A non-null terminated character array.
			char *string;
			size_t string_length;
		};

		struct {
			/// Array of type determinated by the type of the struct.
			void *array;
			size_t array_length;
		};

		struct {
			char **keys;
	        	Node **values;
			size_t table_length;
		};	
	};
} Node;

Node *SBF_CreateNode_I8(int8_t i8) {
	auto node = (Node *)malloc(sizeof(Node));

	node->type = NodeType_I8;
	node->i8 = i8;

	return node;
}

Node *SBF_CreateNode_U8(uint8_t u8) {
	auto node = (Node *)malloc(sizeof(Node));

	node->type = NodeType_U8;
	node->u8 = u8;

	return node;
}

Node *SBF_CreateNode_Char(char c) {
	auto node = (Node *)malloc(sizeof(Node));

	node->type = NodeType_Char;
	node->c = c;

	return node;
}

Node *SBF_CreateNode_I32(int32_t i32) {
	auto node = (Node *)malloc(sizeof(Node));
	
	node->type = NodeType_I32;
	node->i32 = i32;

	return node;
}

Node *SBF_CreateNode_I64(int64_t i64) {
	auto node = (Node *)malloc(sizeof(Node));

	node->type = NodeType_I64;
	node->i64 = i64;

	return node;
}

Node *SBF_CreateNode_U32(uint32_t u32) {
	auto node = (Node *)malloc(sizeof(Node));

	node->type = NodeType_U32;
	node->u32 = u32;

	return node;
}

Node *SBF_CreateNode_U64(uint64_t u64) {
	auto node = (Node *)malloc(sizeof(Node));

	node->type = NodeType_U64;
	node->u64 = u64;

	return node;
}

Node *SBF_CreateNode_F32(float f32) {
	auto node = (Node *)malloc(sizeof(Node));

	node->type = NodeType_F32;
	node->f32 = f32;

	return node;
}

Node *SBF_CreateNode_F64(double f64) {
	auto node = (Node *)malloc(sizeof(Node));

	node->type = NodeType_F64;
	node->f64 = f64;

	return node;
}

Node *SBF_CreateNode_Array(NodeType type, void *array, size_t length) {
	auto node = (Node *)malloc(sizeof(Node));

	node->type = type;
	node->array = array;
	node->array_length = length;

	return node;
}

Node *SBF_CreateNode_String(char *str) {
	auto node = (Node *)malloc(sizeof(Node));

	node->type = NodeType_String;
	node->string = str;
	node->string_length = std::strlen(str);

	return node;
}

Node *SBF_CreateNode_Table(char **keys, Node **values, size_t length) {
	auto node = (Node *)malloc(sizeof(Node));

	node->type = NodeType_T;
	node->keys = keys;
	node->values = values;
	node->table_length = length;

	return node;
}

void SBF_DestroyNode(Node *node) {
	if (!node) return;

	if (node->array_length) free(node->array);
	else if (node->table_length) {
		for (auto x = 0; x < node->table_length; x++) {
			free(node->keys[x]);
			SBF_DestroyNode(node->values[x]);
		}

		free(node->keys);
		free(node->values);
	} else if (node->string_length) {
		free(node->string);
	}

	free(node);
}


NodeType SBF_GetNodeType(const Node *node) { return node->type; }
int8_t SBF_NodeGet_I8(Node *node) { return node->i8; }
uint8_t SBF_NodeGet_U8(Node *node) { return node->u8; }
char SBF_NodeGet_Char(Node *node) { return node->c; }
int32_t SBF_NodeGet_I32(Node *node) { return node->i32; }
int64_t SBF_NodeGet_I64(Node *node) { return node->i64; }
uint32_t SBF_NodeGet_U32(Node *node) { return node->u32; }
uint64_t SBF_NodeGet_U64(Node *node) { return node->u64; }
float SBF_NodeGet_F32(Node *node) { return node->f32; }
double SBF_NodeGet_F64(Node *node) { return node->f64; }
size_t SBF_NodeGet_ArrayLength(Node *node) { return node->array_length; }
int8_t *SBF_NodeGet_I8A(Node *node) { return (int8_t *)node->array; }
uint8_t *SBF_NodeGet_U8A(Node *node) { return (uint8_t *)node->array; }
int32_t *SBF_NodeGet_I32A(Node *node) { return (int32_t *)node->array; }
int64_t *SBF_NodeGet_I64A(Node *node) { return (int64_t *)node->array; }
uint32_t *SBF_NodeGet_U32A(Node *node) { return (uint32_t *)node->array; }
uint64_t *SBF_NodeGet_U64A(Node *node) { return (uint64_t *)node->array; }
float *SBF_NodeGet_F32A(Node *node) { return (float *)node->array; }
double *SBF_NodeGet_F64A(Node *node) { return (double *)node->array; }
size_t SBF_NodeGet_StringLength(const Node *node) { return node->string_length; }
char *SBF_NodeGet_String(Node *node) { return node->string; }
size_t SBF_NodeGet_TableLength(const Node *node) { return node->table_length; }
void SBF_NodeGet_Table(Node *node, char ***keys, Node ***values) {
	*keys = node->keys;
	*values = node->values;
}

Node *SBF_Deserialize(const uint8_t *bytes, size_t length, size_t *begin) {
	static const char *type_names[] = {
		"UNKNOWN", 

		"I32",  "I64",  "F32",  "F64",  "I8",
		"U32",  "U64",                  "U8",
		"Char",
		"I32A", "I64A", "F32A", "F64A", "I8A",
		"U32A", "U64A",                 "U32A",
		"String",
		"T"
	};

	static const size_t type_sizes[] = {
		0,

		4, 8, 4, 8, 1,
		4, 8,       1,
		1,
		8, 8, 8, 8, 8,
		8, 8,       8,
		8,
		0
	};

	if (*begin >= length) return nullptr;

	auto type_byte = bytes[*begin];
	
	if (type_byte < 1 || type_byte > 19) 
		throw SBF::SerdeException(std::string("invalid tag '") + std::to_string(type_byte) + "'");

	auto type = static_cast<SBF::TagType>(type_byte);

	const auto type_name = type_names[static_cast<int>(type)];
	const auto type_size = type_sizes[static_cast<int>(type)];

	*begin = *begin + 1;

	auto expected_length = type_size + 1;
	
	if (length - *begin < expected_length) 
		throw SBF::DeserException(
			std::string("bytes array too small, expected at least ")
				+ std::to_string(expected_length)
				+ " bytes, but got "
				+ std::to_string(length - *begin), 
			type_name, 
			*begin
		);

	size_t array_length = 0; // if is array
	if (type_byte > 9 && type_byte < 19) array_length = SBF::Read<uint64_t>(bytes + *begin);

	expected_length = type_size + 1 + (array_length * type_sizes[static_cast<int>(type) - 9]);
	// Make sure bytes fit the array.
	if (length - *begin < expected_length) 
		throw SBF::DeserException(
			std::string("bytes array too small; expected at least ")
				+ std::to_string(expected_length)
				+ " bytes, but got "
				+ std::to_string(length - *begin),
			type_name, 
			*begin
		);
	
	Node *node = nullptr;

	switch (type) {
	case SBF::TagType::Open_I32:
		node = SBF_CreateNode_I32(SBF::Read<int32_t>(bytes + *begin)); break;

	case SBF::TagType::Open_I64:
		node = SBF_CreateNode_I32(SBF::Read<int64_t>(bytes + *begin)); break;

	case SBF::TagType::Open_F32:
		node = SBF_CreateNode_F32(SBF::Read<float>(bytes + *begin)); break;

	case SBF::TagType::Open_F64:
		node = SBF_CreateNode_F64(SBF::Read<double>(bytes + *begin)); break;

	case SBF::TagType::Open_I8:
		node = SBF_CreateNode_I8(SBF::Read<int8_t>(bytes + *begin)); break;

	case SBF::TagType::Open_U32:
		node = SBF_CreateNode_U32(SBF::Read<uint32_t>(bytes + *begin)); break;

	case SBF::TagType::Open_U64:
		node = SBF_CreateNode_U64(SBF::Read<uint64_t>(bytes + *begin)); break;

	case SBF::TagType::Open_U8:
		node = SBF_CreateNode_U8(SBF::Read<uint8_t>(bytes + *begin)); break;
	
	case SBF::TagType::Open_Char:
		node = SBF_CreateNode_Char((char)SBF::Read<uint8_t>(bytes + *begin)); break;
	
	case SBF::TagType::Open_I32_Array:
		if (!length) {
			node = SBF_CreateNode_Array(NodeType_I32A, nullptr, 0);
		} else {
			auto array = (int32_t *)malloc(sizeof(int32_t) * array_length);
			for (auto x = 0; x < array_length; x++) {
				array[x] = SBF::Read<int32_t>(bytes + *begin + 8 + x*type_sizes[static_cast<int>(type) - 9]);
			}

			node = SBF_CreateNode_Array(NodeType_I32A, array, array_length);
		}
		break;


	case SBF::TagType::Open_I64_Array:
		if (!length) {
			node = SBF_CreateNode_Array(NodeType_I64A, nullptr, 0);
		} else {
			auto array = (int64_t *)malloc(sizeof(int64_t) * array_length);
			for (auto x = 0; x < array_length; x++) {
				array[x] = SBF::Read<int64_t>(bytes + *begin + 8 + x*type_sizes[static_cast<int>(type) - 9]);
			}

			node = SBF_CreateNode_Array(NodeType_I64A, array, array_length);
		}
		break;

	case SBF::TagType::Open_F32_Array:
		if (!length) {
			node = SBF_CreateNode_Array(NodeType_F32A, nullptr, 0);
		} else {
			auto array = (float *) malloc(sizeof(float) * array_length);
			for (auto x = 0; x < array_length; x++) {
				array[x] = SBF::Read<float>(bytes + *begin + 8 + x*type_sizes[static_cast<int>(type) - 9]);
			}

			node = SBF_CreateNode_Array(NodeType_F32A, array, array_length);
		}
		break;

	case SBF::TagType::Open_F64_Array:
		if (!length) {
			node = SBF_CreateNode_Array(NodeType_F64A, nullptr, 0);
		} else {
			auto array = (double *)malloc(sizeof(double) * array_length);
			for (auto x = 0; x < array_length; x++) {
				array[x] = SBF::Read<double>(bytes + *begin + 8 + x*type_sizes[static_cast<int>(type) - 9]);
			}

			node = SBF_CreateNode_Array(NodeType_F64A, array, array_length);
		}
		break;

	case SBF::TagType::Open_I8_Array:
		if (!length) {
			node = SBF_CreateNode_Array(NodeType_I8A, nullptr, 0);
		} else {
			auto array = (int8_t *)malloc(sizeof(int8_t) * array_length);
			for (auto x = 0; x < array_length; x++) {
				array[x] = SBF::Read<int8_t>(bytes + *begin + 8 + x*type_sizes[static_cast<int>(type) - 9]);
			}

			node = SBF_CreateNode_Array(NodeType_I8A, array, array_length);
		}
		break;

	case SBF::TagType::Open_U32_Array:
		if (!length) {
			node = SBF_CreateNode_Array(NodeType_U32A, nullptr, 0);
		} else {
			auto array = (uint32_t *)malloc(sizeof(uint32_t) * array_length);
			for (auto x = 0; x < array_length; x++) {
				array[x] = SBF::Read<uint32_t>(bytes + *begin + 8 + x*type_sizes[static_cast<int>(type) - 9]);
			}

			node = SBF_CreateNode_Array(NodeType_U32A, array, array_length);
		}
		break;

	case SBF::TagType::Open_U64_Array:
		if (!length) {
			node = SBF_CreateNode_Array(NodeType_U64A, nullptr, 0);
		} else {
			auto array = (uint64_t *)malloc(sizeof(uint64_t) * array_length);
			for (auto x = 0; x < array_length; x++) {
				array[x] = SBF::Read<uint64_t>(bytes + *begin + 8 + x*type_sizes[static_cast<int>(type) - 9]);
			}

			node = SBF_CreateNode_Array(NodeType_U64A, array, array_length);
		}
		break;

	case SBF::TagType::Open_U8_Array:
		if (!length) {
			node = SBF_CreateNode_Array(NodeType_U8A, nullptr, 0);
		} else {
			auto array = (uint8_t *)malloc(sizeof(uint8_t) * array_length);
			for (auto x = 0; x < array_length; x++) {
				array[x] = SBF::Read<uint8_t>(bytes + *begin + 8 + x*type_sizes[static_cast<int>(type) - 9]);
			}

			node = SBF_CreateNode_Array(NodeType_U8A, array, array_length);
		}
		break;


	case SBF::TagType::Open_String:
		if (!length) {
			node = SBF_CreateNode_Array(NodeType_String, nullptr, 0);
		} else {
			auto array = (char *)malloc(sizeof(char) * array_length + 1);
			std::memcpy(array, bytes + *begin + 8, array_length);
			array[array_length] = '\0';
			node = SBF_CreateNode_Array(NodeType_String, array, array_length);
		}
		break;

	case SBF::TagType::Open_Table:
		{
			if (*begin >= length)
				throw SBF::DeserException("bytes array too small", type_name, *begin);

			std::vector<char *> keys_vec;
			std::vector<Node *> values_vec;

			const auto cleanup = [&keys_vec, &values_vec]() {
				for (auto key : keys_vec) free(key);
				for (auto node : values_vec) SBF_DestroyNode(node);
			};

			size_t table_length = 0;

			while (true) {
				if (static_cast<SBF::TagType>(bytes[*begin]) == SBF::TagType::Close_Table) break;

				// Deserialize key

				auto key_byte = static_cast<int8_t>(bytes[*begin]);

				if (static_cast<SBF::TagType>(key_byte) != SBF::TagType::Open_String) {
					cleanup();
					throw SBF::DeserException("table entry must be String", type_name, *begin);
				}

				Node * key_node = nullptr;
				try {
					key_node = SBF_Deserialize(bytes, length, begin);
				} catch (SBF::SerdeException &se) {
					cleanup();
					throw SBF::DeserException(
						std::string("failed to deserialize table key #") 
							+ std::to_string(table_length) + ": " 
							+ se.what(), 
						type_name, 
						*begin
					);
				}

				auto key_chars = (char *)malloc(key_node->string_length + 1);
				std::memcpy(key_chars, key_node->string, key_node->string_length);
				key_chars[key_node->string_length] = '\0';

				keys_vec.push_back(key_chars);

				SBF_DestroyNode(key_node);

				// Deserialize value
				
				Node *value_node = nullptr;

				try {
					value_node = SBF_Deserialize(bytes, length, begin);
				} catch (SBF::SerdeException &se) {
					cleanup();
					throw SBF::DeserException(
						std::string("failed to deserialize table value #") 
							+ std::to_string(table_length) 
							+ ": " 
							+ se.what(), 
						type_name, 
						*begin
					);
				}
				
				values_vec.push_back(value_node);

				table_length++;
			}

			char **keys = (char **)malloc(sizeof(char *) * keys_vec.size());
			Node **values = (Node **)malloc(sizeof(Node *) * values_vec.size());

			std::memcpy(keys, keys_vec.data(), sizeof(char *) * keys_vec.size());
			std::memcpy(values, values_vec.data(), sizeof(Node *) * values_vec.size());

			node = SBF_CreateNode_Table(keys, values, table_length);
		}
		break;

	default: throw SBF::SerdeException(std::string("unknown tag '")+std::to_string(type_byte)+"'");
	}
	
	// If it's a table then type_size = 0 and array_length = 0!;
	*begin += type_size + (array_length * type_sizes[static_cast<int>(type) - 9]);

	if (*begin >= length)
		throw SBF::DeserException("bytes array too small", type_name, *begin);

	auto closing_byte = bytes[*begin];

	if ((uint8_t)(closing_byte * -1) != type_byte)
		throw SBF::DeserException(
			std::string("closing tag mismatch; expected ") 
				+ type_name
				+ ", but got "
				+ type_names[(uint8_t)(closing_byte * -1)], 
			type_name, 
			*begin
		);

	*begin = *begin + 1;

	return std::move(node);
}

void SBF_Serialize(const Node *node, uint8_t *bytes, size_t length, size_t *cursor) {
	if (!node) throw std::invalid_argument("node was null");

	auto size = SBF_CalculateSize(node);

	static const auto next = [&cursor](int bytes) {
		*cursor = *cursor + bytes;
	};

	if (length < size) throw std::invalid_argument(
		std::string("bytes array is too small; expected at least ")
			+ std::to_string(size)
			+ " bytes, but got instead "
			+ std::to_string(length)
		);

	uint8_t typeu = static_cast<uint8_t>(node->type);

	using Tag = SBF::TagType;

	switch (node->type) {
	case NodeType_I8: 
		bytes[*cursor] = (uint8_t) Tag::Open_I8;
		next(1);
		bytes[*cursor] = node->i8;
		next(1);
		bytes[*cursor] = (uint8_t) Tag::Close_I8;
		break;

	case NodeType_U8: 
		bytes[*cursor] = (uint8_t) Tag::Open_U8;
		next(1);
		bytes[*cursor] = node->u8; 
		next(1);
		bytes[*cursor] = (uint8_t) Tag::Close_U8;
		break;

	case NodeType_I32: 
		bytes[*cursor] = (uint8_t) Tag::Open_I32;
		next(1);
		SBF::Write<int32_t>(bytes + *cursor, node->i32); 
		next(sizeof(int32_t));
		bytes[*cursor] = (uint8_t) Tag::Close_I32;
		break;

	case NodeType_U32: 
		bytes[*cursor] = (uint8_t) Tag::Open_U32;
		next(1);
		SBF::Write<uint32_t>(bytes + *cursor, node->u32); 
		next(sizeof(uint32_t));
		bytes[*cursor] = (uint8_t) Tag::Close_U32;
		break;

	case NodeType_I64: 
		bytes[*cursor] = (uint8_t) Tag::Open_I64;
		next(1);
		SBF::Write<int64_t>(bytes + *cursor, node->i64); 
		next(sizeof(int64_t));
		bytes[*cursor] = (uint8_t) Tag::Close_I64;
		break;
	
	case NodeType_U64: 
		bytes[*cursor] = (uint8_t) Tag::Open_U64;
		next(1);
		SBF::Write<uint64_t>(bytes + *cursor, node->u64); 
		next(sizeof(uint64_t));
		bytes[*cursor] = (uint8_t) Tag::Close_U64;
		break;
	
	case NodeType_F32: 
		bytes[*cursor] = (uint8_t) Tag::Open_F32;
		next(1);
		SBF::Write<float>(bytes + *cursor, node->f32); 
		next(sizeof(float));
		bytes[*cursor] = (uint8_t) Tag::Close_F32;
		break;
	
	case NodeType_F64: 
		bytes[*cursor] = (uint8_t) Tag::Open_F64;
		next(1);
		SBF::Write<double>(bytes + *cursor, node->f64); 
		next(sizeof(double));
		bytes[*cursor] = (uint8_t) Tag::Close_F64;
		break;
	
	case NodeType_Char: 
		bytes[*cursor] = (uint8_t) Tag::Open_Char;
		next(1);
		bytes[*cursor] = node->c; 
		next(1);
		bytes[*cursor] = (uint8_t) Tag::Close_Char;
		break;

	case NodeType_I8A:
		bytes[*cursor] = (uint8_t) Tag::Open_I8_Array;
		next(1);
		SBF::Write<uint64_t>(bytes + *cursor, node->array_length);
		next(sizeof(uint64_t));
		std::memcpy(bytes + *cursor, (int8_t *)node->array, node->array_length);
		next(node->array_length);
		bytes[*cursor] = (uint8_t) Tag::Close_I8_Array;
		break;
	
	case NodeType_U8A:
		bytes[*cursor] = (uint8_t) Tag::Open_U8_Array;
		next(1);
		SBF::Write<uint64_t>(bytes + *cursor, node->array_length);
		next(sizeof(uint64_t));
		std::memcpy(bytes + *cursor, (uint8_t *)node->array, node->array_length);
		next(node->array_length);
		bytes[*cursor] = (uint8_t) Tag::Close_U8_Array;
		break;
	
	case NodeType_I32A:
		bytes[*cursor] = (uint8_t) Tag::Open_I32_Array;
		next(1);
		SBF::Write<uint64_t>(bytes + *cursor, node->array_length);
		next(sizeof(uint64_t));
		std::memcpy(bytes + *cursor, (int32_t *)node->array, node->array_length);
		next(node->array_length * sizeof(int32_t));
		bytes[*cursor] = (uint8_t) Tag::Close_I32_Array;
		break;
	
	case NodeType_U32A:
		bytes[*cursor] = (uint8_t) Tag::Open_U32_Array;
		next(1);
		SBF::Write<uint64_t>(bytes + *cursor, node->array_length);
		next(sizeof(uint64_t));
		std::memcpy(bytes + *cursor, (uint32_t *)node->array, node->array_length);
		next(node->array_length * sizeof(uint32_t));
		bytes[*cursor] = (uint8_t) Tag::Close_U32_Array;
		break;
	
	case NodeType_I64A:
		bytes[*cursor] = (uint8_t) Tag::Open_I64_Array;
		next(1);
		SBF::Write<uint64_t>(bytes + *cursor, node->array_length);
		next(sizeof(uint64_t));
		std::memcpy(bytes + *cursor, (int64_t *)node->array, node->array_length);
		next(node->array_length * sizeof(int64_t));
		bytes[*cursor] = (uint8_t) Tag::Close_I64_Array;
		break;
	
	case NodeType_U64A:
		bytes[*cursor] = (uint8_t) Tag::Open_U64_Array;
		next(1);
		SBF::Write<uint64_t>(bytes + *cursor, node->array_length);
		next(sizeof(uint64_t));
		std::memcpy(bytes + *cursor, (uint64_t *)node->array, node->array_length);
		next(node->array_length * sizeof(uint64_t));
		bytes[*cursor] = (uint8_t) Tag::Close_U64_Array;
		break;
	
	case NodeType_F32A:
		bytes[*cursor] = (uint8_t) Tag::Open_F32_Array;
		next(1);
		SBF::Write<uint64_t>(bytes + *cursor, node->array_length);
		next(sizeof(uint64_t));
		std::memcpy(bytes + *cursor, (float *)node->array, node->array_length);
		next(node->array_length * sizeof(float));
		bytes[*cursor] = (uint8_t) Tag::Close_F32_Array;
		break;
	
	case NodeType_F64A:
		bytes[*cursor] = (uint8_t) Tag::Open_F64_Array;
		next(1);
		SBF::Write<uint64_t>(bytes + *cursor, node->array_length);
		next(sizeof(uint64_t));
		std::memcpy(bytes + *cursor, (double *)node->array, node->array_length);
		next(node->array_length * sizeof(double));
		bytes[*cursor] = (uint8_t) Tag::Close_F64_Array;
		break;
	
	case NodeType_String:
		bytes[*cursor] = (uint8_t) Tag::Open_String;
		next(1);
		SBF::Write<uint64_t>(bytes + *cursor, node->string_length);
		next(sizeof(uint64_t));
		std::memcpy(bytes + *cursor, (char *)node->string, node->string_length);
		next(node->string_length);
		bytes[*cursor] = (uint8_t) Tag::Close_String;
		break;

	case NodeType_T:
		bytes[*cursor] = (uint8_t) Tag::Open_Table;
		
		next(1);
		
		for (auto i = 0; i < node->table_length; i++) {
			// key
			auto key_len = std::strlen(node->keys[i]);
			
			bytes[*cursor] = (uint8_t) Tag::Open_String;
			next(1);
			SBF::Write<uint64_t>(bytes + *cursor, key_len);
			next(sizeof(uint64_t));
			std::memcpy(bytes + *cursor, node->keys[i], key_len);
			next(key_len);
			bytes[*cursor] = (uint8_t) Tag::Close_String;

			// Value
			next(1);
			SBF_Serialize(node->values[i], bytes, length, cursor);
		
			// SBF_Serialize stops at an offset of one byte
			// |0|0|0|0|0|0|0|0
			// | .. bytes .. |^ stops here
		}

		bytes[*cursor] = (uint8_t) (Tag::Close_Table);
		break;

	default: throw std::invalid_argument(std::string("invalid node type '") + std::to_string(typeu) + "'");
	}

	next(1);

	// If the size of the node matches with the length of the bytes array,
	// then the function stops one byte beyond the boundries of the array.
}

size_t SBF_CalculateSize(const Node *node) {
	static const int8_t sizes[] = {
		0,

		4, 8, 4, 8, 1,
		4, 8,       1,
		1,
		8, 8, 8, 8, 8,
		8, 8,       8,
		8,
		0
	};

	int8_t typei = static_cast<int8_t>(node->type);

	// Single nodes
	if (typei > 0 && typei < 10) 
		return sizes[typei] + 2; // Account for the opening and closing tags.
	
	// Array nodes
	if (typei > 9 && typei < 18) 
		// Array size (length byes) + array length * array element size + opening & closing tags.
		return sizes[typei] + node->array_length * sizes[typei - 9] + 2;
	
	// String nodes (same as array nodes)
	if (typei == 18) 
		// String length bytes + string length + opening & closing tags.
		return sizes[typei] + node->string_length + 2;

	// Table nodes
	size_t table_length = 2;

	for (auto e = 0; e < node->table_length; e++) {
		table_length += std::strlen(node->keys[e]) + sizeof(uint64_t) + 2;
		table_length += SBF_CalculateSize(node->values[e]); // Recursive call!
	}

	return table_length;
}

void SBF_WriteFile(const char *filepath, const Node *node) {
	if (!filepath) throw std::invalid_argument("file path argument must not be null");
	if (!node) throw std::invalid_argument("node pointer argument must not be null");	

	std::ofstream file(filepath, std::ios::binary);

	if (!file.is_open()) {
		throw std::runtime_error("failed to open file");
	}

	auto node_size = SBF_CalculateSize(node);
	
	auto bytes = (uint8_t *)malloc(1 + node_size);
	bytes[0] = 1; // Version

	size_t cursor = 1;
	SBF_Serialize(node, bytes, node_size, &cursor);

	file.write(reinterpret_cast<char *>(bytes), node_size + 1);

	file.close();

	free(bytes);
}

Node *SBF_ReadFile(const char *filepath, uint8_t *version) {
	if (!filepath) throw std::invalid_argument("file path argument must not be null");
	std::filesystem::path path(filepath);
	
	auto bytes = SBF::ReadFileAsBytes(path);

	if (bytes.size() < 3) {
		if (version) *version = 0;
		return nullptr;
	}

	// Ineffective for now.
	if (version) *version = bytes[0];

	size_t cursor = 0;
	return SBF_Deserialize(bytes.data() + 1, bytes.size() - 1, &cursor);
}

