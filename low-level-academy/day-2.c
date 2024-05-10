#include <stdio.h>

int main() {
    int nameSize = 50;
    char name[nameSize];

    printf("Enter your name: ");
    fgets(name, nameSize, stdin);

    printf("Hello, %s\n", name);
}