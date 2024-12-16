#include <stdio.h>
#include <stdlib.h>

#include "stack.h"


void push(Stack *stack, void *data) {
    Node *n = malloc(sizeof(Node));
    n->data = data;
    n->prev = stack->top;
    stack->top = n;
}

void *pop(Stack *stack) {
    Node *popped = stack->top;
    void *data = popped->data;
    stack->top = stack->top->prev;
    free(popped);
    return data;
}

int main(int argc, char const *argv[]) {
    int n = 6;
    char data[] = {'c', 'a', 'n', 'a', 'a', 'n'};

    Stack s = {
        .top = NULL
    };

    for (int i = 0; i < n; i++) {
        push(&s, &(data[i]));
    }

    for (int i = 0; i < n; i++) {
        char *c = (char*) pop(&s);
        printf("%c\n", *c);
    }
}
