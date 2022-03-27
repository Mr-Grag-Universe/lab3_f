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
#include "table_1_funcs.h"
#include "table_creation.h"
#include "table_functions.h"

void delete_t1_k_v(Table * table, KeyType1 key, int version) {
    KeySpace1 * ks = getKey1(table, key);
    if (ks == NULL)
        return;

    if (ks->node == NULL || ks->node->release.numberOfRelease < version) {
        return;
    }

    Node1 * node = ks->node;
    // version = node->release.numberOfRelease - version;
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
    item1->key1.intKey = item->key1.intKey;
    item1->key2.intKey = item->key2.intKey;

    item1->ind1 = item->ind1;
    item1->ind2 = item->ind2;

    item1->p1 = NULL;
    item1->p2 = NULL;

    return item1;
}

Table * find_t1_k_v(Table * table, KeyType1 key, int version) {
    Table * table1 = create_table(1, 1);
    KeySpace1 * ks = getKey1(table, key);
    if (ks == NULL)
        return table1;
    if (ks == NULL || ks->node == NULL)
        return table1;

    //if (ks->busy == false)
    //    return table1;
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
    KeySpace1 * ks = getKey1(table, key);
    if (ks == NULL || ks->node == NULL) {
        return NULL;
    }
    Table * table1 = create_table(1, (ks->node->release.numberOfRelease+1));


    Node1 * node = ks->node;
    //if (ks->busy == false)
    //    return table1;
    while (node) {
        Item * item = copy_item(node->info);
        add_el(table1, item);
        node = node->next;
    }
    return table1;
}


void delete_all_k1(Table * table, KeyType1 key1) {
    KeySpace1 * ks = getKey1(table, key1);
    if (ks == NULL) {
        printf("there is no such intKey\n");
        return;
    }
    int number = number_of_nodes1(ks->node);
    int i = 0;
    KeyType2 * keys = malloc(sizeof(KeyType2) * number);
    Node1 * node = ks->node;
    while (node) {
        keys[i].intKey = node->info->key2.intKey;
        i++;
        node = node->next;
    }
    for (int j = 0; j < number; ++j)
        delete_el(table, key1, keys[j]);
    free(keys);
}

///return table with new table
///with el with key2 and it's version
void delete_all_k2(Table * table, KeyType2 key) {
    KeySpace2 * ks = getKey2(table, key);
    if (ks == NULL) {
        printf("there is no such intKey\n");
        return;
    }
    //int number = number_of_nodes2(ks->node);
    int i = 0;
    if (ks->node == NULL)
        return;
    if (ks->node == NULL)
        return;
    int number = ks->node->release.numberOfRelease+1;
    KeyType1 * keys = malloc(sizeof(KeyType1) * number);
    Node2 * node = ks->node;
    while (node) {
        keys[i].intKey = node->info->key1.intKey;
        i++;
        node = node->next;
    }
    for (int j = 0; j < number; ++j)
        delete_el(table, keys[j], key);
    free(keys);
}

void delete_t2_k_v(Table * table, KeyType2 key, int version) {
    KeySpace2 * ks = getKey2(table, key);
    if (ks == NULL) {
        printf("there is no such intKey\n");
        return;
    }
    Node2 * node = ks->node;
    Node2 * pr_n = ks->node;
    if (node == NULL)
        return;
    if (node->release.numberOfRelease < version) {
        printf("too big version\n");
        return;
    }
    // version = node->release.numberOfRelease - version;
    KeyType1 key1 = (KeyType1) {0};
    while (node) {
        if (node->release.numberOfRelease == version) {
            key1.intKey = node->info->key1.intKey;
            break;
        }
        pr_n = node;
        node = node->next;
    }
    if (node == NULL) {
        printf("not found\n");
        return;
    }
    delete_el(table, key1, key);
}


Table * find_t2_k_v(Table * table, KeyType2 key, int version) {
    Table * table1 = create_table(1, 1);
    KeySpace2 * ks = getKey2(table, key);
    if (ks == NULL)
        return table1;
    if (ks == NULL || ks->node == NULL)
        return table1;

    Node2 * node = ks->node;
    if (ks->node == NULL)
        return table1;
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
    if (ks->node == NULL)
        return table1;
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
            //ks->busy = false;
            memmove(table->ks1+ind1, table->ks1+ind1+1, sizeof(KeySpace1)*(table->msize1 - ind1-1));
        }
    }
}

///remove all els with not 0 release
void clear_table2(Table * table) {
    //KeyType1 * keys1;
    for (int i = 0; i < table->msize2; ++i) {
        KeySpace2 * ks = table->ks2+i;
        if (ks->node == NULL)
            continue;
        while (ks) {
            if (ks->node == NULL) {
                ks = ks->next;
                continue;
            }

            int numberOfKeys = ks->node->release.numberOfRelease;
            KeyType1 * keys = malloc(sizeof(KeyType1) * numberOfKeys);
            Node2 * node = ks->node->next;
            while (numberOfKeys && node) {
                keys[numberOfKeys - 1].intKey = node->info->key1.intKey;
                numberOfKeys--;
                node = node->next;
            }
            numberOfKeys = ks->node->release.numberOfRelease;
            KeySpace2 * tmp = ks->next;
            for (int j = 0; j < numberOfKeys; ++j) {
                delete_el(table, keys[j], ks->key);
            }
            ks->node->release.numberOfRelease = 0;
            ks = tmp;

            free(keys);
        }
    }
}
