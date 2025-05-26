#include <stdio.h>

int main() {
    for(int i = 0; i < 10; i++) { 
        printf("Outer loop\n");
        
        for(int j = 0; j < 5; j++) { 
            printf("Inner loop\n");
        }
    }

    for(int k = 0; k < 3; k++) {
        printf("Another loop without nesting\n");
    }

    return 0;
}
