#include "util.h"
#include "messages.h"

int ctrl_c_pressed = 0;

ChattersMessage chatters;

long long get_time_us() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return ts.tv_sec * 1000000 + ts.tv_nsec / 1000;
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <discovery_server_ip> <name>\n", argv[0]);
        return 1;
    }

    const char* discovery_server_ip = argv[1];
    const char* name = argv[2];
    char* public_ip = get_public_ip();
    if (public_ip == NULL) {
        public_ip = strdup("127.0.0.1");
    }

    int discovery_fd;
    // TODO: Create a socket file descriptor for the discovery server
    //       use AF_INET for ipv4 and SOCK_STREAM for TCP
    discovery_fd = socket(AF_INET, SOCK_STREAM, 0);
    // TODO: Set the socket to non-blocking mode
    int flags = fcntl(discovery_fd, F_GETFL, 0);
    fcntl(discovery_fd, F_SETFL, flags | O_NONBLOCK);


    struct sockaddr_in discovery_address;
    // TODO: Initialize the sockaddr_in struct for the discovery server
    discovery_address.sin_family = AF_INET;
    discovery_address.sin_port = htons(8000);
    inet_pton(AF_INET, discovery_server_ip, &discovery_address.sin_addr);

    int status;
    // TODO: Connect to the discovery server (use connect_until_success)
    status = connect_until_success(discovery_fd, &discovery_address);

    // TODO: Initialize a UserMessage struct with the request opcode
    UserMessage request_msg = {0};
    request_msg.opcode = 0;
    
    // TODO: Encode the UserMessage struct into a byte array
    uint8_t *request_data = encode_user_message(&request_msg);

    // TODO: Send the UserMessage to the discovery server (use send_until_success)
    send_until_success(discovery_fd, request_data, sizeof(request_data));

    uint8_t chatters_data[sizeof(ChattersMessage)] = {0};
    // TODO: Receive a ChattersMessage from discovery server (use recv_until_success)
    recv_until_success(discovery_fd, chatters_data, sizeof(chatters_data));

    ChattersMessage* chatters_msg;
    // TODO: Decode the received ChattersMessage
    chatters_msg = decode_chatters_message(chatters_data, sizeof(chatters_data));

    // Copy the decoded ChattersMessage to the global chatters variable
    memcpy(&chatters, chatters_msg, sizeof(ChattersMessage));

    // Print the list of chatters
    int chatter_count = print_chatters(&chatters);

    // TODO: Close the connection to the discovery server
    close(discovery_fd);

    // If there are no chatters, return
    if (chatter_count == 0) {
        return 0;
    }

    printf("Select a chatter to send a message to: ");
    int chatter_index;
    // TODO: Read chatter index from stdin
    scanf("%d", &chatter_index);

    // If the chatter index is invalid, print an error message and return
    if (chatter_index < 0 || chatter_index >= 32) {
        fprintf(stderr, "Invalid chatter index\n");
        return 1;
    }

    int chat_fd;
    // TODO: Create a socket file descriptor for the selected chatter
    //       use AF_INET for ipv4 and SOCK_STREAM for TCP
    chat_fd = socket(AF_INET, SOCK_STREAM, 0 );

    // TODO: Set the socket to non-blocking mode
    flags = fcntl(chat_fd, F_GETFL, 0);
    fcntl(chat_fd, F_SETFL, flags | O_NONBLOCK);

    struct sockaddr_in chat_address;
    // TODO: Initialize the sockaddr_in struct for the selected chatter
    chat_address.sin_family = AF_INET;
    chat_address.sin_port = htons(chatters.users[chatter_index].port);
    inet_pton(AF_INET, chatters.users[chatter_index].address, &chat_address.sin_addr);

    // TODO: Connect to the selected chatter (use connect_until_success)
    connect_until_success(chat_fd, &chat_address);

    char buffer[256];
    printf("Enter message: ");
    clear_input_buffer();
    // TODO: Read message from stdin
    fgets(buffer, sizeof(buffer), stdin);

    size_t len = strlen(buffer);
    buffer[len - 1] = '\0';

    ChatMessage chat_msg;
    // TODO: Initialize a ChatMessage struct with the user's name, message, and timestamp in microseconds
    chat_msg.timestamp = get_time_us();
    strncpy(chat_msg.name, name, sizeof(chat_msg.name) - 1);
    strncpy(chat_msg.message, buffer, sizeof(chat_msg.message) - 1);

    uint8_t* chat_data;
    // TODO: Encode the ChatMessage struct into a byte array
    chat_data = encode_chat_message(&chat_msg);

    // TODO: Send the ChatMessage to the selected chatter (use send_until_success)
    send_until_success(chat_fd, chat_data, len);

    // TODO: Close the connection to the selected chatter
    close(chat_fd);
    
    // Free the public IP address
    free(public_ip);
}