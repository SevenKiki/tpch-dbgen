#include <stdio.h>
#include <stdlib.h>
#include <iostream>
// #include <mm_malloc.h>

void addString(void*** array, int* size ,const char* str) {
    // 分配内存以存储新的字符串指针
    void* new_ptr = malloc(sizeof(void*));
    memcpy(new_ptr, &str, sizeof(void*));

    // 调整数组的大小，将新指针添加到数组中
    (*size)++;
    array = realloc(*array, sizeof(void*) * (*size));
    void** array_ptr = *array;
    array_ptr[*size - 1] = new_ptr;
}

void printStringArray(void** array, int size) {
    for (int i = 0; i < size; i++) {
        const char* str = *(const char**)array[i];
        printf("%s\n", str);
    }
}

void freeStringArray(void** array, int size) {
    for (int i = 0; i < size; i++) {
        free(array[i]);
    }
    free(array);
}

int main() {
    void** stringArray = NULL;
    int size = 0;

    addString(&stringArray, &size, "Hello");
    addString(&stringArray, &size, "World!");
    addString(&stringArray, &size, "This is");
    addString(&stringArray, &size, "a test.");

    printStringArray(stringArray, size);

    freeStringArray(stringArray, size);

    return 0;
}