#include "ulibs/ulib.hpp"

namespace upl {

    Upp::Upp(): repo_path(""), current_branch(""){
        // Load info from a config file, if exists
    }
    void Upp::init_repo(const std::string& name){
        repo_path = std::filesystem::current_path() / ".upl";

        if (std::filesystem::exists(repo_path)){
            throw std::runtime_error("Repository already initialized here.\n")
        }

        std::filesystem::create_directory(repo_path);
        std::filesystem::create_directory(repo_path / "commits");
        std::filesystem::create_directory(repo_path / "branches");
        std::filesystem::create_directory(repo_path / "staging");
        
        std::ofstream config(repo_path / "config");
        config << "repo_name=" << name << "\n";
        config << "current_branch=main\n";
        config.close();
        
        std::ofstream index_file(repo_path / "index");
        index_file.close();

        current_branch = "main";

        std::cout << "Repository << name << "initialized with success.\n";
    }

    void Upp::status(){ 
    }

    void Upp::add(const std::vector<std::string>& filepaths){
    }

    void Upp::commit(const std::string& msg){
    }

    void Upp::log(){
    }

    void Upp::branch(const std::string& name){
    }

    void Upp::checkouut(const std::string& branch_name){
    }
    
    std::string Upp::generate_commit_hash(){
        auto now = std::chrono::system_clock::now();
        auto now_time = std::chrono::system_clock::to_time_t(now);
        std::string data = std::to_string(now_time);
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(), hash);
        std::stringstream ss;
        for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i){
            ss << std::hex << (int)hash[i];
        }
        return ss.str();
    }

    bool Upp::is_repo_initialized(){
    }

    bool Upp::load_config(){
    }

    bool Upp::save-conig(){
    }

}
