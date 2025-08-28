#include <stdio.h>

#pragma pack(push, 1)
struct Object {
    char a;
    int b;
    char c;
    int d;
};
#pragma pack(pop)

void print_bytes(void *address, int num_bytes) {
    for (int i = 0; i < num_bytes; i++) {
        printf("%p: %02x\n", address + i, *((unsigned char *)address + i));
    }
}

int main() {
    struct Object object = {0};
    object.a = 2;
    object.b = 4;
    object.c = 8;
    object.d = 16;

    printf("Size of object: %lu\n", sizeof(object));

    void *address = &object;
    printf("Address of object: %p\n", address);
    printf("Address of a: %p (+%ld)\n", &object.a, (void*)&object.a - address);
    printf("Address of b: %p (+%ld)\n", &object.b, (void*)&object.b - address);
    printf("Address of c: %p (+%ld)\n", &object.c, (void*)&object.c - address);
    printf("Address of d: %p (+%ld)\n", &object.d, (void*)&object.d - address);

    print_bytes(&object, sizeof(object));

    struct Object array[16] = {0};
    printf("Size of array[16]: %lu\n", sizeof(array));

    return 0;
}