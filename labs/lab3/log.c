#include "messages.h"
#include "util.h"

int ctrl_c_pressed = 0;
void sigint_handler(int signum) {
    if (signum == SIGINT) {
        ctrl_c_pressed = 1;
        printf("Exiting...\n");
    }
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <discovery_server_ip> <name> <port>\n", argv[0]);
        return 1;
    }

    const char* discovery_server_ip = argv[1];
    const char* name = argv[2];
    const unsigned short port = atoi(argv[3]);
    char* public_ip = get_public_ip();
    if (public_ip == NULL) {
        public_ip = strdup("127.0.0.1");
    }

    // TODO: Initialize signal handler for SIGINT
    Signal s(SIGINT);

    int server_fd;
    // TODO: Create a socket file descriptor for the server
    //       use AF_INET for ipv4 and SOCK_STREAM for TCP
    server_fd = socket(AF_INET, SOCK_STREAM,0);

    // TODO: Set the socket to non-blocking mode
    int flags = fcntl(discovery_fd, F_GETFL, 0);
    fcntl(discovery_fd, F_SETFL, flags | O_NONBLOCK);

    struct sockaddr_in address;
    // TODO: Initialize the sockaddr_in struct for the server
    //       use INADDR_ANY for the address and the specified port
    address.sin_addr = INADDR_ANY;
    address.sin_port = INADDR_ANY;
    
    int status;
    // TODO: Bind the socket to the specified address
    status = bind(server_fd, &address, 0);

    // TODO: Listen for up to 32 incoming connections on the socket
    status = connect_until_success(server_fd, &address);

    int discovery_fd;
    // TODO: Create a socket file descriptor for the discovery server
    //       use AF_INET for ipv4 and SOCK_STREAM for TCP
    discovery_fd = socket(AF_INET, SOCK_STREAM, 0);

    // TODO: Set the socket to non-blocking mode
    fcntl(discovery_fd, F_SETFL, flags | O_NONBLOCK);


    struct sockaddr_in discovery_address;
    // TODO: Initialize the sockaddr_in struct for the discovery server
    discovery_address.sin_port =port;

    // TODO: Connect to the discovery server (use connect_until_success)
    connect_until_success(discovery_fd, &discovery_address);

    UserMessage register_msg = {0};
    // TODO: Initialize a UserMessage struct with the register opcode
    //       and the user's port, address, and name
    register_msg.user.address = discovery_address;
    register_msg.user.name = name;
    register_msg.opcode = 0;
    register_msg.user.port = port;


    uint8_t* register_data;
    // TODO: Encode the UserMessage struct into a byte array
    register_data = encode_user_message(&register_msg);

    // TODO: Send the UserMessage to the discovery server (use send_until_success)
    send_until_success(discovery_fd, register_data, sizeof(register_data));

    // TODO: Close the connection to the discovery server
    close(discovery_fd);

    while (!ctrl_c_pressed) {

        int client_fd;
        // TODO: Accept incoming connections from clients (use accept_until_success)
        //       Set status to the return value of accept_until_success
        int status = accept_until_success(server_fd, client_fd );
        
        // If status < 0, accept_until_success encountered an error
        if (status < 0) {
            free(public_ip);
            perror("accept");
            return 1;
        } 
        // If status > 0, ctrl-c was pressed
        else if (status > 0) {
            break;
        }

        uint8_t buffer[sizeof(ChatMessage)] = {0};
        // TODO: Receive a ChatMessage from the client (use recv_until_success)
        //       Set status to the return value of recv_until_success
        status = recv_until_success(client_fd, buffer, sizeof(buffer));

        // If status < 0, recv_until_success encountered an error
        if (status < 0) {
            free(public_ip);
            return 1;
        } 
        // If status > 0, ctrl-c was pressed
        else if (status > 0) {
            break;
        }

        ChatMessage* chat_msg;
        // TODO: Decode the buffer into a ChatMessage
        chat_msg = decode_chat_message(buffer, sizeof(buffer));

        // Print the received message
        printf("%s : \"%s\"\n", chat_msg->name, chat_msg->message);

        // TODO: Close the connection to the client
        close(client_fd);
    }

    // Reset ctrl_c_pressed
    ctrl_c_pressed = 0;

    // TODO: Create a socket file descriptor for the discovery server

    // TODO: Set the socket to non-blocking mode

    // TODO: Connect to the discovery server (use connect_until_success)
    
    UserMessage deregister_msg = {0};
    // TODO: Initialize a UserMessage struct with the deregister opcode
    //       and the user's port, address, and name

    uint8_t* deregister_data;
    // TODO: Encode the UserMessage struct into a byte array

    // TODO: Send the UserMessage to the discovery server (use send_until_success)

    // Free the public_ip string
    free(public_ip);
}