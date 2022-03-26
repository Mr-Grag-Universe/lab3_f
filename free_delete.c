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
    if (T == NULL)
        return;
    for (int i = 0; i < T->numberDiffKeysInT1; ++i) {
        KeySpace1 * ks = T->ks1 + i;
        //if (ks->busy) {
            Node1 * node = ks->node;
            while (node) {
                free_item(node->info);
                node = node->next;
            }
        //}
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
    while (node && node->info->key1.intKey != key1.intKey) {
        pr_node = node;
        node = node->next;
    }
    if (node == NULL) {
        fprintf(stderr, "NULL prt in searching for node\n");
        exit(333);
    }
    if (pr_node == node) {
        ks2->node = ks2->node->next;
    }

    else
        pr_node->next = node->next;
    free(node);

    number_of_nodes2(ks2->node);
}

void delete_ks2(Table * table, KeyType2 key) {
    int ind = hash_func(table, key);
    KeySpace2 * ks = table->ks2+ind;
    KeySpace2 * pr = ks;
    while (ks && keys2_eq(ks->key, key) == false) {
        pr = ks;
        ks = ks->next;
    }

    if (ks == table->ks2+ind) {
        if (ks->next == NULL) {
            ks->node = NULL;
            ks->key.intKey = 0;
        }
        else {
            KeySpace2 * tmp = ks->next;
            ks->key.intKey = ks->next->key.intKey;
            ks->node = ks->next->node;
            ks->next = ks->next->next;
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

    KeySpace1 * ks1 = getKey1(table, key1);
    KeySpace2 * ks2 = getKey2(table, key2);

    size_t n = (table->msize1-1) - ((size_t)ks1-(size_t)(table->ks1))/ sizeof(KeySpace1);
    delete_node1(ks1, key1, key2);
    if (ks1->node == NULL) {
        memmove(ks1, ks1+1, sizeof(KeySpace1) * n); // ((size_t)(table->ks1+table->msize1-1) - (size_t)ks1));
        //table->ks1[table->msize1-1].busy = false;
        table->ks1[table->msize1-1].node = NULL;
        table->numberDiffKeysInT1--;
    }
    delete_node2(ks2, key1, key2);
    if (ks2->node == NULL) {
        delete_ks2(table, key2);
    }

    /*table->numberDiffKeysInT1 = 0;
    for (int i = 0; i < table->msize1; ++i) {
        if (table->ks1[i].node == NULL)
            break;

    }*/
    free_item(item);
}

void delete_el_k1_k2_dialog(Table * table) {
    printf("What do you wanna to delete?\n1) El with 2 keys\n2) Els with 1 intKey");
    int chose_2_or_1_key = -1;
    while (chose_2_or_1_key < 1 || chose_2_or_1_key > 2) {
        printf("\n");
        chose_2_or_1_key = get_int();
    }

    switch (chose_2_or_1_key) {
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
            printf("Which intKey do you have?\n1) key1\n2) key2\n");
            int chose_key = 0;
            while (chose_key < 1 || chose_key > 2) {
                chose_key = get_int();
            }

            printf("enter your intKey:\n");
            int k = get_int();
            printf("Would you like to delete every el with intKey or only one version?\n1) all\n2) version\n");
            int chose_all_or_version = 0;
            while (chose_all_or_version < 1 || chose_all_or_version > 2) {
                chose_all_or_version = get_int();
            }

            if (chose_key == 1) {
                KeyType1 key = (KeyType1){k};
                if (chose_all_or_version == 1) {
                    delete_all_k1(table, key);
                } else if (chose_all_or_version == 2) {
                    int version = -1;
                    printf("Enter your version");
                    while (version < 0) {
                        printf("\n");
                        version = get_int();
                    }
                    delete_t1_k_v(table, key, version);
                }
            } else if (chose_key == 2) {
                KeyType2 key = (KeyType2){k};
                if (chose_all_or_version == 1) {
                    delete_all_k2(table, key);
                }
                else {
                    int version = -1;
                    printf("Enter your version\n");
                    while (version < 0) {
                        version = get_int();
                    }
                    delete_t2_k_v(table, key, version);
                }
            } else
                return;
            break;
        }
        default:
            break;
    }
}
