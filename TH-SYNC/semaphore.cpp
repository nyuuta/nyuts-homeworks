#include <iostream>
#include <pthread.h>
#include <chrono>
#include <semaphore.h>

sem_t sem;

class Integer {
private:
  int inner_value = 0;
public:
  Integer(int value) :inner_value(value) {
    sem_init(&sem, 0, 1);
  }
  void inc() {
    inner_value++;
  }
  int get_inner_value() {
    return inner_value;
  }
  ~Integer(){
    sem_destroy(&sem);
  }
};

void* increment(void* arg){
  Integer* num = (Integer*)arg;
  sem_wait(&sem);
  for (int i = 0; i < 10000; ++i) {
    num->inc();
  }
  sem_post(&sem);
  return nullptr;
}

int main(int argc, char** argv) {
  Integer* num = new Integer(0);
  int thread_count = 50;
  pthread_t* tids = new pthread_t[thread_count];

  auto start = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < thread_count; ++i) {
    pthread_create(&tids[i], NULL, increment, num);  
  }

  for (int i = 0; i < thread_count; ++i) {
    pthread_join(tids[i], NULL);
  }
	
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> duration = end - start;
  std::cout << "Final value is " << num->get_inner_value() << std::endl;
  std::cout << "Time with semaphore inside increment: " << duration.count() << " seconds" << std::endl;

  delete[] tids;
  delete num;
  return 0;
}
