//
//  factorial.c
//  os_workstation
//
//  Created by Devontae Reid on 10/2/17.
//  Copyright © 2017 Devontae Reid. All rights reserved.
//

#include <stdio.h>


int factorial(int n){
    if (n < 1) {
        return 1;
    }else {
        return n * factorial(n-1);
    }
}

int main() {
    
    printf("%d\n",factorial(4));
    return 0;
}
