#include "scalc.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]){
    puts("--------Songer's Calculator--------");
    if(argc < 2){
        puts("\tUsage: scalc \"3*(2+7.342)+7/2\"");
        puts("No input, exit.");
        
        return 1;
    }
    
    //char *test = "-23*(7.42+5)/8";
    printf("---------Result is %f---------\n", parse_expression(argv[1], strlen(argv[1])));

    return 0;
}