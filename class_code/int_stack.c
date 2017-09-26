//
//  main.c
//  os_compiler
//
//  Created by devontae.reid on 9/26/17.
//  Copyright © 2017 devontae.reid. All rights reserved.
//

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAXSTACK 4
#define UNDERFLOW 1
#define OVERFLOW 2

typedef struct simple_stack simple_stack;

struct simple_stack{
    size_t size;
    int data[MAXSTACK];
};

simple_stack* simple_stack_create(){
    simple_stack* ss = (simple_stack*)malloc(sizeof(simple_stack));
    return ss;

}
void simple_stack_delete(simple_stack* ss){
    free(ss);
}

void simple_stack_check_empty(simple_stack* ss){
    if(ss->size == 0){
        fprintf(stderr, "Stack is empty\n");
        exit(UNDERFLOW);
    }
}
void simple_stack_check_overflow(simple_stack* ss){
    if (ss->size == MAXSTACK) {
        fprintf(stderr, "Stack is full\n");
        exit(OVERFLOW);
    }
}

size_t simple_stack_size(simple_stack* ss) { return ss->size; }
bool simple_stack_empty(simple_stack* ss) { return ss->size == 0; }

int simple_stack_peek(simple_stack* ss){
    simple_stack_check_empty(ss);
    return ss->data[ss->size - 1];
}

void simple_stack_push(simple_stack* ss,int value){
    simple_stack_check_overflow(ss);
    ss->data[ss->size++] = value;
}
void simple_stack_pop(simple_stack* ss){
    simple_stack_check_empty(ss);
    ss->data[ss->size-- - 1] = 0;
}
void simple_stack_print(simple_stack* ss){
    simple_stack_check_empty(ss);
    
    for (size_t i = ss->size-1; i > 0; --i) {
        printf("%d->",ss->data[i]);
    }
    printf("%d\n",ss->data[0]);
}


int main() {
    
    simple_stack* ss = simple_stack_create();
    
    for (size_t i = 0; i < MAXSTACK; ++i) {
        simple_stack_push(ss, (int)(i+1));
    }
    
    simple_stack_print(ss);
    
    // POP 4
    simple_stack_pop(ss);
    simple_stack_print(ss);
    
    // POP 3
    simple_stack_pop(ss);
    simple_stack_print(ss);
    
    // POP 2
    simple_stack_pop(ss);
    simple_stack_print(ss);
    
    // POP 1
    simple_stack_pop(ss);
    simple_stack_print(ss);
    
    // EMPTY
    simple_stack_pop(ss);
    simple_stack_print(ss);
    
    return 0;
}

