#ifndef _QUEUE_H_
#define _QUEUE_H_

struct queue;
typedef struct queue queue;

typedef void *(*queue_vcopy)(void *in);
typedef void (*queue_vfree)(void *in);

typedef struct {
	queue_vcopy val_copy;
	queue_vfree val_free;
} queue_cbs;

queue *queue_create(queue_cbs *cbs);

void queue_destroy(queue *q);

void queue_push(queue *q, void *val);

void queue_pop(queue *q, void **val);

int queue_size(queue *q);

#endif /** _QUEUE_H_ **/
