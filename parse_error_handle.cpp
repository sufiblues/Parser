/* Complete recursive descent parser for the calculator language.
    Builds on figure 2.16.  Prints a trace of productions predicted and
    tokens matched.  Does no error recovery: prints "syntax error" and
    dies on invalid input.
    Michael L. Scott, 2008-2017.
*/

#include <iostream>
#include <cstdlib>
#include "scan.h"
#include "scan.cpp"
#include <string>
#include <cstring>
using namespace std;


const string names[33] = {"read", "write", "id", "literal", "gets",
                       "add", "sub", "mul", "div", "lparen", "rparen", "eof","if","fi","do","od","check","eq","neq","less","great","lesseq","greateq"};



static token input_token;


void error () {
    puts("syntax error");
    exit (1);
}

void match_err (token expected){

	if(input_token == expected) {
		fputs("error_matched " , stdout);
		cout << names[input_token];

        	if (input_token == t_id || input_token == t_literal){

            		fputs(": ", stdout);
			cout << token_image;

		}

        	puts("");
        	input_token = scan();

    	}else{

		error();

	}

}


void match (token expected){

	if(input_token == expected) {
		fputs("matched " , stdout);
		cout << names[input_token];

        	if (input_token == t_id || input_token == t_literal){

            		fputs(": ", stdout);
			cout << token_image;

		}

        	puts("");
        	input_token = scan();

    	}else{

		error();

	}

}


void program();
void stmt_list();
void stmt();
void rel();
void expr();
void expr_tail();
void term_tail();
void term();
void factor_tail();
void factor();
void add_op();
void mul_op();
void ro_op();


void program(){

    switch (input_token) {
        case t_id:
        case t_read:
        case t_write:
        case t_if:
        case t_do:
        case t_check:
        case t_eof:
            puts("predict program --> stmt_list eof");
            stmt_list();
            match(t_eof);
            break;
        default:
            cout << "program error at " << names[input_token] << "\n";
            while((input_token != t_id) && (input_token != t_read) && (input_token != t_write) && (input_token != t_if) && (input_token != t_do) && (input_token != t_check) && (input_token!=t_eof))
            {
              match_err(input_token);
            }
            program();
            error ();
    }

}

void stmt_list () {

    switch(input_token){
        case t_id:
        case t_read:
        case t_write:
        case t_if:
        case t_do:
        case t_check:
            puts("predict stmt_list --> stmt stmt_list");
            stmt ();
            stmt_list ();
            break;
        case t_fi:
        case t_od:
        case t_eof:
            puts("predict stmt_list --> epsilon");
            break;
	    //epsilon production
        default:
            cout << "stmt_list error\n";
            while((input_token!=t_fi) && (input_token != t_od)&&(input_token != t_id) && (input_token != t_read) && (input_token != t_write) && (input_token != t_if) && (input_token != t_do) && (input_token != t_check) && (input_token!=t_eof))
            {
              match_err(input_token);
            }
            stmt_list();
            error ();
    }

}



void stmt () {
    switch (input_token) {
        case t_id:
            puts("predict stmt --> id gets rel");
            match (t_id);
            match (t_gets);
            rel();
            break;
        case t_read:
            puts("predict stmt --> read id");
            match(t_read);
            match(t_id);
            break;
        case t_write:
            puts("predict stmt --> write expr");
            match(t_write);
            rel();
            break;
        case t_if:
            puts("predict stmt --> if expr");
            match(t_if);
            rel();
            stmt_list();
            match(t_fi);
            break;
        case t_do:
            puts("predict stmt --> do stmt_list od");
            match(t_do);
            stmt_list();
            match(t_od);
            break;
        case t_check:
            puts("predict stmt --> check rel");
            match(t_check);
            rel();
            break;
        default:
            cout << "stmt error \n";
            while((input_token != t_id) && (input_token != t_read) && (input_token != t_write) && (input_token != t_if) && (input_token != t_do) && (input_token != t_check) && (input_token!=t_eof))
            {
              match_err(input_token);
            }
            stmt();
            error ();
    }
}

void rel() {
  switch (input_token){
    case t_lparen:
    case t_id:
    case t_literal:
      puts("predict rel --> expr expr_tail");
      expr();
      expr_tail();
      break;
    default:
    while((input_token != t_lparen) && (input_token != t_id) && (input_token != t_literal)&&(input_token != t_eof))
      {
        match_err(input_token);
      }
      if(input_token == t_eof)
      {
        program();
      }
      else
      {
        rel();
      }
  }
}

void expr() {
    switch(input_token) {
        case t_id:
        case t_literal:
        case t_lparen:
            puts("predict expr --> term term_tail");
            term();
            term_tail();
            break;
        default:
            while((input_token != t_lparen) && (input_token != t_id) && (input_token != t_literal)&&(input_token != t_eof))
            {
              match_err(input_token);
            }
            if(input_token == t_eof)
            {
              program();
            }
            else expr();

    }
}


void term(){
    switch(input_token) {
        case t_id:
        case t_literal:
        case t_lparen:
            puts("predict term --> factor factor_tail");
            factor();
            factor_tail();
            break;
        default:
            while((input_token != t_lparen) && (input_token != t_id) && (input_token != t_literal)&&(input_token != t_eof))
            {
              match_err(input_token);
            }
            if(input_token == t_eof)
            {
              program();
            }
            else expr();
    }
}


void factor(){
    switch(input_token){
        case t_id :
            puts("predict factor --> id");
            match(t_id);
            break;
        case t_literal:
            puts("predict factor --> literal");
            match(t_literal);
            break;
        case t_lparen:
            puts("predict factor --> lparen rel rparen");
            match(t_lparen);
            rel();
            match(t_rparen);
            break;
        default:
            while((input_token != t_lparen) && (input_token != t_id) && (input_token != t_literal)&&(input_token != t_eof))
            {
              match_err(input_token);
            }
            if(input_token == t_eof)
            {
              program();
            }
            else expr();
    }
}


void expr_tail(){
  switch(input_token){
    case t_eq:
    case t_neq:
    case t_less:
    case t_great:
    case t_lesseq:
    case t_greateq:
      puts("predict expr_tail --> ro expr");
      ro_op();
      expr();
      break;
    case t_eof:
    case t_id:
    case t_read:
    case t_write:
    case t_if:
    case t_do:
    case t_check:
    case t_fi:
    case t_od:
      puts("predict expr_tail --> epsilon");
      break;
    default:
    while((input_token != t_eq) && (input_token != t_neq) && (input_token != t_eq)&&(input_token != t_less)&&(input_token != t_great)&&(input_token != t_lesseq)&&(input_token != t_greateq)
          && (input_token != t_eof) && (input_token != t_id) && (input_token != t_read) && (input_token != t_write) && (input_token != t_if)&&(input_token != t_do)
          && (input_token != t_check) && (input_token != t_fi) && (input_token != t_od))
    {
      match_err(input_token);
    }
    if(input_token == t_eof)
    {
      program();
    }
    else expr_tail();
  }
}


void term_tail () {
    switch (input_token) {
        case t_add:
        case t_sub:
            puts("predict term_tail --> add_op term term_tail");
            add_op();
            term();
            term_tail();
            break;
        case t_rparen:
        case t_id:
        case t_read:
        case t_write:
        case t_eq:
        case t_neq:
        case t_less:
        case t_great:
        case t_lesseq:
        case t_greateq:
        case t_if:
        case t_do:
        case t_check:
        case t_fi:
        case t_od:
        case t_eof:
            puts("predict term_tail --> epsilon");
            break;

        default:
        while((input_token != t_add) && (input_token != t_sub) && (input_token != t_eq) && (input_token != t_neq) && (input_token != t_eq)&&(input_token != t_less)&&(input_token != t_great)&&(input_token != t_lesseq)&&(input_token != t_greateq)
              && (input_token != t_eof) && (input_token != t_id) && (input_token != t_read) && (input_token != t_write) && (input_token != t_if)&&(input_token != t_do)
              && (input_token != t_check) && (input_token != t_fi) && (input_token != t_od) )
        {
          match_err(input_token);
        }
        if(input_token == t_eof)
        {
          program();
        }
        else term_tail();
    }
}


void factor_tail () {
    switch(input_token){
        case t_mul:
        case t_div:
            puts("predict factor_tail --> mul_op factor factor_tail");
            mul_op();
            factor();
            factor_tail();
            break;
        case t_add:
        case t_sub:
        case t_rparen:
        case t_id:
        case t_eq:
        case t_neq:
        case t_less:
        case t_great:
        case t_lesseq:
        case t_greateq:
        case t_read:
        case t_write:
        case t_if:
        case t_do:
        case t_check:
        case t_fi:
        case t_od:
        case t_eof:
            puts("predict factor_tail --> epsilon");
            break;
	     //epsilon production
        default:
        cout << "factor_tail error\n";
        while((input_token != t_mul) && (input_token != t_div) && (input_token != t_add) && (input_token != t_sub) && (input_token != t_eq) && (input_token != t_neq) && (input_token != t_eq)&&(input_token != t_less)&&(input_token != t_great)&&(input_token != t_lesseq)&&(input_token != t_greateq)
              && (input_token != t_eof) && (input_token != t_id) && (input_token != t_read) && (input_token != t_write) && (input_token != t_if)&&(input_token != t_do)
              && (input_token != t_check) && (input_token != t_fi) && (input_token != t_od))
        {
          match_err(input_token);
        }
        if(input_token == t_eof)
        {
          program();
        }
        else factor_tail();

    }
}


void add_op(){
    switch(input_token){
        case t_add:
            puts("predict add_op --> add");
            match(t_add);
            break;
        case t_sub:
            puts("predict add_op --> sub");
            match(t_sub);
            break;
        default: error();
    }
}

void mul_op(){

    switch(input_token){
        case t_mul:
            puts("predict mul_op --> mul");
            match(t_mul);
            break;
        case t_div:
            puts("predict mul_op --> div");
            match(t_div);
            break;
        default: error ();
    }

}

void ro_op(){
  switch (input_token) {
    case t_eq:
        puts("predict ro_op--> eq");
        match(t_eq);
        break;
    case t_neq:
        puts("predict ro_op--> neq");
        match(t_neq);
        break;
    case t_less:
        puts("predict ro_op--> less");
        match(t_less);
        break;
    case t_great:
        puts("predict ro_op--> great");
        match(t_great);
        break;
    case t_lesseq:
        puts("predict ro_op--> lesseq");
        match(t_lesseq);
        break;
    case t_greateq:
        puts("predict ro_op--> greateq");
        match(t_greateq);
        break;
  }
}
int main(){
    input_token = scan();
    program();
    return 0;
}
