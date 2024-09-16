/*MyDualCircularLinkedListsWithSentinels.h*/    
#ifndef MY_INFO_H
#define MY_INFO_H

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <string.h>

#define OK 1 // 涉及指针和内存分配的函数，运行正常返回
#define ERROR 0 // 运行异常返回

#define MAX_FIELD_LEN 128

#define BIT_PER_BYTE 8

typedef struct MyInfo{
    int comp20003_code; // 0 means invalid
    int official_suburb_code; // 0 means invalid
    char official_suburb_name[MAX_FIELD_LEN]; // empty string means invalid
    int year; // 0 means invalid
    char official_state_code[MAX_FIELD_LEN]; // empty string means invalid
    char official_state_name[MAX_FIELD_LEN]; // empty string means invalid
    char official_local_gov_area_code[MAX_FIELD_LEN]; // empty string means invalid
    char official_local_gov_area_name[MAX_FIELD_LEN]; // empty string means invalid
    double latitude; // DBL_MAX means invalid
    double longitude; // DBL_MAX means invalid
} MyInfo;

inline static void initialize_info(MyInfo *ptr_to_info)
{
    ptr_to_info->comp20003_code = 0;
    ptr_to_info->official_suburb_code = 0;
    ptr_to_info->official_suburb_name[0] = '\0';
    ptr_to_info->year = 0;
    ptr_to_info->official_state_code[0] = '\0';
    ptr_to_info->official_state_name[0] = '\0';
    ptr_to_info->official_local_gov_area_code[0] = '\0';
    ptr_to_info->official_local_gov_area_name[0] = '\0';
    ptr_to_info->latitude = DBL_MAX;
    ptr_to_info->longitude = DBL_MAX;
}

/*给定record，输出各个成员的值*/
inline static int print_info_1(MyInfo * ptr_to_info)
{
    printf("COMP20003 Code: %d, ", ptr_to_info->comp20003_code);
    printf("Official Code Suburb: %d, Official Name Suburb: %s, ", ptr_to_info->official_suburb_code, ptr_to_info->official_suburb_name);
    printf("Year: %d, ", ptr_to_info->year);
    printf("Official Code State: %s, Official Name State: %s, ", ptr_to_info->official_state_code, ptr_to_info->official_state_name);
    printf("Official Code Local Government Area: %s, Official Name Local Government Area: %s, ", 
            ptr_to_info->official_local_gov_area_code, ptr_to_info->official_local_gov_area_name);
    printf("Latitude: %.7lf, Longitude: %.7lf\n", ptr_to_info->latitude, ptr_to_info->longitude);
    return 0;
}

inline static int print_info_2(MyInfo * ptr_to_info)
{
    printf("COMP20003 Code: %d,", ptr_to_info->comp20003_code);
    printf("Official Code Suburb: %d, Official Name Suburb: %s,", ptr_to_info->official_suburb_code, ptr_to_info->official_suburb_name);
    printf("Year: %d,", ptr_to_info->year);
    printf("Official Code State: %s, Official Name State: %s,", ptr_to_info->official_state_code, ptr_to_info->official_state_name);
    // char *p = strchr(ptr_to_info->official_local_gov_area_code, ' ');
    // if(p && strchr(p + 1, ' '))
    if(strstr(ptr_to_info->official_local_gov_area_code, ", "))
        printf("Official Code Local Government Area: \"%s\",", ptr_to_info->official_local_gov_area_code);
    else
        printf("Official Code Local Government Area: %s,", ptr_to_info->official_local_gov_area_code);

    // p = strchr(ptr_to_info->official_local_gov_area_name, ' ');
    // if(p && strchr(p + 1, ' '))
    if(strstr(ptr_to_info->official_local_gov_area_name, ", "))
        printf("Official Name Local Government Area: \"%s\",", ptr_to_info->official_local_gov_area_name);
    else
        printf("Official Name Local Government Area: %s,", ptr_to_info->official_local_gov_area_name);

    printf("Latitude: %.7lf,Longitude: %.7lf\n", ptr_to_info->latitude, ptr_to_info->longitude);
    return 0;
}

/*给定record，输出各个成员的值到文件中*/
inline static int fprint_info_1(MyInfo * ptr_to_info, FILE *afp)
{
    fprintf(afp, "COMP20003 Code: %d, ", ptr_to_info->comp20003_code);
    fprintf(afp, "Official Code Suburb: %d, Official Name Suburb: %s, ", ptr_to_info->official_suburb_code, ptr_to_info->official_suburb_name);
    fprintf(afp, "Year: %d, ", ptr_to_info->year);
    fprintf(afp, "Official Code State: %s, Official Name State: %s, ", ptr_to_info->official_state_code, ptr_to_info->official_state_name);
    fprintf(afp, "Official Code Local Government Area: %s, Official Name Local Government Area: %s, ", 
        ptr_to_info->official_local_gov_area_code, ptr_to_info->official_local_gov_area_name);
    // fprintf(afp, "Official Code Local Government Area: %s, Official Name Local Government Area: %s, ", 
    //     ptr_to_info->official_local_gov_area_code, ptr_to_info->official_local_gov_area_name);
    fprintf(afp, "Latitude: %.7lf, Longitude: %.7lf\n", ptr_to_info->latitude, ptr_to_info->longitude);
    return 0;
}

inline static int fprint_info_2(MyInfo * ptr_to_info, FILE *afp)
{
    fprintf(afp, "COMP20003 Code: %d, ", ptr_to_info->comp20003_code);
    fprintf(afp, "Official Code Suburb: %d, Official Name Suburb: %s, ", ptr_to_info->official_suburb_code, ptr_to_info->official_suburb_name);
    fprintf(afp, "Year: %d, ", ptr_to_info->year);
    fprintf(afp, "Official Code State: %s, Official Name State: %s, ", ptr_to_info->official_state_code, ptr_to_info->official_state_name);
    // char *p = strchr(ptr_to_info->official_local_gov_area_code, ' ');
    // if(p && strchr(p + 1, ' '))
    if(strstr(ptr_to_info->official_local_gov_area_code, ", "))
        fprintf(afp, "Official Code Local Government Area: \"%s\", ", ptr_to_info->official_local_gov_area_code);
    else
        fprintf(afp, "Official Code Local Government Area: %s, ", ptr_to_info->official_local_gov_area_code);
    // p = strchr(ptr_to_info->official_local_gov_area_name, ' ');
    // if(p && strchr(p + 1, ' '))
    if(strstr(ptr_to_info->official_local_gov_area_name, ", "))
        fprintf(afp, "Official Name Local Government Area: \"%s\", ", ptr_to_info->official_local_gov_area_name);
    else
        fprintf(afp, "Official Name Local Government Area: %s, ", ptr_to_info->official_local_gov_area_name);
    
    fprintf(afp, "Latitude: %.7lf, Longitude: %.7lf\n", ptr_to_info->latitude, ptr_to_info->longitude);
    return 0;
}

/*给定record，输出各个成员的值到文件中*/
inline static int fprint_line_1(MyInfo * ptr_to_info, FILE *afp)
{
    fprintf(afp, "%d, ", ptr_to_info->comp20003_code);
    fprintf(afp, "%d, %s, ", ptr_to_info->official_suburb_code, ptr_to_info->official_suburb_name);
    fprintf(afp, "%d, ", ptr_to_info->year);
    fprintf(afp, "%s, %s, ", ptr_to_info->official_state_code, ptr_to_info->official_state_name);
    fprintf(afp, "%s, %s, ", ptr_to_info->official_local_gov_area_code, ptr_to_info->official_local_gov_area_name);
    fprintf(afp, "%.7lf, %.7lf\n", ptr_to_info->latitude, ptr_to_info->longitude);
    return 0;
}

/*给定record，输出各个成员的值到文件中*/
inline static int fprint_line_2(MyInfo * ptr_to_info, FILE *afp)
{
    fprintf(afp, "%d,", ptr_to_info->comp20003_code);
    fprintf(afp, "%d,%s,", ptr_to_info->official_suburb_code, ptr_to_info->official_suburb_name);
    fprintf(afp, "%d,", ptr_to_info->year);
    fprintf(afp, "%s,%s,", ptr_to_info->official_state_code, ptr_to_info->official_state_name);

    // char *p = strchr(ptr_to_info->official_local_gov_area_code, ' ');
    // if(p && strchr(p + 1, ' '))
    if(strstr(ptr_to_info->official_local_gov_area_code, ", "))
        fprintf(afp, "\"%s\",", ptr_to_info->official_local_gov_area_code);
    else
        fprintf(afp, "%s,", ptr_to_info->official_local_gov_area_code);

    // p = strchr(ptr_to_info->official_local_gov_area_name, ' ');
    // if(p && strchr(p + 1, ' '))
    if(strstr(ptr_to_info->official_local_gov_area_name, ", "))
        fprintf(afp, "\"%s\",", ptr_to_info->official_local_gov_area_name);
    else
        fprintf(afp, "%s,", ptr_to_info->official_local_gov_area_name);

    fprintf(afp, "%.7lf,%.7lf\n", ptr_to_info->latitude, ptr_to_info->longitude);
    return 0;
}


#endif