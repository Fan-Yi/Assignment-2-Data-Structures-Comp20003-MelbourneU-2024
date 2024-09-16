#ifndef PATRICIA_H
#define PATRICIA_H

#include <stdlib.h>  
#include <stdio.h> 
#include <string.h>
#include <sys/time.h>
#include "MyInfo.h"

#define Size 128 
#define MAX 128

typedef char Key_Word[Size];
typedef unsigned int IndexType;
typedef enum { 
  INTERNAL, EXTERNAL
} TypeNode; 

typedef struct Node_Type *Pointer; 
typedef struct Node_Type {

  TypeNode type;

  // IndexType index;
  Pointer left, right;  
  /* This char "Char_to_compare" that will be used to compare the key in the insertion moment.  
  if this char is bigger than the char in the tree, will be allocated to right.  
  Otherwise, it will be allocated to the left side */
  // char char_to_compare; 
  char *stem;
  size_t stem_len; 
  Key_Word whole_word;
  Key_Word bit_tail; // external nodes contain words
  MyInfo* array_of_p_to_record[MAX];
  size_t record_cnt;
}Node_Type; 
 
typedef Pointer PATRICIA; 
/** 
 *  \brief Verify if the node is an internal node
 *  \brief Verify if the node is an  external node
 *  \param Node The node that will be verified.   
 * 
 **/  
inline static short is_patricia_internal_node_pointer(Pointer p_to_node);
inline static short is_patricia_external_node_pointer(Pointer p_to_node); 
/** 
 *  \brief Creates an internal node, this node contains index like the main information and is used to compare with other nodes
 *  \param  Left  
 *  \param Right 
 **/
void patricia_create_internal_node(Pointer *ptr_to_patricia_tree, Pointer ptr_to_node);    
/** 
 *  \brief Create an external
 **/
Pointer patricia_create_external_node(Key_Word word, char * stem, size_t stem_len, PATRICIA *p, MyInfo * p_to_info); 
/** 
 *  \brief This function is used to find a wished node 
 **/ 
void patricia_node_search(Key_Word searched_word, char *stem, size_t stem_len, PATRICIA p, Pointer *p_to_external_node); 
/** 
 *  \brief This function is an encapsulation of the PATRICIA Insert function, it is in this function that the entire decision, in relation to the type of node, that will be taken
 **/ 

void patricia_locate_closest_match(Key_Word searched_word, char * stem, size_t stem_len, PATRICIA patricia_pointer, Pointer *p_to_external_node, 
                                      int * pCompCnt, int *pNodeAccessCnt, int *pStringCompCnt);

void patricia_internal_insert(Key_Word word, char * stem, size_t stem_len, PATRICIA *ptr_to_patricia_pointer, MyInfo * p_to_info); 
/** 
 *  \brief The function of insertion, specifically the prime function, in this occurs the encapsulation of the internal function 
 **/   
void patricia_insert(Key_Word word, PATRICIA *tree, MyInfo * p_to_info);
/** 
 *  
 **/  
void patricia_print_keyword_alphabetical_order(PATRICIA p);
/** 
 *  
 **/  
void patricia_print_full_info_alphabetical_order(PATRICIA p);

void destroy_patricia_tree(PATRICIA tree);
#endif