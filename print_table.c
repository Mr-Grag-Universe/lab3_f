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
    printf("key1: %d; key2: %d\n", item.key1.key, item.key2.key);
    // printf("release: %d\n", item.);
    printf("data: %s\n", item.info->data);
}

void print_node1(Node1 node) {
    printf("key1: %d, key2: %d\n", node.info->key1.key, node.info->key2.key);
    printf("data: %s\n", node.info->info->data);
}
void print_node2(Node2 node) {
    printf("key1: %d, key2: %d\n", node.info->key1.key, node.info->key2.key);
    printf("data: %s\n", node.info->info->data);
}

void print_table(const Table table) {
    int size1 = table.msize1;
    int size2 = table.msize2;

    printf("The first table.\n");
    for (int i = 0; i < size1; ++i) {
        printf("key: %d, busy: %d\n", table.ks1[i].key.key, table.ks1[i].busy);
        if (table.ks1[i].busy) {
            Node1 * node = table.ks1[i].node;
            while (node) {
                print_node1(*node);
                node = node->next;
            }
        }
        printf("\n");
        //getchar();
    }
    printf("The second table.\n");
    for (int i = 0; i < size2; ++i) {
        printf("hash: %d\n", i);
        KeySpace2 * ks = table.ks2 + i;
        while (ks) {
            //printf("key: %d; busy: %d\n", ks->key.key, ks->busy);
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
