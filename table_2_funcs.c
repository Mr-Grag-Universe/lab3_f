//
// Created by Stephan on 11.03.2022.
//

#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "string.h"
#include "table_structs.h"
#include "table_creation.h"
#include "table_functions.h"
#include "Errors.h"
#include "MyString.h"
#include "table_2_funcs.h"
#include "table_1_funcs.h"

int create_ind2(Table table, KeyType2 key) {
    return hash_func(key)%table.msize2;
}

KeySpace2 * getKey2(Table * table, KeyType2 key) {
    KeySpace2 * KS = table->ks2 + create_ind2(*table, key) % table->msize2;
    while (KS) {
        if (KS->busy) {
            if (keys2_eq(KS->key, key)) {
                return KS;
            }
        }
        KS = KS->next;
    }
    return NULL;
}

void free_node2(Node2 * node) {
    if (node == NULL) {
        return;
    }
    Node2 * n_p = node;
    while (node) {
        n_p = n_p->next;
        node->info = NULL;
        free(node);
        node = n_p;
    }
    free(n_p);
}

void free_table2(Table * table) {
    for (int i = 0; i < table->msize2; ++i) {
        KeySpace2 * ks = table->ks2+i;
        while (ks) {
            KeySpace2 * tmp = ks->next;
            free_node2(ks->node);
            ks->node = NULL;
            if (ks != table->ks2+i)
                free(ks);
            ks = tmp;
        }
    }
    free(table->ks2);
    table->ks2 = NULL;
}

int hash_func(KeyType2 key) {
    return abs(key.key % 100);
}

KeyType2 * cope_key2(KeyType2 key) {
    KeyType2 * res = malloc(sizeof(KeyType2));
    *(res) = (KeyType2) {key.key};
    return res;
}

KeyType2 enter_key2() {
    KeyType2 key;
    printf("enter your key for the new element in the hash table.\n");
    key.key = get_int();

    return key;
}

bool keys2_eq(KeyType2 key1, KeyType2 key2) {
    if (key1.key == key2.key)
        return true;
    return false;
}

Item * find_item_in_n2(Node2 * n, KeyType1 key1) {
    Node2 * node = n;
    while (node && keys1_eq(node->info->key1, key1)) {
        node = node->next;
    }
    if (node == NULL)
        return NULL;
    return node->info;
}

void add_el_in_KS2(Table * table, Item * item) {
    //if (el_k1_k2_in_table(table, item->key1, item->key2) == false) {
    //    printf("Something came wrong.\n");
    //    return;
    //}

    int ind = create_ind2(*table, item->key2);
    KeySpace2 * ks = table->ks2 + ind;

    if (ks->node == NULL) {
        ks->key.key = item->key2.key;
        ks->node = malloc(sizeof(Node2));
        ks->node->info = item;
        ks->node->next = NULL;
        ks->node->release.numberOfRelease = 0;
        ks->busy = true;
        return;
    }

    KeySpace2 * ks_p = table->ks2 + ind;
    while (ks && keys2_eq(ks->key, item->key2) == false) {
        if (ks->busy == false)
            break;
        ks_p = ks;
        ks = ks->next;
    }

    if (ks == NULL) {
        ks_p->next = malloc(sizeof(KeySpace2));
        ks = ks_p;
        ks->busy = true;
        ks = ks->next;
        ks->busy = true;
        ks->next = NULL;
        ks->key.key = item->key2.key;
        ks->node = malloc(sizeof(Node2));
        ks->node->next = NULL;
        ks->node->info = item;
        ks->node->release = (RelType2) {0};
    }
    else {
        Node2 * tmp = ks->node;
        ks->busy = true;
        ks->key.key = item->key2.key;
        ks->node = malloc(sizeof(Node2));
        ks->node->next = tmp;
        if (tmp)
            ks->node->release = (RelType2) {tmp->release.numberOfRelease+1};
        else
            ks->node->release = (RelType2) {0};
        ks->node->info = item;
    }
    item->p2 = ks->node;
}

///looking for a KeySpace with entered key
bool k2_in_table2(Table * table, KeyType2 key) {
    //for (int i = 0; i < table->msize2; ++i) {
        KeySpace2 * ks = table->ks2 + hash_func(key)%table->msize2;
        while (ks) {
            if (ks->busy) {
                if (keys2_eq(ks->key, key)) {
                    return true;
                }
            }
            ks = ks->next;
        }
    //}

    return false;
}

int number_of_nodes2(Node2 * node) {
    if (node == NULL) {
        return 0;
    }
    int x = number_of_nodes2(node->next);
    node->release.numberOfRelease = x;
    return x+1;
}
