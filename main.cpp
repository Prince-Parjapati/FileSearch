#include "file_search.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    // Check command line arguments
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <directory_path> <search_pattern> [num_threads]" << std::endl;
        return 1;
    }

    std::string directory_path = argv[1];
    std::string pattern = argv[2];
    
    // Default to 4 threads if not specified
    int num_threads = (argc >= 4) ? std::stoi(argv[3]) : 4;
    
    std::cout << "Starting search in: " << directory_path << std::endl;
    std::cout << "Searching for files containing: " << pattern << std::endl;
    std::cout << "Using " << num_threads << " threads" << std::endl;
    
    // Start the multithreaded search
    start_multithreaded_search(directory_path, pattern, num_threads);
    
    std::cout << "Search completed!" << std::endl;
    return 0;
}
