#include <stdio.h>
#include <stdint.h>


int main(){
    int original = 16916;//0b0100001000010100;
    char offset = original & 255;
    char page = original >> 8; 
    printf("logical Address:%d\n",original);
    printf("Page           :%d\n",page);
    printf("offset         :%d\n",offset);

    original = 62493;//0b1111010000011101;
    offset = original & 255;
    page = original >> 8; 
    printf("logical Address:%d\n",original);
    printf("Page           :%d\n",page);
    printf("offset         :%d\n",offset);
    return 0;
}