#include <iostream>
#include <unistd.h>
#include <csignal>
#include <cstdlib>

int main(int argc, char** argv) {
	if(argc != 3) {
		std::cerr << "error: no input" << std::endl;
		return 1;
	}
	int signal = atoi(argv[1]);
	int pid = atoi(argv[2]);
	if (kill(pid, signal) == -1) {
        std::cerr << "Error: Failed to send signal " << signal << " to PID " << pid << std::endl;
        return 1;
    }

    	std::cout << "Signal " << signal << " sent to PID " << pid << std::endl;
    return 0;
}
