/*
 * ioutils.h
 *
 *  Created on: Jul 11, 2017
 *      Author: tim
 */

#ifndef IOUTILS_H_
#define IOUTILS_H_

#include <utility>
#include <climits>
#include <iostream>
#include <cassert>
#include <algorithm>
constexpr bool compile_time_is_big_endian()
{
    return (0xFFFFFFFF & 1) == 0x01000000;
}

bool is_big_endian();

template <class T>
constexpr std::string type_to_string()
{
	constexpr bool isint = std::is_integral<T>::value;
	constexpr bool isfp = std::is_floating_point<T>::value;
	static_assert((isint or isfp), "Type 'T' in type_to_string<T>() must be either integral or floating-point.");
	if /* constexpr */ (isint)
	{
		return (std::is_signed<T>::value ? "i" : "u") + std::to_string(sizeof(T) * CHAR_BIT);
	}
	else /* if(isfp) */
	{
		return 'f' + std::to_string(sizeof(T) * CHAR_BIT);
	}
}
std::tuple<bool, bool, size_t> string_to_type(const std::string & str);



template <class T>
void read_as_bytes(std::istream & strm, T & value, size_t count = sizeof(T), char filler = 0)
{
	assert(count <= sizeof(T));
	char * bytes = reinterpret_cast<char*>(&value);
	strm.read(bytes, count);
	std::fill(bytes + count, bytes + sizeof(T), filler);
}
template <class T>
void read_as_bytes_reverse(std::istream & strm, T & value, size_t count = sizeof(T), char filler = 0)
{
	assert(count <= sizeof(T));
	char * bytes = reinterpret_cast<char*>(&value);
	std::fill(bytes, bytes + sizeof(T) - count, filler);
	strm.read(bytes + sizeof(T) - count, count);
	std::reverse(bytes + sizeof(T) - count, bytes + sizeof(T));
}

template <class T>
void change_endianness(T & value)
{
	if /* constexpr */(sizeof(T) > 1 and std::is_integral<T>::value)
	{
		std::reverse(reinterpret_cast<char*>(&value), reinterpret_cast<char*>(&value) + sizeof(value));
	}
}

template <class T>
void write_as_bytes(std::ostream & strm, const T & value)
{
	if /* constexpr */ (sizeof(T) > 1)
	{
		const char * as_bytes = reinterpret_cast<const char *>(&value);
		strm.write(as_bytes, sizeof(T));
	}
	else
	{
		strm.put(static_cast<char>(value));
	}

}
template <size_t I>
static void write_reverse_helper(std::ostream & strm, const char * bytes)
{
	// (strm.put(*(bytes + I)), ...);
	strm.put(*(bytes + I));
	write_reverse_helper<I - 1>(strm, bytes);
}
template <>
void write_reverse_helper<0>(std::ostream & strm, const char * bytes)
{
	// (strm.put(*(bytes + I)), ...);
	strm.put(*bytes);
}
template <class T>
void write_as_bytes_reverse(std::ostream & strm, const T & value)
{
	if /* constexpr */(sizeof(T) > 1)
	{
		const char * as_bytes = reinterpret_cast<const char *>(&value);
		write_reverse_helper<sizeof(T) - 1>(strm, as_bytes);//, std::make_index_sequence<sizeof(T)>);
	}
	else
	{
		strm.put(static_cast<char>(value));
	}

}


#endif /* IOUTILS_H_ */
