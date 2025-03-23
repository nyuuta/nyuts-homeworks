#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string>

bool running = true; 

void* receive(void* arg) {
    int client_sock = *((int*)arg);
    char buffer[1024];
    while (running) {
        ssize_t bytes_read = recv(client_sock, buffer, sizeof(buffer) - 1, 0);
        if (bytes_read <= 0) {
            running = false;
            break;
        }
        buffer[bytes_read] = '\0';
        std::cout << buffer << std::endl;
    }
    return nullptr;
}

int main() {
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("error: failed socket creation");
        return 1;
    }
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8888);
    inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);

    if (connect(client_socket, (sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("error: connection failed");
        close(client_socket);
        return 1;
    }
    std::string name;
    std::cout << "Enter your name: ";
    std::getline(std::cin, name);
    if (send(client_socket, name.c_str(), name.size(), 0) == -1) {
        perror("error: failed to send name");
        close(client_socket);
        return 1;
    }

    pthread_t th;
   if (pthread_create(&th, nullptr, receive, &client_socket) != 0) {
        perror("error: failed to create thread");
        close(client_socket);
        return 1;
    }

    while (running) {
        std::string input;
        std::getline(std::cin, input);
        if (input == "/exit") {
            if (send(client_socket, "/exit", 5, 0) == -1) {
                perror("error: failed to send exit command");
            }
            running = false;
            break;
        }
        if (input == "/list") {
            if (send(client_socket, "/list", 5, 0) == -1) {
                perror("error: failed to send list command");
            }
            continue;
        }
        if (send(client_socket, input.c_str(), input.size(), 0) == -1) {
            perror("error: failed to send message");
            running = false;
            break;
        }
    }
    close(client_socket);
    pthread_join(th, nullptr);
}
