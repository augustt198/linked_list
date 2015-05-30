#include <stdio.h>
#include <assert.h>

#include <pthread.h>

#include "linked_list.h"

void concurrent_test();
void each_test();

int iter_fn(int idx, void *elem) {
    assert(*((int*) elem) == idx);
    return 0;
}

int main() {
    LinkedList list;

    linked_list_new(&list, sizeof(int));

    assert(linked_list_len(&list) == 0);
    assert(linked_list_empty(&list));

    for (int i = 0; i < 3; i++) {
        linked_list_append(&list, &i);
    }

    assert(linked_list_len(&list) == 3);

    for (int i = 0; i < 3; i++) {
        int num;
        assert(linked_list_get(&list, i, &num));
        assert(num == i);
    }

    iter_start(&list);
    for (int i = 0; iter_has_next(&list); i++) {
        int num;
        iter_next(&list, &num);
        assert(num == i);
    }

    // index 1000 should be invalid
    assert(!linked_list_get(&list, 1000, 0));

    for (int i = 0; i < 3; i++) {
        linked_list_truncate(&list);
    }

    assert(linked_list_len(&list) == 0);

    int num = 0;
    linked_list_append(&list, &num);
    linked_list_append(&list, &num);
    
    num = 1;
    linked_list_insert(&list, 1, &num);

    assert(linked_list_len(&list) == 3);

    linked_list_get(&list, 1, &num);
    assert(num == 1);

    num = 2;
    linked_list_prepend(&list, &num);
    
    assert(linked_list_len(&list) == 4);

    linked_list_get(&list, 0, &num);
    assert(num == 2);

    each_test();
    concurrent_test();

    printf("All assertions passed!\n");
}

void each_test() {
    LinkedList list;
    linked_list_new(&list, sizeof(int));
    for (int i = 0; i < 10; i++) {
        linked_list_append(&list, &i);
    }

    linked_list_each(&list, iter_fn);
}

void *thread_fn(void *data) {
    LinkedList *l = (LinkedList*) data;

    for (int i = 0; i < 1000; i++) {
        linked_list_append(l, &i);
    }


    for (int i = 0; i < 1000; i++) {
        linked_list_truncate(l);
    }

    return NULL;
}

void concurrent_test() {
    LinkedList list;
    linked_list_new(&list, sizeof(int));

    pthread_t t1, t2;
    pthread_create(&t1, NULL, thread_fn, &list);
    pthread_create(&t2, NULL, thread_fn, &list);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    assert(linked_list_len(&list) == 0);
}
