/*
 * ArgParser.cpp
 *
 *  Created on: Jul 11, 2017
 *      Author: tim
 */

#include "ArgParser.h"
#include <iostream>
ArgParser::ArgParser(int argc, const char ** argv)
{
	parse_args(argc, argv);
}
void ArgParser::parse_args(int argc, const char ** argv)
{
	std::string arg;
	for(int i = 0; i < argc; ++i)
	{
		arg.assign(argv[i]);
		if(arg.size() < 2 or arg[0] != '-')
		{
			throw std::runtime_error("Argument not understood. (too short or not preceded by a '-' dash)\n");
		}
		else if(arg[1] == '-')
		{
			auto pos = split_arg(arg.cbegin() + 2, arg.cend());
			arg_words_.emplace(std::string(arg.cbegin() + 2, pos), std::string(pos + (pos != arg.cend()), arg.cend()));
		}
		else
		{
			if(arg.size() > 2)
			{
				if(arg[2] == '=')
				{
					if(arg.size() == 3)
					{
						throw std::runtime_error("Provided trailing '=' assignment with no right-hand side.");
					}
					arg_symbs_.emplace(arg[1], std::string(arg.cbegin() + 3, arg.cend()));
				}
				else
				{
					for(auto iter = arg.cbegin() + 1; iter < arg.cend(); ++iter)
					{
						arg_symbs_.emplace(*iter, "");
					}
				}
			}
			else
			{
				arg_symbs_.emplace(arg[1], "");
			}

		}
	}
}
ArgParser::str_iter_t ArgParser::split_arg(str_iter_t begin, str_iter_t end) const
{

	while(begin < end and *begin != '=')
	{
		++begin;
	}
	if(begin < end and std::distance(begin, end) == 1)
	{
		throw std::runtime_error("Provided trailing '=' assignment with no right-hand side.");
	}

	return begin;
}

std::pair<bool, ArgParser::strview_t> ArgParser::operator[](const std::string & arg) const
{
	if(arg.size() > 1 and arg_words_.count(arg))
	{
		const std::string & value = arg_words_.at(arg);
		return {true, {value.c_str(), value.size()}};
	}
	else if(arg.size() == 1 and arg_symbs_.count(arg.front()))
	{
		const std::string & value = arg_symbs_.at(arg.front());
		return {true, {value.c_str(), value.size()}};
	}
	else
	{
		return {false, ""};
	}
}
std::pair<bool, ArgParser::strview_t> ArgParser::operator[](char arg) const
{
	if(arg_symbs_.count(arg))
	{
		const std::string & value = arg_symbs_.at(arg);
		return {true, {value.c_str(), value.size()}};
	}
	else
	{
		return {false, ""};
	}
}
void ArgParser::printargs() const
{
	for(const auto & arg:arg_symbs_)
	{
		std::cout << arg.first << " = " << arg.second << std::endl;
	}
	for(const auto & arg:arg_words_)
	{
		std::cout << arg.first << " = " << arg.second << std::endl;
	}
}

