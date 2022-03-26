//
// Created by Stephan on 26.03.2022.
//
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stdbool.h"
#include "KGetLine.h"
#include "time.h"
#include "spetial_operations.h"
#include "headers/commands.h"
#include "MyString.h"
#include "headers/table_creation.h"
#include "table_functions.h"
#include "spetial_operations.h"

void generate_keys(Table * table, KeyType1 * key1, KeyType2 * key2) {
    if (key1 == NULL || key2 == NULL)
        return;
    (*key1) = (KeyType1){(rand()%101-50)};
    (*key2) = (KeyType2){rand()%100-50};
}

char * generate_string(int len) {
    char * string = (char*) malloc(sizeof(char) * (len + 1));
    for (int j = 0; j < len; ++j) {
        char x = rand()%2;
        string[j] = (char)((int)(x ? 'A' : 'a') + rand() % 26);
    }
    string[len] = '\0';
    return string;
}

Item * generate_item(Table * table) {
    char * data = generate_string(rand()%9+1);
    InfoType * info = malloc(sizeof(InfoType));
    info->data = data;
    KeyType1 key1;
    KeyType2 key2;
    generate_keys(table, &key1, &key2);
    Item * item = create_item(table, info, key1, key2);
    return item;
}

void generate_items(Table * table, int n) {
    for (int i = 0; i < n; ++i) {
        add_el(table, generate_item(table));
    }
}

bool TEST_ALL(Table * table) {
    printf("ADD 1000 NEW ELEMENTS\n");
    generate_items(table, 1000);

    printf("CLEAR TABLE 2\n");
    clear_table2(table);
    generate_items(table, 1000);

    printf("SEARCHING FOR ELEMENTS\n");
    printf("searching for the k1-k2 el\n");
    getchar();
    for (int i = 0; i < 1000; ++i) {
        KeyType1 key1;
        KeyType2 key2;
        generate_keys(table, &key1, &key2);
        Item * item = find_item(*table, key1, key2);
        if (item == NULL)
            continue;
        //print_item(*item);
    }
    printf("searching for the k1 and k2 all\n");
    getchar();

    for (int i = 0; i < 100; ++i) {
        KeyType1 key1;
        KeyType2 key2;
        generate_keys(table, &key1, &key2);
        Table * t1 = find_t1_k(table, key1);
        Table * t2 = find_t2_k(table, key2);
        free_table(t1);
        free_table(t2);
    }
    printf("searching for the k1 and k2 version\n");
    getchar();

    for (int i = 0; i < 100; ++i) {
        int version = rand() % 10 -4;
        KeyType1 key1;
        KeyType2 key2;
        generate_keys(table, &key1, &key2);
        Table * t1 = find_t1_k_v(table, key1, version);
        Table * t2 = find_t2_k_v(table, key2, version);
        free_table(t1);
        free_table(t2);
    }

    printf("DELETING ELEMENTS\n");
    for (int i = 0; i < 1000; ++i) {
        KeyType1 key1;
        KeyType2 key2;
        generate_keys(table, &key1, &key2);
        delete_el(table, key1, key2);
    }

    for (int i = 0; i < 100; ++i) {
        KeyType1 key1;
        KeyType2 key2;
        generate_keys(table, &key1, &key2);
        delete_all_k1(table, key1);
        delete_all_k2(table, key2);
    }

    generate_items(table, 100);
    for (int i = 0; i < 100; ++i) {
        int version = rand() % 10 -4;
        KeyType1 key1;
        KeyType2 key2;
        generate_keys(table, &key1, &key2);
        delete_t1_k_v(table, key1, version);
        delete_t2_k_v(table, key2, version);
    }

    return true;
}