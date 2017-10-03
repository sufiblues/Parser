/* Simple ad-hoc scanner for the calculator language.
    Michael L. Scott, 2008-2017.
*/

#ifndef SCAN_CPP
#define SCAN_CPP


#include <iostream>
#include <cstdlib>
#include <string>
#include <cctype>
#include <cstdio>
#include <cstring>

#include "scan.h"
using namespace std;

char token_image[100];
int linecounter = 0;

token scan() {
    static int c = ' ';
        /* next available char; extra (int) width accommodates EOF */
    int i = 0;              /* index into token_image */

    /* skip white space */
    while (isspace(c)) {
        if(c == '\n')
        {
          linecounter++;
        }
        c = getc(stdin);
    }
    if (c == EOF)
        return t_eof;
    if (isalpha(c)) {
        do {
            token_image[i++] = c;
            c = getc(stdin);
        } while (isalpha(c) || isdigit(c) || c == '_');
        token_image[i] = '\0';
        if (!strcmp(token_image, "read")) return t_read;
        else if (!strcmp(token_image,"write")) return t_write;
        else if (!strcmp(token_image,"if"))
        {
                return t_if;
        }

        else if (!strcmp(token_image,"fi")) return t_fi;
        else if (!strcmp(token_image,"do")) return t_do;
        else if (!strcmp(token_image,"od")) return t_od;
        else if (!strcmp(token_image,"check")) return t_check;
        else if (!strcmp(token_image,"==")) {return t_eq;}
        else if (!strcmp(token_image,"<>")) return t_neq;
        else if (!strcmp(token_image,"<")) return t_less;
        else if (!strcmp(token_image,">")) return t_great;
        else if (!strcmp(token_image,"<=")) return t_lesseq;
        else if (!strcmp(token_image,">=")) return t_greateq;

        else return t_id;
    }
    else if (isdigit(c)) {
        do {
            token_image[i++] = c;
            c = getc(stdin);
        } while (isdigit(c));
        token_image[i] = '\0';
        return t_literal;
    } else switch (c) {
        case ':':
            if ((c = getc(stdin)) != '=') {
                cerr << "error\n";
                exit(1);
            } else {
                c = getc(stdin);
                return t_gets;
            }
            break;
        case '+': c = getc(stdin); return t_add;
        case '$': c = getc(stdin); return t_eof;
        case '-': c = getc(stdin); return t_sub;
        case '*': c = getc(stdin); return t_mul;
        case '/': c = getc(stdin); return t_div;
        case '(': c = getc(stdin); return t_lparen;
        case ')': c = getc(stdin); return t_rparen;
        case '<': c = getc(stdin);
		if((c) == '>' ){

			c = getc(stdin);
			return t_neq;

		}else{

			if((c) == '='){

				c = getc(stdin);
				return t_lesseq;

			}


		}

		return t_less;

        case '>': c = getc(stdin);

		if((c) == '='){

			c = getc(stdin);
			return t_greateq;

		}


		return t_great;

	case '=':
            if((c = getc(stdin)) == '=')
            {
		c = getc(stdin);
               return t_eq;
            }
            else
            {
              cerr <<"error\n";
              exit(1);
            }
    /*    case '<':
            switch (c = getc(stdin)) {
              case '=':
                  return t_lesseq;
              case ' >':
                  return t_neq;
            }
        case '>':
            if((c = getc(stdin)) == '=')
            {
              return t_greateq;
            }
            else
            {
              cerr << "error\n";
              exit(1);
            }*/
        default:
            cout << "error\n";
            exit(1);
    }
}



#endif
