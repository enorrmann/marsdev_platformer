#include "stack.h"

void init(Stack *s, int elemSize, int maxElements)
{
	byte *storage;

	/* Try to allocate memory */
	storage = (byte *)malloc(elemSize * maxElements);
	if (storage == NULL)
	{
		return;
	}

	/* Initialize an empty stack */
	s->top = 0;
	s->maxElements = maxElements;
	s->elemSize = elemSize;
	s->storage = storage;
}

int isEmpty(Stack *s)
{
	/* top is 0 for an empty stack */
	return (s->top == 0);
}

int size(Stack *s)
{
	return s->top;
}

void push(Stack *s, void *elem)
{
	if (s->top == s->maxElements)
	{
		return;
	}
	int start = s->top * s->elemSize, i;
	for (i = 0; i < s->elemSize; i++)
	{
		*(s->storage + start + i) = *((byte *)(elem + i));
	}
	s->top = s->top + 1;
}

void *pop(Stack *s)
{
	if (isEmpty(s))
	{
		return;
	}
	void *elem = top(s);
	s->top = s->top - 1;
	return elem;
}

void *top(Stack *s)
{
	if (isEmpty(s))
	{
		return;
	}
	int start = (s->top - 1) * s->elemSize, i;
	byte *elem;
	elem = (byte *)malloc(s->elemSize);
	for (i = 0; i < s->elemSize; i++)
	{
		*(elem + i) = *(s->storage + start + i);
	}
	return (void *)elem;
}

void destroy(Stack *s)
{
	free(s->storage);
	s->top = 0;
}