#include "list.h"
#include "basics.h"

void list_link(int* first, int* next,
    int head, int node)
{
  next[node] = first[head];
  first[head] = node;
}

void list_link_after(int* first, int* next,
    int head, int node, int prev)
{
  if (prev == NULL_IDX) {
    list_link(first, next, head, node);
  } else {
    next[node] = next[prev];
    next[prev] = node;
  }
}

void list_unlink_after(int* first, int* next,
    int head, int node, int prev)
{
  if (prev == NULL_IDX)
    first[head] = next[node];
  else
    next[prev] = next[node];
}

static int find_prev(int* first, int* next,
    int head, int node)
{
  int prev;
  if (node == first[head])
    return NULL_IDX;
  for (prev = first[head]; next[prev] != node; prev = next[prev]);
  return prev;
}

void list_unlink(int* first, int* next,
    int head, int node)
{
  list_unlink_after(first, next, head, node,
      find_prev(first, next, head, node));
}

void dlist_link(int* first, int* last, int* next, int* prev,
    int head, int node)
{
  list_link_after(first, next, head, node, last[head]);
  list_link_after(last, prev, head, node, NULL_IDX);
}

void dlist_unlink(int* first, int* last, int* next, int* prev,
    int head, int node)
{
  list_unlink_after(first, next, head, node, prev[node]);
  list_unlink_after(last, prev, head, node, next[node]);
}
