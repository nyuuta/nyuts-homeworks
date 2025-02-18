#include <iostream>
#include <unistd.h>      
#include <sys/wait.h>   
#include <semaphore.h>
#include <fcntl.h>  

sem_t* sticks[5];
void philosopher(int id) {
    int prior_stick_ind = id;              
    int non_prior_stick_ind = (id + 1) % 5;
    if (id == 0) std::swap(prior_stick_ind, non_prior_stick_ind);
    while (true) {
        std::cout << "philosopher " << id << " is thinking\n";
        sleep(1);
        sem_wait(sticks[prior_stick_ind]);
        sem_wait(sticks[non_prior_stick_ind]);
        std::cout << "philosopher " << id << " is eating\n";
        sleep(1);
        sem_post(sticks[prior_stick_ind]);
        sem_post(sticks[non_prior_stick_ind]);
    }
}

int main() {
    for (int i = 0; i < 5; ++i) {
        std::string sem_name = "/stick" + std::to_string(i);
        sticks[i] = sem_open(sem_name.c_str(), O_CREAT, 0644, 1);
    }
    pid_t pids[5];
    for (int i = 0; i < 5; ++i) {
        pids[i] = fork();
        if (pids[i] == 0) {
            philosopher(i);
            exit(0);
        }
    }
    for (int i = 0; i < 5; ++i) {
        wait(nullptr);
    }
    for (int i = 0; i < 5; ++i) {
        sem_close(sticks[i]);
        std::string sem_name = "/stick" + std::to_string(i);
        sem_unlink(sem_name.c_str());
    }

    return 0;
}
