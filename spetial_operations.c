//
// Created by Stephan on 17.03.2022.
//

#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "string.h"
#include "time.h"
#include "Errors.h"
#include "KGetLine.h"
#include "MyString.h"
#include "headers/table_structs.h"
#include "headers/table_1_funcs.h"
#include "headers/table_2_funcs.h"
#include "table_1_funcs.h"
#include "table_creation.h"
#include "table_functions.h"
#include "MyString.h"

/*
 * для первой
 */

void delete_t1_k_v(Table * table, KeyType1 key, int version) {
    KeySpace1 * ks = getKey1(table->ks1, key, table->msize1);
    if (ks->node == NULL || ks->node->release.numberOfRelease < version) {
        return;
    }

    Node1 * node = ks->node;
    while (node && node->release.numberOfRelease != version) {
        node = node->next;
    }

    if (node == NULL)
        return;

    delete_el(table, key, node->info->key2);
}

Item * copy_item(Item * item) {
    Item * item1 = malloc(sizeof(Item));
    item1->info = malloc(sizeof(InfoType));
    item1->info->data = str_copy(item->info->data); // лохо
    item1->key1.key = item->key1.key;
    item1->key2.key = item->key2.key;
    //cope_key1(&(item->key1), item->key1);
    //cope_key2(&(item->key2), item->key2);

    item1->ind1 = item->ind1;
    item1->ind2 = item->ind2;

    item1->p1 = NULL;
    item1->p2 = NULL;

    return item1;
}

Table * find_t1_k_v(Table * table, KeyType1 key, int version) {
    Table * table1 = create_table(1, 1);
    KeySpace1 * ks = getKey1(table->ks1, key, table->msize1);
    if (ks == NULL || ks->node == NULL)
        return table1;

    Node1 * node = ks->node;
    while (node && node->release.numberOfRelease != version) {
        node = node->next;
    }

    if (node == NULL)
        return table1;

    Item * item = copy_item(node->info);
    add_el(table1, item);

    return table1;
}

Table * find_t1_k(Table * table, KeyType1 key) {
    KeySpace1 * ks = getKey1(table->ks1, key, table->msize1);
    if (ks == NULL || ks->node == NULL) {
        return NULL;
    }
    Table * table1 = create_table(1, (ks->node->release.numberOfRelease+1));
    Node1 * node = ks->node;
    while (node) {
        Item * item = copy_item(node->info);
        add_el(table1, item);
        node = node->next;
    }
    return table1;
}

/*
 * для второй
 */

///return table with new table
///with el with key2 and it's version
Table * find_t2_k_v(Table * table, KeyType2 key, int version) {
    Table * table1 = create_table(1, 1);
    KeySpace2 * ks = getKey2(table, key);
    if (ks == NULL || ks->node == NULL)
        return table1;

    Node2 * node = ks->node;
    while (node && node->release.numberOfRelease != version) {
        node = node->next;
    }

    if (node == NULL)
        return table1;

    Item * item = copy_item(node->info);
    add_el(table1, item);

    return table1;
}

///return a new table with els
///which have the same keys2
Table * find_t2_k(Table * table, KeyType2 key) {
    KeySpace2 * ks = getKey2(table, key);
    if (ks == NULL || ks->node == NULL) {
        return NULL;
    }
    Table * table1 = create_table((ks->node->release.numberOfRelease+1), 1);
    Node2 * node = ks->node;
    while (node) {
        Item * item = copy_item(node->info);
        add_el(table1, item);
        node = node->next;
    }
    return table1;
}

///not use
void delete_data_in_node2_sp(Table * table, Node2 * node) {
    while (node) {
        int ind1 = node->info->ind1;
        Node1 * ptr = node->info->p1;
        KeySpace1 * ks = table->ks1+ind1;
        if (ks->node == NULL)
            return;
        if (ks->busy == false)
            return;

        Node1 * x = ks->node;
        while (x && x->next != ptr) {
            x = x->next;
        }
        if (x == NULL)
            return;
        if (x == ptr) {
            Node1 * tmp = ks->node;
            ks->node = x->next;
            free(tmp);
        }
        else {
            x->next = ptr->next;
            free(ptr);

            free_item(node->info);
            node = node->next;
        }
        if (ks->node == NULL) {
            ks->busy = false;
            memmove(table->ks1+ind1, table->ks1+ind1+1, sizeof(KeySpace1)*(table->msize1 - ind1-1));
        }
    }
}

///remove all els with not 0 release
void clear_table2(Table * table) {
    //KeyType1 * keys1;
    for (int i = 0; i < table->msize2; ++i) {
        KeySpace2 * ks = table->ks2+i;
        if (k)
        while (ks) {
            if (ks->node == NULL) {
                ks = ks->next;
                continue;
            }

            int n_o_k = ks->node->release.numberOfRelease;
            KeyType1 * keys = malloc(sizeof(KeyType1) * n_o_k);
            Node2 * node = ks->node->next;
            while (n_o_k && node) {
                keys[n_o_k-1].key = node->info->key1.key;
                n_o_k--;
                node = node->next;
            }
            n_o_k = ks->node->release.numberOfRelease;
            KeySpace2 * tmp = ks->next;
            for (int j = 0; j < n_o_k; ++j) {
                delete_el(table, keys[j], ks->key);
            }
            ks = tmp;
            //ks = ks->next;

            // delete_data_in_node2_sp(table, ks->node->next);
            //free_node2(ks->node->next);

            free(keys);
        }
    }
    //getchar();
}
