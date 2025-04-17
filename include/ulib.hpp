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
};
}

#endif
