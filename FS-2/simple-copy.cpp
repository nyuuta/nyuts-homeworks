#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>

int main(int argc, char** argv) {
    	if (argc != 3) {
        	return 1;
    	}
	
	int src = open(argv[1], O_RDONLY);
	if(src == -1){
		perror("Ошибка при открытии исходного файла");
		return 1;
	}

	int dest = open(argv[2], O_CREAT | O_TRUNC | O_WRONLY, 0664);
	if(dest == -1){
                perror("Ошибка при открытии целевого файла");
		close(src);
                return 1;
        }

	char buffer[1024];
	ssize_t bytes_read;
	while((bytes_read = read(src, buffer, sizeof(buffer))) > 0){
		if (write(dest, buffer, bytes_read) != bytes_read) {
            		perror("Ошибка при записи в целевой файл");
            		close(src);
            		close(dest);
          		return 1;
        	}	
	}
	
	if (bytes_read == -1) {
        	perror("Ошибка при чтении исходного файла");
    	}

    	close(src);
    	close(dest);

	 std::cout << "Файл скопирован из " << argv[1] << " в " << argv[2] << std::endl;

    	return 0;
}

