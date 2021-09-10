/*This code is a cooperation result with my friend YUANG CAO
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/*
* definition of linked list node
* the value is the element in the list
* the pointer points to the next element in the list
* if it exists
*/
typedef struct node_struct {
  int8_t value;
  struct node_struct* next;
} node_struct;

// when the list is not empty, this variable will always contain
// the element in the first position in the list (the “head” of the list)
static node_struct* head;

// This function will perform the insertion sort and maintain the linked list
// you will need to maintain the links and properly place the new element
void insert_in_order(node_struct* new_element) {
    /*If the new element is smaller than the
       head value, it should be put in the head
       and its pointer should point to the old head
    */
    if(head==NULL||head->value>=new_element->value){
        new_element->next=head;
        head=new_element;
    }
    /*If the new_element is not smaller than the head value
        it should be compared with the rest elements
    */
    else{
            node_struct* p1=NULL;
            node_struct* p2=NULL;

            p1=head;
            p2=head;

            while(p2!=NULL){
                if(p2->value>new_element->value){
                    break;
                }
                p1=p2;
                p2=p2->next;
            }
            p1->next=new_element;
            new_element->next=p2;
    }
}

// this function creates a new entry into the list to be inserted
void add_element(int8_t value) {
  // create a new element in our list
  node_struct* new_element = (node_struct*)malloc(sizeof(node_struct));
  if (new_element == NULL) {
    printf("malloc failed \n");
    return;
  }
  // assign our values
  new_element->value = value;
  new_element->next = NULL;

  insert_in_order(new_element);
}

// prints the entirety of our list
void print_list() {
  if (head == NULL) {
    printf("list is empty \n");
    return;
  }

  node_struct*
