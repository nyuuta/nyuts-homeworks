#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

void do_command(char** argv){
	pid_t pid = fork();
	if(pid < 0){
		std::cerr << "failed fork" << std::endl;
		exit(EXIT_FAILURE);
	}else if(pid == 0){
		execvp(argv[0], argv);
		std::cerr << "failed exec" << std::endl;
		exit(EXIT_FAILURE);
	}else{
		int status;
        	struct timeval start, end;
        
        	gettimeofday(&start, NULL);
        	waitpid(pid, &status, 0);
        	gettimeofday(&end, NULL);
        	double duration = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
        	std::cout << "Command completed with " << status << " exit code and took " << duration << " seconds." << std::endl;
	}


}


int main(int argc, char** argv){
	if(argc < 2){
		std::cerr << "invalid arguments" << std::endl;
		exit(EXIT_FAILURE);
	}

	do_command(argv + 1);
	return 0;
}
