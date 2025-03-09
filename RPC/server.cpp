#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
// ADD 5 8
std::string parse_and_perform(const std::string& command) {
    size_t space_1 = command.find(' ');
    size_t space_2 = command.find(' ', space_1 + 1);
    std::string operation = command.substr(0, space_1);
    int a = std::stoi(command.substr(space_1 + 1, space_2 - space_1 - 1));
    int b = std::stoi(command.substr(space_2 + 1));
    if (operation == "ADD") return std::to_string(a + b);
    if (operation == "SUB") return std::to_string(a - b);
    if (operation == "MUL") return std::to_string(a * b);
    if (operation == "DIV") return std::to_string(a / b);

    return "error: unknown operation";
    

}

int main()
{
   int server_socket;
   struct sockaddr_in server_address;
   server_address.sin_family = AF_INET;
   server_address.sin_port = htons(8080);
   server_address.sin_addr.s_addr = INADDR_ANY;
   
   if((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
       std::cerr << "error: can't create socket" << std::endl;
       exit(errno);
   }
   if(bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
       std::cerr << "error: bind failed" << std::endl;
       close(server_socket);
       exit(errno);
   }
   if(listen(server_socket, 10) < 0) {
       std::cerr << "error: can't listen" << std::endl;
       close(server_socket);
       exit(errno);
   }
   std::cout << "Waiting for connection" << std::endl;
   while(true) {
       int client_socket;
       struct sockaddr_in client_address;
       unsigned int client_addr_len = sizeof(client_address);
       if((client_socket = accept(server_socket,(struct sockaddr *)&client_address, &client_addr_len)) < 0) {
           std::cerr << "error: can't accept" << std::endl;
           continue;
       }
       std::cout << "Connected client with address: " << inet_ntoa(client_address.sin_addr) << std::endl;
       char buffer[1001];
       int rs = recv(client_socket, buffer, 1000, 0);
       if (rs == -1) {
        std::cerr << "error: can't receive" << std::endl;
        close(client_socket);
        continue;
       }
       std::string command(buffer);
       std::string message = parse_and_perform(command);
       int sent = send(client_socket, message.c_str(), message.size(), 0);
       if(sent == -1) {
            std::cerr << "error: can't sent";
            break;
       }
       close(client_socket);
  
   }
   close(server_socket);
   
   
}
