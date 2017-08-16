/*
 * ioutils.cpp
 *
 *  Created on: Jul 11, 2017
 *      Author: tim
 */

#include "ioutils.h"
#include <tuple>


static bool is_BE()
{
	volatile union endianness_check_ {
		uint_least32_t i;
		char bytes[sizeof(uint_least32_t)];
	} checker;
	checker.i = 0x01020304;
	return (checker.bytes[0] == 1);
}
static const volatile bool sys_is_be = is_BE();
bool is_big_endian()
{
	return sys_is_be;
}
std::tuple<bool, bool, size_t> string_to_type(const std::string & str)
{
	bool issigned = str.front() == 'i';
	bool isintegral = issigned or (str.front() == 'u');
	size_t bitcount = std::atoi(str.c_str() + 1);
	return std::make_tuple(isintegral, issigned, bitcount);
}
