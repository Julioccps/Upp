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
    std::filesystem::path repo_path;
    std::string current_branch;
    std::vector<std::string> staging_area;

    std::string generate_commit_hash();

    bool is_repo_initialized() const;
    bool load_config();
    bool save_config() const;
};
}

#endif
