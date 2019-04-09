#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include <stdarg.h>
#define SAFEALLOC(var,Type) if((var=(Type*)malloc(sizeof(Type)))==NULL){printf("not enough memory\n");exit(-2);}

char *pCrtChr;
int line=1;
enum{BREAK,CHAR,DOUBLE,ELSE,FOR,IF,INT,RETURN,STRUCT,VOID,WHILE,END,ID,CT_INT,CT_REAL,CT_CHAR,CT_STRING,COMMA,SEMICOLON,LPAR,RPAR,LBRACKET,RBRACKET,LACC,RACC,ADD,SUB,MUL,DIV,DOT,AND,OR,NOT,ASSIGN,EQUAL,NOTEQ,LESS,LESSEQ,GREATER,GREATEREQ};//add the others



char *conv(int code)
{
    switch(code)
    {
        case BREAK:return "BREAK";
        break;
        case CHAR:return "CHAR";
        break;
        case DOUBLE:return "DOUBLE";
        break;
        case ELSE:return "ELSE";
        break;
        case FOR:return "FOR";
        break;
        case INT:return "INT";
        break;
        case IF:return "IF";
        break;
        case RETURN:return "RETURN";
        break;
        case STRUCT:return "STRUCT";
        break;
        case VOID:return "VOID";
        break;
        case WHILE:return "WHILE";
        break;
        case END:return "END";
        break;
        case ID:return "ID";
        break;
        case CT_INT:return "CT_INT";
        break;
        case CT_REAL:return "CT_REAL";
        break;
        case CT_CHAR:return "CT_CHAR";
        break;
        case CT_STRING:return "CT_STRING";
        break;
        case COMMA:return "COMMA";
        break;
        case SEMICOLON:return "SEMICOLON";
        break;
        case LPAR:return "LPAR";
        break;
        case RPAR:return "RPAR";
        break;
        case LBRACKET:return "LBRACKET";
        break;
        case RBRACKET:return "RBRACKET";
        break;
        case LACC:return "LACC";
        break;
        case RACC:return "RACC";
        break;
        case ADD:return "ADD";
        break;
        case SUB:return "SUB";
        break;
        case MUL:return "MUL";
        break;
        case DIV:return "DIV";
        break;
        case DOT:return "DOT";
        break;
        case AND:return "AND";
        break;
        case OR:return "OR";
        break;
        case NOT:return "NOT";
        break;
        case ASSIGN:return "ASSIGN";
        break;
        case EQUAL:return "EQUAL";
        break;
        case NOTEQ:return "NOTEQ";
        break;
        case LESS:return "LESS";
        break;
        case LESSEQ:return "LESSEQ";
        break;
        case GREATER:return "GREATER";
        break;
        case GREATEREQ:return "GREATEREQ";
        break;
        default:printf("Unkown id: %d",code);
    }
}



char *createString(char *first,char *last)
{
   int l;
   l=last-first;
   char *start;
   start=malloc(sizeof(char)*l);
   int i;
   for(i=0;i<l;i++)
   {
      *(start+i)=*(first+i);
   }
   start[l]='\0';
   for(i=0;i<strlen(start);i++)
   {
       if(start[i]=='\\')
       {
           switch(start[i+1])
           {
               //abfnrtv'?"\\0
               case '\'':start[i]='\'';break;
               case '\"':start[i]='\"';break;
               case 'a':start[i]='\a';break;
               case 'b':start[i]='\b';break;
               case 'f':start[i]='\f';break;
               case 'n':start[i]='\n';break;
               case 'r':start[i]='\r';break;
               case 't':start[i]='\t';break;
               case 'v':start[i]='\v';break;
               case '?':start[i]='\?';break;
               case '\\':start[i]='\\';break;
               case '0':start[i]='\0';break;
               default:printf("Error at \\ characters!");exit(0);
            }
           strcpy(start+i+1,start+i+2);
       }
   }
   return start;
}
int createInteger(char *first,char *last)
{
   char *nxt;
   char *number=createString(first,last);
   return strtol(number,&nxt,0);
}
float createFloat(char *start,char *after)
{
    char *number=createString(start,after);
    return atof(number);
}
typedef struct _Token
{
    int code; // codul (numele)
    int base;
    union
        {
        char *text; // folosit pentru ID, CT_STRING (alocat dinamic)
        long int i; // folosit pentru CT_INT, CT_CHAR
        double r; // folosit pentru CT_REAL
        };
    int line; // linia din fisierul de intrare
    struct _Token *next; // inlantuire la urmatorul AL
}Token;
Token *lastToken=NULL;
Token *tokens=NULL;
Token *addTk(int code)
{
    Token *tk;
    SAFEALLOC(tk,Token);
    tk->code=code;
    tk->line=line;
    tk->next=NULL;
    if(lastToken)
    {
        lastToken->next=tk;
    }
    else
    {
        tokens=tk;
    }
    lastToken=tk;
    return tk;
}

void afisare()
{
    Token *t;
    FILE *out;
    out=fopen("out.txt","w");
    for(t=tokens;t!=NULL;t=t->next)
    {
        fprintf(out,"%d ",t->line);
        fprintf(out,"%s ",conv(t->code),t->code);
        switch(t->code)
        {
            case CT_INT:switch(t->base)
                        {
                            case 0:break;
                            case 10:fprintf(out,":%d",t->i);break;
                            case 8:fprintf(out,":0%o",t->i);break;
                            case 16:fprintf(out,":0x%X",t->i);break; 
                            default: fprintf(out,"Error at int!");exit(0);
                        }
            break;
            case ID:fprintf(out,":%s",t->text);
            break;
            case CT_STRING:
            fprintf(out,":%s",t->text);
            break;
            case CT_CHAR:fprintf(out,":%c",t->i);
            break;
            case CT_REAL:fprintf(out,":%f",t->r);
            break;
        }
        fprintf(out,"\n");
    }
}

int getNextToken()
{
    char *characters="ab\\fnrtv\"'?\\0";
    char ch;//Caracterul curent
    int s=0;//Starea
    char *pStartCh;
    int base=0;
    int nch;
    Token *tk;
    for(;;)
    {
        ch=*pCrtChr;
        //printf("Starea:%d,Caracterul:%c(%d)\n",s,ch,ch);
        switch(s)
        {
            case 0:
            if(isdigit(ch) && ch!='0'){s=1;pStartCh=pCrtChr;pCrtChr++;}
            else
            {
                if(isdigit(ch) && ch=='0'){s=2;pStartCh=pCrtChr;pCrtChr++;}            
                else
                { 
                    if(ch=='\''){s=14,pCrtChr++;}
                    else
                    {
                        if(ch=='\"'){s=18,pCrtChr++;}
                        else
                        {
                            if(ch=='/'){s=22,pCrtChr++;}
                            else
                            {
                                if(ch==' ' || ch=='\n' || ch=='\r' || ch=='\t'){pCrtChr++;if(ch=='\n')line++;}
                                else
                                {
                                    if(isalpha(ch) || ch=='_'){s=26;pStartCh=pCrtChr;pCrtChr++;}
                                    else
                                    {
                                        if(ch==','){s=28;pCrtChr++;}
                                        else
                                        {
                                            if(ch==';'){s=29;pCrtChr++;}
                                            else
                                            {
                                                if(ch=='('){s=30;pCrtChr++;}
                                                else
                                                {
                                                    if(ch==')'){s=31;pCrtChr++;}
                                                    else
                                                    {
                                                        if(ch=='['){s=32;pCrtChr++;}
                                                        else
                                                        {
                                                            if(ch==']'){s=33;pCrtChr++;}
                                                            else
                                                            {
                                                                if(ch=='{'){s=34;pCrtChr++;}
                                                                else
                                                                {
                                                                    if(ch=='}'){s=57;pCrtChr++;}
                                                                    else
                                                                    {
                                                                        if(ch=='+'){s=35;pCrtChr++;}
                                                                        else
                                                                        {
                                                                            if(ch=='*'){s=36;pCrtChr++;}
                                                                            else
                                                                            {
                                                                                if(ch=='-'){s=37;pCrtChr++;}
                                                                                else
                                                                                {
                                                                                    if(ch=='.'){s=38;pCrtChr++;}
                                                                                    else
                                                                                    {
                                                                                        if(ch=='&'){s=39;pCrtChr++;}
                                                                                        else
                                                                                        {
                                                                                            if(ch=='|'){s=41;pCrtChr++;}
                                                                                            else
                                                                                            {
                                                                                                if(ch=='!'){s=43;pCrtChr++;}
                                                                                                else
                                                                                                {
                                                                                                    if(ch=='='){s=46;pCrtChr++;}
                                                                                                    else
                                                                                                    {
                                                                                                        if(ch=='<'){s=49;pCrtChr++;}
                                                                                                        else
                                                                                                        {
                                                                                                            if(ch=='>'){s=52;pCrtChr++;}
                                                                                                            else
                                                                                                            {
                                                                                                                if(ch=='\0'){addTk(END);return END;}
                                                                                                            }
                                                                                                            
                                                                                                        }
                                                                                                        
                                                                                                    }
                                                                                                    
                                                                                                }
                                                                                                
                                                                                            }
                                                                                            
                                                                                        }
                                                                                        
                                                                                    }
                                                                                    
                                                                                }
                                                                                
                                                                            }
                                                                            
                                                                        }
                                                                        
                                                                    }
                                                                }
                                                            }
                                                        }    
                                                    }
                                                    
                                                }
                                                
                                            }
                                            
                                        }
                                        
                                    }
                                    
                                }
                                
                            }
                        }
                    }
                }
            }               
            break;
            case 1:base=10;
                    if(isdigit(ch)){pCrtChr++;}
                   else
                   {
                       if(ch=='.'){s=8;pCrtChr++;}
                       else
                       {
                           if(ch=='e' || ch=='E'){s=10;pCrtChr++;}
                           else
                           {
                               s=4;
                           }
                           
                       }
                       
                   }
                   break;
            case 2: if(ch=='.'){s=8,pCrtChr++;}
                    else
                    {
                        if(ch=='e' || ch=='E'){s=10;pCrtChr++;}
                        else
                        {
                            if(isdigit(ch) && ch!='8' && ch!='9'){s=3;pCrtChr++;}
                            else
                            {
                                if(ch=='8' || ch=='9'){s=7;pCrtChr++;}
                                else
                                {
                                    if(ch=='x'){s=5;pCrtChr++;}
                                    else
                                    {
                                        s=4;
                                    }
                                        
                                }
                                    
                            }
                
                        }
            
                    }
                    break;
            case 3: base=8;
                    if(ch=='8' || ch=='9'){s=7;pCrtChr++;}
                    else
                    {
                        if(isdigit(ch) && ch!='8' && ch!='9'){pCrtChr++;}
                        else
                        {
                            if(ch=='.'){s=8,pCrtChr++;}
                            else
                            {
                                if(ch=='e' || ch=='E'){s=10;pCrtChr++;}
                                else
                                {
                                    s=4;
                                }
                            }
                        }
                    }
                    break;
            case 4: tk=addTk(CT_INT);
                    tk->base=base;
                    tk->i=createInteger(pStartCh,pCrtChr);
                    pStartCh=NULL;
                    return tk->code;
            case 5: base=16;
                    if(isdigit(ch) || isalpha(ch)){s=6;pCrtChr++;}
                    break;
            case 6: if(isdigit(ch) || isalpha(ch)){pCrtChr++;}
                    else
                        {
                                s=4;
                        }
                    break;
            case 7:if(isdigit(ch)){pCrtChr++;}
                   else
                   {
                       if(ch=='.'){s=8;pCrtChr++;}
                       else
                       {
                           if(ch=='e' || ch=='E'){s=10;pCrtChr++;}
                       }
                   break;
            case 8:if(isdigit(ch)){s=9;pCrtChr++;}
                   break;
            case 9:if(isdigit(ch)){pCrtChr++;}
                   else
                   {
                       if(ch=='e' || ch=='E'){s=10;pCrtChr++;}
                       else
                       {
                           s=13;
                       }
                          
                   }

                   break;
            case 10:if(ch=='+' || ch=='-'){s=11;pCrtChr++;}
                    else
                    {
                        s=11;
                    }
                    break;
            case 11:if(isdigit(ch)){s=12;pCrtChr++;}
                    break;
            case 12:if(isdigit(ch)){pCrtChr++;}
                    else
                    {
                        s=13;
                    }
                    break;
            case 13:tk=addTk(CT_REAL);
                    tk->r=createFloat(pStartCh,pCrtChr);
                    pStartCh=NULL;
                    return tk->code;        
            case 14:if(ch=='\\'){s=15;pCrtChr++;}
                    else 
                    {
                        if(ch!='\\' && ch!='\''){s=16;pCrtChr++;}
                    }
                    break;
            case 15:if(strchr(characters,ch)){s=16;pCrtChr++;}
                    break;
            case 16:if(ch=='\''){s=17;pCrtChr++;}
                    break;
            case 17:tk=addTk(CT_CHAR);
                    tk->i=*(pCrtChr-2);
                    return tk->code; 
            case 18:pStartCh=pCrtChr;
                    if(ch=='\\'){s=19;pCrtChr++;}
                    else
                    {
                        if(ch!='\"' && ch!='\\'){s=20;pCrtChr++;}
                        else
                        {
                            if(ch=='\"'){s=21;pCrtChr++;}
                        }
                    }
                    break;
            case 19:if(strchr(characters,ch)){s=20,pCrtChr++;}
                    break;
            case 20:if(ch!='\"' && ch!='\\'){pCrtChr++;}
                    else
                    {
                        if(ch=='\"'){s=21;pCrtChr++;}
                        else
                        {
                            if(ch=='\\'){s=19;pCrtChr++;}
                        }
                    }
                    break;
            case 21:tk=addTk(CT_STRING);
                    tk->text=createString(pStartCh,pCrtChr-1);
                    return CT_STRING;
            case 22:if(ch=='/'){s=25;pCrtChr++;}
                    else
                    {
                        if(ch=='*'){s=23;pCrtChr++;}
                        else
                        {
                            s=56;
                        }
                        
                    }
                    break;
            case 23:if(ch=='*'){s=24;pCrtChr++;}
                    else
                    {
                        if(ch=='\n')line++;
                        pCrtChr++;
                    }
                    break;
            case 24:if(ch=='*'){pCrtChr++;}
                    else
                        {
                            if(ch=='/'){s=0;pCrtChr++;}
                            else
                            {
                                if(ch=='\n')line++;
                                s=23;
                                pCrtChr++;
                            }
                            
                        }
                    break;
            case 25:if(ch!='\n' && ch!='\r' && ch!='\0'){pCrtChr++;}
                    else
                    {
                        s=0;
                    }
                    break;
            case 26:if(isalpha(ch) || isdigit(ch) || ch=='_'){pCrtChr++;}
                    else
                    {
                        s=27;
                    }
                    break;
            case 27:nch=pCrtChr-pStartCh;
                    if(nch==6 && !memcmp(pStartCh,"double",6))tk=addTk(DOUBLE);
                    else if(nch==6 && !memcmp(pStartCh,"return",6))tk=addTk(RETURN);
                        else if(nch==6 && !memcmp(pStartCh,"struct",6))tk=addTk(STRUCT);
                            else if(nch==5 && !memcmp(pStartCh,"while",5))tk=addTk(WHILE);
                                else if(nch==5 && !memcmp(pStartCh,"break",5))tk=addTk(BREAK);
                                    else if(nch==4 && !memcmp(pStartCh,"char",4))tk=addTk(CHAR);
                                        else if(nch==4 && !memcmp(pStartCh,"else",4))tk=addTk(ELSE);
                                            else if(nch==4 && !memcmp(pStartCh,"void",4))tk=addTk(VOID);
                                                else if(nch==3 && !memcmp(pStartCh,"int",3))tk=addTk(INT);
                                                    else if(nch==3 && !memcmp(pStartCh,"for",3))tk=addTk(FOR);
                                                        else if(nch==2 && !memcmp(pStartCh,"if",2))tk=addTk(IF);
                                                            else {
                                                                tk=addTk(ID);
                                                                tk->text=createString(pStartCh,pCrtChr);
                                                            }
                    pStartCh=NULL;
                    return tk->code;
            case 28:addTk(COMMA);
                    return COMMA;
            case 29:addTk(SEMICOLON);
                    return SEMICOLON;
            case 30:addTk(LPAR);
                    return LPAR;
            case 31:addTk(RPAR);
                    return RPAR;
            case 32:addTk(LBRACKET);
                    return LBRACKET;
            case 33:addTk(RBRACKET);
                    return RBRACKET;
            case 34:addTk(LACC);
                    return LACC;
            case 35:addTk(ADD);
                    return ADD;
            case 36:addTk(MUL);
                    return MUL;
            case 37:addTk(SUB);
                    return SUB;
            case 38:addTk(DOT);
                    return DOT;
            case 39:if(ch=='&'){s=40;pCrtChr++;}
                    break;
            case 40:addTk(AND);
                    return AND;
            case 41:if(ch=='|'){s=42;pCrtChr++;}
                    break;
            case 42:addTk(OR);
                    return OR;
            case 43:if(ch=='='){s=44;pCrtChr++;}
                    else
                    {
                        s=44;
                    }
                break;
            case 44:addTk(NOT);
                    return NOT;
            case 45:addTk(NOTEQ);
                    return NOTEQ;
            case 46:if(ch=='='){s=48;pCrtChr++;}
                    else{s=47;}
                    break;
            case 47:addTk(ASSIGN);
                    return ASSIGN;
            case 48:addTk(EQUAL);
                    return EQUAL;
            case 49:if(ch=='='){s=51;pCrtChr++;}
                    else
                    {
                        s=50;
                    }
                    break;
            case 50:addTk(LESS);
                    return LESS;
            case 51:addTk(LESSEQ);
                    return LESSEQ;
            case 52:if(ch=='='){s=54;pCrtChr++;}
                    else{s=53;}
                    break;
            case 53:addTk(GREATER);
                    return GREATER;
            case 54:addTk(GREATEREQ);
                    return GREATEREQ;
            case 55:addTk(END);
                    return END;
            case 56:addTk(DIV);
                    return DIV;
            case 57:addTk(RACC);
                    return RACC;
            default: printf("Unknown character %c state %s",ch,s);exit(0);       
        }
    }
}
}

void tkerr(const Token *tk,const char *fmt,...)
{
va_list va;
va_start(va,fmt);
fprintf(stderr,"error in line %d: ",tk->line);
vfprintf(stderr,fmt,va);
fputc('\n',stderr);
va_end(va);
exit(-1);
}


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

int typeBase()
{
    Token *startTk=crtTk;
    if(consume(INT) || consume(DOUBLE) || consume(CHAR))
        return 1;
    else
        if(consume(STRUCT))
        {
            if(consume(ID))
                return 1;
            else
            {
                tkerr(crtTk,"Missing ID for STRUCT");
            }
            
        }
    crtTk=startTk;
    return 0;
    
}

int arrayDecl()
{
    Token *startTk=crtTk;
    if(consume(LBRACKET))
    {
        expr();
        if(consume(RBRACKET))
            return 1;
        else
        {
            tkerr(crtTk,"Missing right bracket!");
        }
    }
    crtTk=startTk;
    return 0;
}

int declStruct()
{
    Token *startTk=crtTk;
    if(consume(STRUCT))
    {
        if(consume(ID))
        {
            if(consume(LACC))
            {
                for(;;)
                {
                    if(!declVar())
                    break;
                }
                if(consume(RACC))
                {
                    if(consume(SEMICOLON))
                        return 1;
                    else
                    {
                        tkerr(crtTk,"Missing semicolon!");
                    }
                }
                else tkerr(crtTk,"Missing right accolade!");   
            }
        }
        else
        {
            tkerr(crtTk,"Missing ID for STRUCT");
        }
        
    }
    return 0;
    crtTk=startTk;
}

int declVar()
{
    Token *startTk=crtTk;
    if(typeBase())
    {
        if(consume(ID))
        {
            arrayDecl();
            for(;;)
            {
                if(consume(COMMA))
                {
                    if(!consume(ID))
                    {
                        tkerr(crtTk,"Missing ID after comma!");
                    }
                }
                else
                break;
            }
            if(consume(SEMICOLON))
            {
                return 1;
            }
            else
            {
                tkerr(crtTk,"Missing semicolon!");
            }
        }
    }
    else
    {
        tkerr(crtTk,"Missing ID after typeBase!");
    }
    
    crtTk=startTk;
    return 0;
}

int typeName()
{
    Token *startTk=crtTk;
    if(typeBase())
    {
        arrayDecl();
        return 1;
    }
    crtTk=startTk;
    return 0;
}

int declFunc()
{
    Token *startTk=crtTk;
    if(typeBase())
    {
        consume(MUL);
        if(consume(ID))
        {
            if(consume(LPAR))
            {
                funcArg();
                for(;;)
                {
                    if(consume(COMMA))
                        funcArg();
                }
                if(consume(RPAR))
                {
                    if(stmCompound())
                    {
                        return 1;
                    }
                }
            }
            else
            {
                tkerr(crtTk,"Missing ID after typebase");
            }
            
        }
    }
    else 
    {
        if(consume(VOID))
        {
            if(consume(ID))
            {
                if(consume(LPAR))
                {
                    funcArg();
                    for(;;)
                    {
                        if(consume(COMMA))
                            funcArg();
                    }
                    if(consume(RPAR))
                    {
                        if(stmCompound())
                        {
                            return 1;
                        }
                    }
                }
            }
        }
    }
    crtTk=startTk;
    return 0;
}

int funcArg()
{
    Token *startTk=crtTk;
    if(typeBase())
    {
        if(consume(ID))
        {
            arrayDecl();
            return 1;
        }
    }
    crtTk=startTk;
    return 0;
}

int stm()
{
    Token *startTk=crtTk;
    if(consume(IF))
    {
        if(consume(LPAR))
        {
            if(expr())
            {
                if(consume(RPAR))
                {
                    if(stm())
                    {
                        if(consume(ELSE))
                        {
                            if(stm())
                                return 1;
                        }
                        return 1;
                    }
                }
            }
        }
    }
    else if(consume(WHILE))
        {
            if(consume(LPAR))
            {
                if(expr())
                {
                    if(consume(RPAR))
                    {
                        if(stm())
                        {
                            return 1;
                        }
                    }
                }
            }
        }
        else 
        if(consume(FOR))
        {
            if(consume(LPAR))
            {
                expr();
                if(consume(SEMICOLON))
                {
                    expr();
                    if(consume(SEMICOLON))
                    {
                        expr();
                        if(consume(RPAR))
                        {
                            if(stm())
                            {
                                return 1;
                            }
                        }
                    }
                }
            }
        }
        else 
        if(consume(BREAK))
        {
            if(consume(SEMICOLON))
            {
                return 1;
            }
        }
        else
        if(consume(RETURN))
        {
            expr();
            if(consume(SEMICOLON))
            {
                return 1;
            }
        }
        else
        {
            expr();
            if(consume(SEMICOLON))
            {
                return 1;
            }
        }
        crtTk=startTk;
        return 0;
}

int main()
{
    char inbuf[10001];
    FILE *fis;
    FILE *out;
    if((fis=fopen("8.c","r"))==NULL){
        printf("Eroare la deschiderea fisierului!");
        exit(0);
    }
    int n=fread(inbuf,1,10000,fis);
    inbuf[n]='\0';
    int o;
    pCrtChr=inbuf;
    fclose(fis);
    while(o=getNextToken()!=END);
    afisare();
    return 0;
}

