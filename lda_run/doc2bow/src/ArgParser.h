/*
 * ArgParser.h
 *
 *  Created on: Jul 11, 2017
 *      Author: tim
 */

#ifndef ARGPARSER_H_
#define ARGPARSER_H_
#include <unordered_map>
#include <string>
#include <experimental/string_view>
class ArgParser {
public:
	using str_iter_t = std::string::const_iterator;
	using strview_t = std::experimental::string_view;
	ArgParser() = default;
	ArgParser(int argc, const char ** argv);
	void parse_args(int argc, const char ** argv);
	std::pair<bool, strview_t> operator[](const std::string & arg) const;
	std::pair<bool, strview_t> operator[](char arg) const;
	template <class F>
	void get_arg(const std::string & argname, char argsymb, F & argval_action, bool error_if_both = true) const
	{
		bool has_name = arg_words_.count(argname);
		bool has_symb = arg_symbs_.count(argsymb);
		if(has_name)
		{
			if(has_symb)
			{
				if(error_if_both)
					throw std::runtime_error("Conflicting values provided for both '" + argname + "' and '" + argsymb + "'. (provide only one)");
				else
				{
					argval_action(arg_words_.at(argname));
					argval_action(arg_symbs_.at(argsymb));
				}
			}
			else
			{
				argval_action(arg_words_.at(argname));
			}
		}
		else if(has_symb)
		{
			argval_action(arg_symbs_.at(argsymb));
		}
	}
	template <class F>
	void get_arg(char argsymb, F & argval_action, bool error_if_both = true) const
	{
		get_arg("", argsymb, argval_action, error_if_both);
	}
	template <class F>
	void get_arg(const std::string & argname, F & argval_action, bool error_if_both = true) const
	{
		get_arg(argname, '\0', argval_action, error_if_both);
	}
	void printargs() const;
private:
	str_iter_t split_arg(str_iter_t begin, str_iter_t end) const;
	std::unordered_map<std::string, std::string> arg_words_;
	std::unordered_map<char, std::string>  arg_symbs_;
};

#endif /* ARGPARSER_H_ */
