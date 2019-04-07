#include<stdio.h>
#include<stdlib.h>
typedef struct _Token
{
    int code; // codul (numele)
    union
        {
        char *text; // folosit pentru ID, CT_STRING (alocat dinamic)
        long int i; // folosit pentru CT_INT, CT_CHAR
        double r; // folosit pentru CT_REAL
        };
    int line; // linia din fisierul de intrare
    struct _Token *next; // inlantuire la urmatorul AL
}Token;


Token *crtTk;//Tokenul curent

int consume(int code)//Consuma atomi terminali
{
    if(crtTk->code==code)
    {
        crtTk=crtTk->next;
        return 1;
    }
    else
        return 0;
}

int declVar()
{

}

int declFunc()
{

}

int declStruct()
{
    Token *startTk=crtTk;
    if(consume(STRUCT))//Trebuie definite toate chestile mai sus 
    {
        if(consume(ID))
        {
            if(consume(LACC))
            {
                while(declVar());
                if(consume(RACC))
                {
                    if(consume(SEMICOLON))
                    {
                        return 1;
                    }
                }
            }
        }
    }
    crtTk=startTk;
    return 0;
}

int unit()
{
    while(declStruct() || declFunc() || declVar());
    if(consume(END))
        return 1;
    return 0;
}

/*int exprOr()
{
    if(exprOr())
    {
        if(consume(OR))
        {

        }
    }
}
Recursivitate de stanga pentru ca se tot apeleaza functia exprOr la infinit deoarece recursivitatea nu are nici o regula de inchidere
*/

int exprOrPrim()
{
    if(consume(OR))
    {
        if(exprAnd())
        {
            if(exprOrPrim())
            {
                return 1;
            }
        }
    }
    return 1;
}
int exprOr()
{
    if(exprAnd())
    {
        if(exprOrPrim())
        {
            return 1;
        }
    }
    return 0;
}

int main()
{
    //Lexical pana aici;
    crtTk=tokens;
    if(unit())
    {
        puts("Syntax Ok");
    }
    else
    {
        puts("Syntax error!");
    }

}
