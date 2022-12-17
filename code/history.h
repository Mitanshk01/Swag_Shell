#ifndef __HISTORY_H
#define __HISTORY_H

typedef char *deque_element;

struct Deque
{
  unsigned int capacity;
  int front, rear;
  deque_element *arr;
  unsigned int numitems;
};

typedef struct Deque *deque;
deque Create_History(unsigned int capacity);
deque_element pop_front(deque de);
deque_element pop_back(deque de);
void push_front(deque de, deque_element el);
void push_back(deque de, deque_element el);
deque_element front(deque de);
deque_element back(deque de);
int isempty(deque de);
int size(deque de);

#endif