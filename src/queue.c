#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include "queue.h"

int queue_size(queue_t *queue)
{
    int number_of_elements = 0;
    queue_t *initial_element = queue;
    queue_t *current_element = queue;
    while (current_element != NULL)
    {
        number_of_elements++;
        current_element = current_element->next;
        if (current_element == initial_element)
        {
            break;
        }
    }
    return number_of_elements;
}

void queue_print(char *name, queue_t *queue, void print_elem(void *))
{
    if (queue == NULL)
    {
        printf("%s%s", name, "[]\n");
        return;
    }

    queue_t *initial_element = queue;
    queue_t *element = queue;

    printf("%s%s", name, "[");
    while (true)
    {
        print_elem(element);
        element = element->next;

        if (element == initial_element)
        {
            break;
        }
        printf(" ");
    }
    printf("]\n");

    return;
}

int queue_append(queue_t **queue, queue_t *elem)
{
    if (queue == NULL)
    {
        fprintf(stderr, "error: queue is null\n");
        return -1;
    }

    if (elem == NULL)
    {
        fprintf(stderr, "error: element is null\n");
        return -1;
    }

    if (elem->next != NULL || elem->prev != NULL)
    {
        fprintf(stderr, "error: element is already in another queue\n");
        return -1;
    }

    if (*queue == NULL)
    {
        *queue = elem;
        elem->next = elem;
        elem->prev = elem;
        return 0;
    }

    queue_t *first_element = *queue;
    queue_t *last_element = first_element->prev;
    elem->next = first_element;
    elem->prev = last_element;
    first_element->prev = elem;
    last_element->next = elem;

    return 0;
}

int queue_remove(queue_t **queue, queue_t *elem)
{
    if (queue == NULL)
    {
        fprintf(stderr, "error: queue is null\n");
        return -1;
    }

    if (*queue == NULL)
    {
        fprintf(stderr, "error: queue is empty\n");
        return -1;
    }

    if (elem == NULL)
    {
        fprintf(stderr, "error: element is null\n");
        return -1;
    }

    if (elem->next == NULL || elem->prev == NULL)
    {
        fprintf(stderr, "error: element is in another queue\n");
        return -1;
    }

    bool element_in_queue = false;
    queue_t *element_aux = *queue;

    while (true)
    {
        element_aux = element_aux->next;
        if (element_aux == elem)
        {
            element_in_queue = true;
            break;
        }
        if (element_aux == *queue)
        {
            break;
        }
    }

    if (!element_in_queue)
    {
        fprintf(stderr, "error: element isn't in queue\n");
        return -1;
    }

    if (*queue == elem && elem->next == elem)
    {
        *queue = NULL;
        elem->next = NULL;
        elem->prev = NULL;
        return 0;
    }
    if (*queue == elem)
    {
        *queue = elem->next;
    }

    elem->next->prev = elem->prev;
    elem->prev->next = elem->next;

    elem->next = NULL;
    elem->prev = NULL;
    return 0;
}