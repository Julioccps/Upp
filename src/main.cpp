#include <iostream>
#include <string>
#include <ulib.hpp>

namespace fs = std::filesystem;

int main(int argc, char* argv[]){
    upl::Upp upp;
    for (int i = 1; i< argc; ++i) {
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
            if (argc == i){
                std::cerr << "Error: no file path given" << std::endl;
                return -1;
            }
            std::vector<std::string> file_paths;
            if (argv[i+1] != "." && argv[i+1] != "*"){
                std::istringstream iss(argv[i+1]);
                std::string file;
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
        else if (arg == "commit"){
            std::string narg = argv[i + 1];
            if ((argc - i) < 1){
                std::cerr << "Error: commit need -m and or -F to work" << std::endl;
                return -1;
            }
            else if (narg != "-m" || narg != "-F" || (i - argc) < 2){
                std::cerr << "Error: arguments not reconized or incomplete" << std::endl;
                return -1;
            }
            for (int j = i + 1; j < argc; j++){
                std::string aarg = argv[j];
                if (narg == "-m"){
                    upp.commit(aarg);
                    return 0;
                }
                else if (narg == "-F"){
                    std::vector<std::string> fp = {aarg};
                    upp.add(fp);
                }
            }
        }
        else if (arg == "log"){
            upp.log();
        }

    }
    return 0;
}
