#include "Patricia.h"
#include "helper_functions.h"

// #define debug_mode

/*assuming 0 to left and 1 to right with '\0' at the end, no words are prefixes of others*/

/********************************************************************************************************************************************/
inline static short is_patricia_external_node_pointer(Pointer p_to_node) {return p_to_node->type == EXTERNAL;}
inline static short is_patricia_internal_node_pointer(Pointer p_to_node) {return p_to_node->type == INTERNAL;}

/********************************************************************************************************************************************/
void patricia_create_internal_node(Pointer* ptr_to_patricia_pointer, Pointer ptr_to_node)
{
  Pointer p;
  p = (Pointer)malloc(sizeof(Node_Type));
#ifdef debug_mode
printf("comparing for determining father, lchild, rchild, first pointed by %p, second by %p:\n", *ptr_to_patricia_pointer, ptr_to_node);
show_bit((*ptr_to_patricia_pointer)->stem, (*ptr_to_patricia_pointer)->stem_len);
show_bit(ptr_to_node->stem, ptr_to_node->stem_len);
#endif
  p->type = INTERNAL;
  int bit_cnt_compared;
  int diff_loc = first_diff_loc((*ptr_to_patricia_pointer)->stem, ptr_to_node->stem_len, 
                                      ptr_to_node->stem, ptr_to_node->stem_len, &bit_cnt_compared);
#ifdef debug_mode
printf("diff_loc: %d\n", diff_loc);
printf("diff bit in stem pointed to by patricia_pointer: %u\n", getBit((*ptr_to_patricia_pointer)->stem, diff_loc));
#endif
  if(getBit((*ptr_to_patricia_pointer)->stem, diff_loc) == 0)
  {
    p->left = *ptr_to_patricia_pointer; p->right = ptr_to_node;
  }
  else
  {
    p->left = ptr_to_node; p->right = *ptr_to_patricia_pointer;
  }
  // prefix
  p->stem = createStem(p->left->stem, 0, diff_loc);
  p->stem_len = diff_loc;
  char * left_stem = createStem(p->left->stem, diff_loc, p->left->stem_len - diff_loc);
  char * right_stem = createStem(p->right->stem, diff_loc, p->right->stem_len - diff_loc);
  free(p->left->stem);
  p->left->stem = left_stem;
  p->left->stem_len = p->left->stem_len - diff_loc;
  free(p->right->stem);
  p->right->stem = right_stem;
  p->right->stem_len = p->right->stem_len - diff_loc;
#ifdef debug_mode
// printf("father: ");
// printf("p: %p, p->left: %p, p->right: %p\n", p, p->left, p->right);
// show_bit(p->stem, p->stem_len);
// printf("p->stem_len: %lu\n", p->stem_len);
// printf("left child: ");
// show_bit(p->left->stem, p->left->stem_len);
// printf("p->left: %p, \n", p->left);
// is_patricia_external_node_pointer(p->left) ? printf("pointing to an external\n") : printf("pointing to an internal\n");
// printf("p->left->stem_len: %lu\n", p->left->stem_len);
// printf("right child: ");
// show_bit(p->right->stem, p->right->stem_len);
// printf("p->right: %p, \n", p->right);
// is_patricia_external_node_pointer(p->right) ? printf("pointing to an external\n") : printf("pointing to an internal\n");
// printf("p->right->stem_len: %lu\n", p->right->stem_len);
#endif
  *ptr_to_patricia_pointer = p;
}

/***************************************************************************************************************************************/
// create a new external node that contains a string.
Pointer patricia_create_external_node(Key_Word word, char * stem, size_t stem_len, PATRICIA *p, MyInfo * p_to_info)
{
  *p = (Pointer)malloc(sizeof(Node_Type));
  (*p)->type = EXTERNAL;
  (*p)->stem = malloc(stem_len / BIT_PER_BYTE + 1);
  assert((*p)->stem);
  memcpy((*p)->stem, stem, stem_len / BIT_PER_BYTE + 1);
  (*p)->stem_len = stem_len;
  (*p)->left = (*p)->right = NULL;
  (*p)->record_cnt = 1;
  (*p)->array_of_p_to_record[0] = p_to_info;
  strcpy((*p)->whole_word, word);
#ifdef debug_mode
// printf("having created an external node containing %s pointed by %p\n", word, *p);
#endif
  return *p;
}

/********************************************************************************************************************************************/


void patricia_node_search(Key_Word searched_word, char * stem, size_t stem_len, PATRICIA patricia_pointer, Pointer *p_to_external_node_pointer)
{
  // its possible implement the height and comparison calculus here
  if (is_patricia_external_node_pointer(patricia_pointer)) // leaf, patricia_pointer->stem_len == diff_loc == stem_len
  {
    if (strcmp(searched_word, patricia_pointer->whole_word) == 0) //
    {
      *p_to_external_node_pointer = patricia_pointer; // This word was been found!
    }
    return;
  } //
  int bit_cnt_compared;
  int diff_loc = first_diff_loc(stem, stem_len, patricia_pointer->stem, patricia_pointer->stem_len, &bit_cnt_compared);

  if(diff_loc < patricia_pointer->stem_len) // stem and patricia_pointer->stem do not match, 
  {
    return;
  }
  /*patricia_pointer->stem_len == diff_loc < stem_len means 
  that there exists a word which is a prefix of the searched word, 
  in our setting, this cannot happen*/

  unsigned int aux_bit = getBit(stem, diff_loc);
  unsigned temp_stem_len = stem_len - diff_loc;
  char * temp_stem = createStem(stem, diff_loc, temp_stem_len);
  if (aux_bit == 0)
  {
    patricia_node_search(searched_word, temp_stem, temp_stem_len, patricia_pointer->left, p_to_external_node_pointer);
  }
  else
  {
    patricia_node_search(searched_word, temp_stem, temp_stem_len, patricia_pointer->right, p_to_external_node_pointer);
  }
  free(temp_stem);
}

static void collect_pointers(PATRICIA patricia_pointer, PATRICIA **addr_of_array_of_patricia_pointers, int* p_to_cand_node_pointer_cnt)
{
  if (is_patricia_external_node_pointer(patricia_pointer))
  {
    (*addr_of_array_of_patricia_pointers)[(*p_to_cand_node_pointer_cnt)++] = patricia_pointer;
    return;
  }
  if(patricia_pointer->left)
  {
    collect_pointers(patricia_pointer->left, addr_of_array_of_patricia_pointers, p_to_cand_node_pointer_cnt);
  }
  if(patricia_pointer->right)
  {
    collect_pointers(patricia_pointer->right, addr_of_array_of_patricia_pointers, p_to_cand_node_pointer_cnt);
  }
}

void patricia_locate_closest_match(Key_Word searched_word, char * stem, size_t stem_len, 
                                      PATRICIA patricia_pointer, Pointer *p_to_external_node_pointer, 
                                        int *pCompCnt, int *pNodeAccessCnt, int *pStringCompCnt)
{

  if (is_patricia_external_node_pointer(patricia_pointer)) // leaf
  {
    
    // if (strcmp(searched_word, patricia_pointer->whole_word) == 0) // 
    // {
#ifdef debug_mode
printf("having found an external node\n");
#endif

      if(stem_len > BIT_PER_BYTE)
      {
        int bit_cnt_compared;
        int diff_loc = first_diff_loc(stem, stem_len, patricia_pointer->stem, patricia_pointer->stem_len, &bit_cnt_compared);
// printf("---comparison for diff loc, %d bits compared\n", diff_loc);
// show_bit(stem, stem_len);
// show_bit(patricia_pointer->stem, patricia_pointer->stem_len);
        (*pCompCnt) += bit_cnt_compared;
        (*pNodeAccessCnt)++;
      }

      *p_to_external_node_pointer = patricia_pointer; // accept this word, even if it is not an exact match
#ifdef debug_mode
// printf("found a word %s\n", patricia_pointer->whole_word);
#endif
    // }
    return;
  } //

  if(stem_len <= BIT_PER_BYTE) // the final byte
  {
#ifdef debug_mode
// printf("no exact matches\n");
#endif
    PATRICIA* list_of_patricia_pointers = malloc(sizeof(PATRICIA) * MAX);
    assert(list_of_patricia_pointers);
    int pointer_cnt = 0;
    collect_pointers(patricia_pointer, &list_of_patricia_pointers, &pointer_cnt);
    int min_edist = __INT_MAX__;
    PATRICIA closest_node_pointer = NULL;
    for(int i = 0; i < pointer_cnt; i++)
    {
      // printf("%s\n", list_of_patricia_pointers[i]->whole_word);
      char * close_word = list_of_patricia_pointers[i]->whole_word;
      int eDist = editDistance(searched_word, close_word, strlen(searched_word), strlen(close_word));
      if(min_edist > eDist)
      {
        min_edist = eDist;
        closest_node_pointer = list_of_patricia_pointers[i];
      }
    }
    *p_to_external_node_pointer = closest_node_pointer;
    free(list_of_patricia_pointers);
    return;
  }

  (*pNodeAccessCnt)++;
  int bit_cnt_compared;
  int diff_loc = first_diff_loc(stem, stem_len, patricia_pointer->stem, patricia_pointer->stem_len, &bit_cnt_compared);
// printf("+++***comparison for diff loc, %d bits compared\n", diff_loc);
// show_bit(stem, stem_len);
// show_bit(patricia_pointer->stem, patricia_pointer->stem_len);
  (*pCompCnt) += bit_cnt_compared;
// printf("node access cnt: %d\n", *pNodeAccessCnt);
// printf("bit cnt compared in this time: %d\n", bit_cnt_compared);
#ifdef debug_mode
// show_bit(stem, stem_len);
// show_bit(patricia_pointer->stem, patricia_pointer->stem_len);
// printf("diff_loc: %d\n", diff_loc);
#endif

  if(diff_loc < patricia_pointer->stem_len) // do not match
  {
#ifdef debug_mode
// printf("no exact matches\n");
#endif
    PATRICIA* list_of_patricia_pointers = malloc(sizeof(PATRICIA) * MAX);
    assert(list_of_patricia_pointers);
    int pointer_cnt = 0;
    collect_pointers(patricia_pointer, &list_of_patricia_pointers, &pointer_cnt);
    int min_edist = __INT_MAX__;
    PATRICIA closest_node_pointer = NULL;
    for(int i = 0; i < pointer_cnt; i++)
    {
      // printf("%s\n", list_of_patricia_pointers[i]->whole_word);
      char * close_word = list_of_patricia_pointers[i]->whole_word;
      int eDist = editDistance(searched_word, close_word, strlen(searched_word), strlen(close_word));
      if(min_edist > eDist)
      {
        min_edist = eDist;
        closest_node_pointer = list_of_patricia_pointers[i];
      }
    }
    *p_to_external_node_pointer = closest_node_pointer;
    free(list_of_patricia_pointers);
    return;
  }

  unsigned int aux_bit = getBit(stem, diff_loc);
  unsigned temp_stem_len = stem_len - diff_loc;
  char * temp_stem = createStem(stem, diff_loc, temp_stem_len);

  if(temp_stem_len <= BIT_PER_BYTE)
  {
    PATRICIA* list_of_patricia_pointers = malloc(sizeof(PATRICIA) * MAX);
    assert(list_of_patricia_pointers);
    int pointer_cnt = 0;
    collect_pointers(patricia_pointer, &list_of_patricia_pointers, &pointer_cnt);
    int min_edist = __INT_MAX__;
    PATRICIA closest_node_pointer = NULL;
    for(int i = 0; i < pointer_cnt; i++)
    {
      // printf("%s\n", list_of_patricia_pointers[i]->whole_word);
      char * close_word = list_of_patricia_pointers[i]->whole_word;
      int eDist = editDistance(searched_word, close_word, strlen(searched_word), strlen(close_word));
      if(min_edist > eDist)
      {
        min_edist = eDist;
        closest_node_pointer = list_of_patricia_pointers[i];
      }
    }
    *p_to_external_node_pointer = closest_node_pointer;
    free(list_of_patricia_pointers);
    free(temp_stem);
    return;
  }

  if (aux_bit == 0)
  {
    patricia_locate_closest_match(searched_word, temp_stem, temp_stem_len, patricia_pointer->left, p_to_external_node_pointer, pCompCnt, pNodeAccessCnt, pStringCompCnt);
  }
  else
  {
    patricia_locate_closest_match(searched_word, temp_stem, temp_stem_len, patricia_pointer->right, p_to_external_node_pointer, pCompCnt, pNodeAccessCnt, pStringCompCnt);
  }
  free(temp_stem);
}

/******************************************************************************************************************************************/
void patricia_internal_insert(Key_Word word, char * stem, size_t stem_len, PATRICIA *ptr_to_patricia_pointer, MyInfo * p_to_info)
{
  Pointer ptr_to_new_external_node = NULL;

  if (is_patricia_external_node_pointer(*ptr_to_patricia_pointer))
  {
    int bit_cnt_compared;
    int diff_loc = first_diff_loc(stem, stem_len, (*ptr_to_patricia_pointer)->stem, (*ptr_to_patricia_pointer)->stem_len, &bit_cnt_compared);

    if (diff_loc == stem_len && diff_loc == (*ptr_to_patricia_pointer)->stem_len){
      (*ptr_to_patricia_pointer)->array_of_p_to_record[(*ptr_to_patricia_pointer)->record_cnt++] = p_to_info;
    }
    else { // 
      patricia_create_external_node(word, stem, stem_len, &ptr_to_new_external_node, p_to_info);
      patricia_create_internal_node(ptr_to_patricia_pointer, ptr_to_new_external_node);
    }
    free(stem);
    return;
  } 

  // point to interal node
  int bit_cnt_compared;
  int diff_loc = first_diff_loc(stem, stem_len, (*ptr_to_patricia_pointer)->stem, (*ptr_to_patricia_pointer)->stem_len, &bit_cnt_compared);
#ifdef debug_mode
printf("2. comparing for finding longest common prefix, first to be inserted, second lying in internal nodes\n");
show_bit(stem, stem_len);
show_bit((*ptr_to_patricia_pointer)->stem, (*ptr_to_patricia_pointer)->stem_len);
printf("len: %ld, %ld\n", stem_len, (*ptr_to_patricia_pointer)->stem_len);
printf("diff_loc: %d\n", diff_loc);
#endif
  if(diff_loc < (*ptr_to_patricia_pointer)->stem_len)
  {
    patricia_create_external_node(word, stem, stem_len, &ptr_to_new_external_node, p_to_info);
    patricia_create_internal_node(ptr_to_patricia_pointer, ptr_to_new_external_node);
    free(stem);
    return;
  }
  // problem here only if diff_loc == (*ptr_to_patricia_pointer) can we do the following

  unsigned int aux_bit = getBit(stem, diff_loc); // the bit beyond
  char *new_stem = createStem(stem, diff_loc, stem_len - diff_loc); // create a stem from the bit mentioned above
  size_t new_stem_len = stem_len - diff_loc; // the length of the stem above
#ifdef debug_mode
printf("new_stem: ");
show_bit(new_stem, new_stem_len);
printf("new_stem_len: %ld\n", new_stem_len);
#endif
  free(stem);
  if (aux_bit == 0)
  {
#ifdef debug_mode
printf("about to insert into the left subtree\n");
#endif
    patricia_internal_insert(word, new_stem, new_stem_len, &(*ptr_to_patricia_pointer)->left, p_to_info); // word should be shrunk
  }
  else
  {
#ifdef debug_mode
printf("about to insert into the right subtree\n");
#endif
    patricia_internal_insert(word, new_stem, new_stem_len, &(*ptr_to_patricia_pointer)->right, p_to_info); // word should be shrunk
  }
}

/******************************************************************************************************************************************/
void patricia_insert(Key_Word word, PATRICIA *p_to_tree, MyInfo * p_to_info)
{

  if (*p_to_tree == NULL)
  {
    (*p_to_tree) = patricia_create_external_node(word, word, (strlen(word) + 1) * BIT_PER_BYTE, p_to_tree, p_to_info);
    return;
  }

  // This tree already has some nodes
  PATRICIA p = *p_to_tree;
  unsigned int aux_bit;
  int diff_index;
  // initialize a stem from word
  char * q = malloc(strlen(word) + 1);
  assert(q);
  strcpy(q, word);
  size_t q_len = (strlen(word) + 1) * BIT_PER_BYTE;
#ifdef debug_mode
printf("to insert %s\n", word);
#endif
  while (p && !is_patricia_external_node_pointer(p)) // still have children and have not arrived at external nodes yet
  {
#ifdef debug_mode
printf("1. comparing for finding longest common prefix, first to be inserted, second lying in internal nodes\n");
show_bit(q, q_len);
show_bit(p->stem, p->stem_len);
#endif
    int bit_cnt_compared;
    diff_index = first_diff_loc(q, q_len, p->stem, p->stem_len, &bit_cnt_compared);
    aux_bit = getBit(q, diff_index);
#ifdef debug_mode
printf("diff_index, aux_bit: %d, %u\n", diff_index, aux_bit);
#endif
    if (aux_bit == 0)
    {
#ifdef debug_mode
printf("%s", "going left\n");
#endif
      p = p->left;
    }
    else // aux_bit == 1 
    {
#ifdef debug_mode
printf("%s", "going right\n");
#endif
      p = p->right;
    }
    if(q_len % BIT_PER_BYTE == 0)
      left_shift(q, q_len / BIT_PER_BYTE, diff_index);
    else
      left_shift(q, q_len / BIT_PER_BYTE + 1, diff_index);
    q_len -= diff_index;
  }
  free(q);
  // have reached leaf nodes
  if (strcmp(p->whole_word, word) == 0)
  {
#ifdef debug_mode
// printf("The tree already contain this keyword\n");
#endif
    p->array_of_p_to_record[p->record_cnt++] = p_to_info;
    return;
  }

  char * stem = malloc((strlen(word) + 1) * sizeof(char));
  size_t stem_len = (strlen(word) + 1) * sizeof(char) * BIT_PER_BYTE;
  strcpy(stem, word);
#ifdef debug_mode
printf("%s", "stem: ");
show_bit(stem, stem_len);
printf("stem_len: %lu\n", stem_len);
#endif
  patricia_internal_insert(word, stem, stem_len, p_to_tree, p_to_info);
}

/****************************************************************************************************/
void patricia_print_keyword_alphabetical_order(PATRICIA p)
{
  if (p == NULL)
    return;

  if (is_patricia_internal_node_pointer(p))
    patricia_print_keyword_alphabetical_order(p->left);

  if (is_patricia_external_node_pointer(p))
    printf("%s\n", p->whole_word);

  if (is_patricia_internal_node_pointer(p))
    patricia_print_keyword_alphabetical_order(p->right);
}

/****************************************************************************************************/
void patricia_print_full_info_alphabetical_order(PATRICIA p)
{
  if (p == NULL)
    return;

  if (is_patricia_internal_node_pointer(p))
    patricia_print_full_info_alphabetical_order(p->left);
    
  if (is_patricia_external_node_pointer(p))
  {
    for(int i = 0; i < p->record_cnt; i++)
    {
      print_info_2(p->array_of_p_to_record[i]);
    }
  }

  if (is_patricia_internal_node_pointer(p))
    patricia_print_full_info_alphabetical_order(p->right);
}


void destroy_patricia_tree(PATRICIA tree)
{
  if(!tree) return;
  free(tree->stem);
  destroy_patricia_tree(tree->left);
  destroy_patricia_tree(tree->right);
  free(tree);
}
