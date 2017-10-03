/* Definitions the scanner shares with the parser
    Michael L. Scott, 2008-2017.
*/
#ifndef SCAN_H
#define SCAN_H

#include <string>
using namespace std;

enum token{t_read, t_write, t_id, t_literal, t_gets, t_add, t_sub, t_mul, t_div, t_lparen, t_rparen, t_eof,t_if,t_fi,t_do,t_od,t_check,t_eq,t_neq,t_less,t_great,t_lesseq,t_greateq};

extern char token_image[100];

extern token scan();

#endif
