#include <iostream>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>

void initialize(int argc, char** argv)
{
 if(argc < 2){
	std::cerr << "error: no input file" << std::endl;
	exit(EXIT_FAILURE);
 }
 
 int fd = open(argv[1], O_RDONLY); 
 if(fd < 0){
 	std::cerr << "error: cannot open file" << std::endl;
	exit(EXIT_FAILURE);
 }

 if(dup2(fd, STDIN_FILENO) < 0){
 	std::cerr << "error: failed to redirect" << std::endl;
	close(fd);
	exit(EXIT_FAILURE);
 }

 close(fd);

}

int main(int argc, char** argv)
{
  
  initialize(argc, argv);


  std::string input;
  std::string reversed;
  std::cin >> input;

  for (int i = input.size() - 1; i >= 0; --i) {
        reversed += input[i];
    }
  std::cout << reversed << std::endl;

  return 0;
}
