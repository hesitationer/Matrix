/* This file is part of the OpenMV project.
   Copyright (c) 2013-2017 Ibrahim Abdelkader <iabdalkader@openmv.io> & Kwabena W. Agyeman <kwagyeman@openmv.io>
   This work is licensed under the MIT license, see the file LICENSE for details.
*/

#include <string.h> /* Used for memcpy() */
#include <stdbool.h>

#include "collections.h"
#include "fb_alloc.h"

////////////// Bitmap //////////////

void bitmap_bit_set(char *arrayPtr, int index) {
  arrayPtr[index >> CHAR_SHIFT] |= 1 << (index & CHAR_MASK);
}

char bitmap_bit_get(char *arrayPtr, int index) {
  return (arrayPtr[index >> CHAR_SHIFT] >> (index & CHAR_MASK)) & 1;
}

/*
void bitmap_clear(char *arrayPtr) {
  memset(arrayPtr, 0, ((NEW_FRAME + CHAR_MASK) >> CHAR_SHIFT) * sizeof(char));
}
*/
////////////// Lifo //////////////

void lifo_alloc_all(lifo_t *ptr, size_t *size, size_t struct_size) {

  uint32_t tmp_size;

  ptr->data = (char *) fb_alloc_all(&tmp_size);
  ptr->data_len = struct_size;
  ptr->size = tmp_size / struct_size;
  ptr->len = 0;
  *size = ptr->size;
}

void lifo_free(lifo_t *ptr) {
  if (ptr->data) {
    fb_free();
  }
}

size_t lifo_size(lifo_t *ptr) {
  return ptr->len;
}

void lifo_enqueue(lifo_t *ptr, void *data) {
  memcpy(ptr->data + (ptr->len * ptr->data_len), data, ptr->data_len);
  ptr->len += 1;
}

void lifo_dequeue(lifo_t *ptr, void *data) {
  if (data) {
    memcpy(data, ptr->data + ((ptr->len - 1) * ptr->data_len), ptr->data_len);
  }
  ptr->len -= 1;
}

////////////// List //////////////

void list_init(list_t *ptr, size_t data_len) {
  ptr->head_ptr = NULL;
  ptr->tail_ptr = NULL;
  ptr->size = 0;
  ptr->data_len = data_len;
}

void list_copy(list_t *dst, list_t *src) {
  memcpy(dst, src, sizeof(list_t));
}

size_t list_size(list_t *ptr) {
  return ptr->size;
}

/////////// list_push_back() ///////////
// Adds a new element at the end of the list container
// The content of *data is added to the new element

// struct list_t :  node_t *head_ptr, *tail_ptr;
//                  size, data_len;

// struct node_t :  struct node *next_ptr, *prev_ptr;
//                  char data[];

void list_push_back(list_t *ptr, void *data, node_t *tmpNode) {

  // node_t *tmpNode = (node_t *) alloc(sizeof(node_t) + ptr->data_len);
  memcpy(tmpNode->data, data, ptr->data_len); // Copy the input data to a temporary node

  if (ptr->size++) { // Help! I don't understand this if() syntax!!!!!!!!!!!!!!
    tmpNode->next_ptr = NULL;
    tmpNode->prev_ptr = ptr->tail_ptr;
    ptr->tail_ptr->next_ptr = tmpNode;
    ptr->tail_ptr = tmpNode;
  } else {
    tmpNode->next_ptr = NULL;
    tmpNode->prev_ptr = NULL;
    ptr->head_ptr = tmpNode;
    ptr->tail_ptr = tmpNode;
  }
}

// struct list_t :  node_t *head_ptr, *tail_ptr;
//                  size, data_len;

// struct node_t :  struct node *next_ptr, *prev_ptr;
//                  char data[];

void list_pop_front(list_t *ptr, void *data, node_t *tmpNode) {

  tmpNode = ptr->head_ptr;

  if (data) {
    memcpy(data, tmpNode->data, ptr->data_len);
  }
  tmpNode->next_ptr->prev_ptr = NULL;
  ptr->head_ptr = tmpNode->next_ptr;
  ptr->size -= 1;
  // free(tmp); // Do I nead to free !?
}

////////////// Iterators //////////////

node_t *iterator_start_from_head(list_t *ptr) {
  return ptr->head_ptr;
}

node_t *iterator_next(node_t *lnk) {
  return lnk->next_ptr;
}

// void *memcpy(void *str1, const void *str2, size_t n)
// str1 − This is pointer to the destination array where the content is to be copied, type-casted to a pointer of type void*.
// str2 − This is pointer to the source of data to be copied, type-casted to a pointer of type void*.
// n − This is the number of bytes to be copied.

void iterator_get(list_t *ptr, node_t *lnk, void *data) {
  memcpy(data, lnk->data, ptr->data_len);
}

/*
  void iterator_set(list_t *ptr, node_t *lnk, void *data) {
  memcpy(lnk->data, data, ptr->data_len);
  }
*/
