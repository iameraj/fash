#include <conio.h>
#include <windows.h>

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "fash.hpp"

// Checks if the path exists and is valid.
bool fash::path_exists(const std::string& path) {
	DWORD attr = GetFileAttributes(path.c_str());
	if (attr == INVALID_FILE_ATTRIBUTES) {
		// Path is invalid or does not exists
		return false;
	} else {
		return true;
	}
}

// replace forward slashes with backslashes
void fash::normalize_slashes(std::string& path) {
	std::string normalized = path;
	std::replace(path.begin(), path.end(), '/', '\\');
	return;
}

// minimzes path to its equivalent shorter string
std::string fash::minimize_path(const std::string& path) {
	std::vector<std::string> parts;
	std::istringstream ss(path);
	std::string token;

	// Split the path by '/' or '\'
	while (std::getline(ss, token, '\\')) {
		if (token == "" || token == ".") {
			continue;
		}
		if (token == "..") {
			if (!parts.empty() && parts.back() != "..") {
				parts.pop_back();
			} else {
				parts.push_back(token);
			}
		} else {
			parts.push_back(token);
		}
	}

	// Join the parts back into a minimized path
	std::ostringstream minimized_path;
	for (size_t i = 0; i < parts.size(); ++i) {
		if (i != 0) {
			minimized_path << "\\";
		}
		minimized_path << parts[i];
	}

	return minimized_path.str();
}

// prints out welcome message
int fash::welcome_msg(void) {
	std::cout << "Hello welcome to Fast Shell!\n" << std::endl;
	std::cout << "\tPress l to ls" << std::endl;
	std::cout << "\tPress f to cd" << std::endl;
	std::cout << "\tPress m to mkdir" << std::endl;
	std::cout << "\tPress r to rmdir" << std::endl;
	std::cout << "\tPress d to rm" << std::endl;
	std::cout << "\tPress c to clear" << std::endl;
	std::cout << "\tPress x to exit" << std::endl;
	return 0;
}

// prints out exit message
int fash::exit_msg(void) {
	std::cout << "\nBye!" << std::endl;
	return 0;
}

// runs a command in powershell
// TODO: instead of hardcoding powershell, I should set the shell according to
// the OS/platfrom in the session initialization and use it here
int fash::run_sh(void) {
	std::string command;
	std::cout << "[I]: ";
	std::cin >> command;
	this->normalize_slashes(command);
	system(("powershell -c " + command).c_str());
	return 0;
}

// List directories at current path
// TODO: implement listing of dirs other than current path
int fash::list_dir(void) {
	WIN32_FIND_DATA findFileData;
	HANDLE hFind =
		FindFirstFile((this->current_path + "\\*").c_str(), &findFileData);

	if (hFind == INVALID_HANDLE_VALUE) {
		std::cout << "Error: " << GetLastError() << std::endl;
		return 1;
	}
	do {
		std::cout << findFileData.cFileName << std::endl;
	} while (FindNextFile(hFind, &findFileData) != 0);

	FindClose(hFind);
	return 0;
}

// Changes sessions current path to the specified path. The path in input is
// taken as a relative path to the current path, so using full paths (paths
// starting from C:\ ) will give an error and not work as intended
int fash::change_dir(void) {
	// TODO: for now the path can be changed to a file, which ofcourse should
	// not happen
	// TODO: implement usage of full paths
	std::string new_path;
	std::cout << "[I]: ";
	std::cin >> new_path;
	this->normalize_slashes(new_path);
	if (fash::path_exists(this->current_path + "\\" + new_path)) {
		this->current_path += ("\\" + new_path);
		this->current_path = this->minimize_path(this->current_path);
		std::cout << "[O]: current path set to " << this->current_path
			<< std::endl;
	} else {
		std::cout << "Path does not exists" << std::endl;
	}

	return 0;
}

// Makes directory relative to the current path
int fash::make_dir(void) {
	std::string new_dir_name;
	std::cout << "[I]: ";
	std::getline(std::cin, new_dir_name);
	if (!new_dir_name.length()) {
		return 0;
	}
	std::string new_dir_path = this->current_path + "\\" + new_dir_name;

	std::cout << "Creating new dir: " << new_dir_name << std::endl
		<< "As: " << new_dir_path << std::endl;
	if (CreateDirectory((new_dir_path).c_str(), NULL)) {
		std::cout << "Dir created successfully!\n";
	} else {
		DWORD error = GetLastError();
		if (error == 183) {
			std::cout << "File with same name already exists";
		} else {
			std::cout << "Following error occured: " << std::endl;
		}
	}
	return 0;
};

// Removes direcotry relative to the current path
int fash::remove_dir(void) {
	std::string dir_name;
	std::cout << "[I]: ";
	std::getline(std::cin, dir_name);
	if (!dir_name.length()) {
		return 0;
	}
	std::string new_dir_path = this->current_path + "\\" + dir_name;

	std::cout << "Deleting new dir: " << dir_name << std::endl
		<< "At: " << new_dir_path << std::endl;

	if (RemoveDirectory((new_dir_path).c_str())) {
		std::cout << "Dir Deleted successfully!\n";
	} else {
		DWORD error = GetLastError();
		std::cout << "Following error occured: " << error;
	}
	return 0;
};

// Removes item relative to the current path
int fash::remove_item(void) {
	std::string file_name;
	std::cout << "[I]: ";
	std::getline(std::cin, file_name);
	if (!file_name.length()) {
		return 0;
	}
	std::string file_path = this->current_path + "\\" + file_name;

	std::cout << "Deleting file: " << file_name << std::endl
		<< "At: " << file_path << std::endl;
	if (DeleteFile((file_path).c_str())) {
		std::cout << " file Deleted successfully!\n";
	} else {
		DWORD error = GetLastError();
		std::cout << "Following error occured: " << error;
	}
	return 0;
};

// Copies item relative to the current path
int fash::copy_item(void) {
	std::string old_file_path;
	std::cout << "[I]: ";
	std::getline(std::cin, old_file_path);
	if (!old_file_path.length()) {
		return 0;
	}
	std::string new_file_path;
	std::cout << "[I]: ";
	std::getline(std::cin, new_file_path);
	if (!new_file_path.length()) {
		return 0;
	}

	if (CopyFile((this->current_path + "\\" + old_file_path).c_str(),
				(this->current_path + "\\" + new_file_path).c_str(), FALSE)) {
		std::cout << "File copied successfully from" << std::endl;
		return 0;
	} else {
		DWORD error = GetLastError();
		std::cout << "Following error occurred\n" << error << std::endl;
		return 1;
	}
};
// TODO: Nearly all of the function do not support usage of full paths.
// Currently, we have one function, fash::path_exists, that could help in this
// we can check if a path exists in itself, without appending it to the current
// path, to check if its a full path, otherwise its a relative path and we can
// append it to current path

int fash::clear_scr(void) {
	system("cls");
	return 0;
}
