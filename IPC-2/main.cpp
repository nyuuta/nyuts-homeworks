#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <sys/wait.h>


bool is_prime(int num) {
    if(num <= 1) return false;
    for(int i = 2; i * i <= num; ++i) {
        if(num % i == 0) return false;
    }
    return true;
}

int mth_prime(int m) {
	int count = 0;
	int num = 2;
	while(true) {
		if(is_prime(num)) {
			count++;
			if(count == m) {
				return num;
			}
		}
	num++;
	}
}


int main() {
	int pipefd_child[2], pipefd_parent[2];
	if(pipe(pipefd_child) == -1 || pipe(pipefd_parent) == -1) {
		std::cerr << "error:pipe" << std::endl;
		exit(1);
	}
	pid_t pid = fork();
	if(pid < 0) {
		std::cerr << "error:fork" << std::endl;
                exit(1);
	}
	else if(pid == 0) {
		close(pipefd_child[1]);
		close(pipefd_parent[0]);
		while(true) {
		int m;
		read(pipefd_child[0], &m, sizeof(m));
		std::cout << "[child] calculating " << m <<"-th prime num..." << std::endl;
		int prime = mth_prime(m);
		write(pipefd_parent[1], &prime, sizeof(prime));
		}
		close(pipefd_child[0]);
		close(pipefd_parent[1]);
		exit(0);
	}else {
		close(pipefd_child[0]);
		close(pipefd_parent[1]);
		std::string input;
		while(true) {
			std::cout << "[parent] input the number ";
			std::cin >> input;
			if(input == "exit") {
				std::cout << "[parent] ending" << std::endl;
				break;
			}

			int m = std::stoi(input);
			std::cout << "[parent] sending " << m << "-th number to the child..." << std::endl;
			write(pipefd_child[1], &m, sizeof(m));
			int prime;
			read(pipefd_parent[0], &prime, sizeof(prime));
			std::cout << "[parent] received calculation result of " << m << "-th prime = " << prime << std::endl;
		}
		close(pipefd_child[1]);
		close(pipefd_parent[0]);
		wait(nullptr);
	}

}
