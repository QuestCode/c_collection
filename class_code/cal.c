//
//  cal.c
//  os_workstation
//
//  Created by Devontae Reid on 9/27/17.
//  Copyright © 2017 Devontae Reid. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#define COMMAND_LENGTH 128
#define MAX_LINES 80

int main(void) {
    
    char Command[COMMAND_LENGTH];
    int pid;
    char *argsv[MAX_LINES/2 + 1];
    int argsc;
    char *cmd;
    
    
    printf("cal sh> ");
    fgets(Command,COMMAND_LENGTH,stdin);
    Command[strlen(Command)-1] = '\0';
    cmd = strtok(Command," ");
    
    
    while (!strcmp(Command,"cal")) {
        
        
        pid = fork();
        
        if (pid == -1) {
            perror("Could not fork");
            exit(EXIT_FAILURE);
        }
        
        
        if (pid == 0) { // In the child
            argsc = 0;
            argsv[argsc] = strtok(Command," ");
            
            
            
            while (argsv[argsc++] != NULL) {
                argsv[argsc] = strtok(NULL," ");
            }
            
            execvp(argsv[0],argsv);
            perror("Error in exec");
            exit(EXIT_FAILURE);
            
        } else { // Sleep to allow child to execute
            
            sleep(5);
            printf("cal sh> ");
            fgets(Command,COMMAND_LENGTH,stdin);
            Command[strlen(Command)-1] = '\0';
        }
    }
    
    return(EXIT_SUCCESS);
}
