#include "stack.h"

#include <stdlib.h>

Stack * stack_new(void)
{
    Stack *s = malloc(sizeof(Stack));
    s->size = 1;
    s->top = 0;
    s->data = malloc(sizeof(int) * s->size);
    return s;
}

int stack_empty(Stack *s)
{
    return s->top == 0;
}

void stack_free(Stack *s)
{
    free(s->data);
    free(s);
}

unsigned int stack_size(Stack *s)
{
    return s->top;
}

void stack_push(Stack *s, int i)
{
    if (s->top >= s->size) {
        s->size = s->size * 2;
        s->data = realloc(s->data, s->size * sizeof(int));
    }
    s->data[s->top++] = i;
}

int stack_peek(Stack *s)
{
    return s->data[s->top-1];
}

int stack_pop(Stack *s)
{
    s->top--;
    return s->data[s->top];
}
