//
// Created by Stephan on 17.03.2022.
//

#ifndef LAB3_SPETIAL_OPERATIONS_H
#define LAB3_SPETIAL_OPERATIONS_H
#include "table_structs.h"
void delete_t1_k_v(Table * table, KeyType1 key, int version);
Table * find_t1_k_v(Table * table, KeyType1 key, int version);
Table * find_t2_k_v(Table * table, KeyType2 key, int version);
Table * find_t2_k(Table * table, KeyType2 key);
Table * find_t1_k(Table * table, KeyType1 key);
void clear_table2(Table * table);
#endif //LAB3_SPETIAL_OPERATIONS_H
