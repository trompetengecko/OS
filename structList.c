#include <stdio.h>
#include <stdlib.h>

// typedef struct {
//   int x;
//   int y;
// } myStruct;

// myStruct* createStruct(int x, int y) {
//   myStruct* s = malloc(sizeof(myStruct));
//   s->x = x;
//   s->y = y;
//   return s;
// }

// int main() {
//   myStruct* s1 = createStruct(1, 2);
//   myStruct* s2 = createStruct(3, 4);
//   // do something with s1 and s2
//   free(s1);
//   free(s2);
//   return 0;
// }

struct list {
  int data;
  struct list* next;
};

struct list* append(struct list*, int);
// struct list* delete (struct list *, int);
// struct list* find (struct list *, int);

struct list* append(struct list* currList, int n) {
  if (currList->next == NULL) {
    append((currList->next), 2);
    printf("List has already next link. Creating new link\n");

  } else {
    struct list L2;
    currList->next = &L2;
    L2.data = 2;
    printf("My number: %d\n", currList->next->data);
  }
}

int main(void) {
  struct list L0;
  struct list L1;
  L0.data = 0;
  L0.next = &L1;
  L1.data = 1;

  printf("My number: %d\n", L0.data);
  append(&L0, 2);

  printf("My number: %d\n", L1.data);
  return 42;
}
