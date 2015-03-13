#ifndef LIST_H
#define LIST_H

void list_link_after(int* first, int* next,
    int head, int node, int prev);
void list_unlink_after(int* first, int* next,
    int head, int node, int prev);

void dlist_link(int* first, int* last, int* next, int* prev,
    int head, int node);
void dlist_unlink(int* first, int* last, int* next, int* prev,
    int head, int node);

void list_unlink(int* first, int* next,
    int head, int node);

#endif
