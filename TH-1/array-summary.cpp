#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <pthread.h>
#include <chrono>

struct Data {
    const std::vector<int>& arr;
    long long part_sum = 0;
    std::size_t start;
    std::size_t end;

    Data(const std::vector<int>& arr) : arr(arr) {} // Конструктор с параметром
};

void* partial_sum(void* arg) {
    Data* data = static_cast<Data*>(arg);
    for (std::size_t i = data->start; i < data->end; ++i) {
        data->part_sum += data->arr[i];
    }
    return nullptr;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "error with input" << std::endl;
        return 1;
    }

    std::size_t N = std::stoi(argv[1]);
    std::size_t M = std::stoi(argv[2]);
    std::vector<int> arr(N);
    for (std::size_t i = 0; i < N; ++i) {
        arr[i] = rand() % 100;
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    long long sum_without_threads = 0;
    for (std::size_t i = 0; i < N; ++i) {
        sum_without_threads += arr[i];
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> dur_without_threads = end_time - start_time;

    pthread_t* threads = new pthread_t[M];

    // Используем std::vector<Data> вместо массива
    std::vector<Data> thread_data(M, Data(arr));  // Конструируем элементы с нужным конструктором

    long long sum_with_threads = 0;
    std::size_t block_size = N / M;
    start_time = std::chrono::high_resolution_clock::now();

    for (std::size_t i = 0; i < M; ++i) {
        thread_data[i].part_sum = 0;
        thread_data[i].start = i * block_size;
        thread_data[i].end = (i == M - 1) ? N : (i + 1) * block_size;

        if (pthread_create(&threads[i], nullptr, partial_sum, &thread_data[i]) != 0) {
            std::cerr << "Error creating thread " << i << std::endl;
            return 1;
        }
    }

    for (std::size_t i = 0; i < M; ++i) {
        pthread_join(threads[i], nullptr);
    }

    for (std::size_t i = 0; i < M; ++i) {
        sum_with_threads += thread_data[i].part_sum;
    }

    end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_with_threads = end_time - start_time;

    std::cout << "Time spent without threads: " << dur_without_threads.count() << " seconds" << std::endl;
    std::cout << "Time spent with " << M << " threads: " << duration_with_threads.count() << " seconds" << std::endl;

    delete[] threads;

    return 0;
}

