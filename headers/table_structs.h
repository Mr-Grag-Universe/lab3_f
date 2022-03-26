//
// Created by Stephan on 11.03.2022.
//

#ifndef QUEUE_TABLE_STRUCTS_H
#define QUEUE_TABLE_STRUCTS_H

struct KeySpace1;
struct KeySpace2;
struct Items;
struct Node1;
struct Node2;

struct InfoType;
struct KeyType1;
struct KeyType2;
struct RelType1;
struct RelType2;

typedef struct InfoType {
    char * data;
} InfoType;

typedef struct KeyType1 {
    int intKey;
} KeyType1;
typedef struct KeyType2 {
    int intKey;
} KeyType2;

typedef struct RelType1 {
    int numberOfRelease;
} RelType1;
typedef struct RelType2 {
    int numberOfRelease;
} RelType2;

typedef struct KeySpace1 {
    //bool busy;
    // ключ элемента
    KeyType1 key;
    // указатель на информацию
    struct Node1 *node;
    int numberOfNodes;
} KeySpace1;
typedef struct KeySpace2 {
    //bool busy;
    // ключ элемента
    // хэши от ключей - индекс в таблице
    KeyType2 key;
    // указатель на информацию
    struct Node2 *node;
    // указатель на следующий элемент
    struct KeySpace2 *next;
    int numberOfNodes;
} KeySpace2;

typedef struct Items {
    // указатель на информацию
    InfoType * info;
    // опциональные поля, для оптимизации выполнения операций,
    // состав и наличие которых должны быть обоснованы:
    // ключ элемента из 1-го пространства ключей
    struct KeyType1 key1;
    // ключ элемента из 2-го пространства ключей
    struct KeyType2 key2;
    // связь с элементом 1-го пространства ключей по индексу
    int ind1;
    // связь с элементом 2-го пространства ключей по индексу
    int ind2;
    // связь с элементом 1-го пространства ключей по указателю
    //PointerType1 *p1;
    struct Node1 * p1;
    // связь с элементом 2-го пространства ключей по указателю
    //PointerType2 *p2;
    struct Node2 * p2;
} Item;

typedef struct Node1 {
    // номер версии
    RelType1 release;
    // указатель на информацию
    Item *info;
    // указатель на следующий элемент
    struct Node1 *next;
} Node1;
typedef struct Node2 {
    // номер версии элемента
    RelType2 release;
    // указатель на информацию
    Item *info;
    // указатель на следующий элемент
    struct Node2 *next;
} Node2;

typedef struct Table {
    // указатель на первое пространство ключей
    struct KeySpace1 *ks1;
    // указатель на второе пространство ключей
    struct KeySpace2 *ks2;

    // опциональные поля, ограничивающие размер пространства ключей,
    // их наличие определяется типом организации соответствующего пространства,
    // в соответствии с условиями индивидуального задания

    // размер области первого пространства ключей
    int msize1;
    // размер области второго пространства ключей
    int msize2;

    // опциональные поля с текущим количеством элементов
    // в пространстве ключей,
    // их наличие определяется типом организации соответствующего пространства,
    // в соответствии с условиями индивидуального задания

    // количество элементов в области первого пространства ключей
    int csize1;
    // количество элементов в области второго пространства ключей
    int csize2;

    int numberDiffKeysInT1;
} Table;

#endif //QUEUE_TABLE_STRUCTS_H

// KeySpace1
//    KeyType1      - ключ элемента - password1
//    []Node1       - указатель на информацию
//       RelType1       - номер версии
//       Item           - указатель на информацию
//          InfoType        - указатель на информацию
//          KeyType    1 2  - ключ элемента - password1
//          IndexType  1 2  - связь с элементом 1 2 пространства ключей по индексу
//          PointerType1 2  - связь с элементом 1 2 пространства ключей по указателю
//       Node1
//          ...
