#ifndef CUSTOM_ALLOCATOR_H
#define CUSTOM_ALLOCATOR_H

#define CUSTOM_MEMORY_SIZE 64 // custom memory size
#define MAX_TYPE_NAME_SIZE 10 // data type name's maximum size
#define MAX_DATA_NAME_SIZE 15 // data name's maximum size

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>


typedef enum data_type { short_type, char_type, 
                         float_type, long_type, invalid } data_type;

data_type which_type(char *type)
{
// returns enum "data_type"'s value that corresponds to data type that input is directing
    if (strcmp(type, "short") == 0) return short_type;
    else if (strcmp(type, "char") == 0) return char_type;
    else if (strcmp(type, "float") == 0) return float_type;
    else if (strcmp(type, "long") == 0) return long_type;
    else return invalid; // invalid input that is not short, char, float, long
}

typedef struct data{
    int order; // order of node, this value of the last node is the length of the list
    int bytes; // allocated size until the current node, this value of the last node is the total allocated bytes
    char name[MAX_DATA_NAME_SIZE]; // name of node 
    struct data *prev; // pointer to the previous node  
} data;

// if there's enough memory for the request for allocation, or a deallocation is requested
// print out memory dump(function dump_mem) and the name of data allocated in memory(function curr_data)
void dump_mem(const char *mem, size_t len) {
    const char *buffer = mem; 
    size_t i;
    for (i = 0; i < len; i++) {
        if (i > 0 && i % 16 == 0) {
            printf("\n");
        }
        printf("%02x ", buffer[i] & 0xff);
    }
    puts("");
}

void curr_data(data **data_list){
    char curr_data_arr[(*data_list)->order][MAX_DATA_NAME_SIZE];
    data *cp_list = *data_list;
    // save current data in array
    for (int i = 0; i < (*data_list)->order; i++){
        strncpy(curr_data_arr[i], cp_list->name, MAX_DATA_NAME_SIZE);
        cp_list = cp_list->prev;
    } 
    // print current data from array
    for (int i = (*data_list)->order - 1; i >= 0; i--){
        printf("%s\n", curr_data_arr[i]);
    } 
}

void memory_and_data(char *memory_dump, data **data_list){
    // show memory dump
    puts("There is memory dump!");
    dump_mem(memory_dump, 64);
    printf("\n");
    puts("------Data you have now------");
    curr_data(data_list);
    printf("\n");
}



char required_bytes(char *type) 
{
// it returns the required bytes for the requested type
    switch(which_type(type)){
        default:
        case short_type:
            return sizeof(short);
        case char_type:
            return sizeof(char);
        case float_type:
            return sizeof(float);
        case long_type:
            return sizeof(long);
    }
}

/* ALLOCATION */

// if there's enough memory for the request for allocation
void add_node(int new_bytes, char *name, data **data_list)
{
// add node to the existing linked list "data_list"
    data *old_data = *data_list; 
    *data_list = (data *)malloc(sizeof(data));
    (*data_list)->order = old_data->order + 1;
    (*data_list)->bytes = old_data->bytes + new_bytes;
    strncpy((*data_list)->name, name, MAX_DATA_NAME_SIZE); 
    (*data_list)->prev = old_data;
}

void write_to_mem(char **memory_ptr, char *type) 
{
// write the value to the memory dump according to the data type
    switch(which_type(type)){
        default:
        case short_type:
            scanf("%hd", (short *)(*memory_ptr));
            *memory_ptr += sizeof(short);
            break;
        case char_type:
            scanf(" %c", *memory_ptr);
            *memory_ptr += sizeof(char);
            break;
        case float_type:
            scanf("%f", (float *)(*memory_ptr));
            *memory_ptr += sizeof(float);
            break;
        case long_type:
            scanf("%ld", (long *)(*memory_ptr));
            *memory_ptr += sizeof(long);
            break;
    }
}

// for saving various data_type as an array
typedef union val_type { short short_val;
                         char char_val;
                         float float_val;
                         long long_val; } val_type;

void save_struct(val_type *ele_val, char *type)
{
// save values of the elements of struct 
    switch(which_type(type)){ 
        case short_type:
            scanf("%hd", &(ele_val->short_val));
            break;
        case char_type:
            scanf(" %c", &(ele_val->char_val));
            break;
        case float_type:
            scanf("%f", &(ele_val->float_val));
            break;
        case long_type:
            scanf("%ld", &(ele_val->long_val));
            break;
        case invalid:
            while(getchar() != '\n'){};
    }
}

// after checking there is enough memory for struct type
void write_to_mem_for_Struct(char **memory_ptr, char *type, val_type *ele_val)
{
// write previously saved values to the memory dump according to the data type
    switch(which_type(type)){
        default:
        case short_type:
            *((short *)(*memory_ptr)) = ele_val->short_val;
            (*memory_ptr) += sizeof(short);
            break;
        case char_type:
            **memory_ptr = ele_val->char_val;
            (*memory_ptr) += sizeof(char);
            break;
        case float_type:
            *((float *)(*memory_ptr)) = ele_val->float_val;
            (*memory_ptr) += sizeof(float);
            break;
        case long_type:
            *((long *)(*memory_ptr)) = ele_val->long_val;
            (*memory_ptr) += sizeof(long);
            break;
    }
}

char allocate(data **data_list, char **memory_ptr) 
{
    puts("Input the type of data you want to allocate and the name of the data");
    int bytes = (*data_list)->bytes;
    int new_bytes = 0;
    char *type = (char *) malloc(MAX_TYPE_NAME_SIZE); 
    char *name = (char *) malloc(MAX_DATA_NAME_SIZE); 
    scanf("%s %s", type, name);
    // if the type is struct
    if (strcmp(type, "struct") == 0)
    {
        puts("How many data should be in the struct");
        int ele_num;
        scanf("%d", &ele_num);
        char ele_type[ele_num][MAX_TYPE_NAME_SIZE];
        val_type ele_val[ele_num];
        puts("Please input each type and its value");
        // first save inputs 
        for (int i = 0; i < ele_num; i++){
            scanf("%s ", ele_type[i]); 
            save_struct(&(ele_val[i]), ele_type[i]);
            new_bytes += required_bytes(ele_type[i]);
            if (bytes + new_bytes - 64 > 0){ // not enough memory
                printf("There is not enough memory for the data, you can only use %d byte(s)\n", 64-bytes);
                free(type); 
                free(name); 
                return 0;
            }
            if (which_type(ele_type[i]) == invalid){
                free(type);
                free(name);
                return 0;
            }
        }
        //enough memory
        add_node(new_bytes, name, data_list);
        for (int i = 0; i < ele_num; i++) 
            write_to_mem_for_Struct(memory_ptr, ele_type[i], &(ele_val[i]));
    }
    // if the type is invalid
    else if (which_type(type) == invalid){ 
        free(type); 
        free(name); 
        return 0;
    }
    // if the type is short, char, float, long
    else 
    {
        new_bytes += required_bytes(type);
        // not enough memory
        if (bytes + new_bytes - 64 > 0){
            printf("There is not enough memory for the data, you can only use %d byte(s)\n", 64-bytes);
            free(type); 
            free(name); 
            return 0;
        } else { // enough memory
            puts("Please input a value for the data type");
            // add node
            add_node(new_bytes, name, data_list);
            // write to dump_mem 64byte memory
            write_to_mem(memory_ptr, type);
        }
    }
    free(type); 
    free(name); 
    return 1;
}


/* DEALLOCATION */

void remove_node(data **data_list, int order, int bytes)
{
    data *cp_list = *data_list;
    if (order == 0){ // if the target is the uppermost node
        *data_list = (*data_list)->prev;
        free(cp_list);
    }
    else { 
        cp_list->order -= 1; // update order value :decreased, since one node is deallocated
        cp_list->bytes -= bytes; // update bytes value :decreased, since one node is deallocated
        for (int i = 0; i < order-1; i++){
            cp_list = cp_list->prev;
            cp_list->order -= 1; // update order value
            cp_list->bytes -= bytes; // update bytes value
        }
        data *delete_list = cp_list->prev;
        cp_list->prev = cp_list->prev->prev; // reconnect linked list (deallocation)
        free(delete_list); // free the target node
    }
}

int find_data(data **data_list, char *data_name){
// return how many step to go back from the uppermost node to find target node
    data *cp_list = *data_list;
    for (int i = 0; i < (*data_list)->order; i++) {
        if (strcmp(cp_list->name, data_name) == 0) return i;
        cp_list = cp_list->prev;
    }
    return 0;
}

int find_bytes(data **data_list, char *data_name){
// return the bytes allocated for the target node
    data *cp_list = *data_list;
    for (int i = 0; i < (*data_list)->order; i++) {
        if (strcmp(cp_list->name, data_name) == 0) return cp_list->bytes - cp_list->prev->bytes; 
        cp_list = cp_list->prev;
    }
    return 0; 
}

int move_memory_ptr(data **data_list, char *data_name, char **memory_ptr){ 
// move pointer to the location where the deallocation target starts
    data *cp_list = *data_list;
    int bytes = (*data_list)->bytes;
    for (int i = 0; i < (*data_list)->order; i++) {
        if (strcmp(cp_list->name, data_name) == 0){
            bytes -= cp_list->prev->bytes;
            *memory_ptr -= bytes;
            return bytes;
        }
        cp_list = cp_list->prev;
    }
    return -1;
}

int rewrite_to_mem(data **data_list, char *data_name, char **memory_ptr)
{
// from the moved memory_ptr, move the object that is "bytes" ahead to forward
    int bytes = move_memory_ptr(data_list, data_name, memory_ptr);
    if (bytes == -1) return 1; // to signal that there's no matching name in the data list
    int deallocate_size = find_bytes(data_list, data_name);
    for (int i = 0; i < bytes - deallocate_size; i++){
        **memory_ptr = *((*memory_ptr) + deallocate_size);
        *memory_ptr += 1;
    }
// initialize not rewritten parts with 0
    for (int i = 0; i < deallocate_size; i++){
        **memory_ptr = 0;
        *memory_ptr += 1;
    }
    *memory_ptr -= deallocate_size;
    return 0;
}

int deallocate(data **data_list, char *name, char **memory_ptr)
{
    // write to dump_mem 64byte memory
    if (rewrite_to_mem(data_list, name, memory_ptr) == 1) return 1;
    // remove node in data_list
    remove_node(data_list, find_data(data_list, name), find_bytes(data_list, name));
    return 0;
}


#endif
