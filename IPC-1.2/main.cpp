// Если у дочернего процесса есть право отправить сигнал он завершится
// а Дочерний процесс завершится после отправки сигнала с помощью init.


#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>

int main() {
	pid_t pid = fork();
	if(pid == -1) {
		std::cerr << "fork failed" << std::endl;
		return 1;
	}

	if(pid == 0) {
		std::cout << "child process " << getpid() << " parent process " << getppid() << std::endl;
		if(kill(getppid(), SIGTERM) == 0) {
			std::cout << "signal SIGTERM sent to parent process  " << getppid() << std::endl;
        }else {
            	std::cerr << "failed to send signal to parent process" << std::endl;
	 }
	exit(0);
	}else {
		std::cout << "parent process " << getpid() << " is running" << std::endl;
        	while (true) {
            		sleep(1);
        	}
	}


}
