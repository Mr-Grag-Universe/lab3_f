//
// Created by Stephan on 20.03.2022.
//
#include "stdlib.h"
#include "string.h"
#include "headers/table_structs.h"
#include "headers/table_1_funcs.h"
#include "headers/table_2_funcs.h"
#include "table_creation.h"

KeySpace1 create_KS1(int i) {
    KeySpace1 ks;
    ks.node = NULL;
    ks.key = (KeyType1) {0};
    return ks;
}
KeySpace2 create_KS2(int i) {
    KeySpace2  ks;
    ks.node = NULL;
    ks.next = NULL;
    ks.key = (KeyType2) {0};
    return ks;
}

Table * create_table(int msize1, int msize2) {
    Table * table = malloc(sizeof(Table));
    table->msize1 = msize1;
    table->msize2 = msize2;
    table->numberDiffKeysInT1 = 0;

    table->ks1 = malloc(sizeof(KeySpace1) * (msize1));
    for (int i = 0; i < msize1; ++i) {
        table->ks1[i] = create_KS1(i);
        //table->ks1[i].busy = false;
    }

    table->ks2 = malloc(sizeof(KeySpace2) * (msize2));
    for (int i = 0; i < msize2; ++i) {
        table->ks2[i] = create_KS2(i);
        table->ks2[i].node = NULL;
    }

    return table;
}