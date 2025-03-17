#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <poll.h>
#include <pthread.h>

#define max_clients 1024


std::string parse_and_perform(const std::string& command) {
    if(command == "exit") {
	return 0;
    }
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

void* receive(void* arg) {
    int client_socket = *(int*)arg;
    char buffer[1001];
    while(true) {
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
    }
    close(client_socket);
    delete (int*)arg;
    return nullptr;
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
   
   struct pollfd fds[max_clients + 1];
   fds[0].fd = server_socket;
   fds[0].events = POLLIN;
   int nfds = 1;
   
   while(true) {
        int p = poll(fds, nfds, -1);
        if(p < 0) {
            std::cerr << "error: poll failed" << std::endl;
            break;
        }
        
        if(fds[0].revents & POLLIN) {
            int client_socket;
            struct sockaddr_in client_address;
            unsigned int client_addr_len = sizeof(client_address);
            if((client_socket = accept(server_socket,(struct sockaddr *)&client_address, &client_addr_len)) < 0) {
                std::cerr << "error: can't accept" << std::endl;
                continue;
            }
            std::cout << "Connected client with address: " << inet_ntoa(client_address.sin_addr) << std::endl;
            
            if(nfds < max_clients + 1) {
                fds[nfds].fd = client_socket;
                fds[nfds].events = POLLIN;
                nfds++;
            }else{
                std::cerr << "error: too many clients" << std::endl;
                close(client_socket);
                continue;
            }
        }
        
        
        for(int i = 1; i < nfds; ++i) {
            if(fds[i].revents & POLLIN) {
                pthread_t th;
                int* client_socket = new int(fds[i].fd);
                if(pthread_create(&th, nullptr, receive, client_socket) != 0) {
                    std::cerr << "error: can't create thread" << std::endl;
                    delete client_socket;
                    exit(errno);
                }
                pthread_detach(th);
            }
        }
   }
   close(server_socket);
}
