#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <cerrno>

int main(int argc, char** argv) {
       	if (argc != 2) {
       		 std::cerr << "Использование: " << argv[0] << " <путь к файлу>" << std::endl;
        	return 1;
    	}

	int fd = open(argv[1],O_RDWR);
	if(fd == -1){
		perror("Ошибка при открытии файла");
		return 1;
	}
	
	char buffer;
	ssize_t file_size = 0;
	while((file_size = read(fd, &buffer, 1 )) > 0){
		file_size++;
	}

	lseek(fd, 0, SEEK_SET);
	char null_symb = '\0';

	for(size_t i = 0; i < file_size; ++i){
		if(write(fd, &null_symb, 1) != 1){
			perror("Ошибка при записи в файл");
			close(fd);
			return 1;
		}
	}
	
	close(fd);
	if(unlink(argv[1]) != 0){
      		std::cerr<<"error - cant delete file"<<std::endl;
      		return errno;
    	}
	std::cout << "Файл " << fd << " успешно удален." << std::endl;
    	return 0;
}

