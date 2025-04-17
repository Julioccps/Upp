#ifndef ULIB_HPP
#define ULIB_HPP

#include <iostream>
#include <string>
namespace upl {
class Upp {
    public:
        Upp();  // Constructor
        ~Upp(); // Destructor
        // 

        // Functions
        void init_repo(std::string name);        // Initialize Repository named <name>
        void status();                           // Show tracked/untracked/modified files
        void add(std::string* filepaths[]);      // Stage files
        void commit(std::string msg);            // Creates commits
        void log();                              // Show commit history
        void branch(std::string name);           // Create a branch named <name>
        void checkout(std::string branch_name);  // Changes to branch <branch_name>

    }
}
