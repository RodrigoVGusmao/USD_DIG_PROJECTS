#include<stdio.h>
#include<stdint.h>
#include<string.h>
#include"getString.h"
#include"vector_c.h"

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
                        "dec";

typedef struct {
    uint16_t year;
    uint8_t month;
    uint8_t day;
} date;

int main()
{
    vector_v dates = NULL_v;
    initVector(&dates, 0, date, NULL);

    while(1)
    {
        date currentDate;
        char* szMonth = getString(' ');
        char* szSub = strstr(month, szMonth);
        if(szSub == 0)
            break;
        currentDate.month = (month-szSub)/3;
        currentDate.day = getLong(' ', )
    }
}
