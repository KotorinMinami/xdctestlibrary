#ifndef _READER_H_
#define _READER_H_

#include<string.h>
#include"books.h"

typedef struct reader
{
    char *id , *name;
    books* borrowed;
    struct reader *next;
}reader;

reader* createReader(char* _id, char* _name)
{
    reader* p= (reader*) malloc (sizeof(reader));
    p->id=(char*)malloc(25*sizeof(char));
    strcpy(p->id , _id);
    p->name= (char*)malloc(25*sizeof(char));
    strcpy(p->name , _name);
    p->borrowed=NULL;
    p->next=NULL;
    return p;
}


int borrowBook(reader* person , books* book)
{
    if(person->borrowed == NULL && addBookBorrower(book, person->id))
    {
        person->borrowed = createBook(book->id,book->name,book->num);
        return 0;
    }
    else
    {
        return 1;
    }
}

int returnBook(reader* person , books* book)
{
    if(person->borrowed != NULL)
    {
        deleteBookBorrower(book, person->id);
        person->borrowed = NULL;
        return 0;
    }
    else
    {
        return 1;
    }
}

void deletePersonInfo(reader* person)
{
    free(person);
    person = NULL;
}

#endif