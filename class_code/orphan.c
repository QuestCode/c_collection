//
//  orphan.c
//  os_compiler
//
//  Created by devontae.reid on 9/26/17.
//  Copyright © 2017 devontae.reid. All rights reserved.
//

#include<stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
    // Create a child process
    int pid = fork();
    if (pid > 0)
        printf("in parent process");
    // Note that pid is 0 in child process
    // and negative if fork() fails
    else if (pid == 0){
        sleep(30);
        printf("in child process");
    }
    return 0;
}
