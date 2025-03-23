#include <iostream>
#include <csignal>
#include <unistd.h>
#include <cstring>

int sigusr2_count = 0;

void sigusr1_handler(int signum, siginfo_t* info, void* extra_info) {}

void sigusr2_handler(int signum, siginfo_t* info, void* extra_info) { 
	std::cout << "Received signal " << signum << std::endl; 
	sigusr2_count++;
}

void sigint_handler(int signum, siginfo_t* info, void* extra_info) {
	std::cout << " total SIGUSR2 count:" << sigusr2_count << std::endl;
	exit(0);
}

int main() {
	pid_t pid = getpid();
	std::cout << "PID: " << pid << std::endl;
	struct sigaction sa1, sa2, saint;

	memset(&sa1, 0, sizeof(sa1));
	sa1.sa_handler = SIG_IGN;
	sigaction(SIGUSR1, &sa1, nullptr);

	memset(&sa2, 0, sizeof(sa2));
        sa2.sa_sigaction = sigusr2_handler;
        sa2.sa_flags = SA_SIGINFO;
        sigaction(SIGUSR2, &sa2, nullptr);

	memset(&saint, 0, sizeof(saint));
        saint.sa_sigaction = sigint_handler;
        saint.sa_flags = SA_SIGINFO;
        sigaction(SIGINT, &saint, nullptr);

	while(true) {
		std::cout << "I am still alive!!" << std::endl;
        	sleep(5); 	
	}
}
