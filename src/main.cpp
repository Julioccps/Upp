#include <iostream>
#include <string>
#include "ulib.hpp"

namespace fs = std::filesystem;

int main(int argc, char* argv[]){
    upl::Upp upp;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--help"){
            std::cout << "Usage:\n"
                      << "\033[31minit\033[0m \033[1mInitializes repository\033[0m"
                      << "\033[31status\033[0m \033[1mShows tracked/untracked/modified files\033[0m"
                      << "\033[31madd\033[0m <file> \033[1mStage file(s)| use '.' for all\033[0m"
                      << "\033[31mcommit -m\033[0m \"msg\" \033[1mCreate commits with \"msg\"\033[0m"
                      << "\033[31mlog\033[0m \033[1mShow commit history\033[0m"
                      << "\033[31mbranch\033[0m <name> \033[1mCreates a branch named <name>\033[0m"
                      << "\033[31mcheckout\033[0m <branch> \033[1mChanges current branch to <branch>\033[0m";
        }

        else if (arg == "init"){
            upp.init_repo();
        }

        else if (arg == "status"){
            upp.status();
        }

        else if (arg == "add"){
            std::vector<std::string> file_paths;
            if (argv[i+1] != "." && argv[i+1] != "*"){
                std::istringstream iss(argv[i+1]);
                while (iss >> file){
                    file_paths.push_back(file);
                }
            }
            else {
                for (const auto& entry : fs::directory_iterator(fs::current_path())) {
                    if (fs::is_regular_file(entry.status())) {
                        std::string nome = entry.path().filename().string();
                        if (nome != ".upl") {
                            file_paths.push_back(nome);
                        }
                    }
                }
            }
            upp.add(file_paths);
            i++;
        }

    }
    return 0;
}
