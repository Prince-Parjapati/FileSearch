#include "file_search.h"
#include <dirent.h>
#include <sys/stat.h>
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <mutex>
#include <thread> 

std::queue<std::string> directory_queue;
std::mutex queue_mutex;
std::mutex log_mutex;

std::string search_pattern;

bool match_pattern(const std::string& filename, const std::string& pattern) {
    return filename.find(pattern) != std::string::npos;
}

void search_thread() {
    while (true) {
        std::string dir;

        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            if (directory_queue.empty()) return;  
            dir = directory_queue.front();
            directory_queue.pop();
        }

        DIR* dp = opendir(dir.c_str());
        if (!dp) continue;

        struct dirent* entry;
        while ((entry = readdir(dp)) != nullptr) {
            std::string name = entry->d_name;
            if (name == "." || name == "..") continue;

            std::string full_path = dir + "/" + name;

            struct stat st;
            if (stat(full_path.c_str(), &st) == -1) continue;

            if (S_ISDIR(st.st_mode)) {
                std::lock_guard<std::mutex> lock(queue_mutex);
                directory_queue.push(full_path);
            } else {
                if (match_pattern(name, search_pattern)) {
                    std::lock_guard<std::mutex> lock(log_mutex);
                    std::cout << "[MATCH] " << full_path << std::endl;
                }
            }
        }
        closedir(dp);
    }
}

void start_multithreaded_search(const std::string& root_path, const std::string& pattern, int thread_count) {
    search_pattern = pattern;
    directory_queue.push(root_path);

    std::vector<std::thread> threads;
    threads.reserve(thread_count);

    for (int i = 0; i < thread_count; ++i) {
        threads.emplace_back(search_thread);
    }

    for (auto& thread : threads) {
        thread.join();
    }
}
