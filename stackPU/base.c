#include "../outer/stack_canary.h"

stack_t atoi_my(char* start){
    stack_t res = 0;
    while (start[0] >= '0' && start[0] <= '9'){
        res = res*10 + start[0] - '0';
        start++;
    }
    return res;
}

int parse_line(char* buffer, Stack* stack){
    stack_t temp = 0;
    switch (buffer[0]) {
        case 'P':
            stack_push(stack, atoi_my(buffer + 5));
            break;
        case 'O':
            printf("%d\n", stack_pop(stack));
            break;
        case 'A':
            temp = stack_pop(stack);
            temp = stack_pop(stack) + temp;
            stack_push(stack, temp);
            break;
        case 'D':
            temp = stack_pop(stack);
            temp = stack_pop(stack) / temp;
            stack_push(stack, temp);
            break;
        case 'S':
            temp = stack_pop(stack);
            temp = stack_pop(stack) - temp;
            stack_push(stack, temp);
            break;
        case 'M':
            temp = stack_pop(stack);
            temp = stack_pop(stack) * temp;
            stack_push(stack, temp);
            break;
        case 'R':
            temp = stack_pop(stack);
            printf("%d\n", temp);
            return 1;
    }
    return 0;
}

int main(){

    Stack stack = {};
    stack_create(&stack, 0);

    char buffer[100] = {};

    while (fgets(buffer, sizeof(buffer), stdin) != NULL){
        parse_line(buffer, &stack);
    }

    // printf("%zu", sizeof(long long));
}