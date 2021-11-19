  typedef struct Node {
        int data;
        struct Node* next;
    }Node;
bool has_cycle(Node* head) {

    struct Node *slow_p = head, *fast_p = head;
 
    while (slow_p && fast_p && fast_p->next) {
        slow_p = slow_p->next;
        fast_p = fast_p->next->next;
        if (slow_p == fast_p) {
            return 1;
        }
    }
    return 0;
}
