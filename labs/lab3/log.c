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
    signal(SIGINT, sigint_handler);

    int server_fd;
    // TODO: Create a socket file descriptor for the server
    //       use AF_INET for ipv4 and SOCK_STREAM for TCP
    server_fd = socket(AF_INET, SOCK_STREAM,0);

    // TODO: Set the socket to non-blocking mode
    int flags = fcntl(server_fd, F_GETFL, 0);
    fcntl(server_fd, F_SETFL, flags | O_NONBLOCK);

    struct sockaddr_in address;
    // TODO: Initialize the sockaddr_in struct for the server
    //       use INADDR_ANY for the address and the specified port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);
    
    int status;
    // TODO: Bind the socket to the specified address
    status = bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    // TODO: Listen for up to 32 incoming connections on the socket
    status = listen(server_fd, 32);

    int discovery_fd;
    // TODO: Create a socket file descriptor for the discovery server
    //       use AF_INET for ipv4 and SOCK_STREAM for TCP
    discovery_fd = socket(AF_INET, SOCK_STREAM, 0);

    // TODO: Set the socket to non-blocking mode
    fcntl(discovery_fd, F_SETFL, flags | O_NONBLOCK);

    struct sockaddr_in discovery_address;
    // TODO: Initialize the sockaddr_in struct for the discovery server
    discovery_address.sin_family = AF_INET;
    discovery_address.sin_port = htons(5000);
    // TODO: confirm this?
    inet_pton(AF_INET, discovery_server_ip, &discovery_address.sin_addr);

    // TODO: Connect to the discovery server (use connect_until_success)
    connect_until_success(discovery_fd, &discovery_address);

    UserMessage register_msg = {0};
    // TODO: Initialize a UserMessage struct with the register opcode
    //       and the user's port, address, and name
    register_msg.opcode = 1;
    register_msg.user.port = port;

    // address
    strncpy(register_msg.user.address, public_ip, sizeof(register_msg.user.address));
    register_msg.user.address[sizeof(register_msg.user.address) - 1] = '\0';

    // copy name
    strncpy(register_msg.user.name, name, sizeof(register_msg.user.name));
    register_msg.user.name[sizeof(register_msg.user.name) - 1] = '\0';


    uint8_t* register_data;
    // TODO: Encode the UserMessage struct into a byte array
    register_data = encode_user_message(&register_msg);

    // TODO: Send the UserMessage to the discovery server (use send_until_success)
    send_until_success(discovery_fd, register_data, sizeof(UserMessage));

    // TODO: Close the connection to the discovery server
    close(discovery_fd);

    while (!ctrl_c_pressed) {

        int client_fd;
        // TODO: Accept incoming connections from clients (use accept_until_success)
        //       Set status to the return value of accept_until_success
        int status = accept_until_success(server_fd, &client_fd );
        
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
    int discovery_server = socket(AF_INET, SOCK_STREAM, 0); 

    // TODO: Set the socket to non-blocking mode
    fcntl(discovery_server, F_SETFL, flags | O_NONBLOCK); 

    // TODO: Connect to the discovery server (use connect_until_success)
    connect_until_success(discovery_server, &discovery_address);
    
    UserMessage deregister_msg = {0};
    // TODO: Initialize a UserMessage struct with the deregister opcode
    //       and the user's port, address, and name
    deregister_msg.opcode = 2;
    deregister_msg.user.port = port;

    // address
    strncpy(deregister_msg.user.address, public_ip, sizeof(deregister_msg.user.address));
    deregister_msg.user.address[sizeof(deregister_msg.user.address) - 1] = '\0';

    // copy name
    strncpy(deregister_msg.user.name, name, sizeof(deregister_msg.user.name));
    deregister_msg.user.name[sizeof(deregister_msg.user.name) - 1] = '\0';


    uint8_t* deregister_data;
    // TODO: Encode the UserMessage struct into a byte array
    deregister_data = encode_user_message(&deregister_msg);

    // TODO: Send the UserMessage to the discovery server (use send_until_success)
    send_until_success(discovery_server, deregister_data, sizeof(deregister_data));

    // Free the public_ip string
    free(public_ip);
    free(register_data);
    free(deregister_data);
}
