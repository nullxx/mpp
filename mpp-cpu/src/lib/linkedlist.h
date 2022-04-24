#ifndef linkedlist_h
#define linkedlist_h
typedef struct node {
    void *value;
    struct node *next;
} LlNode;

LlNode *create_ll_node(void *value, LlNode *next);
LlNode *push_ll_node(LlNode *head, void *val, LlNode *next);

typedef struct {
    LlNode *prev_node;
    LlNode *node;
    LlNode *next_node;
    int found;
} get_node_t;

get_node_t get_node_from_value(LlNode *head, void *value);
int delete_node_from_value(LlNode **head, void *value);

#endif /* linkedlist_h */