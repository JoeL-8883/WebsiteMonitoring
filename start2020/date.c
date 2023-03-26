#include "date.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
typedef struct date Date;

// Takes string input but converts to int
struct date {
    char *datestr;
    int day;
    int month;
    int year;
};


Date *date_create(char* datestr) {
    struct date *d;

    if ((d = (struct date *)malloc(sizeof(struct date))) != NULL) {
        // Split input into day, month and year then convert to int
        sscanf(datestr, "%d/%d/%d", &d->day, &d->month, &d->year); 

    }

    return d;
}

Date *date_duplicate(Date *d) {
    Date *duplicated_date;

    // Assign values of d to created date
    if ((duplicated_date = (struct date *)malloc(sizeof(struct date))) != NULL) {
        duplicated_date->datestr = d->datestr;
        duplicated_date->day = d->day;
        duplicated_date->month = d->month;
        duplicated_date->year = d->year;

    }
    return duplicated_date;
}
int date_compare(Date *d1, Date *d2) {
    if (d1->year - d2->year != 0) { 
        return d1->year - d2->year;
    } else if(d1->month - d2->month) {
        return d1->month - d2->month;
    } else if(d1->day < d2->day) {
        return d1->day - d2->day;
    } else {
        return 0;
    }
}

void date_destroy(Date *d) {
    free(d);
}

