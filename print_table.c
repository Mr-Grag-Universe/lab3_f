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

void print_item(const Item item) {
    printf("key1: %d; key2: %d\n", item.key1.intKey, item.key2.intKey);
    // printf("release: %d\n", item.);
    printf("data: %s\n", item.info->data);
}

void print_node1(Node1 node) {
    if (node.info == NULL)
        return;
    printf("key1: %d, key2: %d\n", node.info->key1.intKey, node.info->key2.intKey);
    printf("data: %s\n", node.info->info->data);
}
void print_node2(Node2 node) {
    if (node.info == NULL)
        return;
    printf("key1: %d, key2: %d\n", node.info->key1.intKey, node.info->key2.intKey);
    printf("data: %s\n", node.info->info->data);
}

void print_table(const Table table) {
    int size1 = table.numberDiffKeysInT1;
    int size2 = table.msize2;

    printf("The first table.\n");
    for (int i = 0; i < table.numberDiffKeysInT1; ++i) {
        printf("Key: %d\n", table.ks1[i].key.intKey);
        //if (table.ks1[i].busy) {
            Node1 * node = table.ks1[i].node;
            while (node) {
                print_node1(*node);
                node = node->next;
            }
        //}
        printf("\n");
        //getchar();
    }
    printf("The second table.\n");
    for (int i = 0; i < size2; ++i) {
        printf("hash: %d\n", i);
        KeySpace2 * ks = table.ks2 + i;
        while (ks) {
            //printf("intKey: %d; busy: %d\n", ks->intKey.intKey, ks->busy);
            if (ks->busy) {
                Node2 * node = ks->node;
                while (node) {
                    print_node2(*node);
                    node = node->next;
                }
                printf("\n");
            }
            ks = ks->next;
        }
        //getchar();
    }
}
