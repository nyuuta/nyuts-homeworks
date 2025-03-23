#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <cstdlib>
#include <algorithm>

int main(int argc, char** argv){
   if(argc < 3){
      std::cerr << "No input file" << std::endl;
      exit(EXIT_FAILURE);
   }

   int src_fd = open(argv[1], O_RDONLY);
   if(src_fd < 0){
      std::cerr << "Error with opening the file" << std::endl;
      exit(EXIT_FAILURE);
   }

   int dest_fd = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644);
   if(dest_fd < 0){
      std::cerr << "Error with opening the file" << std::endl;
      close(src_fd);
      exit(EXIT_FAILURE);
   }

   char buffer[4096];
   size_t bytes_read;
   size_t holes = 0, total = 0, data = 0;
   ssize_t bytes_written;
   while((bytes_read = read(src_fd, buffer, sizeof(buffer))) > 0){
      for(size_t i = 0; i < bytes_read; i++){
	 bytes_written = write(dest_fd, &buffer[i], 1);
         if(bytes_written < 0){
            std::cerr << "Error with writing the file " << std::endl;
            close(src_fd);
            close(dest_fd);
            exit(EXIT_FAILURE);
         }
         if(buffer[i] == '\0') holes++;
         else{
            data++;
         }
         total++;
      }
   }
     std::cout << "Successfully copied: " << total << "(data: " << data << ", holes: " << holes << ")" << std::endl;
     close(src_fd);
     close(dest_fd);
}
