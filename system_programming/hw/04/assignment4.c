// add const for required place
#include "assignment4.h"

int main(){
    // memory dump
    char *memory_dump = (char *)malloc(CUSTOM_MEMORY_SIZE); 
    // initialize memory dump with 0 
    for (int i = 0; i < CUSTOM_MEMORY_SIZE; i++)
        memory_dump[i] = 0;
    // memory ptr
    char *memory_ptr1 = memory_dump;
    char **memory_ptr = &memory_ptr1;
    // data list
    data data_initialize = { .order = 0,
                             .bytes = 0,
                             .name = "",
                             .prev = NULL };

    data *data_list1 = &data_initialize;
    data **data_list = &data_list1;

    while (1){
        puts("Do you want to allocate data (1) or deallocate data (2) ?");
        int a;
        scanf("%d", &a);
        if (a == 1) { // allocate
            if (allocate(data_list, memory_ptr)) // if there's enough memory -> allocate
                memory_and_data(memory_dump, data_list);
        } else if (a == 2){ // deallocate
            puts("Input the name of data you want to deallocate");
            char *name = (char *)malloc(MAX_DATA_NAME_SIZE);
            scanf("%s", name);
            if (deallocate(data_list, name, memory_ptr) == 0){ // if there is matching name ->deallocate
                printf("%s has been deallocated\n", name);
                memory_and_data(memory_dump, data_list);
            }
            free(name);
        }
    }
    
    return 0;
}