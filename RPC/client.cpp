#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>


int main()
{
   int client_socket;
   struct sockaddr_in server_address;
   server_address.sin_family = AF_INET;
   server_address.sin_port = htons(8080);
   server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
   
   
   if((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
       std::cerr << "error: can't create socket" << std::endl;
       exit(errno);
   }
   if(connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
       std::cerr << "error: can't connect" << std::endl;
       exit(errno);
   }
  
  std::string message;
  std::cout << "enter: ";
  std::getline(std::cin, message);
  int sent = send(client_socket, message.c_str(), message.size(), 0);
  if(sent == -1){
      std::cerr << "error: can't send" << std::endl;
      exit(errno);
  }
  char buffer[1001];
  int received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
  if (received == -1) {
        std::cerr << "error: can't receive" << std::endl;
        exit(errno);
  }
  buffer[received] = '\0';
  std::cout << "result: " << buffer << std::endl;
  close(client_socket);
   
}
