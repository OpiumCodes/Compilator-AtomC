//Tratare erori
#include<stdio.h>
#include<stdlib.h>
/*
Exemplu declaratii de variabile.
int v[10],x,y;

*/
int declVar()
{
    Token *startTk=crtTk;
    if(typebase())
    {
        if(consume(ID))
        {
            if(arrayDecl())
            {
                
            }
            for(;;)
            {
                if(consume(COMMA))
                {
                    if(consume(ID))
                    {
                        if(arrayDecl()){}
                    }
                    else
                    {
                        tkerr(crtTk,"Lipseste numele variabilei dupa virgula!");
                    }
                }
                else
                break;
            }
            if(consume(SEMICOLON))//Eroare daca s-a consumat arrayDecl() sau COMMA
                return 1;
        }
    }
    crtTk=startTk;
    return 0;
}