#include "headers.h"

deque Create_History(unsigned int capacity)
{
  deque de;
  de = (deque)malloc(sizeof(struct Deque));
  de->front = 0;
  de->rear = -1;
  de->numitems = 0;
  de->capacity = capacity;
  de->arr = (deque_element *)malloc(sizeof(deque_element) * capacity);
  for (int i = 0; i < capacity; i++)
    de->arr[i] = (char *)malloc(sizeof(char) * 400);

  return de;
}

deque_element pop_front(deque de)
{
  deque_element el = de->arr[de->front];
  de->numitems--;
  de->front = (de->front + 1) % de->capacity;
  return el;
}

deque_element pop_back(deque de)
{
  deque_element el = de->arr[de->rear];

  de->numitems--;

  de->rear = (de->rear - 1) % de->capacity;

  if (de->numitems == 0)
  {
    de->rear = -1;
    de->front = 0;
  }

  return el;
}

void push_back(deque de, deque_element el)
{
  if (de->capacity == de->numitems)
    pop_front(de);
  de->numitems++;
  de->rear = (de->rear + 1) % de->capacity;
  strcpy(de->arr[de->rear], el);
  return;
}

deque_element front(deque de)
{
  deque_element el = de->arr[de->front];
  return el;
}

deque_element back(deque de)
{
  deque_element el = de->arr[de->rear];
  return el;
}

int isempty(deque de)
{
  return (de->numitems == 0);
}

int size(deque de)
{
  return de->numitems;
}