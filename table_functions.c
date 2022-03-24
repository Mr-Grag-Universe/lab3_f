//
// Created by Stephan on 14.03.2022.
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

void clear_table(Table * T) {
    // clear_ks1(T->ks1);
    // clear_ks2(T->ks2);
    T->msize1 = 0;
    T->msize2 = 0;
    T->csize1 = 0;
    T->csize2 = 0;
}

/*
 * функции для обнаружения элемента в таблице
 */

enum TableCondition {
    TABLE_IS_OVER,
    THERE_IS_NO_FREE_KS,
    THERE_IS_FREE_KS,
};

enum TableCondition table_condition(Table * table) {
    enum TableCondition tc = TABLE_IS_OVER;
    for (int i = 0; i < table->msize1; ++i) {
        if (table->ks1[i].busy) {
            Node1 * node = table->ks1[i].node;
            int n = 0;
            while (node) {
                node = node->next;
                n++;
            }
            if (n == table->msize2)
                continue;
            else {
                tc = THERE_IS_NO_FREE_KS;
                continue;
            }
        }
        else
            return THERE_IS_FREE_KS;
    }
    return tc;
}

bool can_insert(Table * table, Item * item) {
    if (table->ks1[table->msize1-1].busy) {
        if (item->key1.key < table->ks1[0].key.key) {
            return false;
        }
        if (item->key1.key > table->ks1[table->msize1-1].key.key) {
            return false;
        }
    }
    return true;
}

void add_el(Table * table, Item * item) {
    //KeyType1 key1 = item->key1;
    //KeyType2 key2 = item->key2;

    if (0 && table_condition(table) == TABLE_IS_OVER) {
        printf("This table is overflow\n");
        exit(TABLE_IS_OVERFLOW);
    }
    if (can_insert(table, item) == false) {
        free_item(item);
        return;
    }

    add_el_in_KS1(table, item);
    add_el_in_KS2(table, item);
    //print_table(*table);
}

/*
 * вспомогательные функции
 * создают диалог
 * считывают инфу
 * преобразуют инфу в итем
 * и передают в основную функцию добавления
 */

bool el_k1_k2_in_table(Table * table, KeyType1 key1, KeyType2 key2) {
    bool x = k1_in_table1(table, key1);
    //bool y = k2_in_table2(table, key2);
    return x; // && y;
}

Item * create_item(Table * table, InfoType * info, KeyType1 key1, KeyType2 key2) {
    if (0 && (key1.key < 0 || key1.key > table->msize1)) {
        // free_item(item);
        printf("Unresolved key\n");
        return NULL;
    }
    Item * item = malloc(sizeof(Item));
    item->info = info;
    item->key1.key = key1.key;
    item->key2.key = key2.key;
    item->ind1 = 0;
    item->ind2 = hash_func(key2)%table->msize2;

    bool x = el_k1_k2_in_table(table, item->key1, item->key2);
    bool n1 = k1_in_table1(table, key1);
    bool n2 = k2_in_table2(table, key2);
    bool y = can_insert(table, item);
    if (x || y == false) {

        free(item);
        return NULL;
    }

    return item;
}

InfoType * get_info() {
    InfoType * info = malloc(sizeof(InfoType));
    printf("Enter your data here:\n-> ");
    info->data = get_line();
    return info;
}

void add_info_dialog(Table * table) {
    printf("enter your info.\n");
    InfoType * info = get_info();       // считываем введенную информацию

    ///get_line();

    KeyType1 key1 = enter_key1();
    KeyType2 key2 = enter_key2();

    Item * item = create_item(table, info, key1, key2);    // на основе полученных данных формируем итем

    if (item == NULL) {
        free(info->data);
        free(info);
        printf("this table is empty or your key is wrong\n");
        //exit(TABLE_IS_OVERFLOW);
        return;
    }
    printf("item was created\n");
    add_el(table, item);                // добавляем итем в таблицу
}