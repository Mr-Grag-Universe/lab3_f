//
// Created by Stephan on 11.03.2022.
//

#ifndef LAB3_TABLE_2_FUNCS_H
#define LAB3_TABLE_2_FUNCS_H
KeySpace2 create_KS2(int i);
void add_el_in_KS2(Table * table, Item * item);
int hash_func(Table * table, KeyType2 key);
void free_table2(Table * table);
bool k2_in_table2(Table * table, KeyType2 key);
KeySpace2 * get_KS2(Table table, KeyType2 key);
bool keys2_eq(KeyType2 key1, KeyType2 key2);
KeyType2 * cope_key2(KeyType2 key);
KeyType2 enter_key2();
bool keys2_eq(KeyType2 key1, KeyType2 key2);
int number_of_nodes2(Node2 * node);
#endif //LAB3_TABLE_2_FUNCS_H
