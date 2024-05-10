#include <stdio.h>

int main() {
    int age;

    printf("Enter your age: ");
    scanf("%d", &age);

    if (age >= 18) {
        printf("You're eligible to vote!\n");
    } else {
        int diff = 18 - age;
        printf("You're not eligible to vote yet. Come back in %d years.\n", diff);
    }
}