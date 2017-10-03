Ali Hamdani
CSC 254
ahamdani@u.rochester.edu
Partner:Alexander Senatore

~~~~~~~~~~~~Files~~~~~~~~~~~~~~~~
scan.cpp- This file has the scan method that we use to take input from a stream. The file also has global variables that take into account useful counters like line counters that we use for error handling. 

scan.h - This file has the enumerator for the tokens.

parseWithAG.cpp - This file has the parse tree with an attribute grammer for the simple calculator grammar. It will print out an AST if the grammar does not have any errors and if it does then the error handler will give you an output. 

predictset.txt- this file has the predict set,follow set,and first set for all the nonTerminals in the attributegrammar.

input1.txt,input2.txt,input3.txt,input4.txt-These are the text files that contain input that is run by the make file.

parse_error_handler.cpp- this is just hte parser with an appropiate error handler for some reason the error handler for the attribute grammar does not work.

~~~~~~~~~~~~Description~~~~~~~~~~~~~
Part1:
The first part of the project was to convert all the C code into C++ code. This was not too bad since most of the syntax transfers over. However the syntax to create enumerators was different so the scan.h file was something that we had to change. Another issue that we had was trying to find appropiate c++ functions that were compatible with the source code. It was kinda trivial to do it with the parse.cpp file since the only function that was C specific was printf and its like. However for the scan.cpp file it was a lot harder since getchar had a very specific function. We eventually found a different function that does something very similar to it that made the whole process a little easier. 

Part2:
I just wanted to say that the directions were a little wrong. we had to extend the language with R and ro along with if/do/check statements. However the end result was kinda similar. What we did was construct predict sets for all the productions in the grammar. This required use to also construct the first  and follow sets. After that was done we first created the appropiate functions for R and ro_op. We then went to program/stmt_list/stmt and extended all of those functions with appropiate switch statements. Then we changed the rest of the language to take into account the new operators. 

Part3:This part of the project was probably the most tedious because I chose to do it brute force. The esscence of it is if there is an error (i.e. the token does not match any of the tokens for the predict set) then the error handler will shoot up. This handler will first delete the token then keep deleting tokens untill it finds something that can start the current production again or something that is in the follow set of the current production or if it is part of a starter set of "safe symbols". 

Part4:This part of the project was probably the most difficult because of our misunderstanding of attribute grammars. What we first did was we created a global string called AST then tried to append string to that untill we completed the porgram but we quickly found out that did not work, So our next option was to create an attribute grammae and we did this by having each function that we defined return a string. the function itself would append the appropiate strings and return them. the only issue we had was if the attribute grammar we created needed to look for a term in the previous part of the tree. For example TT->ao T TT, The first term tail needs to look for the previous term but with our current structure it could not do that. So what we did was we passed an argument to the function. We now we have the previous term we can proceed with the attribute grammar as listed in the psuedocode.

~~~~~~~~~~~Instructions~~~~~~~~~
go to the directory > edit the input files we have given you for your own personal test cases > run "make test" in the commmand line. 

to test the error andler:
cd dir> g++ parser_error_handler.cpp -o parser > ./parser > enter program > read output.

~~~~~~~~~~~Division of Labor~~~~~
Alec and I would work together side by side on this project. Sometimes we would come to an impass about what approach we would use to solve a problem. In that case we would both try our approaches and see which one would work in the end. 

~~~~~~~~~~~Problems~~~~~~~~~~~
So first of all our attribute grammar sometimes does not work with parenthesis however I included a differnt parser does work with all cases. I do not know what the problem is but we get segfaults with the attribute grammar. If we had more time we would definately come to office hours to get it fixed because there is not reason why it should not work. 



