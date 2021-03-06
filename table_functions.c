//
// Created by Stephan on 14.03.2022.
//

#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
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

enum TableCondition {
    TABLE_IS_OVER,
    THERE_IS_NO_FREE_KS,
    THERE_IS_FREE_KS,
};

enum TableCondition table_condition(Table * table) {
    enum TableCondition tc = TABLE_IS_OVER;
    for (int i = 0; i < table->numberDiffKeysInT1; ++i) {
        //if (table->ks1[i].busy) {
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
        //else
        //    return THERE_IS_FREE_KS;
    //}
    return tc;
}

bool can_insert_in_t1(Table * table, KeyType1 key1, KeyType2 key2) {
    int indexForInsert = binarySearch(table, key1);
    if (indexForInsert >= table->msize1)
        return false;

    KeySpace1 ks = table->ks1[indexForInsert];
    if (table->numberDiffKeysInT1 == table->msize1) {
        if (keys1_eq(ks.key, key1)) {
            //KeySpace2 * ks2 = getKey2(table, key2);
            //bool thereIsFreeNode = (ks.node == NULL || ks2->node == NULL);
            return true; //thereIsFreeNode;
        }
        return false;
    }

    return true;
}

bool add_el(Table * table, Item * item) {
    KeyType1 key1 = item->key1;
    KeyType2 key2 = item->key2;
    InfoType * info = item->info;

    if (can_insert_in_t1(table, key1, key2) == false || el_k1_k2_in_table(table, key1, key2)) {
        free_item(item);
        printf("Unresolved Key...");
        return false;
    }

    add_el_in_KS1(table, item);
    add_el_in_KS2(table, item);
    return true;
}

bool el_k1_k2_in_table(Table * table, KeyType1 key1, KeyType2 key2) {
    int ind = binarySearch(table, key1);
    if (table->msize1 <= ind || ind < 0)
        return false;
    if (keys1_eq(key1, table->ks1[ind].key)) {
        Node1 * n = table->ks1[ind].node;
        while (n) {
            if (keys2_eq(n->info->key2, key2))
                return true;
            n = n->next;
        }
    }
    return false;
}

Item * create_item(Table * table, InfoType * info, KeyType1 key1, KeyType2 key2) {
    Item * item = malloc(sizeof(Item));
    if (item == NULL) {
        return NULL;
    }
    item->info = info;
    item->key1.intKey = key1.intKey;
    item->key2.intKey = key2.intKey;
    item->ind1 = 0;
    item->ind2 = hash_func(table, key2);

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
    InfoType * info = get_info();       // ?????????????????? ?????????????????? ????????????????????

    KeyType1 key1 = enter_key1();
    KeyType2 key2 = enter_key2();

    if (can_insert_in_t1(table, key1, key2) == false || el_k1_k2_in_table(table, key1, key2)) {
        free(info->data);
        free(info);
        printf("Unresolved intKey...");
        return;
    }

    Item * item = create_item(table, info, key1, key2);    // ???? ???????????? ???????????????????? ???????????? ?????????????????? ????????

    if (item == NULL) {
        free(info->data);
        free(info);
        printf("this table is empty or your intKey is wrong\n");
        return;
    }
    printf("item was created\n");
    // ?????????????????? ???????? ?? ??????????????
    if (add_el(table, item) == false)
        printf("something came wrong");
}