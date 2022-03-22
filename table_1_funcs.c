//
// Created by Stephan on 11.03.2022.
//
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "string.h"
#include "time.h"
#include "table_structs.h"
#include "table_creation.h"
#include "table_functions.h"
#include "Errors.h"
#include "MyString.h"
#include "table_2_funcs.h"

void free_node1(Node1 * node) {
    if (node == NULL) {
        return;
        //(INVALID_FREE);
    }
    Node1 * n_p = node;
    while (node) {
        n_p = n_p->next;
        ///free_item(node->info);
        free(node);
        node = n_p;
    }
    free(n_p);
}

void free_table1(Table * table) {
    for (int i = 0; i < table->msize1; ++i) {
        free_node1(table->ks1[i].node);
    }
    free(table->ks1);
}

KeyType1 enter_key1() {
    ///srand(time(NULL));
    KeyType1 key;
    printf("enter your key for the new element in the lookthrough table.\n");
    key.key = get_int();
    return key;
}

bool keys1_eq(KeyType1 key1, KeyType1 key2) {
    if (key1.key == key2.key)
        return true;
    return false;
}

int comparator(const void * ks1, const void * ks2) {
    KeySpace1 * KS1 = (KeySpace1*) ks1;
    KeySpace1 * KS2 = (KeySpace1*) ks2;

    if (KS1->key.key < KS2->key.key) {
        return 1;
    }
    if (KS1->key.key > KS2->key.key) {
        return -1;
    }
    return 0;
}

int number_of_ks1_b(Table table) {
    for (int i = 0; i < table.msize1; ++i) {
        if (table.ks1[i].busy == false)
            return i;
    }
    return table.msize1;
}

int binarySearch(Table * table, KeyType1 key) {
    int left = 0, right = number_of_ks1_b(*table);
    while (left != right) {
        int m = (left+right)/2;
        KeySpace1 * p = (table->ks1+m);
        if (p->busy) {
            KeySpace1 ks = (KeySpace1) {1, key, NULL};
            if (comparator(&ks, p) < 0) {
                left = m + 1;
            }
            else if (comparator(&key, p) == 0)
                return m;
            else
                right = m;
        }
        else right = m;
    }
    return left;
}

KeyType1 * cope_key1(KeyType1 key) {
    KeyType1 * res = malloc(sizeof(KeyType1));
    *(res) = (KeyType1) {key.key};
    return res;
}

KeySpace1 * get_KS1(Table * table, KeyType1 key) {
    if (table->ks1[0].busy == false) {
        table->ks1->key.key = key.key;
        return table->ks1;
    }

    KeySpace1 * ks = &((KeySpace1) {true, key, NULL});
    //getchar();
    size_t x = binarySearch(table, key);
    if (table->ks1[x].busy) {
        if (table->ks1[x].key.key == key.key)
            return table->ks1+x;
    }
    //getchar();
    KeySpace1 * ks1 = table->ks1 + x;//table.ks1, ks, sizeof(*ks), table.msize1.index, comparator);
    if (table->ks1 + table->msize1-1 == ks1) {
        if (keys1_eq(ks1->key, ks->key)) {
            return ks1;
        }
        else
            return NULL;
    }

    int num = number_of_ks1_b(*table);
    memmove(table->ks1 + x+1, table->ks1 + x, sizeof(KeySpace1) * (num-x));
    //print_table(*table);
    table->ks1[x].key.key = key.key;
    table->ks1[x].busy = true;
    table->ks1[x].node = NULL;

    return ks1;
}

void add_el_in_KS1(Table * table, Item * item) {
    //KeySpace1 * key = get_KS1(table, item->key1);
    int ind = binarySearch(table, item->key1);
    if (ind >= table->msize1) {
        free_item(item);
        printf("we tried, but we cant\n");
        return;
    }

    KeySpace1 * key = table->ks1+ ind;
    //getKey1(table->ks1, item->key1, table->msize1);
    //getchar();
    //item->p1 = key;
    //printf("%p: %p\n", key, key->node);
    if (key == NULL) {
        fprintf(stderr, "\nImpossible key.\n");
        //exit(IMPOSSIBLE_KEY);
        return;
    }

    if (keys1_eq(key->key, item->key1)) {
        key->busy = true;

        //Node1 * node = key->node;
        Node1 * node = malloc(sizeof(Node1));
        node->info = item;
        node->next = key->node;
        key->node = node;
        if (node->next)
            node->release.numberOfRelease = node->next->release.numberOfRelease+1;
        else
            node->release.numberOfRelease = 0;
    }
    else {
        if (ind == table->msize1-1) {
            if (table->ks1[ind].busy) {
                free_item(item);
                printf("we tried, but we cant\n");
                return;
            }
        }
        int num = number_of_ks1_b(*table);
        memmove(table->ks1 + ind+1, table->ks1 + ind, sizeof(KeySpace1) * (num-ind));
        //print_table(*table);
        table->ks1[ind].key.key = item->key1.key;
        table->ks1[ind].node = NULL;
        key->busy = true;
        Node1 * node = malloc(sizeof(Node1));
        node->info = item;
        node->next = key->node;
        key->node = node;
        if (node->next)
            node->release.numberOfRelease = node->next->release.numberOfRelease+1;
        else
            node->release.numberOfRelease = 0;
    }
}

KeySpace1 * getKey1(KeySpace1 * KS, KeyType1 key, int size) {
    for (int i = 0; i < size; ++i) {
        if (KS[i].busy) {
            if (keys1_eq(KS[i].key, key)) {
                return KS+i;
            }
        }
    }
    return NULL;
}

bool k1_in_table1(Table * table, KeyType1 key) {
    int ind = binarySearch(table, key);
    if (keys1_eq(key, table->ks1[ind].key))
        return true;
    /*for (int i = 0; i < table->msize1; ++i) {
        if (table->ks1[i].busy) {
            if (keys1_eq(key, table->ks1[i].key))
                return true;
        }
    }*/
    return false;
}

bool el_in_KS1(Table * table, KeyType1 key1, KeyType2 key2) {
    if (k1_in_table1(table, key1) == false)
        return false;
    KeySpace1 * ks = get_KS1(table, key1);
    Node1 * node = ks->node;
    while (node) {
        if (keys2_eq(key2, node->info->key2))
            return true;
        node = node->next;
    }
    return false;
}

int number_of_nodes1(Node1 * node) {
    if (node == NULL) {
        return 0;
    }
    int x = number_of_nodes1(node->next);
    node->release.numberOfRelease = x;
    return x+1;
}
