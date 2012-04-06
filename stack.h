#ifndef STACK_H
#define STACK_H

#include <stdio.h>

typedef struct {
    int size;
    int top;
    int *data;
} Stack;

Stack * stack_new(void);
int stack_empty(Stack *s);
void stack_free(Stack *s);
void stack_push(Stack *s, int i);
unsigned int stack_size(Stack *s);
int stack_peek(Stack *s);
int stack_pop(Stack *s);

#endif /* end of include guard: STACK_H */
