#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//-----------------------------------------------------------------------------
int main(void) {

    printf("The nice value for this process is %d\n",nice(0));
    printf("The nice value for this process is %d\n",nice(5));
    printf("The nice value for this process is %d\n",nice(-5));

    return(EXIT_SUCCESS);
}
