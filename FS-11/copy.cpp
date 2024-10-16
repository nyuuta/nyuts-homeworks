#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>

int main() {
    std::string source_file, destination_file;
    off_t source_offset, dest_offset, bytes_to_copy;

    std::cout << "Enter the name of the source file: ";
    std::cin >> source_file;

    std::cout << "Enter the name of the destination file: ";
    std::cin >> destination_file;

    std::cout << "Enter the initial offset in the source file (in bytes): ";
    std::cin >> source_offset;

    std::cout << "Enter the initial offset in the destination file (in bytes): ";
    std::cin >> dest_offset;

    std::cout << "Enter the number of bytes to copy: ";
    std::cin >> bytes_to_copy;

    int src_fd = open(source_file.c_str(), O_RDONLY);
    if (src_fd < 0) {
        std::cerr << "Error opening the file "<< std::endl;
        exit(EXIT_FAILURE);
    }
    
    int dest_fd = open(destination_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd < 0) {
        std::cerr << "Error opening the file "<< std::endl;
	close(src_fd);
        exit(EXIT_FAILURE);
    }
    lseek(src_fd, source_offset, SEEK_SET);
    lseek(dest_fd, dest_offset, SEEK_SET);

    char buffer[4096];
    ssize_t total_copied = 0;
    while (total_copied < bytes_to_copy) {
        ssize_t bytes_read = read(src_fd, buffer, sizeof(buffer));
        if (bytes_read <= 0) break;
        ssize_t bytes_written = write(dest_fd, buffer, std::min(bytes_read, bytes_to_copy - total_copied));
        if (bytes_written < 0) break;
        total_copied += bytes_written;
    }

    std::cout << "Successfully copied " << total_copied << " bytes." << std::endl;
    close(src_fd);
    close(dest_fd);
    return 0;
}

