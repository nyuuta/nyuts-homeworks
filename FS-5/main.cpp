#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

int main(int argc, char** argv){
	if(argc < 2){
		std::cerr << "error: no input file" << std::endl;
		return 1;	
	}

	int fd = open(argv[1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if(fd < 0){
		std::cerr << "error: cannot open file" << std::endl;
		return 1;
	}

	int new_fd = dup(fd);
	if(new_fd < 0){
		std::cerr << "error: failed to dup" << std::endl;
		close(fd);
		return 1;
	}

	if(write(fd, "first line\n",11) < 0){
		std::cerr << "error: failed writing" << std::endl;
		close(fd);
		return 1;
	}

	if(write(new_fd, "second line\n",12) < 0){
                std::cerr << "error: failed writing" << std::endl;
                close(fd);
		close(new_fd);
                return 1;
        }

	close(fd);
	close(new_fd);



}
