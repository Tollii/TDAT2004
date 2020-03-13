#include <iostream>
#include <list>
#include <functional>
#include <thread>
#include <mutex>
#include <vector>
#include <atomic>

using namespace std;

class Workers {
private:
    const int thread_num;
    vector<thread> threads;
    list<function<void()>> tasks;
    mutex task_mutex;
    condition_variable task_cv;
    atomic<bool> stopped{false};

public:
    explicit Workers(int thread_num) : thread_num(thread_num) {
        if(thread_num < 1)
            cerr << "Invalid number of threads" << endl;
    }

    void start() {
        for(int i = 0; i < thread_num; i++) {
            threads.emplace_back([this] {
                while(true) {
                    function<void()> task;
                    {
                        unique_lock<mutex> lock(task_mutex);
                        if(stopped.load())
                            return;

                        task_cv.wait(lock);
                        if(!tasks.empty()) {
                            task = *tasks.begin();
                            tasks.pop_front();
                        }
                    }

                    if(task)
                        task();
                }
            });
        }
    }

    void stop() {
        stopped.store(true);
        task_cv.notify_all();

        cout << "Joining threads..." << endl;
        for(auto &thread : threads)
            thread.join();

        cout << "Stopped and joined threads" << endl;
    }

    void post(const function<void()> &task) {
        if(stopped.load())
            cerr << "Stopped has already been called" << endl;
        {
            unique_lock<mutex> lock(task_mutex);
            tasks.emplace_back(task);
        }
        task_cv.notify_one();
    }

    void post_timeout(const function<void()> &task) {
        if(stopped.load())
            cerr << "Stopped has already been called" << endl;
        {
            this_thread::sleep_for(2s);
            unique_lock<mutex> lock(task_mutex);
            tasks.emplace_back(task);
        }
        task_cv.notify_one();
    }

};

int main() {

    Workers worker_threads(4);
    Workers event_loop(1);

    worker_threads.start();
    event_loop.start();

    worker_threads.post([] {
        cout << "Task A start" << endl;
        this_thread::sleep_for(8s);
        cout << "Task A end" << endl;
    });

    worker_threads.post_timeout([] {
        cout << "Task Awasdasd start" << endl;
        this_thread::sleep_for(8s);
        cout << "Task A end" << endl;
    });

    worker_threads.post([] {
        cout << "Task B start" << endl;
        this_thread::sleep_for(5s);
        cout << "Task B end" << endl;
        // Might run parallel to task A
    });

    event_loop.post([] {
        cout << "Task C start" << endl;
        this_thread::sleep_for(2s);
        cout << "Task C end" << endl;
        // Might run parallel to task A and B
    });

    event_loop.post([] {
        cout << "Task D start" << endl;
        this_thread::sleep_for(1s);
        cout << "Task D end" << endl;
        // Will run after task C
        // Might run in parallel with task A and B
    });

    this_thread::sleep_for(15s);
    event_loop.stop();
    worker_threads.stop();
    return -1;
}