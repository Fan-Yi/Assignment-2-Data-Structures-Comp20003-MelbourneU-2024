/*MyInfo.c*/
#include "MyInfo.h"
#include <string.h>

#define HEAD_LINE "COMP20003 Code,Official Code Suburb,Official Name Suburb,Year,Official Code State,Official Name State,\
Official Code Local Government Area,Official Name Local Government Area,Latitude,Longitude"


inline static int int_match(int n1, int n2) // 0 match any int
{
    if(n1 && n2 && n1 != n2)
    {
        return 0;
    }
    return 1;
}

inline static int year_match(int n1, int n2)
{
    // printf("determining year match, n1: %d, n2: %d\n", n1, n2);
    return int_match(n1, n2);
}

inline static int double_match(double d1, double d2) // DBL_MAX match any dbl
{
    if(d1 != DBL_MAX && d2 != DBL_MAX && d1 != d2) // literals of Type Double with few sinigficant digits can be saved exactly
    {
        return 0;
    }
    return 1;
}

inline static int str_match(char* s1, char* s2) // empty string can match any string
{
    if(*s1 && *s2 && strcmp(s1, s2))
    {
        return 0;
    }
    return 1;
}

inline static int official_suburb_name_match(char* s1, char* s2, int *pCompCnt)
{
    *pCompCnt = 0;
    if(!s1 || !s2)
    {
        return 1;
    }
    char * t1 = s1, *t2 = s2;

    while(*t1 && *t2)
    {
        (*pCompCnt)++;
        if(*t1 != *t2)
        {
            return 0;
        }
        t1++; t2++;
    }
    if(*t1 || *t2)
    {
        (*pCompCnt)++;
        return 0;
    }
    (*pCompCnt)++;
    return 1;    
}

inline static int info_match(MyInfo* p, MyInfo* q, int * pCompCnt)
{
    // check comp20003_code
    if(!int_match(p->comp20003_code, q->comp20003_code)) // 
    {
        return 0;
    }
// printf("comp20003_code match\n");
    // check official_suburb_code
    if(!int_match(p->official_suburb_code, q->official_suburb_code))
    {
        return 0;
    }
// printf("official_suburb_code match\n");
    // check official_suburb_name
// printf("checking info match:\n");
// printf("%s\n", p->official_suburb_name);
// printf("%s\n", q->official_suburb_name);
    if(!official_suburb_name_match(p->official_suburb_name, q->official_suburb_name, pCompCnt))
    {
        return 0;
    }
// printf("official_suburb_name match\n");
    // check year
    if(!year_match(p->year, q->year))
    {
        return 0;
    }
// printf("year match\n");
    // check official_state_code
    if(!str_match(p->official_state_code, q->official_state_code))
    {
        return 0;
    }
    // check official_state_name
    if(!str_match(p->official_state_name, q->official_state_name))
    {
        return 0;
    }
// printf("official_state_name match\n");
    // check official_local_gov_area_code
    if(!str_match(p->official_local_gov_area_code, q->official_local_gov_area_code))
    {
        return 0;
    }
    // check official_local_gov_area_name
    if(!str_match(p->official_local_gov_area_name, q->official_local_gov_area_name))
    {
        return 0;
    }
// printf("official_local_gov_area_name match\n");
    // check latitude
    if(!double_match(p->latitude, q->latitude))
    {
        return 0;
    }
    // check longitude
    if(!double_match(p->longitude, q->longitude))
    {
        return 0;
    }
    return 1;
}