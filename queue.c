#include <stddef.h>
#include <stdlib.h>

#include "queue.h"

struct queue_node {
	void *val;
	struct queue_node *next;
};
typedef struct queue_node queue_node;

struct queue {
	queue_cbs    cbs;
	queue_node  *head;
	queue_node  *tail;
	size_t	     size;
};

static void *queue_passthrough_copy(void *v) {
	return v;
}

static void queue_passthrough_destroy(void *v) {
	return;
}

queue *queue_create(queue_cbs *cbs) {
	queue *q;

	q = calloc(1, sizeof(*q));

	q->cbs.val_copy = queue_passthrough_copy;
	q->cbs.val_free = queue_passthrough_destroy;
	if (cbs != NULL) {
		if (cbs->val_copy != NULL) q->cbs.val_copy = cbs->val_copy;
		if (cbs->val_free != NULL) q->cbs.val_free = cbs->val_free;
	}

	return q;
}

void queue_destroy(queue *q) {
	queue_node *next;
	queue_node *cur;

	if (q->head == NULL)
		return;

	while (q->tail != NULL && q->tail->next != q->head)
		q->tail = q->tail->next;
	q->tail->next = NULL;
			
	next = q->head;
	while (next != NULL) {
		cur = next;
		q->cbs.val_free(cur->val);
		next = cur->next;
		free(cur);
	}

	free(q);
}

void queue_push(queue *q, void *val) {
	queue_node *cur;
		
	if (q->tail != NULL && q->tail->next != NULL && (q->tail->next != q->head || (q->tail->next == q->head && q->head->val == NULL)))
		cur = q->tail->next;
	else	
		cur = calloc(1, sizeof(*cur));
	
	if (val != NULL)
		val = q->cbs.val_copy(val);
	cur->val = val;

	if (q->head == NULL) {
		q->head = cur;
		q->tail = cur;
	}

	if (cur->next == NULL)
		cur->next = q->head;

	q->tail->next = cur;
	q->tail = cur;

	q->size++;
}

void queue_pop(queue *q, void **val) {
	if (q->head == NULL)
		return;

	if (val != NULL)
		*val = q->head->val;
	q->cbs.val_free(q->head->val);
	q->head->val = NULL;

	if (q->head->next->val != NULL)
		q->head = q->head->next;

	q->size--;
}

int queue_size(queue *q) {
	return q->size;
}
