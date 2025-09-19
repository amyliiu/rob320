#pragma once

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#pragma pack(push, 1)
typedef struct {
    long long timestamp;
    char message[256];
    char name[32];
} ChatMessage;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    unsigned short port;
    char address[16];
    char name[32];
} User;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    int opcode; // 0: request, 1: register, 2: deregister
    User user;
} UserMessage;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    User users[32];
} ChattersMessage;
#pragma pack(pop)

int print_chatters(ChattersMessage* msg) {
    printf("Chatters:\n");
    int chatter_count = 0;
    for (int i = 0; i < 32; i++) {
        if (msg->users[i].port != 0) {
            printf("[%d] : %s (%s:%d)\n", i, msg->users[i].name, msg->users[i].address, msg->users[i].port);
            chatter_count++;
        }
    }
    if (chatter_count == 0) {
        printf("No chatters!\n");
    }
    return chatter_count;
}

uint8_t* encode_chat_message(ChatMessage* msg) {
    // TODO: Cast the ChatMessage pointer to a uint8_t pointer so that the
    //       message can be transported as bytes
    uint8_t *res = malloc(sizeof(ChatMessage));
    memcpy(res, msg, sizeof(ChatMessage));
    return res;
}

ChatMessage* decode_chat_message(uint8_t* data, size_t len) {
    // TODO: Cast the uint8_t pointer to a ChatMessage pointer so that the
    //       message can be read as a ChatMessage struct
    //       If the length of the data is not equal to the size of the
    //       ChatMessage struct, return NULL

    if(len !=sizeof(ChatMessage)) {
        return NULL;
    }
    ChatMessage *res = malloc(sizeof(ChatMessage));
    memcpy(res, data, sizeof(ChatMessage));
    return res;
}

uint8_t* encode_user_message(UserMessage* msg) {
    // TODO: Cast the UserMessage pointer to a uint8_t pointer so that the
    //       message can be transported as bytes
    uint8_t *res = malloc(sizeof(UserMessage));
    memcpy(res, msg, sizeof(UserMessage));
    return res;
}

UserMessage* decode_user_message(uint8_t* data, size_t len) {
    // TODO: Cast the uint8_t pointer to a UserMessage pointer so that the
    //       message can be read as a UserMessage struct
    //       If the length of the data is not equal to the size of the
    //       UserMessage struct, return NULL
    if(len != sizeof(UserMessage)) {
        return NULL;
    }
    UserMessage *res = malloc(sizeof(UserMessage));
    memcpy(res, data, sizeof(UserMessage));
    return res;
}

uint8_t* encode_chatters_message(ChattersMessage* msg) {
    // TODO: Cast the ChattersMessage pointer to a uint8_t pointer so that the
    //       message can be transported as bytes
    uint8_t *res = malloc(sizeof(ChattersMessage));
    memcpy(res, msg, sizeof(ChattersMessage));
    return res;
}

ChattersMessage* decode_chatters_message(uint8_t* data, size_t len) {
    // TODO: Cast the uint8_t pointer to a ChattersMessage pointer so that the
    //       message can be read as a ChattersMessage struct
    //       If the length of the data is not equal to the size of the
    //       ChattersMessage struct, return NULL
    if(len != sizeof(ChattersMessage)){
        return NULL;
    }
    ChattersMessage *res = malloc(sizeof(ChattersMessage));
    memcpy(res, data, sizeof(ChattersMessage));
    return res;
}

