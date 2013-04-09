/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

grammar test;



expression: prio15Expression;

prio15Expression: 
    NUMBER/* |
    IDENTIFIER |
    CONSTANT |
    '(' expression ')'*/;

label:
	IDENTIFIER |
	CONSTANT;

constant:
	INTEGER |
	STRING;


//=============================================================================================


MULTILINE_COMMENT: '/*' .*? '*/' {skip();};

NUMBER: INTEGER ('.' INTEGER)?;

LINE_COMMENT
  :  '//' ~( '\r' | '\n' )* {skip();}
  ;

IDENTIFIER: ('a'..'z' |'A'..'Z' |'_' ) ('a'..'z' |'A'..'Z' |'_' |'0'..'9' )*;

fragment INTEGER: ('0'..'9')+ ;

STRING: '"' .*? '"';

WHITESPACE: (' ' |'\t' |'\n' |'\r' )+ {skip();} ;