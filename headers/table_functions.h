//
// Created by Stephan on 11.03.2022.
//

#ifndef LAB3_TABLE_FUNCTIONS_H
#define LAB3_TABLE_FUNCTIONS_H
void free_table(Table * T);
void clear_table(Table * T);
void add_info_dialog(Table * table);
void print_table(const Table table);
void print_node1(Node1 node);
void print_node2(Node2 node);
void print_item(const Item item);
int binarySearch(Table * table, KeyType1 key);
Item * find_item_in_n1(Node1 * n, KeyType2 key2);
Item * find_item(Table table, KeyType1 key1, KeyType2 key2);
void free_item(Item * item);
bool el_k1_k2_in_table(Table * table, KeyType1 key1, KeyType2 key2);
void find_el_k1_k2_dialog(Table * table);
void delete_el_k1_k2_dialog(Table * table);
KeySpace1 * getKey1(Table * table, KeyType1 key1);
KeySpace2 * getKey2(Table * table, KeyType2 key);
void delete_el(Table * table, KeyType1 key1, KeyType2 key2);
bool add_el(Table * table, Item * item);
Item * create_item(Table * table, InfoType * info, KeyType1 key1, KeyType2 key2);
void free_node2(Node2 * node);
#endif //LAB3_TABLE_FUNCTIONS_H
