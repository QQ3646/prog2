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
std::mutex check_mutex;
std::atomic<int> working_threads;
std::atomic<int> counter_new = 1;

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

        {
            std::unique_lock<std::mutex> l(check_mutex);
            if (std::find(checked_url.begin(), checked_url.end(), t_url) == checked_url.end()) {
                {
                    std::lock_guard<std::mutex> x(stack_mutex);
                    url.push(t_url);
                }
                checked_url.push_back(t_url);
                check_mutex.unlock();
                ++counter_new;
            }
        }

    }
    s_m.notify_all();
    --working_threads;
}

void waitingTreads() {
    while (working_threads > 0 || !url.empty()) {
        std::string c_url;
        {
            if (counter == counter_new)
                return;
            std::unique_lock<std::mutex> l(stack_mutex);
            s_m.wait(l, []() -> bool { return !url.empty() || counter == counter_new; });

            if (counter_new == counter) {
                return;
            }

            c_url = url.top();
            url.pop();
        }
        ++working_threads;
        findAndNext(c_url);

    }
}

int main() {

    std::string start_url;
    size_t thread_count;
    std::cin >> thread_count >> start_url;

    std::vector<std::thread> threads;
    threads.reserve(thread_count);

    url.push(start_url);
    checked_url.push_back(start_url);
    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < thread_count; ++i) {
        threads.emplace_back(waitingTreads);
    }

    while(working_threads > 0 || !url.empty()) {
        std::this_thread::sleep_for(std::chrono::microseconds (1000));
        s_m.notify_all();
    }

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()
                                                                          - start_time);

    for (int i = 0; i < thread_count; ++i) {
        threads[i].join();
    }
    std::cout << counter << " " << duration.count();
    return 0;
}
