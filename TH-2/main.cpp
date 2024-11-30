#include <pthread.h>
#include <queue>
#include <functional>
#include <iostream>
#include <unistd.h>

pthread_mutex_t mtx;
pthread_cond_t cond;
bool stop = false;


class ThreadPool {
public:
    ThreadPool(int numThreads) : numThreads_(numThreads) {
        pthread_mutex_init(&mtx, nullptr);
        pthread_cond_init(&cond, nullptr);
        threads = new pthread_t[numThreads_];
        for (size_t i = 0; i < numThreads_; ++i) {
            pthread_create(&threads[i], nullptr, ThreadPool::worker, this);
        }
    }

    ~ThreadPool() {
            pthread_mutex_lock(&mtx);
            stop = true;
            pthread_cond_broadcast(&cond);
            pthread_mutex_unlock(&mtx);
        for (size_t i = 0; i < numThreads_; ++i) {
            pthread_join(threads[i], nullptr);
        }
        pthread_cond_destroy(&cond);
        pthread_mutex_destroy(&mtx);
        delete[] threads;
    }

    void addTask(std::function<void()> task) {
        pthread_mutex_lock(&mtx);
        taskQueue.push(task);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mtx);
    }

private:
    static void* worker(void* arg) {
        ThreadPool* pool = static_cast<ThreadPool*>(arg);
        pool->executeTask();
        return nullptr;
    }

    void executeTask() {
        while (true) {
            std::function<void()> task;
            pthread_mutex_lock(&mtx);

            while (taskQueue.empty() && !stop) {
                pthread_cond_wait(&cond, &mtx);
            }
            if (stop && taskQueue.empty()) {
                pthread_mutex_unlock(&mtx);
                break;
            }
            task = taskQueue.front();
            taskQueue.pop();
            pthread_mutex_unlock(&mtx);
            task();
        }
    }

    pthread_t* threads;
    size_t numThreads_;
    std::queue<std::function<void()>> taskQueue;
};


void sampleTask(int id) {
    std::cout << "task " << id << " is running\n";
}

int main() {
    ThreadPool pool(3);
    for (int i = 0; i < 10; ++i) {
        pool.addTask([i]() { sampleTask(i); });
	sleep(1);
    }
    return 0;
}


