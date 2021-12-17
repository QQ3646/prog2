#include <thread>
#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <atomic>
#include <mutex>
#include <stack>
#include <condition_variable>
#include <algorithm>
//#include <sys/stat.h>

const std::string WORK_DIRECTORY = R"(C:\Users\hellm\CLionProjects\prog5\sample_sites\)";

std::stack<std::string> url;
std::vector<std::string> checked_url;

std::atomic<int> counter{0};
std::condition_variable s_m;
std::mutex stack_mutex;
std::atomic<bool> work_finished;


std::string readAllFile(std::string &name_of_file) {
    std::fstream input_stream(WORK_DIRECTORY + name_of_file);
    if (!input_stream.is_open())
    {
        ::std::cerr << "cannot open the file " << WORK_DIRECTORY + name_of_file << ::std::endl;
    }
    std::string return_str(std::istreambuf_iterator<char>(input_stream), {});

    return return_str;
}

void saveValueFile(std::string &name_of_file, std::string &value) {
    std::ofstream output_f(WORK_DIRECTORY + "copy_" + name_of_file);
    output_f << value;
    output_f.close();
}

void findAndNext(std::string &name_of_file) {
    work_finished = false;
    std::string file_value = readAllFile(name_of_file);
    saveValueFile(name_of_file, file_value);

    ++counter;

    std::string::size_type pos = file_value.find("<a href");
    while (pos != std::string::npos) {
        std::string::size_type start_url = file_value.find('\"', pos);
        std::string::size_type end_url = file_value.find('\"', start_url + 1);
        pos = file_value.find("<a href", end_url + 2);

        std::string t_url = file_value.substr(start_url + 1, end_url - start_url - 1);

        auto p = t_url.find("file://");
        if (p != std::string::npos) {
            t_url = t_url.substr(p + 7);
        }

        stack_mutex.lock();
        if(std::find(checked_url.begin(), checked_url.end(), t_url) == checked_url.end()) {
            url.push(t_url);
            checked_url.push_back(t_url);
        }
        stack_mutex.unlock();

        s_m.notify_all();
    }
    work_finished = true;
}

void waitingTreads() {
    while (!work_finished.load() || !url.empty()) {
        std::unique_lock<std::mutex> l(stack_mutex);
        s_m.wait(l, []() -> bool { return !url.empty(); });

        std::string c_url = url.top();
        url.pop();

        l.unlock();
        findAndNext(c_url);
    }
}

int main() {
    auto start_time = std::chrono::high_resolution_clock::now();

    std::string start_url;
    size_t thread_count;
    std::cin >> thread_count >> start_url;

    std::vector<std::thread> threads;
    threads.reserve(thread_count);

    url.push(start_url);
    checked_url.push_back(start_url);

    for (int i = 0; i < thread_count; ++i) {
        threads.emplace_back(waitingTreads);
    }

    while(!work_finished.load() || !url.empty()) {}

    for (int i = 0; i < thread_count; ++i) {
        threads[i].join();
    }

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()
                                                                          - start_time);
    std::cout << counter << " " << duration.count();
    return 0;
}
