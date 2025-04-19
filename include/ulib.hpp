#ifndef ULIB_HPP
#define ULIB_HPP

#include <iostream>
#include <string>
#include <filesystem>
#include <openssl/sha.h>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <vector>
#include <chrono>
#include <zlib.h>

namespace upl {
class Upp {
public:
    Upp();
    ~Upp();

    void init_repo(const std::string& name);
    void status();
    void add(const std::vector<std::string>& filepaths);
    void commit(const std::string& msg);
    void log();
    void branch(const std::string& name);
    void checkout(const std::string& branch_name);

private:
    void compress_blob(std::string &header, std::string &file_path);
    void decompress_blob(std::string &file_path);
    std::string create_blob(const std::string &file_path);
    void update_index(const std::string& file_path, const std::string& hash);
    void write_file(std::string &file_path, const std::vector<unsigned char>& data);

    std::string read_file(std::string &file_path);
    sdt::string create_tree();
    std::filesystem::path repo_path;
    std::string current_branch;
    std::vector<std::string> staging_area;
    
    std::string hash_function(const std::string &content);

    bool is_repo_initialized() const;
    bool load_config();
    bool save_config() const;
};
}

#endif
