#include "ulibs/ulib.hpp"

namespace upl {

    Upp::Upp(): repo_path(""), current_branch(""){
        // Load info from a config file, if exists
    }
    void Upp::init_repo(const std::string& name){
        repo_path = std::filesystem::current_path() / ".upl";

        if (is_repo_initialized()){
            throw std::runtime_error("Repository already initialized here.\n")
        }

        std::filesystem::create_directory(repo_path);
        std::filesystem::create_directory(repo_path / "commits");
        std::filesystem::create_directory(repo_path / "branches");
        std::filesystem::create_directory(repo_path / "objects");
        
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
        for (const auto& path: filepaths){
            std::string hash = create_blob(path);
            update_index(path, hash);
        }
    }

    void Upp::commit(const std::string& msg){
    }

    void Upp::log(){
    }

    void Upp::branch(const std::string& name){
    }

    void Upp::checkouut(const std::string& branch_name){
    }
    
    void Upp::compress_blob(std::string& header, std::string &file_path){
    }

    void Upp::decompress_blob(std::string &file_path){
    }

    std::string Upp::create_blob(const std::string &file_path){
        
        std::string content = read_file(file_path);

        std::string blob_content = "blob " + std::to_string(content.size()) + '\0';
        std::string full_content = header + content;

        std::string hash = hash_function(blob_content);

        uLong sourceLen = full_content.size();
        uLong destLen = compressBound(sourceLen);
        std::vector<Bytef> compressed(destLen);

        int res = compress(compressed.data(), &destLen,
                       reinterpret_cast<const Bytef*>(full_content.data()), sourceLen);
        if (res != Z_OK) throw std::runtime_error("Erro na compressão");

        compressed.resize(destLen);

        std::string object_dir = ".upp/objects/" + hash.substr(0, 2);
        std::filesystem::create_directories(object_dir);

        std::string object_path = object_dir + "/" + hash.substr(2);

        write_file(object_path, compressed);

        return hash;
    }

    void update_index(const std::string& file_path, const std::string& hash){
        std::ofstream index_file(".upp/index", std::ios::app);
        index_file << hash << " " << file_path << "\n";
    }

    void Upp::write_file(std::string &file_path, const std::vector<unsigned char>& data){
        std::ofstream out(path, std::ios::binary);
        out.write(reinterpret_cast<const char*>(data.data()), data.size());
    }

    std::string Upp::read_file(std::string &file_path){
        std::ifstream in(file_path, std::ios::binary)
        std::ostringstream ss;
        ss << in.rdbuf();

        returnn ss.str();

    }

    std::string Upp::hash_function(const std::string content){

        auto now = std::chrono::system_clock::now();
        auto now_time = std::chrono::system_clock::to_time_t(now);

        std::string data = std::to_string(now_time);
        data.append(content);

        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(), hash);
        std::stringstream ss;

        for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i){
            ss << std::hex << (int)hash[i];
        }
        return ss.str();
    }

    bool Upp::is_repo_initialized(){
        bool q = false;
        
        if (std::filesystem::exists("/.npl"))
            q = true;
        
        return q
    }

    bool Upp::load_config(){
    }

    bool Upp::save-conig(){
    }

}
