#ifndef _BOOKS_H_
#define _BOOKS_H_

#include<stdlib.h>
#include<string.h>

typedef struct books
{
    char *id,*name;
    int num,n;
    char** borrower;
    struct books *next;
}books;

books* createBook(char* _id , char* _name , int _num)
{
    books* b =  (books*) malloc(sizeof(books));
    b->id = (char*) malloc(25*sizeof(char));
    strcpy(b->id , _id);
    b->name = (char*) malloc(25*sizeof(char));
    strcpy(b->name , _name);
    b->num = _num;
    b->n=0;
    b->borrower= (char**) malloc(b->num*sizeof(char*));
    for(int i=0;i<b->num;i++)
    {
        b->borrower[i]=NULL;
    }
    b->next=NULL;
    return b;
}

int addBookBorrower(books* book, char* id)
{
    if(book==NULL)  return 1;
    for(int i=0;i<book->num;i++)
    {
        if(book->borrower[i]!=NULL && strcmp(book->borrower[i] , id)==0)
        {
            return 1;
        }
    }
    for(int i=0; i<book->num; i++)
    {
        if(book->borrower[i]==NULL)
        {
            book->borrower[i] = (char*)malloc(25*sizeof(char));
            strcpy(book->borrower[i] , id);
            book->n++;
            return 0;
        }
    }
    return 1;
}


int deleteBookBorrower(books* book, char* id)
{
    if(book!=NULL)
    for(int i=0; i<book->num; i++)
    {
        if(book->borrower[i] != NULL && strcmp(book->borrower[i], id)==0)
        {
            free(book->borrower[i]);
            book->borrower[i]=NULL;
            book->n--;
            return 0;
        }
    }
    return 1;
}



void deleteBook(books* book)
{
    free(book);
    book = NULL;
}

#endif