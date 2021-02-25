#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (!q) {
        return NULL;
    }
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    while (q->head) {
        list_ele_t *tmp = q->head;
        q->head = q->head->next;
        free(tmp->value);
        free(tmp);
    }
    /* Free queue structure */
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (!q) {
        return false;
    }
    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if (!newh) {
        return false;
    }
    newh->value = NULL;
    if (s) {
        char *value = malloc((strlen(s) + 1) * sizeof(char));
        if (!value) {
            free(newh);
            return false;
        }
        strncpy(value, s, strlen(s));
        value[strlen(s)] = '\0';
        newh->value = value;
    }
    newh->next = q->head;
    q->head = newh;
    /* Check whether queue is empty*/
    if (!q->tail) {
        q->tail = newh;
    }
    q->size++;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (!q) {
        return false;
    }
    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if (!newh) {
        return false;
    }
    newh->value = NULL;
    if (s) {
        char *value = malloc((strlen(s) + 1) * sizeof(char));
        if (!value) {
            free(newh);
            return false;
        }
        strncpy(value, s, strlen(s));
        value[strlen(s)] = '\0';
        newh->value = value;
    }
    newh->next = NULL;
    /* Check whether queue is empty*/
    if (!q->head) {
        q->head = newh;
    } else {
        q->tail->next = newh;
    }
    q->tail = newh;
    q->size++;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || !q->head) {
        return false;
    }
    if (sp) {
        size_t tmp = bufsize > (strlen(q->head->value) + 1)
                         ? (strlen(q->head->value) + 1)
                         : bufsize;
        strncpy(sp, q->head->value, tmp);
        sp[tmp - 1] = '\0';
    }
    /*Check whether is there only one element*/
    if (q->tail == q->head) {
        q->tail = NULL;
    }
    list_ele_t *tmp = q->head;
    q->head = q->head->next;
    free(tmp->value);
    free(tmp);
    q->size--;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || !q->head || q->head == q->tail) {
        return;
    }
    q->tail = q->head;
    list_ele_t *pre = q->head;
    list_ele_t *now = q->head->next;
    list_ele_t *next = q->head->next->next;
    while (next) {
        now->next = pre;
        pre = now;
        now = next;
        next = next->next;
    }
    now->next = pre;
    q->head = now;
    q->tail->next = NULL;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || !q->head) {
        return;
    }
    q->head = merge_sort(q->head);
    while (q->tail->next) {
        q->tail = q->tail->next;
    }
}
list_ele_t *merge(list_ele_t *left, list_ele_t *right)
{
    list_ele_t *head = NULL;
    list_ele_t *tail = NULL;
    while (left && right) {
        if (strcmp(left->value, right->value) <= 0) {
            if (!head) {
                head = left;
                tail = left;
            } else {
                tail->next = left;
                tail = left;
            }
            left = left->next;
        } else {
            if (!head) {
                head = right;
                tail = right;
            } else {
                tail->next = right;
                tail = right;
            }
            right = right->next;
        }
    }
    if (left) {
        tail->next = left;
    }
    if (right) {
        tail->next = right;
    }
    return head;
}

list_ele_t *merge_sort(list_ele_t *head)
{
    if (!head || !head->next) {
        return head;
    }
    list_ele_t *fast = head->next, *slow = head;
    while (fast && fast->next) {
        fast = fast->next->next;
        slow = slow->next;
    }
    fast = slow->next;
    slow->next = NULL;
    list_ele_t *left = merge_sort(head);
    list_ele_t *right = merge_sort(fast);
    return merge(left, right);
}
