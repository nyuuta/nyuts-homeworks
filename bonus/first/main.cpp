#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <cstring>

void file_content(const char* file) {
	int fd = open(file, O_RDONLY);
	if(fd < 0) {
		std::cerr << "error: can't open the file" << std::endl;
		return;
	}
	char buff[1024];
	ssize_t bytes_read;
	ssize_t bytes_written;
	while((bytes_read = read(fd, buff, sizeof(buff))) > 0) {
		bytes_written = write(1, buff, bytes_read);
		if(bytes_written < 0) {
			std::cerr << "error: can't write" << std::endl;
			close(fd);
			return;
		}
	}
	if(bytes_read < 0) {
		std::cerr << "error: can't read the file" << std::endl;
        	close(fd);
		return;
	}
	close(fd);
}

void directory_files(const char* path) {
	DIR* dir = opendir(path);
	if(!dir) {
		std::cerr << "error: can't open the directory" << std::endl;
		return;
	}

	struct dirent* read_dir;
	while((read_dir = readdir(dir))) {
		if(strcmp(read_dir->d_name, ".") == 0 || strcmp(read_dir->d_name, "..") == 0) {
			continue;
		}
	char absolute_path[1024];
	snprintf(absolute_path, sizeof(absolute_path), "%s/%s", path, read_dir->d_name);
	struct stat st;
	stat(absolute_path, &st);
	if(S_ISDIR(st.st_mode)) {
		directory_files(absolute_path);		
	}else if(S_ISREG(st.st_mode)) {
		file_content(absolute_path);
	}
	}
	closedir(dir);

}

int main(int argc, char** argv) {
	if(argc != 2) {
		std::cerr << "no input" << std::endl;
		return 1;
	}

	directory_files(argv[1]);

}
