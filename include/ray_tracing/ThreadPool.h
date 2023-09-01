#include <vector>
#include <queue>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>

using namespace std;

class ThreadPool {
public:
	vector<thread> workers;
	queue<function<void()>> tasks;
	mutex queue_mutex;
	condition_variable condition;
	bool stop;

    ThreadPool(size_t threads) : stop(false) {
        for (size_t i = 0; i < threads; ++i)
            workers.emplace_back([this] {
                while (true){
                    function<void()> task;

                    {
                        unique_lock<mutex> lock(queue_mutex);
                        condition.wait(lock, [this] { return stop || !tasks.empty(); });
                        if (stop && tasks.empty())
                            return;
                        task = move(tasks.front());
                        tasks.pop();
                    }

                    task();
                }
            });
    }

    ~ThreadPool() {
        {
            unique_lock<mutex> lock(queue_mutex);
            stop = true;
        }
        condition.notify_all();
        for (thread& worker : workers)
            worker.join();
    }

    void enqueue(function<void()> task) {
        {
            unique_lock<mutex> lock(queue_mutex);
            if (stop)
                throw runtime_error("enqueue on stopped ThreadPool");
            tasks.emplace(task);
        }
        condition.notify_one();
    }
};