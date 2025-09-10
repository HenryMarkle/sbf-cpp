#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <exception>

namespace SBF {

class SerdeException : public std::exception {

	const std::string _msg;

public:
	inline explicit SerdeException(const std::string msg) : _msg(msg) {}
	inline explicit SerdeException(const char *msg) : _msg(msg) {}

	inline const char *what() const noexcept override { return _msg.c_str(); }

};

class DeserException : public SerdeException {

public:

	size_t position; 
	const char *tag;

	inline explicit DeserException(const std::string msg, const char *tag,  size_t position) : SerdeException(std::string("[T: ") + tag + "] [B: "+std::to_string(position)+"]: "+msg) {
		this->position = position;
		this->tag = tag;
	}

};

};
