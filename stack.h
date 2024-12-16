typedef struct Node {
    void *data;
    struct Node *prev;
} Node;

typedef struct Stack {
    Node *top;
} Stack;


void push(Stack *stack, void *data);

void *pop(Stack *stack);
