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

Item * find_item_in_n1(Node1 * n, KeyType2 key2) {
    Node1 * node = n;
    while (node && keys2_eq(node->info->key2, key2) == false) {
        node = node->next;
    }
    if (node == NULL)
        return NULL;
    return node->info;
}

Item * find_item(Table table, KeyType1 key1, KeyType2 key2) {
    if (!el_k1_k2_in_table(&table, key1, key2)) {
        printf("There is no element with such keys.\n");
        return NULL;
    }

    int ind = binarySearch(&table, key1);
    if (keys1_eq(table.ks1[ind].key, key1)) {
        Item * item = find_item_in_n1(table.ks1[ind].node, key2);
        return item;
        //print_item(*item);
    }
    else {
        printf("Something came wrong. Kik any programmer to fix this.\n");
    }
    /*
    for (int i = 0; i < table.msize1; ++i) {
        if (table.ks1[i].busy) {
            if (table.ks1[i].key.key == key1.key) {
                Node1 * node = table.ks1[i].node;
                while (node) {
                    if (keys2_eq(node->info->key2, key2)) {
                        printf("release1: %d; release2: %d\n", node->release.numberOfRelease, node->info->p2->release.numberOfRelease);
                        print_item(*(node->info));
                        return;
                    }
                    node = node->next;
                }
            }
        }
    }*/

    printf("Something came wrong. Kik any programmer to fix this.\n");
    return NULL;
}

void find_items_k1(Table table, KeyType1 key) {
    KeySpace1 * ks = getKey1(table.ks1, key, table.msize1);
    if (ks) {
        Node1 * node = ks->node;
        while (node) {
            print_node1(*node);
            node = node->next;
        }
        return;
    }
    printf("There is not items with such key.\n");
}
void find_items_k2(Table table, KeyType2 key) {
    KeySpace2 * ks = getKey2(&table, key);
    if (ks) {
        Node2 * node = ks->node;
        while (node) {
            print_node2(*node);
            node = node->next;
        }
        return;
    }
    printf("There is not items with such key.\n");
}

void find_el_k1_k2_dialog(Table * table) {
    printf("What are you searching for?\n1) El with 2 keys\n2) Els with 1 key\n");
    int x = -1;
    while (x < 1 || x > 2) {
        x = get_int();
    }

    switch (x) {
        ///normal 2 keys searching
        case 1: {
            printf("enter your keys:\n");
            printf("key1:\n");
            int k1 = get_int();
            printf("key2:\n");
            int k2 = get_int();
            KeyType1 key1 = (KeyType1){k1};
            KeyType2 key2 = (KeyType2){k2};
            Item * item = find_item(*table, key1, key2);
            if (item == NULL) {
                return;
            }
            print_item(*item);
            break;
        }
        ///special search with 1 key
        case 2: {
            printf("Which key do you have?\n1) key1\n2) key2\n");
            x = 0;
            while (x < 1 || x > 2) {
                x = get_int();
            }
            printf("enter your key:\n");
            int k = get_int();

            ///with key1
            if (x == 1) {
                KeyType1 key = (KeyType1){k};

                printf("Would you like to find every el with key or only one version?\n1) all\n2) version\n");
                x = 0;
                while (x < 1 || x > 2) {
                    x = get_int();
                }

                ///find every element with key1
                if (x == 1) {
                    Table * t = find_t1_k(table, key);
                    print_table(*t);
                    free_table(t);
                    //find_items_k1(*table, key);
                }
                ///find 1 el with key1 and version
                else if (x == 2) {
                    int version = -1;
                    printf("Enter your version\n");
                    while (version < 0)
                        version = get_int();

                    Table * t = find_t1_k_v(table, key, version);
                    print_table(*t);
                    free_table(t);
                }
            }
            ///with key2
            else if (x == 2) {
                KeyType2 key = (KeyType2){k};

                printf("Would you like to find every el with key or only one version?\n1) all\n2) version\n");
                x = 0;
                while (x < 1 || x > 2) {
                    x = get_int();
                }

                ///find every element with key2
                if (x == 1) {
                    Table * t = find_t2_k(table, key);
                    print_table(*t);
                    free_table(t);
                    //find_items_k1(*table, key);
                }
                ///find 1 el with key1 and version
                else if (x == 2) {
                    int version = -1;
                    printf("Enter your version\n");
                    while (version < 0)
                        version = get_int();

                    Table * t = find_t2_k_v(table, key, version);
                    print_table(*t);
                    free_table(t);
                }
                //KeyType2 key = (KeyType2){k};
                //find_items_k2(*table, key);
            } else
                return;
            break;
        }
        default:
            break;
    }
}