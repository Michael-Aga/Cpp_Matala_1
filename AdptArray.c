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
    created_array->MyCopyFunc = copy;// Give the array its funcs
    created_array->MyDelFunc = delete;
    created_array->MyPrintFunc = print;

    return created_array;
};

void DeleteAdptArray(PAdptArray to_delete)
{//free the memory by first going to every cell in the array if its not NULL we free him, after that free the whole array and the whole struct.
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
        return FAIL; //Check if the place is negative or if the Struct couldnt fine memory

    if (place > currentArray->size)// Chech if the size of the current array is smaller than the index to place the added element
    {
        currentArray->myArray = (PElement)realloc(currentArray->myArray, (sizeof(PElement) * (place + 1)));//Take a new spot in the memory for a bigger array
        if (currentArray->myArray == NULL)// Check if the memoery was allocated
        {
            free(currentArray);// If not free the struct
            return FAIL;
        }

        for (int i = currentArray->size; i < place + 1; i++)// Go in the array from the last place we know until the new place and initialize the cells
        {
            currentArray->myArray[i] = 0;
        }

        currentArray->myArray[place] = currentArray->MyCopyFunc(toAdd);//Assign a copy of the element to the selected cell
        currentArray->size = place + 1;// We begin from 0 so we have to put + 1 in the size
        return SUCCESS;
    }

    else
    {
        if (currentArray->myArray[place] != 0) // Check if the cell is occupied if so delete the cell and assign a copy of the element
        {
            currentArray->MyDelFunc(currentArray->myArray[place]);
            currentArray->myArray[place] = currentArray->MyCopyFunc(toAdd);
            return SUCCESS;
        }

        else // The cell is not occupied and has 0 in it so just assign a copy of the element
        {
            currentArray->myArray[place] = currentArray->MyCopyFunc(toAdd);
            return SUCCESS;
        }
    }
};

PElement GetAdptArrayAt(PAdptArray currentArray, int to_find)
{
    if (to_find < 0 || to_find > currentArray->size || currentArray->myArray[to_find] == NULL) 
        return NULL;// Cant accept negative num and also if its bigger than the array its already a NULL and we handle NULL because the client didnt 0_0 !

    return currentArray->MyCopyFunc(currentArray->myArray[to_find]);// Return the element in the wanted cell
};

int GetAdptArraySize(PAdptArray myArray)
{
    if (myArray == NULL) // If the struct couldnt allocate memory return -1, as the readme file said
    {
        return -1;
    }

    else
    {
        return myArray->size;
    }
};

void PrintDB(PAdptArray pArray) // To print we just want the elemnts and not the 0 so we avoid 0 and print everything else
{
    for (int i = 0; i < pArray->size; i++)
    {
        if (pArray->myArray[i] != 0)
            pArray->MyPrintFunc(pArray->myArray[i]);
    }
};