#include<stdio.h>
#include<string.h>
#include"books.h"
#include"reader.h"

FILE* booksFile , *readersFile;
books* firstBook , *a_book;
reader* firstReader , *a_reader;

books* findBook(char* id_name)
{
    books* abook=firstBook->next;
    while(abook!=NULL && (strcmp(id_name , abook->id) && strcmp(id_name , abook->name)))   
        abook=abook->next;
    return abook;
}

void loadData()
{
    booksFile = fopen("/home/minami/projects/library_manager/books_and_readers/books.txt" , "r");
    readersFile = fopen("/home/minami/projects/library_manager/books_and_readers/readers.txt" , "r");
    char book_name[25]={0},book_id[25]={0},book_borrower[25]={0},exact[25]={0};
    int book_num;
    fscanf(booksFile,"%s%s%s%s\n" , &book_name , &exact , &book_id , &book_borrower);
    firstBook=createBook(book_id , book_name , 0);
    a_book = firstBook;
    memset(book_name,0,sizeof(book_name));
    memset(book_borrower,0,sizeof(book_borrower));
    memset(book_id,0,sizeof(book_id));
    memset(exact , 0 , sizeof(exact));
    while(fscanf(booksFile,"%s%s%s%s\n" , &book_name , &exact , &book_id , &book_borrower)!=EOF)
    {
        book_num=0;
        for(int i=0;i<strlen(exact);i++)    book_num=book_num*10+exact[i]-'0';
        if(strcmp(book_id , "null"))
        {
            a_book->next =createBook(book_id,book_name,book_num);
            a_book=a_book->next;
            if(strcmp(book_borrower , "null"))
                addBookBorrower(a_book, book_borrower);
        }
        else
        {
            addBookBorrower(a_book , book_borrower);
        }
        memset(book_name,0,sizeof(book_name));
        memset(book_borrower,0,sizeof(book_borrower));
        memset(book_id,0,sizeof(book_id));
    }
    fclose(booksFile);
    
    char reader_name[25]={0} , reader_id[25]={0} , reader_borrow[25]={0};
    fscanf(readersFile , "%s%s%s\n" , reader_name , reader_id , reader_borrow);
    firstReader=createReader(reader_id , reader_name);
    a_reader=firstReader;
    memset(reader_borrow,0,sizeof(reader_borrow));
    memset(reader_id,0,sizeof(reader_id));
    memset(reader_name,0,sizeof(reader_name));
    while(fscanf(readersFile , "%s%s%s\n" , &reader_name , &reader_id , &reader_borrow) != EOF)
    {
        a_reader->next = createReader(reader_id , reader_name);
        a_reader=a_reader->next;
        if(strcmp(reader_borrow , "null"))
        {
            a_book=findBook(reader_borrow);
            borrowBook(a_reader,a_book);
        }
        memset(reader_borrow,0,sizeof(reader_borrow));
        memset(reader_id,0,sizeof(reader_id));
        memset(reader_name,0,sizeof(reader_name));
    }
    fclose(readersFile);
}

reader* findReader(char* _id)
{
    reader* r=firstReader->next;
    while(r!=NULL && strcmp(r->id , _id))    
        r=r->next;
    return r;
}

void storeData()
{
    booksFile = fopen("/home/minami/projects/library_manager/books_and_readers/books.txt" , "w");
    readersFile = fopen("/home/minami/projects/library_manager/books_and_readers/readers.txt" , "w");

    char a[25]="number";
    char b[25]="readers";
    char c[5] = "null";
    fprintf(booksFile,"%-20s%-20s%-20s%-20s\n", firstBook->name , a, firstBook->id , b);
    while(firstBook->next!=NULL)
    {
        a_book=firstBook->next;
        firstBook->next=firstBook->next->next;
        if(a_book->n==0)
        {
            fprintf(booksFile, "%-20s%-20d%-20s%-20s\n",a_book->name,a_book->num,a_book->id,c);
        }
        else
        {
            int i=0;
            while(a_book->borrower[i]==NULL)    i++;
            fprintf(booksFile, "%-20s%-20d%-20s%-20s\n",a_book->name,a_book->num,a_book->id,a_book->borrower[i]);
            for(i++;i<a_book->num;i++)
            {
                if(a_book->borrower[i]!=NULL)
                {
                    fprintf(booksFile, "%-20s%-20s%-20s%-20s\n",c,c,c,a_book->borrower[i]);
                }
            }
        }
        free(a_book);
    }
    free(firstBook);
    fclose(booksFile);

    char d[25]="borrowed_book";
    fprintf(readersFile , "%-20s%-20s%-20s\n", firstReader->name, firstReader->id , d);
    while(firstReader->next!=NULL)
    {
        a_reader=firstReader->next;
        firstReader->next=firstReader->next->next;
        if(a_reader->borrowed==NULL)
        {
            fprintf(readersFile , "%-20s%-20s%-20s\n", a_reader->name, a_reader->id , c);
        }
        else
        {
            fprintf(readersFile , "%-20s%-20s%-20s\n", a_reader->name, a_reader->id , a_reader->borrowed->name);
        }
        free(a_reader);
    }
    free(firstReader);
    fclose(readersFile);
}

int printReader(books* b)
{
    if(0==b->n)
    {
        printf("There is no one borrowing this book!\n");
        return 1;
    }
    printf("The book has been borrowed by:\n");
    for(int i=0;i<b->num;i++)
    {
        if(b->borrower[i]!=NULL)
        {
            printf("%s\n", b->borrower[i]);
        }
    }
    printf("in total %d persons\nThe rest number of the book is %d\n", b->n, (b->num-b->n));
    return 0;
}

void editReaderInfo(reader* person, char* item, char* goal)
{
    if(item[0] == 'i')
    {
        if(person->borrowed!=NULL) deleteBookBorrower(findBook(person->borrowed->id), person->id);
        strcpy(person->id , goal);
        if(person->borrowed!=NULL) addBookBorrower(findBook(person->borrowed->id), person->id);
    }
    else if (item[0] == 'n')
    {
        strcpy(person->name , goal);
    }
    
}

void editBookInfo(books* book , const char* item , char* goal)
{
    char** temp=book->borrower;
    int t=book->num;
    if(item[1] == 'd')
    {
        strcpy(book->id , goal);
        for(int i=0;i<t;i++)
        {
            if(temp[i] != NULL)
            {
                strcpy(findReader(temp[i])->borrowed->id , goal);
            }
        }
    }
    else if ( item[1] == 'a')
    {
        strcpy(book->name , goal);
        for(int i=0;i<t;i++)
        {
            if(temp[i] != NULL)
            {
                strcpy(findReader(temp[i])->borrowed->name , goal);
            }
        }
    }
    else if (item[1] == 'u')
    {
        book->num = 0;
        book->n=0;
        for (int i = 0; i < strlen(goal); i++)
        {
            book->num = book-> num *10 + goal[i]-'0';
        }        
        book->borrower=NULL;
        book->borrower= (char**)malloc(book->num*sizeof(char*));
        for(int i=0;i<book->num || i<t;i++)
        {
            if(i<book->num)
                book->borrower[i]=NULL;
            if(i<t && i<book->num)
            {
                addBookBorrower(book , temp[i]);
            }
            else if(t>book->num &&temp[i] != NULL)
            {
                findReader(temp[i])->borrowed=NULL;
            }
        }
        free(temp);
    }
    
}

int main()
{
    printf("Welcome to the library manager system!\nLoading the data...\n");
    loadData();
    printf("Are you the reader or the manager?\n1.reader\n2.manager\n");
    int choice;
    scanf("%d" , &choice);
    if(choice==1)
    {
        char in_id[25];
    a:  printf("please enter your id:\n");
        scanf("%s" , &in_id);
        a_reader=findReader(in_id);
        if(a_reader==NULL)
        {
            printf("Haven't found the id...\nMaybe you should check out the id you enter correct or register?\n1.register\n2.enter the right id\n");
            scanf("%d" , &choice);
            if(choice==1)
            {
                printf("please enter you name:\n");
                char in_name[25];
                scanf("%s" , &in_name);
                a_reader=firstReader;
                while (a_reader->next!=NULL)
                {
                    a_reader=a_reader->next;
                }
                int i=strlen(in_id)-1;
                a_reader->next = createReader(in_id , in_name);
                a_reader = a_reader->next;
                printf("welcome! %s\nyour id is %s\n",a_reader->name , a_reader->id);
            }
            else
            {
                goto a;
            }
        }
    b:  printf("welcome %s\nWould you like to ?\n1.borrow a book\n2.return the book\n3.search the book\n" , a_reader->name);
        scanf("%d",&choice);
        char in_book[25];
        if(choice==1)
        {
            if(a_reader->borrowed != NULL)
            {
                printf("you have borrow the book %s\nplease return it first!\n",a_reader->borrowed->name);
            }
            else
            {
                printf("please enter the book's name or id:\n");
                scanf("%s", &in_book);
                a_book=findBook(in_book);
                if(a_book==NULL)
                {
                    printf("haven't found the book %s\n");
                    goto b;
                }
                if(borrowBook(a_reader , a_book))
                {
                    printf("The rest number of the book is 0");
                }
            }
        }
        else if(choice==2)
        {
            if(a_reader->borrowed ==NULL)
            {
                printf("you haven't borrowed a book yet\n");
            }
            else
            {
                returnBook(a_reader , findBook(a_reader->borrowed->id));
            }
        }
        else
            {
                printf("search:");
                scanf("%s", &in_book);
                a_book=findBook(in_book);
                printf("The book's name is %s.\nThe book's id is %s.\nThere are %d books in whole.\n" , a_book->name , a_book->id , a_book->num);
                printReader(a_book);
            }
        printf("would you like to continue?[1:yes,2:no]\n");
        scanf("%d", &choice);
        if(choice==1)
        {
            goto b;
        }
    }
    else
    {
    c:  printf("what would you do?\n1.input a book\n2.edit the book's information\n");
        printf("3.delete a book\n4.input a new reader\n5.show the book list or the reader list\n");
        printf("6.edit the reader's information\n7.delete the reader\n8.show the book information\n");
        scanf("%d", &choice);
        char a[100],_id[25],_name[25];
        int num,j;
        switch (choice)
        {
        case 1:
            {
                printf("please enter the book's name, id, num:\n");
                scanf("%s%s%d",&_name , &_id , &num);
                a_book=firstBook;
                while(a_book->next!=NULL)   a_book=a_book->next;
                a_book->next=createBook(_id , _name , num);
            }
            break;
        case 2:
            {
                printf("would you edit the book (name or id):\n");
                scanf("%s", &_name);
                a_book=findBook(_name);
                if(a_book==NULL)
                {
                    printf("The book %s not found, maybe you should add one?\n", _name);
                    goto c;
                }
                printf("would you want to edit:\n");
                scanf("%s",&_id);
                if(!strcmp(_id , "borrower"))
                {
                    printf("would you like to:\n1.add a borrower\n2.delete a borrower\n3.edit the borrower id\n");
                    scanf("%d",&choice);
                    printReader(a_book);
                    if(choice==1)
                    {
                        if(a_book->n==a_book->num)
                        {
                            printf("There are no books free");
                        }
                        else
                        {
                            printf("would you want to add:(id)\n");
                            scanf("%s" , &_id);
                            borrowBook(findReader(_id) , a_book);
                        }
                    }
                    else if(choice==2)
                    {
                        if(a_book->n==0)
                        {
                            printf("There is no one who borrows the book\n");
                        }
                        else
                        {
                            printf("would you want to delete:(id)\n");
                            scanf("%s" , &_id);
                            a_reader=findReader(_id);
                            returnBook(a_reader , findBook(a_reader->borrowed->id));
                        }
                    }
                    else
                    {
                        printf("would you edit the reader id:\n");
                        scanf("%s", &_id);
                        deleteBookBorrower(a_book , _id);
                        printf("into:");
                        scanf("%s",&a);
                        addBookBorrower(a_book , a);
                    }
                    break;
                }
                printf("into:");
                scanf("%s",&a);
                editBookInfo(a_book , _id , a);
            }
            break;
        case 3:
            {
                printf("would you like to delete:");
                scanf("%s", &_name);
                a_book=firstBook;
                while(a_book->next!=NULL && (strcmp(_name , a_book->next->id) && strcmp(_name , a_book->next->name)))   
                    a_book=a_book->next;
                books *p=a_book->next;
                if(p==NULL)
                {
                    printf("The book %s not found, maybe you should add one?\n", _name);
                    goto c;
                }
                a_book->next=a_book->next->next;
                for(int i=0;i<p->num;i++)
                {
                    if(p->borrower[i]!=NULL)
                    {
                        returnBook(findReader(p->borrower[i]) , p);
                    }
                }
                deleteBook(p);
            }
            break;
        case 4:
            {
                printf("please enter the reader's name, id:\n");
                scanf("%s%s",&_name , &_id);
                a_reader=firstReader;
                while(a_reader->next!=NULL)   a_reader=a_reader->next;
                a_reader->next=createReader(_id , _name );
                break;
            }
        case 5:
            {
                printf("would you like to show:\n1.books list\n2.readers list\n");
                scanf("%d", &choice);
                if(choice==1)
                {
                    a_book=firstBook->next;
                    if(a_book==NULL)
                    {
                        printf("There is no book in the library");
                    }
                    else
                    {
                        while(a_book!=NULL)
                        {
                            printf("book's name:%s, book's id:%s, book's num:%d\n",a_book->name ,  a_book->id , a_book->num);
                            a_book=a_book->next;
                        }
                    }
                }
                else
                {
                    a_reader=firstReader->next;
                    if(a_reader==NULL)
                    {
                        printf("There is no one borrowing the book in the library");
                    }
                    else
                    {
                        while(a_reader!=NULL)
                        {
                            printf("reader's name:%s, reader's id:%s\n", a_reader->name ,  a_reader->id );
                            a_reader=a_reader->next;
                        }
                    }
                }
                break;
            }
        case 6:
            {
                printf("would you edit the reader's id:\n");
                scanf("%s", &_name);
                a_reader=findReader(_name);
                if(a_reader==NULL)
                {
                    printf("The reader id %s not found, maybe you should add one?\n", _name);
                    goto c;
                }
                printf("would you want to edit:\n");
                scanf("%s",&_id);
                if(!strcmp(_id , "borrowed"))
                {
                    printf("into:");
                    scanf("%s",&a);
                    a_book=findBook(a);
                    if(a_reader->borrowed!=NULL)    returnBook(a_reader , findBook(a_reader->borrowed->id));
                    borrowBook(a_reader, a_book);
                    break;
                }
                printf("into:");
                scanf("%s",&a);
                editReaderInfo(a_reader , _id , a);
                break;
            }
        case 7:
            {
                printf("would you delete the reader's id:\n");
                scanf("%s", &_id);
                reader* r=firstReader;
                while(r->next!=NULL && strcmp(r->next->id , _id))    
                    r=r->next;
                a_reader=r->next;
                if(a_reader==NULL)
                {
                    printf("The reader id %s not found, maybe you should add one?\n", _name);
                    goto c;
                }
                r->next=r->next->next;
                if(a_reader->borrowed!=NULL)
                {
                    deleteBookBorrower(findBook(a_reader->borrowed->id) , a_reader->id);
                }
                deletePersonInfo(a_reader);
                break;   
            }
        default:
            {
                printf("search:");
                scanf("%s", &a);
                a_book=findBook(a);
                printf("The book's name is %s.\nThe book's id is %s.\nThere are %d books in whole.\n" , a_book->name , a_book->id , a_book->num);
                printReader(a_book);
            }
            break;
        }
        printf("would you like to continue?[1:yes,2:no]\n");
        scanf("%d", &choice);
        if(choice==1)
        {
            goto c;
        }
    }
    storeData();
    printf("Thank you for your use!\n");
    return 0;
}