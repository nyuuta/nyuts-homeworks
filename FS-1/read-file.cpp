#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>


int main(int argc, char **argv) {
	if(argc == 2){
   		 char buffer[100];

		 int fd = open(argv[1], O_RDONLY);
   		 if (fd == -1) {
       			 perror("Ошибка при открытии файла");
       			 return 1;
   		 }

   	 	ssize_t bytes_read;
	 	if ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        		std::cout.write(buffer, bytes_read);
   	 	}
	 	if(bytes_read == -1){
	 		perror("Ошибка при чтении  файла");
                	return 1;
	 	}


 		std::cout << "Содержимое файла: " << buffer << std::endl;

		close(fd);
    		return 0;
	}
	return 1;

}

