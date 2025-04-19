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
        std::map<std::string, std::string> index_entries;

        if (std::filesystem::exists(".upp/index")) {
            std::ifstream index_file(".upp/index");
            std::string line;
            while (std::getline(index_file, line)) {
                std::istringstream iss(line);
                std::string mode, type, hash, path;
                iss >> mode >> type >> hash >> path;
                index_entries[path] = hash;
            }
        }

        for (const auto& file_path : file_paths) {
            std::string hash = create_blob(file_path);

            if (index_entries[file_path] != hash) {
                index_entries[file_path] = hash;
            }
        }
        std::ofstream index_file(".upp/index");
        for (const auto& [path, hash] : index_entries) {
            index_file << "100644 blob " << hash << " " << path << "\n";
        }    
    }

    void Upp::commit(const std::string& message){
        std::string tree_hash = create_tree(const std::string& message);

        std::string parent_hash;
        if (std::filesystem::exists(".upp/commits")) {
            parent_hash = read_file(".upp/commits");
        }

        std::string author = get_author();

        std::string commit_hash = create_commit(tree_hash, parent_hash, author, message);

        write_file(".upp/commits", commit_hash);
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

    std::string Upp::create_tree(){
        std::ifstream index_file(".upp/index");
    	if (!index_file)
        	throw std::runtime_error("It was not possible to open the index");

    	std::ostringstream tree_content;
    	std::string line;

    	while (std::getline(index_file, line)) {
        	std::istringstream iss(line);
        	std::string mode, type, hash, path;
        	iss >> mode >> type >> hash >> path;

        	tree_content << mode << " " << type << " " << hash << "\t" << path << "\n";
    	}

    	std::string content = tree_content.str();
    	std::string header = "tree " + std::to_string(content.size()) + '\0';
    	std::string full_tree = header + content;

    	std::string tree_hash = hash_function(full_tree);

    	std::string object_dir = ".upp/objects/" + tree_hash.substr(0, 2);
    	std::string object_path = object_dir + "/" + tree_hash.substr(2);

    	if (std::filesystem::exists(object_path))
        	return tree_hash;

    	std::filesystem::create_directories(object_dir);

    	uLong sourceLen = full_tree.size();
    	uLong destLen = compressBound(sourceLen);
    	std::vector<Bytef> compressed(destLen);

    	int res = compress(compressed.data(), &destLen,
                       reinterpret_cast<const Bytef*>(full_tree.data()), sourceLen);

    	if (res != Z_OK)
        	throw std::runtime_error("Compression error");

	    compressed.resize(destLen);

	    write_file(object_path, compressed);

    	return tree_hash;
    }

    std::string Upp::create_commit(const std::string tree_hash,
                                   const std::string parent_hash, 
                                   const std::string author, 
                                   const std::string message){

        std::ostringstream commit_content;
    	commit_content << "tree " << tree_hash << "\n";

    	if (!parent_hash.empty())
        	commit_content << "parent " << parent_hash << "\n";

    	commit_content << "author " << author << "\n";

    	std::time_t now = std::time(nullptr);
    	commit_content << "date " << now << "\n\n";

    	commit_content << message << "\n";

    	std::string content = commit_content.str();
    	std::string header = "commit " + std::to_string(content.size()) + '\0';
    	std::string full_commit = header + content;

    	std::string commit_hash = hash_function(full_commit);

    	std::string object_dir = ".upp/objects/" + commit_hash.substr(0, 2);
    	std::string object_path = object_dir + "/" + commit_hash.substr(2);

    	if (std::filesystem::exists(object_path))
        	return commit_hash;

    	std::filesystem::create_directories(object_dir);

    	uLong sourceLen = full_commit.size();
    	uLong destLen = compressBound(sourceLen);
    	std::vector<Bytef> compressed(destLen);

    	int res = compress(compressed.data(), &destLen,
        	               reinterpret_cast<const Bytef*>(full_commit.data()), sourceLen);

    	if (res != Z_OK)
        	throw std::runtime_error("Compression error");

    	compressed.resize(destLen);

    	write_file(object_path, compressed);

    	return commit_hash;
    }

    std::string Upp::create_blob(const std::string &file_path){
        
        std::string content = read_file(file_path);

        std::string blob_content = "blob " + std::to_string(content.size()) + '\0';
        std::string full_content = header + content;

        std::string hash = hash_function(blob_content);
            
        std::string object_dir = ".upp/objects/" + hash.substr(0, 2);
        std::string object_path = object_dir + "/" + hash.substr(2);
        
        if (std::filesystem::exists(object_path))
            return hash;

        uLong sourceLen = full_content.size();
        uLong destLen = compressBound(sourceLen);
        std::vector<Bytef> compressed(destLen);

        int res = compress(compressed.data(), &destLen,
                       reinterpret_cast<const Bytef*>(full_content.data()), sourceLen);
        if (res != Z_OK) throw std::runtime_error("Compression error");

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

    void Upp::set_author(const std::string& author){
        std::ofstream config(".upp/config");
        if (!config)
            throw std::runtime_error("Could not open .upp/config");

        config << "author=" << author << "\n";
    }
    
    std::string Upp::get_author(){
        std::ifstream config_file(".upp/config");
        if (!config_file)
            return "unknown <unknown@localhost>";

        std::string line;
        while (std::getline(config_file, line)) {
            if (line.rfind("author=", 0) == 0)
                return line.substr(7);
        }
        return "unknown <unknown@localhost>";
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
