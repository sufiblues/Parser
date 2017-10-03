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

int counter = 0;

const string names[33] = {"read", "write", "id", "literal", "gets",
                       "add", "sub", "mul", "div", "lparen", "rparen", "eof","if","fi","do","od","check","eq","neq","less","great","lesseq","greateq"};



static token input_token;

void error () {
    puts("syntax error");
    exit (1);
}

int predict_SL[7] = {t_id,t_read,t_write,t_if,t_do,t_check,t_eof};
int predict_S[6] = {t_id,t_read,t_write,t_if,t_do,t_check};
int predict_E[3] = {t_lparen,t_id, t_literal};
int predict_R[3] = {t_lparen,t_id, t_literal};
int predict_T[3] = {t_lparen,t_id, t_literal};
int predict_F[3] = {t_lparen,t_id, t_literal};
int predict_ET[14] = {t_rparen,t_eq,t_neq,t_less,t_great,t_lesseq,t_greateq,t_id,t_read,t_write,t_if,t_do,t_check,t_eof};
int predict_TT[16] = {t_add,t_sub,t_rparen,t_eq,t_neq,t_less,t_great,t_lesseq,t_greateq,t_id,t_read,t_write,t_if,t_do,t_check,t_eof};
int predict_FT[18] = {t_mul,t_div,t_add,t_sub,t_rparen,t_eq,t_neq,t_less,t_great,t_lesseq,t_greateq,t_id,t_read,t_write,t_if,t_do,t_check,t_eof};
int predict_ro[6] = {t_eq,t_neq,t_less,t_great,t_lesseq,t_greateq};
int predict_ao[2] = {t_add,t_sub};
int predict_mo[2] = {t_mul,t_div};

void expected_token(int tokens[])
{
  cout << "Expecting token(s) :";

  for(int i = 0 ; i <= sizeof(tokens)/sizeof(int); i++ )
  {
    cout << names[tokens[i]] << ", ";
  }

}

void match_err (token expected){
	counter ++;
    if(input_token == expected) {
  //      fputs("error_matched " , stdout);

            if (input_token == t_id || input_token == t_literal){

        //            fputs(": ", stdout);

        }

            puts("");
            input_token = scan();

        }else{

        error();

    }

}

void match (token expected){

    if(input_token == expected) {
//        fputs("matched " , stdout);
      //  cout << names[input_token];

            if (input_token == t_id || input_token == t_literal){

    //                fputs(": ", stdout);
    //        cout << token_image;

        }

    //        puts("");
            input_token = scan();
        }else{
        error();
    }

}


string program();
string stmt_list();
string stmt();
string rel();
string expr();
string expr_tail(string expr_prev);
string term_tail(string term_prev);
string term();
string factor_tail(string fact_prev);
string factor();
string add_op();
string mul_op();
string ro_op();

string AST = " ";

string program(){
string ret = "";
    switch (input_token) {
        case t_id:
        case t_read:
        case t_write:
        case t_if:
        case t_do:
        case t_check:
        case t_eof:
            //puts("predict program --> stmt_list eof");
            ret.append("(program \n[");
            ret.append(stmt_list());
            ret.append("]\n)");
            match(t_eof);
            return ret;
            break;
        default:
	     counter++;

            cout << "program error at " << names[input_token] << "\n";
            match_err(input_token);
            while((input_token != t_id) && (input_token != t_read) && (input_token != t_write) && (input_token != t_if) && (input_token != t_do) && (input_token != t_check) && (input_token!=t_eof))
            {
              match_err(input_token);
            }
            counter++;
            program();

    }

}

string stmt_list () {
string ret = "";
    switch(input_token){
        case t_id:
        case t_read:
        case t_write:
        case t_if:
        case t_do:
        case t_check:
            //puts("predict stmt_list --> stmt stmt_list");

            ret.append(stmt ());
            ret.append(stmt_list ());
            return ret;
        case t_fi:
        case t_od:
        case t_eof:
            return ret;

        //epsilon production
        default:
	     counter++;

            expected_token(predict_SL);
            cout << " got " << names[input_token];
            cout << " at line " << linecounter;
            match_err(input_token);
            while((input_token!=t_fi) && (input_token != t_od)&&(input_token != t_id) && (input_token != t_read) && (input_token != t_write) && (input_token != t_if) && (input_token != t_do) && (input_token != t_check) && (input_token!=t_eof))
            {
              match_err(input_token);
            }
            stmt_list();
            counter++;
}}

string stmt () {
    string lookahead = token_image;
    string ret = "(";
    switch (input_token) {
        case t_id:
            //puts("predict stmt --> id gets rel");
            match (t_id);
            ret.append(":= \"");
            ret.append(lookahead);
            ret.append("\" ");
            if(input_token == t_gets)
            {
               match (t_gets);
            }
            else if(input_token == t_eof){
            }
            else{
              cout << "Error Expecting := at line:" << linecounter << '\n';
              match_err(input_token);
            }
            ret.append(rel());
            ret.append(")\n");
            return ret;
            break;
        case t_read:
            //puts("predict stmt --> read id");
            match(t_read);
            ret.append("read \"");
            ret.append(token_image);
            ret.append("\"");
            if(input_token == t_id)
            {
              cout << token_image;
              match(t_id);
            }
            else if (input_token == t_eof)
            {
              cout << "Error Expecting id got eof at line: " << linecounter << '\n' ;
            }
            else{
              cout << "Error Expecting id got " << token_image << " at line: " << linecounter << '\n';
              match_err(input_token);
              return ret;
            }
            ret.append(")\n");
            return ret;
            break;
        case t_write:
            //puts("predict stmt --> write expr");
            match(t_write);
            ret.append("write ");

            ret.append(rel());
            ret.append(")\n");

            return ret;
            break;
        case t_if:
            //puts("predict stmt --> if expr");
            match(t_if);
            ret.append("if \n ");

            ret.append(rel());
            ret.append("\n[");
            ret.append(stmt_list());
            ret.append("]\n");

            if(input_token == t_fi)
            {
              match(t_fi);
            }
            else if(input_token == t_eof)
            {
              cout << "Error Expecting fi got eof at line: " << linecounter << '\n' ;
            }
            else {
              cout << "Error expecting fi\n";
              match(input_token);
            }
            ret.append(")\n");
            return ret;
            break;
        case t_do:
          //  puts("predict stmt --> do stmt_list od");
            match(t_do);
            ret.append("do \n [");
            ret.append(stmt_list());
            ret.append("]\n");
            if(input_token == t_od)
            {
              match(t_od);
            }
            else if(input_token == t_eof)
            {
              cout << "Error Expecting od got eof at line: " << linecounter << '\n' ;
            }
            else {
              cout << "Error Expecting od\n";
              match(input_token);
            }
            ret.append(")\n");
            return ret;
            break;
        case t_check:
        //    puts("predict stmt --> check rel");
            match(t_check);
            ret.append("check ");
            ret.append(rel());
            ret.append(")\n");
            return ret;
            break;
        default:
	     counter++;

            expected_token(predict_S);
            cout << " got " << names[input_token];
            cout << " at line " << linecounter;
            while((input_token != t_id) && (input_token != t_read) && (input_token != t_write) && (input_token != t_if) && (input_token != t_do) && (input_token != t_check) && (input_token!=t_eof) && (input_token != t_fi) && (input_token != t_od))
            {
              match_err(input_token);
            }
            if((input_token != t_id) && (input_token != t_read) && (input_token != t_write) && (input_token != t_if) && (input_token != t_do) && (input_token != t_check))
            {
              stmt();
            }
            counter++;
    }
}

string rel() {
  string ret = "";
  string exprlook = "";
  switch (input_token){
    case t_lparen:
    case t_id:
    case t_literal:
      //puts("predict rel --> expr expr_tail");
      exprlook = expr();
      ret.append(expr_tail(exprlook));
      return ret;
      break;
    default:
       counter++;

      expected_token(predict_R);
      cout << " got " << names[input_token];
      cout << " at line " << linecounter;
      while((input_token != t_rparen) && (input_token != t_read)&&(input_token != t_write) && (input_token != t_if) && (input_token != t_do)&&(input_token != t_check)&&(input_token != t_fi)&&(input_token != t_fi)&&(input_token != t_lparen) && (input_token != t_id) && (input_token != t_literal)&&(input_token != t_eof))
      {
        match_err(input_token);
      }
      if(input_token == t_lparen || input_token == t_id || input_token == t_literal)
      {
        rel();
      }
      counter++;
  }
}

string expr() {
    string ret = "";
    string termlook = "";
    switch(input_token) {
        case t_id:
        case t_literal:
        case t_lparen:
        //    puts("predict expr --> term term_tail");
            termlook = term();
            ret.append(term_tail(termlook));
            return ret;
            break;
        default:
	     counter++;

            expected_token(predict_E);
            cout << " got " << names[input_token];
            cout << " at line " << linecounter;
            while((input_token != t_eq) && (input_token != t_neq) && (input_token != t_eq)&&(input_token != t_less)&&(input_token != t_great)&&(input_token != t_lesseq)&&(input_token != t_greateq)&&
            (input_token != t_id) && (input_token != t_read) && (input_token != t_write) && (input_token != t_if) && (input_token != t_do) && (input_token != t_check) && (input_token != t_fi) && (input_token != t_od) &&
            (input_token != t_lparen) && (input_token != t_id) && (input_token != t_literal)&&(input_token != t_eof)&&(input_token != t_rparen))
            {
              match_err(input_token);
            }
            if(input_token == t_lparen || input_token == t_id || input_token == t_literal)
            {
              expr();
            }

            counter++;
    }

}

string term(){
    string ret = "";
    string factlook = "";
    switch(input_token) {
        case t_id:
        case t_literal:
        case t_lparen:
        //    puts("predict term --> factor factor_tail");
            factlook = factor();
            ret.append(factor_tail(factlook));
            return ret;
        default:
	     counter++;

            expected_token(predict_T);
            cout << " got " << names[input_token];
            cout << " at line " << linecounter;
            while((input_token != t_add)&&(input_token != t_sub)&&(input_token != t_eq) && (input_token != t_neq) && (input_token != t_eq)&&(input_token != t_less)&&(input_token != t_great)&&(input_token != t_lesseq)&&(input_token != t_greateq)&&
            (input_token != t_id) && (input_token != t_read) && (input_token != t_write) && (input_token != t_if) && (input_token != t_do) && (input_token != t_check) && (input_token != t_fi) && (input_token != t_od) &&
            (input_token != t_lparen) &&(input_token != t_rparen) && (input_token != t_id) && (input_token != t_literal)&&(input_token != t_eof))
            {
              match_err(input_token);
            }
            if(input_token == t_lparen || input_token == t_id || input_token == t_literal)
            {
              return term();
            }

            counter++;
    }
    return ret;

}

string factor(){
    string ret = "";
    string lookahead = token_image;
    switch(input_token){
        case t_id :
        //    puts("predict factor --> id");

            match(t_id);
            ret.append("( id \"");
            ret.append(lookahead);
            ret.append("\")");
            return ret;
            break;
        case t_literal:
        //    puts("predict factor --> literal");

            match(t_literal);
            ret.append("( num \"");
            ret.append(lookahead);
            ret.append("\")");
            return ret;
            break;
        case t_lparen:
      //      puts("predict factor --> lparen rel rparen");
            match(t_lparen);
            ret.append("(");
            ret.append(rel());
            ret.append(")");
            if(input_token == t_rparen)
            {
              match(t_rparen);
            }
            else if(input_token == t_eof){
              cout << "Error Expecting ) got eof at line: " << linecounter << '\n';
            }
            else {
              cout << "Error Expecting )\n";
              match(input_token);
            };
            return ret;
            break;
        default:
	     counter++;

        expected_token(predict_F);
        cout << " got " << names[input_token];
        cout << " at line " << linecounter;
        while((input_token != t_div) && (input_token != t_mul)&&(input_token != t_add)&&(input_token != t_sub)&&(input_token != t_eq) && (input_token != t_neq) && (input_token != t_eq)&&(input_token != t_less)&&(input_token != t_great)&&(input_token != t_lesseq)&&(input_token != t_greateq)&&
        (input_token != t_id) && (input_token != t_read) && (input_token != t_write) && (input_token != t_if) && (input_token != t_do) && (input_token != t_check) && (input_token != t_fi) && (input_token != t_od) &&
        (input_token != t_lparen) &&(input_token != t_rparen) && (input_token != t_id) && (input_token != t_literal)&&(input_token != t_eof))
        {
          match_err(input_token);
        }
        if(input_token == t_lparen || input_token == t_id || input_token == t_literal)
        {
          factor();
        }
        counter++;
    }
}

string expr_tail(string expr_prev){
  string ret = "(";
  switch(input_token){
    case t_eq:
    case t_neq:
    case t_less:
    case t_great:
    case t_lesseq:
    case t_greateq:
    //  puts("predict expr_tail --> ro expr");
      ret.append(ro_op());
      ret.append(expr_prev);
      ret.append(expr());
      ret.append(")");
      return ret;
    case t_eof:
    case t_id:
    case t_read:
    case t_write:
    case t_if:
    case t_do:
    case t_check:
    case t_fi:
    case t_od:
  //    puts("predict expr_tail --> epsilon");
      return expr_prev;
    default:
       counter++;

    expected_token(predict_ET);
    cout << " got " << names[input_token];
    cout << " at line " << linecounter;

    while((input_token != t_eq) && (input_token != t_neq) && (input_token != t_eq)&&(input_token != t_less)&&(input_token != t_great)&&(input_token != t_lesseq)&&(input_token != t_greateq)
          && (input_token != t_eof) && (input_token != t_id) && (input_token != t_read) && (input_token != t_write) && (input_token != t_if)&&(input_token != t_do)
          && (input_token != t_check) && (input_token != t_fi) && (input_token != t_od) && (input_token != t_rparen))
    {
      match_err(input_token);
    }
    if((input_token == t_eq) || (input_token == t_neq) || (input_token == t_eq)||(input_token == t_less)||(input_token == t_great)||(input_token == t_lesseq)||(input_token == t_greateq))
    {
      expr_tail(expr_prev);
    }
    counter++;
  }
}

string term_tail (string term_prev) {
    string ret = "(";
    string termlook = "";
    switch (input_token) {
        case t_add:
        case t_sub:
        //    puts("predict term_tail --> add_op term term_tail");
            ret.append(add_op());
            cout << term_prev;
            ret.append(term_prev);
            termlook = term();
            ret.append(term_tail(termlook));
            ret.append(")");
            return ret;
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
        //    puts("predict term_tail --> epsilon");
            return term_prev;

        default:
	     counter++;

        expected_token(predict_TT);
        cout << " got " << names[input_token];
        cout << " at line " << linecounter;
        while((input_token != t_add)&& (input_token != t_sub)&&(input_token != t_eq) && (input_token != t_neq) && (input_token != t_eq)&&(input_token != t_less)&&(input_token != t_great)&&(input_token != t_lesseq)&&(input_token != t_greateq)
              && (input_token != t_eof) && (input_token != t_id) && (input_token != t_read) && (input_token != t_write) && (input_token != t_if)&&(input_token != t_do)
              && (input_token != t_check) && (input_token != t_fi) && (input_token != t_od) && (input_token != t_rparen))
        {
          match_err(input_token);
        }
        if((input_token == t_add) || (input_token == t_sub))
        {
          term_tail(term_prev);
        }
        counter++;
    }
}

string factor_tail (string fact_prev) {
    string ret = "(";
    string factlook = "";
    string temp = "";
    switch(input_token){
        case t_mul:
        case t_div:
        //    puts("predict factor_tail --> mul_op factor factor_tail");
            ret.append(mul_op());
            ret.append(fact_prev);
            factlook = factor();
            temp = factor_tail(factlook);
            ret.append(temp);
            ret.append(")");
            return ret;

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
        //    puts("predict factor_tail --> epsilon");
            return fact_prev;
         //epsilon production
        default:
	     counter++;

        expected_token(predict_FT);
        cout << " got " << names[input_token];
        cout << " at line " << linecounter;
        while((input_token!=t_div)&&(input_token!=t_mul)&&(input_token != t_add)&& (input_token != t_sub)&&(input_token != t_eq) && (input_token != t_neq) && (input_token != t_eq)&&(input_token != t_less)&&(input_token != t_great)&&(input_token != t_lesseq)&&(input_token != t_greateq)
              && (input_token != t_eof) && (input_token != t_id) && (input_token != t_read) && (input_token != t_write) && (input_token != t_if)&&(input_token != t_do)
              && (input_token != t_check) && (input_token != t_fi) && (input_token != t_od) && (input_token != t_rparen))
        {
          match_err(input_token);
        }
        if((input_token == t_mul) || (input_token == t_div))
        {
          factor_tail(fact_prev);
        }
       

    }
}

string add_op(){
    switch(input_token){
        case t_add:
        //    puts("predict add_op --> add");
            match(t_add);
            return "+";

        case t_sub:
      //      puts("predict add_op --> sub");
            match(t_sub);
	          return "-";

        default:
        expected_token(predict_ao);
        cout << " got " << names[input_token];
        cout << " at line " << linecounter;
        return "";
    }
}

string mul_op(){
    switch(input_token){
        case t_mul:

        //    puts("predict mul_op --> mul");
            match(t_mul);
		        return "*";
        case t_div:
        //    puts("predict mul_op --> div");
            match(t_div);
		        return "/";
        default:
        expected_token(predict_mo);
        cout << " got " << names[input_token];
        cout << " at line " << linecounter;
        return "";
    }

}

string ro_op(){
  switch (input_token) {
    case t_eq:
    //    puts("predict ro_op--> eq");
        match(t_eq);
   	return "==";
        break;
    case t_neq:
    //    puts("predict ro_op--> neq");

        match(t_neq);
	return "<>";
        break;
    case t_less:
    //    puts("predict ro_op--> less");
	return "<";
        break;
    case t_great:
    //    puts("predict ro_op--> great");
        match(t_great);
	return ">";
        break;
    case t_lesseq:
    //    puts("predict ro_op--> lesseq");
        match(t_lesseq);
        return "<=";
        break;
    case t_greateq:
    //    puts("predict ro_op--> greateq");
        match(t_greateq);
        return ">=";
        break;
    default:
        expected_token(predict_ro);
        cout << " got " << names[input_token];
        cout << " at line " << linecounter;
        return "";
  }
}

int main(int r, char** argv){

    //cout << "\nHERE:"<<argv[3]<<"\n";
    freopen(argv[3], "r", stdin);
    input_token = scan();
    if(counter == 0){
    	cout << program();
    }
    return 0;
}
