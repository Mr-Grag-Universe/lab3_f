#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stdbool.h"
#include "KGetLine.h"
#include "spetial_operations.h"
#include "headers/commands.h"
#include "time.h"
#include "MyString.h"
#include "headers/table_creation.h"
#include "table_functions.h"
#include "tests.h"

//typedef menu_funcs (void (*) (Table *));
//typedef menu_funcs_arr (void (**) (Table *));

void clrscr(){
    system("@cls||clear");
}

char ** init_menu_points() {
    char ** menu_points = malloc(sizeof(char*) * (UNKNOWN_COMMAND));
    menu_points[EXIT] = "exit";
    menu_points[ADD_NEW_EL] = "add a new element";
    menu_points[CLEAR_TABLE2] = "clear table 2";
    menu_points[DELETE_EL] = "delete an element";
    menu_points[FIND_EL] = "find an element";
    menu_points[PRINT_TABLE] = "print table";
    return menu_points;
}

void print_menu(char ** menu) {
    printf("This is menu:\n");
    for (int i = 0; i < UNKNOWN_COMMAND; ++i) {
        printf("%d) %s\n", i, menu[i]);
    }
}

bool check_command(char * command) {

    return true;
}

bool execute_command(Table * table, Command command) {
    switch (command) {
        case EXIT: {
            printf("OK. Goodbye!\n");
            return true;
        }
        case ADD_NEW_EL: {
            add_info_dialog(table);
            return false;
        }
        case FIND_EL: {
            find_el_k1_k2_dialog(table);
            return false;
        }
        case DELETE_EL: {
            delete_el_k1_k2_dialog(table);
            return false;
        }
        case PRINT_TABLE: {
            print_table(*table);
            return false;
        }
        case CLEAR_TABLE2: {
            clear_table2(table);
            return false;
        }
        default: {
            printf("Your command is wrong!\n");
            return false;
        }
    }
    return false;
}

Command get_command_code(char * command) {
    if (is_digit(command)) {
        int x = atoi(command);
        if (x < 0 || x > UNKNOWN_COMMAND)
            return UNKNOWN_COMMAND;
        return x;
    }

    if (!strcmp(command, "exit"))
        return EXIT;
    else if (!strcmp(command, "find"))
        return FIND_EL;
    else if (!strcmp(command, "add"))
        return ADD_NEW_EL;
    else if (!strcmp(command, "delete"))
        return DELETE_EL;
    else if (!strcmp(command, "print"))
        return PRINT_TABLE;
    else if (!strcmp(command, "clear2"))
        return CLEAR_TABLE2;
    else return UNKNOWN_COMMAND;
}

Table * start_session(int m1, int m2) {
    Table * table = create_table(m1, m2);
    return table;
}

int main() {
    srand(time(NULL));
    bool finish = false;
    char ** menu = init_menu_points();
    int msize1 = 10;
    int msize2 = 10;
    Table * table = start_session(msize1, msize2); //create_table(msize1, msize2);

    TEST_ALL(table);

    while (!finish) {
        /// clrscr();
        print_menu(menu);

        char * command = get_line();
        if (command[0] == '\0') {
            free(command);
            continue;
        }
        //delete_double_spaces(&command);

        if (check_command(command) == false) {
            printf("Your command is wrong.");
            free(command);
            continue;
        }

        Command command_code = get_command_code(command);

        finish = execute_command(table, command_code);

        free(command);
    }

    free_table(table);
    free(menu);

    return 0;
}
