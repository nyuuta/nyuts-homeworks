#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <cstring>
int main()
{
    struct sockaddr_in server_addr;
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(client_socket == -1) {
        std::cerr << "error: can't create a socket";
        exit(errno);
    } 
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80);
    server_addr.sin_addr.s_addr = inet_addr("146.190.62.39");
    int connected = connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(connected == -1) {
        std::cerr << "error: can't connect";
        exit(errno); 
    }
    std::string message = "GET / HTTP/1.1\r\nHost: httpforever.com\r\nConnection: close\r\n\r\n";
    int sent = send(client_socket, message.c_str(), message.size(), 0);
    if(sent == -1) {
        std::cerr << "error: can't send";
        exit(errno); 
    }
    char buffer[1001];
    int rs = 0;
    while((rs = recv(client_socket, buffer, 1000, 0)) > 0) {
        buffer[rs] = '\0';
        std::cout << buffer << std::endl;
    }
    if(rs == -1) {
        std::cerr << "error: can't receive";
        exit(errno); 
    }
    close(client_socket);
    
}
