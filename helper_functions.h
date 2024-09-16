#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

#include <assert.h>
#include <stdlib.h>

/* Helper function. Gets the bit at bitIndex from the string s. */
int getBit(char *s, unsigned int bitIndex);

int first_diff_loc(char *s1, int len1, char *s2, int len2, int * pComparedCnt);

void show_bit(char *s, int len);

// given an array s, set its location at bitIndex to be bit_value
void setBit(char * s, unsigned int bitIndex, unsigned int bit_value);

void left_shift(char * s, size_t byte_cnt, unsigned int n);

/* Allocates new memory to hold the numBits specified and fills the allocated
    memory with the numBits specified starting from the startBit of the oldKey
    array of bytes. */
char *createStem(char *oldKey, unsigned int startBit, unsigned int numBits);

int editDistance(char *str1, char *str2, int n, int m);

#endif