/*
In this code I got help from the Tirgul :
https://ariel-ac-il.zoom.us/rec/play/WQnJJyS-GKk5S0nALEUY7jTVJPt1G4Z8oyipSaOp5xYluof6gzbD31NqX33FCfn43_qyY1CTp0u2ri0.iRFvt5c4pZV7-jrh
*/

#include "AdptArray.h"
#include "book.h"
#include "Person.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef struct AdptArray_ // Here we are making a struct of the array with an array inside with his size and the func of the array
{
    PElement *myArray;
    int size;

    COPY_FUNC MyCopyFunc;
    DEL_FUNC MyDelFunc;
    PRINT_FUNC MyPrintFunc;

} *PAdptArray;

PAdptArray CreateAdptArray(COPY_FUNC copy, DEL_FUNC delete, PRINT_FUNC print) // This is how we are creating an array, at start the array is "empty"
{
    PAdptArray created_array;
    created_array = (PAdptArray)malloc(sizeof(struct AdptArray_));
    if (created_array == NULL) // If we cant locate memory for the struct we return NULL
        return NULL;

    created_array->myArray = (PElement)malloc(sizeof(PElement));
    if (created_array->myArray == NULL)// If we cant locate memory for the array inside the struct we free the struct and return NULL
    {
        free(created_array);
        return NULL;
    }

    memset(created_array->myArray, 0, sizeof(PElement));// We initialize the array to 0

    created_array->size = 0;// We start the size with 0 because its empty
    created_array->MyCopyFunc = copy;// 
    created_array->MyDelFunc = delete;
    created_array->MyPrintFunc = print;

    return created_array;
};

void DeleteAdptArray(PAdptArray to_delete)
{
    for (int i = 0; i < to_delete->size; i++)
    {
        if(to_delete->myArray[i] != NULL)
        to_delete->MyDelFunc(to_delete->myArray[i]);
    }
    free(to_delete->myArray);
    free(to_delete);
};

Result SetAdptArrayAt(PAdptArray currentArray, int place, PElement toAdd)
{
    if (currentArray == NULL || place < 0)
        return FAIL;

    if (place > currentArray->size)
    {
        currentArray->myArray = (PElement)realloc(currentArray->myArray, (sizeof(PElement) * (place + 1)));
        if (currentArray->myArray == NULL)
        {
            free(currentArray);
            return FAIL;
        }

        for (int i = currentArray->size; i < place + 1; i++)
        {
            currentArray->myArray[i] = 0;
        }

        currentArray->myArray[place] = currentArray->MyCopyFunc(toAdd);
        currentArray->size = place + 1;
        return SUCCESS;
    }

    else
    {
        if (currentArray->myArray[place] != 0)
        {
            currentArray->MyDelFunc(currentArray->myArray[place]);
            currentArray->myArray[place] = currentArray->MyCopyFunc(toAdd);
            return SUCCESS;
        }

        else
        {
            currentArray->myArray[place] = currentArray->MyCopyFunc(toAdd);
            return SUCCESS;
        }
    }
};

PElement GetAdptArrayAt(PAdptArray currentArray, int to_find)
{
    if (to_find < 0 || to_find > currentArray->size || currentArray->myArray[to_find] == NULL)
        return NULL;

    return currentArray->MyCopyFunc(currentArray->myArray[to_find]);
};

int GetAdptArraySize(PAdptArray myArray)
{
    if (myArray == NULL)
    {
        return -1;
    }

    else
    {
        return myArray->size;
    }
};

void PrintDB(PAdptArray pArray)
{
    for (int i = 0; i < pArray->size; i++)
    {
        if (pArray->myArray[i] != 0)
            pArray->MyPrintFunc(pArray->myArray[i]);
    }
};