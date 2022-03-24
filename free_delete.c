//
// Created by Stephan on 21.03.2022.
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
#include "spetial_operations.h"


void free_item(Item * item) {
    if (item == NULL)
        return;
    free(item->info->data);
    free(item->info);
    free(item);
}

void free_items(Table * T) {
    for (int i = 0; i < T->msize1; ++i) {
        KeySpace1 * ks = T->ks1 + i;
        if (ks->busy) {
            Node1 * node = ks->node;
            while (node) {
                free_item(node->info);
                node = node->next;
            }
        }
    }
}

void free_table(Table * T) {
    free_items(T);
    //printf("free data");
    //getchar();
    free_table1(T);
    //printf("free table 1");
    //getchar();
    free_table2(T);
    //printf("free table 2");
    //getchar();
    free(T);
}

void delete_node1(KeySpace1 * ks1, KeyType1 key1, KeyType2 key2) {
    if (ks1->node == NULL) {
        fprintf(stderr, "NULL prt in searching for node\n");
        exit(333);
    }
    Node1 * node = ks1->node;
    Node1 * pr_node = ks1->node;
    while (node && keys2_eq(node->info->key2, key2) == false) {
        pr_node = node;
        node = node->next;
    }

    if (node == NULL) {
        fprintf(stderr, "NULL prt in searching for node\n");
        exit(333);
    }

    if (pr_node == node)
        ks1->node = ks1->node->next;
    else
        pr_node->next = node->next;

    free(node);

    number_of_nodes1(ks1->node);
}
void delete_node2(KeySpace2 * ks2, KeyType1 key1, KeyType2 key2) {
    Node2 * node = ks2->node;
    Node2 * pr_node = ks2->node;
    while (node && node->info->key1.key != key1.key) {
        pr_node = node;
        node = node->next;
    }
    if (node == NULL) {
        fprintf(stderr, "NULL prt in searching for node\n");
        exit(333);
    }
    if (pr_node == node)
        ks2->node = ks2->node->next;
    else
        pr_node->next = node->next;
    free(node);

    number_of_nodes2(ks2->node);
}

void delete_ks2(Table * table, KeyType2 key) {
    int ind = hash_func(key) % table->msize2;
    KeySpace2 * ks = table->ks2+ind;
    KeySpace2 * pr = ks;
    while (ks && keys2_eq(ks->key, key) == false) {
        pr = ks;
        ks = ks->next;
    }

    if (ks == table->ks2+ind) {
        if (ks->next == NULL) {
            ks->busy = false;
            ks->key.key = 0;
        }
        else {
            KeySpace2 * tmp = ks->next;
            ks->key.key = ks->next->key.key;
            ks->node = ks->next->node;
            ks->next = ks->next->next;
            ks->busy = ks->next->busy;
            free(tmp);
        }
    }
    else {
        if (ks) {
            KeySpace2 * tmp = ks->next;
            free(pr->next);
            pr->next = tmp;
        }
        else {
            return;
        }
    }
}

void delete_el(Table * table, KeyType1 key1, KeyType2 key2) {
    if (!el_k1_k2_in_table(table, key1, key2)) {
        printf("There is no element with such keys.\n");
        return;
    }

    Item * item = find_item(*table, key1, key2);
    if (item == NULL) {
        fprintf(stderr, "Something came wrong. Kik any programmer to fix this.\n");
        return;
    }

    KeySpace1 * ks1 = getKey1(table->ks1, key1, table->msize1);
    KeySpace2 * ks2 = getKey2(table, key2);

    size_t n = (table->msize1-1) - ((size_t)ks1-(size_t)(table->ks1))/ sizeof(KeySpace1);
    delete_node1(ks1, key1, key2);
    if (ks1->node == NULL) {
        memmove(ks1, ks1+1, sizeof(KeySpace1) * n); // ((size_t)(table->ks1+table->msize1-1) - (size_t)ks1));
        table->ks1[table->msize1-1].busy = false;
    }
    delete_node2(ks2, key1, key2);
    if (ks2->node == NULL) {
        delete_ks2(table, key2);
    }
    //if (ks2->node == NULL) {
    //    ks2->busy = false;
    //}
    //getchar();
    free_item(item);
}

void delete_el_k1_k2_dialog(Table * table) {
    printf("What do you wanna to delete?\n1) El with 2 keys\n2) Els with 1 key");
    int x = -1;
    while (x < 1 || x > 2) {
        printf("\n");
        x = get_int();
    }

    switch (x) {
        case 1: {
            printf("enter keys of this element:\n");
            printf("key1:\n");
            int k1 = get_int();
            printf("key2:\n");
            int k2 = get_int();

            KeyType1 key1 = (KeyType1){k1};
            KeyType2 key2 = (KeyType2){k2};

            if (!el_k1_k2_in_table(table, key1, key2)) {
                printf("there is not such element in table\n");
                return;
            }

            delete_el(table, key1, key2);
            break;
        }
        case 2: {
            printf("Which key do you have?\n1) key1\n2) key2\n");
            x = 0;
            while (x < 1 || x > 2) {
                printf("\n");
                x = get_int();
            }

            printf("enter your key:\n");
            int k = get_int();

            if (x == 1) {
                printf("Would you like to delete every el with key or only one version?\n1) all\n2) version\n");
                x = 0;
                while (x < 1 || x > 2) {
                    printf("\n");
                    x = get_int();
                }
                KeyType1 key = (KeyType1){k};
                if (x == 1) {
                    KeySpace1 * ks = getKey1(table->ks1, key, table->msize1);
                    int number = number_of_nodes1(ks->node);
                    int i = 0;
                    KeyType2 * keys = malloc(sizeof(KeyType2) * number);
                    Node1 * node = ks->node;
                    while (node) {
                        keys[i].key = node->info->key2.key;
                        i++;
                        node = node->next;
                    }
                    for (int j = 0; j < number; ++j)
                        delete_el(table, key, keys[j]);
                    free(keys);
                } else if (x == 2) {
                    int version = -1;
                    printf("Enter your version");
                    while (version < 0) {
                        printf("\n");
                        version = get_int();
                    }
                    delete_t1_k_v(table, key, version);
                }
            } else if (x == 2) {
                KeyType2 key = (KeyType2){k};
                KeySpace2 * ks = getKey2(table, key);
                int number = number_of_nodes2(ks->node);
                int i = 0;
                KeyType1 * keys = malloc(sizeof(KeyType1) * number);
                Node2 * node = ks->node;
                while (node) {
                    keys[i].key = node->info->key1.key;
                    i++;
                    node = node->next;
                }
                for (int j = 0; j < number; ++j)
                    delete_el(table, keys[j], key);
                free(keys);
            } else
                return;
            break;
        }
        default:
            break;
    }
}
