#include "helper_functions.h"
#include <stdio.h>

#define debug_mode

/* Number of bits in a single character. */
#define BITS_PER_BYTE 8
/* Helper function. Gets the bit at bitIndex from the string s. */
int getBit(char *s, unsigned int bitIndex){
    assert(s && bitIndex >= 0);
    unsigned int byte = bitIndex / BITS_PER_BYTE;
    unsigned int indexFromLeft = bitIndex % BITS_PER_BYTE;
    /* 
        Since we split from the highest order bit first, the bit we are interested
        will be the highest order bit, rather than a bit that occurs at the end of the
        number. 
    */
    unsigned int offset = (BITS_PER_BYTE - indexFromLeft - 1) % BITS_PER_BYTE;
    unsigned char byteOfInterest = s[byte];
    unsigned int offsetMask = 1 << offset;
    unsigned int maskedByte = byteOfInterest & offsetMask;
    /*
        The masked byte will still have the bit in its original position, to return
        either 0 or 1, we need to move the bit to the lowest order bit in the number.
    */
    unsigned int bitOnly = maskedByte >> offset;
    return bitOnly;
}

// given an array s, set its location at bitIndex to be bit_value
void setBit(char * s, unsigned int bitIndex, unsigned int bit_value)
{
    assert(s && bitIndex >= 0);
    unsigned int byte = bitIndex / BITS_PER_BYTE;
    unsigned int indexFromLeft = bitIndex % BITS_PER_BYTE;
    unsigned int offset = (BITS_PER_BYTE - indexFromLeft - 1) % BITS_PER_BYTE;
    unsigned int offsetMask = 1 << offset;
    if(bit_value) // set 1
    {
        s[byte] |= offsetMask;
    }
    else // set 0
    {
        s[byte] &= (~offsetMask);
    }
}

int first_diff_loc(char *s1, int len1, char *s2, int len2, int * pComparedCnt)
{
#ifdef debug_mode
// printf("in first_diff_loc()\n");
// printf("len1, len2: %d, %d\n", len1, len2);
#endif
    int i, j;
    int min_len = len1 < len2 ? len1 : len2;
    *pComparedCnt = 0;
    for(i = 0; i < min_len; i++)
    {
#ifdef debug_mode
// printf("s1 at %d is %u, s2 at %d is %u\n", i, getBit(s1, i), i, getBit(s2, i));
#endif
        if(getBit(s1, i) == getBit(s2, i)) 
        {
            (*pComparedCnt)++;
            continue;
        }
        (*pComparedCnt)++;
        break;
    }
    return i;
}

void left_shift(char * s, size_t byte_cnt, unsigned int n)
{
#ifdef debug_mode
// printf("in left_shift()\n");
#endif
    for(int i = 0; i < n; i++) // n times
    {
        for(unsigned int j = 0; j < byte_cnt; j++) // iterate over all bytes
        {
            s[j] <<= 1;
            if(j < byte_cnt - 1)
                setBit(&s[j], BITS_PER_BYTE - 1, getBit(&s[j+1], 0));
        }
    }
}

void show_bit(char *s, int len)
{
    for(int i = 0; i < len; i++)
    {
        printf("%d", getBit(s, i));
    }
    printf("\n");
}
/*************************/

/* Allocates new memory to hold the numBits specified and fills the allocated
    memory with the numBits specified starting from the startBit of the oldKey
    array of bytes. */
char *createStem(char *oldKey, unsigned int startBit, unsigned int numBits){
    assert(numBits > 0 && startBit >= 0 && oldKey);
    int extraBytes = 0;
    if(numBits % BITS_PER_BYTE > 0){
        extraBytes = 1;
        }
    int totalBytes = (numBits / BITS_PER_BYTE) + extraBytes;
    char *newStem = malloc(sizeof(char) * totalBytes);
    assert(newStem);
    for(unsigned int i = 0; i < totalBytes; i++){
        newStem[i] = 0;
    }
    for(unsigned int i = 0; i < numBits; i++){
        unsigned int indexFromLeft = i % BITS_PER_BYTE;
        unsigned int offset = (BITS_PER_BYTE - indexFromLeft - 1) % BITS_PER_BYTE;
        unsigned int bitMaskForPosition = 1 << offset;
        unsigned int bitValueAtPosition = getBit(oldKey, startBit + i);
        unsigned int byteInNewStem = i / BITS_PER_BYTE;
        newStem[byteInNewStem] |= bitMaskForPosition * bitValueAtPosition;
    }
    return newStem;
}


static int min(int a, int b, int c);

/* Returns min of 3 integers 
    reference: https://www.geeksforgeeks.org/edit-distance-in-c/ */
int min(int a, int b, int c) {
    if (a < b) {
        if(a < c) {
            return a;
        } else {
            return c;
        }
    } else {
        if(b < c) {
            return b;
        } else {
            return c;
        }
    }
}

/* Returns the edit distance of two strings
    reference: https://www.geeksforgeeks.org/edit-distance-in-c/ */
int editDistance(char *str1, char *str2, int n, int m){
    assert(m >= 0 && n >= 0 && (str1 || m == 0) && (str2 || n == 0));
    // Declare a 2D array to store the dynamic programming
    // table
    int dp[n + 1][m + 1];

    // Initialize the dp table
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= m; j++) {
            // If the first string is empty, the only option
            // is to insert all characters of the second
            // string
            if (i == 0) {
                dp[i][j] = j;
            }
            // If the second string is empty, the only
            // option is to remove all characters of the
            // first string
            else if (j == 0) {
                dp[i][j] = i;
            }
            // If the last characters are the same, no
            // modification is necessary to the string.
            else if (str1[i - 1] == str2[j - 1]) {
                dp[i][j] = min(1 + dp[i - 1][j], 1 + dp[i][j - 1],
                    dp[i - 1][j - 1]);
            }
            // If the last characters are different,
            // consider all three operations and find the
            // minimum
            else {
                dp[i][j] = 1 + min(dp[i - 1][j], dp[i][j - 1],
                    dp[i - 1][j - 1]);
            }
        }
    }

    // Return the result from the dynamic programming table
    return dp[n][m];
}