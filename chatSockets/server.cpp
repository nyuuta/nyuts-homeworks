#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <vector>

struct client_info {
    int socket;
    struct sockaddr_in address;
    std::string name;
};

std::vector<client_info> clients;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void send_to_all(const std::string& message, int socket) {
    pthread_mutex_lock(&mutex);
    for (const auto& client : clients) {
        if (client.socket != socket) {
            send(client.socket, message.c_str(), message.size(), 0);
        }
    }
    pthread_mutex_unlock(&mutex);
}

void remove_client(int client_socket) {
    pthread_mutex_lock(&mutex);
    for (auto it = clients.begin(); it != clients.end(); ++it) {
        if (it->socket == client_socket) {
            clients.erase(it);
            break;
        }
    }
    pthread_mutex_unlock(&mutex);
}

void* handle_client(void* arg) {
    struct client_info* client_info_ptr = (client_info*)arg;
    int client_socket = client_info_ptr->socket;
    char buffer[1024];
    ssize_t bytes_read;

    bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_read <= 0) {
        close(client_socket);
        return nullptr;
    }
    buffer[bytes_read] = '\0';
    std::string client_name = buffer;
    pthread_mutex_lock(&mutex);
    clients.push_back({client_socket, client_info_ptr->address, client_name});
    pthread_mutex_unlock(&mutex);

    send_to_all(client_name + " joined the chat!\n", client_socket);

    while (true) {
        bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_read <= 0) break;
        buffer[bytes_read] = '\0';
        std::string message(buffer);
        if (message == "/exit") break;
        if (message == "/list") {
            std::string users = "Online users:\n";
            pthread_mutex_lock(&mutex);
            for (const auto& client : clients) {
                users += "• " + client.name + "\n";
            }
            pthread_mutex_unlock(&mutex);
            send(client_socket, users.c_str(), users.size(), 0);
            continue;
        }

        send_to_all("[" + client_name + "]: " + message, client_socket);
    }
    remove_client(client_socket);

    send_to_all(client_name + " left the chat.\n", client_socket);
    close(client_socket);
    delete client_info_ptr;
    return nullptr;
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("socket creation error");
        exit(errno);
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(8888);

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("bind failed");
        close(server_socket);
        exit(errno);
    }

    if (listen(server_socket, 10) < 0) {
        perror("listen failed");
        close(server_socket);
        exit(errno);
    }

    std::cout << "Waiting for connection\n";

    while (true) {
        struct sockaddr_in client_address;
        socklen_t client_addr_len = sizeof(client_address);
        int client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_addr_len);

        if (client_socket < 0) {
            perror("accept failed");
            continue;
        }

        pthread_t th;
        client_info* client_info_ptr = new client_info{client_socket, client_address, ""}; 
        if (pthread_create(&th, nullptr, handle_client, (void*)client_info_ptr) != 0) {
            perror("error creating threads");
            close(client_socket);
            delete client_info_ptr;
        }
        pthread_detach(th);
    }

    close(server_socket);
    return 0;
}
