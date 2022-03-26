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
    if (table == NULL)
        return;
    for (int i = 0; i < table->numberDiffKeysInT1; ++i) {
        free_node1(table->ks1[i].node);
    }
    free(table->ks1);
}

KeyType1 enter_key1() {
    KeyType1 key;
    printf("enter your intKey for the new element in the lookthrough table.\n");
    key.intKey = get_int();
    return key;
}

bool keys1_eq(KeyType1 key1, KeyType1 key2) {
    if (key1.intKey == key2.intKey)
        return true;
    return false;
}

int comparator(const void * ks1, const void * ks2) {
    KeySpace1 * KS1 = (KeySpace1*) ks1;
    KeySpace1 * KS2 = (KeySpace1*) ks2;

    if (KS1->key.intKey < KS2->key.intKey) {
        return 1;
    }
    if (KS1->key.intKey > KS2->key.intKey) {
        return -1;
    }
    return 0;
}

int binarySearch(Table * table, KeyType1 key) {
    int left = 0, right = table->numberDiffKeysInT1;
    while (left != right) {
        int m = (left+right)/2;
        KeySpace1 * p = (table->ks1+m);
        KeySpace1 ks = (KeySpace1) {key, NULL};
        if (comparator(&ks, p) < 0) {
            left = m + 1;
        }
        else if (comparator(&key, p) == 0)
            return m;
        else
            right = m;
    }
    return left;
}

KeyType1 * cope_key1(KeyType1 key) {
    KeyType1 * res = malloc(sizeof(KeyType1));
    *(res) = (KeyType1) {key.intKey};
    return res;
}

KeySpace1 * get_KS1(Table * table, KeyType1 key) {
    if (table->numberDiffKeysInT1 == 0) {
        table->ks1->key.intKey = key.intKey;
        return table->ks1;
    }

    KeySpace1 * ks = &((KeySpace1) {key, NULL});

    size_t x = binarySearch(table, key);

    KeySpace1 * ks1 = table->ks1 + x;
    if (table->ks1 + table->msize1-1 == ks1) {
        if (keys1_eq(ks1->key, ks->key)) {
            return ks1;
        }
        else
            return NULL;
    }

    int num = table->numberDiffKeysInT1;
    memmove(table->ks1 + x+1, table->ks1 + x, sizeof(KeySpace1) * (num-x));
    table->ks1[x].key.intKey = key.intKey;
    table->ks1[x].node = NULL;

    return ks1;
}

void add_el_in_KS1(Table * table, Item * item) {
    //KeySpace1 * intKey = get_KS1(table, item->key1);
    int ind = binarySearch(table, item->key1);
    if (ind >= table->msize1) {
        free_item(item);
        printf("we tried, but we cant\n");
        return;
    }

    KeySpace1 * key = table->ks1+ ind;
    if (key == NULL) {
        fprintf(stderr, "\nImpossible intKey.\n");
        //exit(IMPOSSIBLE_KEY);
        return;
    }

    if (keys1_eq(key->key, item->key1)) {
        //table->numberDiffKeysInT1++;

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
        /*
        if (ind == table->msize1-1) {
            if (table->ks1[ind].busy) {
                free_item(item);
                printf("we tried, but we cant\n");
                return;
            }
        }
         */
        int num = table->numberDiffKeysInT1; //number_of_ks1_b(*table);
        memmove(table->ks1 + ind+1, table->ks1 + ind, sizeof(KeySpace1) * (num-ind));
        table->ks1[ind].key.intKey = item->key1.intKey;
        table->ks1[ind].node = NULL;
        //table->numberDiffKeysInT1++;
        Node1 * node = malloc(sizeof(Node1));
        node->info = item;
        node->next = key->node;
        key->node = node;
        if (node->next)
            node->release.numberOfRelease = node->next->release.numberOfRelease+1;
        else
            node->release.numberOfRelease = 0;
        table->numberDiffKeysInT1++;
    }
}

KeySpace1 * getKey1(Table * table, KeyType1 key1) {
    int index = binarySearch(table, key1);
    if (index >= table->msize1)
        return NULL;
    if (keys1_eq(table->ks1[index].key, key1))
        return table->ks1 + index;
    return NULL;
}

bool k1_in_table1(Table * table, KeyType1 key, KeyType2 key2) {
    int ind = binarySearch(table, key);
    if (table->msize1 <= ind || ind < 0)
        return false;
    if (keys1_eq(key, table->ks1[ind].key)) {
        Node1 * n = table->ks1[ind].node;
        while (n) {
            if (keys2_eq(n->info->key2, key2))
                return true;
            n = n->next;
        }
    }
    return false;
}

bool el_in_KS1(Table * table, KeyType1 key1, KeyType2 key2) {
    if (k1_in_table1(table, key1, key2) == false)
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
