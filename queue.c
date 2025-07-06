#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

static element_t *alloc_new_node(struct list_head *head, char *s);
static void q_delete_node(struct list_head *target);
static struct list_head *q_get_mid(struct list_head *head);

/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *q_head =
        (struct list_head *) malloc(sizeof(struct list_head));
    if (q_head)
        INIT_LIST_HEAD(q_head);
    return q_head;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (!head)
        return;
    element_t *entry, *safe;
    list_for_each_entry_safe(entry, safe, head, list) {
        free(entry->value);
        free(entry);
    }
    free(head);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    element_t *new_node = alloc_new_node(head, s);
    if (!new_node)
        return false;

    list_add(&new_node->list, head);

    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    element_t *new_node = alloc_new_node(head, s);
    if (!new_node)
        return false;

    list_add_tail(&new_node->list, head);

    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;
    element_t *node = list_first_entry(head, element_t, list);
    list_del(&node->list);
    strncpy(sp, node->value, bufsize);
    return node;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;
    element_t *node = list_last_entry(head, element_t, list);
    list_del(&node->list);
    strncpy(sp, node->value, bufsize);
    return node;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head || list_empty(head))
        return 0;
    int counter = 0;
    struct list_head *node;
    list_for_each(node, head) {
        counter++;
    }
    return counter;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/

    struct list_head *mid = q_get_mid(head);
    if (!mid)
        return false;

    q_delete_node(mid);
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (!head || list_empty(head))
        return false;

    struct list_head *cur, *safe;
    struct list_head *temp, *temp2;
    char *left, *right;

    bool dup = false;

    list_for_each_safe(cur, safe, head) {
        temp = safe;
        left = list_entry(cur, element_t, list)->value;
        while (temp != head) {
            temp2 = temp->next;
            right = list_entry(temp, element_t, list)->value;
            if (strcmp(left, right) == 0) {
                if (safe == temp)
                    safe = safe->next;
                q_delete_node(temp);
                dup = true;
            }
            temp = temp2;
        }
        if (dup) {
            q_delete_node(cur);
            dup = false;
        }
    }
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    struct list_head *slow = head;

    while (1) {
        if (slow->next == head || slow->next->next == head)
            return;

        struct list_head *fast = slow->next->next;
        slow = slow->next;

        list_del_init(slow);
        list_add(slow, fast);
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    struct list_head *cur = head;
    struct list_head *next = head->next;
    struct list_head *prev = head->prev;

    while (next != prev) {
        cur->prev = next;
        cur->next = prev;
        prev = cur;
        cur = next;
        next = next->next;
    }
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}

static element_t *alloc_new_node(struct list_head *head, char *s)
{
    element_t *new_node = (element_t *) malloc(sizeof(element_t));
    if (!new_node)
        return NULL;
    size_t s_len = strlen(s);
    new_node->value = (char *) malloc(s_len + 1);
    if (!new_node->value) {
        free(new_node);
        return NULL;
    }
    strncpy(new_node->value, s, s_len + 1);
    INIT_LIST_HEAD(&new_node->list);
    return new_node;
}

static void q_delete_node(struct list_head *target)
{
    list_del(target);
    element_t *node = list_entry(target, element_t, list);
    free(node->value);
    free(node);
}

static struct list_head *q_get_mid(struct list_head *head)
{
    if (!head || list_empty(head))
        return NULL;

    struct list_head *slow = head;
    struct list_head *fast = head;

    while (fast->next != head && fast->next->next != head) {
        slow = slow->next;
        fast = fast->next->next;
    }
    if (fast->next->next == head)
        slow = slow->next;
    return slow;
}