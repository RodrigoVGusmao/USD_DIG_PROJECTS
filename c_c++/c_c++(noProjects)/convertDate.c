#include<stdio.h>
#include<stdint.h>
#include"getString.h"

const char month[36] = "jan"
                        "feb"
                        "mar"
                        "apr"
                        "may"
                        "jun"
                        "jul"
                        "aug"
                        "sep"
                        "oct"
                        "nov"
                        "dec"

typedef struct {
    uint16_t year;
    uint8_t month;
    uint8_t day;
}

int main()
{
    while(true)
    {
        char* szMonth = getString(' ');
    }
}
