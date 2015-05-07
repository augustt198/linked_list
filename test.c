#include <stdio.h>
#include <assert.h>

#include "linked_list.h"

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

    // index 1000 should be invalid
    assert(!linked_list_get(&list, 1000, 0));

    for (int i = 0; i < 3; i++) {
        linked_list_truncate(&list);
    }

    assert(linked_list_len(&list) == 0);

    printf("All assertions passed!\n");
}
