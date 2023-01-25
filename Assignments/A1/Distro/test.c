#include "AI_search.h"

int test_Deque(void) {
  Deque *deque = Deque_new();
  assert(deque != NULL);
  int size=0;
  for (int i=0; i<10; ++i) {
    assert(deque != NULL);
    Deque_push_front(deque, i, i + 1);
    ++size;
    assert(deque->size == size);
    Deque_push_back(deque, i + 1, i);
    ++size;
    assert(deque->size == size);
  }
  for (int i=0; i<10; ++i) {
    Cord cord = Deque_pop_front(deque);
    --size;
    assert(cord.x == 9-i);
    assert(cord.y == 10-i);
    assert(deque->size == size);
  }
  for (int i=0; i<10; ++i) {
    Cord cord = Deque_pop_front(deque);
    --size;
    assert(cord.x == i+1);
    assert(cord.y == i);
    assert(deque->size == size);
  }
  assert(deque->size == 0);
  free(deque);
  deque = NULL;
  printf("PASSED test_Deque\n");
  return 0;
}

int test_MinHeap(void) {
  printf("PASSED test_MinHeap\n");
  return 0;
}

int main() {
  test_Deque();
  return 0;
}
