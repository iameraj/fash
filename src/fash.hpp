#include <string>
#include <iostream>
// FaSh - Fast Shell parent class for each session
class fash {
	private:
		// Current path of the session. starts with the parent directory.
		std::string current_path;

		// Every function is a implementation of the respective commands. the
		// function here should not take any arguments.They have to ask for
		// necessary arguments themselves. They shall return a exit status as [int]
		// helper function will be static and flexible according to the needs
	public:
		// provide the starting path to the shell
		fash(std::string path) {
			this->current_path = path;
			std::cout << "...initializing fash at - " << path << std::endl;
			system("pause");
			system("cls");
		};
		int welcome_msg(void);
		int exit_msg(void);
		int echo(void);
		int run_sh(void);
		int list_dir(void);
		int change_dir(void);
		int make_dir(void);
		int remove_dir(void);
		int remove_item(void);
		int copy_item(void);
		int clear_scr(void);

		static bool path_exists(const std::string& path);
		static void normalize_slashes(std::string& path);
		static std::string minimize_path(const std::string& path);
};
