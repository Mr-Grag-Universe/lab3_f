//
// Created by Stephan on 11.03.2022.
//

#ifndef LAB3_TABLE_1_FUNCS_H
#define LAB3_TABLE_1_FUNCS_H
KeySpace1 create_KS1(int i);
KeyType1 enter_key1();
int create_ind1();
void add_el_in_KS1(Table * table, Item * item);
void free_table1(Table * table);
bool k1_in_table1(Table * table, KeyType1 key);
KeySpace1 * get_KS1(Table table, KeyType1 key);
bool keys1_eq(KeyType1 key1, KeyType1 key2);
void cope_key1(KeyType1 * res, KeyType1 key);
int number_of_nodes1(Node1 * node);
#endif //LAB3_TABLE_1_FUNCS_H
