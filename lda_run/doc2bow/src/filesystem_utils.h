/*
 * filesystem_utils.h
 *
 *  Created on: Jul 7, 2017
 *      Author: tim
 */

#ifndef FILESYSTEM_UTILS_H_
#define FILESYSTEM_UTILS_H_


#include <experimental/filesystem>
#include <string>
#include <fstream>
#include <iostream>
#include <set>
namespace fs = std::experimental::filesystem;


template <class OutputIt>
void get_directory_files_impl(const fs::path & dirpath, OutputIt & dest)
{
	std::string dirname = dirpath.native() + (dirpath.native().back() == '/' ? "" : "/");
	using dir_iter_t = fs::directory_iterator;
	fs::directory_entry ignore_ent(dirname + ".ignore");
	std::set<std::string> ignore{".ignore"};
	fs::file_status status = ignore_ent.status();
	if(fs::exists(status) and fs::is_regular_file(status))
	{
		std::ifstream ignore_file(dirname + ".ignore");
		std::string fname;
		while(std::getline(ignore_file, fname))
		{
			ignore.insert(dirname + fname);
			std::cout << "IGNORE:  " << dirname + fname << '\n';
		}
		ignore.insert(ignore_ent.path().native());
		std::cout << "IGNORE:  " << ignore_ent.path().native() << '\n';
	}
	auto iter = dir_iter_t(dirpath);

	for(auto entity:iter)
	{
		status = entity.status();
		if(ignore.count(entity.path().native()))
		{
			continue;
		}
		else if(fs::is_directory(status) and not fs::is_symlink(status))
		{
			get_directory_files_impl(entity.path(), dest);
		}
		else if(fs::is_regular_file(status))
		{
			(*dest) = entity.path().native();
			++dest;
		}
	}

}
template <class OutputIt>
void get_directory_files(const fs::path & dirpath, OutputIt dest)
{
	return get_directory_files_impl(dirpath, dest);
}


#endif /* FILESYSTEM_UTILS_H_ */
