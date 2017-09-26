//
//  zombie.c
//  os_compiler
//
//  Created by devontae.reid on 9/26/17.
//  Copyright © 2017 devontae.reid. All rights reserved.
//

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    pid_t pid = fork();
    if (pid < 0) {
        perror("Error forking");
    }
    if (pid == 0) { /* Child */
        printf("Child %d is sleeping\n", pid);
        sleep(10);
    } else { /* Parent */
        printf("(PID) : %d\n", pid);
    }
}
