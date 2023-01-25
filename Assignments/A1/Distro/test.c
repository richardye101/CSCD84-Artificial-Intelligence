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
  MinHeap* min_heap = MinHeap_new();
  assert(min_heap != NULL);
  int size =0;
  MinHeap_insert(min_heap, 7, 8, 24);
  ++size;
  assert(min_heap->size == size);
  MinHeap_insert(min_heap, 0, 1, -1);
  ++size;
  assert(min_heap->size == size);
  MinHeap_insert(min_heap, 4, 5, 6);
  ++size;
  assert(min_heap->size == size);
  MinHeap_insert(min_heap, 2, 3, 1);
  ++size;
  assert(min_heap->size == size);
  MinHeap_insert(min_heap, 6, 7, 10);
  ++size;
  assert(min_heap->size == size);
  MinHeap_insert(min_heap, 3, 4, 2);
  ++size;
  assert(min_heap->size == size);
  MinHeap_insert(min_heap, 1, 2, 0);
  ++size;
  assert(min_heap->size == size);
  MinHeap_insert(min_heap, 5, 6, 8);
  ++size;
  assert(min_heap->size == size);
  for (int i=0; i<8; ++i) {
    assert(min_heap != NULL);
    Cord cord = MinHeap_pop(min_heap);
    assert(cord.x == i && cord.y == i + 1);
    -- size;
    assert(min_heap->size == size);
  }
  assert(min_heap->size == 0);
  free(min_heap);
  min_heap = NULL;
  printf("PASSED test_MinHeap\n");
  return 0;
}

int main() {
  if (test_Deque() == 0 && test_MinHeap() == 0) {
    printf("--PASSED ALL TEST CASES--\n");
  }
  return 0;
}
