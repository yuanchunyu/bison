%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.0.4"
%defines
%define api.namespace {py}
%define parser_class_name {py_parser}
%define api.token.constructor {false}
%define api.value.type {AstNode*}
%define parse.assert
%define api.token.prefix {}

%code requires
{
# include <string>

    namespace py {
    class ParserContext;
    class AstNode;
    }
}
// The parsing context.
%param { ParserContext& ctx }
%locations
%initial-action
{
  // Initialize the initial location.
    @$.begin.filename = @$.end.filename = &ctx.fileName;
};
%define parse.trace
%define parse.error verbose


%code provides
{
// A way to make syntax exceptions
// The body of the parser::syntax_error is defined as inline
// in the .cc file, so define a function here to create one of these
// exception types so that it may be used outside of the py_parser.cc
// file. 
namespace py
{
    py_parser::syntax_error syntax_error(const py_parser::location_type& l, const std::string& m);
}
   
}

%code
{
    #include "ParserContext.h"
}


%token
DEF "def"
PASS "pass"
IF "if"
ELSE "else"
ELIF "elif"
OR "or"
NOT "not"
AND "and"
IN "in"
IS "is"
FOR "for"
DEL "del"
ENDMARKER
NEWLINE
INDENT
DEDENT
LPAR "("
RPAR ")"
LSQB "["
RSQB "]"
COLON ":"
COMMA ","
SEMI ";"
PLUS "+"
MINUS "-"
STAR "*"
SLASH "/"
VBAR "|"
AMPER "&"
LESS "<"
GREATER ">"
EQUAL "="
DOT "."
PERCENT "%"
BACKQUOTE "`"
LBRACE "{"
RBRACE "}"
EQEQUAL "=="
NOTEQUAL "!="
LESSEQUAL "<="
GREATEREQUAL ">="
TILDE "~"
CIRCUMFLEX "^"
LEFTSHIFT "<<"
RIGHTSHIFT ">>"
DOUBLESTAR "**"
PLUSEQUAL "+="
MINEQUAL "-="
STAREQUAL "*="
SLASHEQUAL "/="
PERCENTEQUAL "%="
AMPEREQUAL "&="
VBAREQUAL "|="
CIRCUMFLEXEQUAL "^="
LEFTSHIFTEQUAL "<<="
RIGHTSHIFTEQUAL ">>="
DOUBLESTAREQUAL "**="
DOUBLESLASH "//"
DOUBLESLASHEQUAL "//="

LARROW "<-"
RARROW "->"
ELLIPSIS "..."

AT "@"
AWAIT "await"
ASYNC "async"
FROM "from"
IMPORT "import"
YIELD "yield"
BREAK "break"
CONTINUE "continue"
RAISE "raise"
RETURN "return"
AS "as"
NAME
NUMBER
STRING
;

//%token <std::string> NAME "name"
//%token <int> NUMBER "number"
//%token <std::string> STRING "string"
//%type <AstNode*> atom


// left associativity
//%left "(" ")"
//%left "-" "+"
//%left "*" "/"  "%" "//"
 //Primary	  left to right	 ()  [ ]  .  -> dynamic_cast typeid

//%left "<<"  ">>"
//%left "<"  ">"  "<="  ">="
//%left "=="  "!="
//%left "&" "^" "|"
//%left "and" "or"
//%left ","



// %right "="  "+="  "-="  "*="   "/="  "<<="  ">>="  "%="   "&="  "^="  "|="

 // ++  --  +  -  !  ~  &  *  (type_name)  sizeof new delete

 //   C++ Pointer to Member	left to right	.*->*
 //Conditional	 right to left	 ? :
 //Assignment	 right to left	 =  +=  -=  *=   /=  <<=  >>=  %=   &=  ^=  |=
 //Comma	 left to right	 ,

%printer { yyoutput << $$; } <>

%%



// # Start symbols for the grammar:
// #       single_input is a single interactive statement;
// #       file_input is a module or sequence of commands read from an input file;
// #       eval_input is the input for the eval() functions.
// # NB: compound_stmt in single_input is followed by extra NEWLINE!
// single_input: NEWLINE | simple_stmt | compound_stmt NEWLINE
// file_input: (NEWLINE | stmt)* ENDMARKER
// eval_input: testlist NEWLINE* ENDMARKER



%start module;

module:
    file_input
    {
        AstNodeSeq *seq = dynamic_cast<AstNodeSeq*>($1);
        assert(seq);
        ctx.ast->module = ctx.ast->CreateModule(@$, seq->seq);

        // make the parser printer happy, it expects a valid node
        $$ = ctx.ast->module;
        delete seq;
    }
;


// *python3
// single_input: NEWLINE | simple_stmt | compound_stmt NEWLINE
//single_input:
//    NEWLINE
//    | simple_stmt
//    | compound_stmt NEWLINE
//;

// *python3
// file_input: (NEWLINE | stmt)* ENDMARKER
file_input:
     newline_stmt_seq ENDMARKER
     {
         $$ = $1;
     }
;

// (NEWLINE | stmt)
newline_or_stmt:
    NEWLINE
    {
        $$ = NULL; // newline_or_stmt NEWLINE
    }
    | stmt
    {
        $$ = $1; // newline_or_stmt stmt
    }
    ;

// a sequence of zero or more newline_stmt
// (NEWLINE | stmt)*
newline_stmt_seq:
    %empty
    {
        $$ = NULL;
    }
    | newline_stmt_seq newline_or_stmt
    {
        $$ = AstNodeSeq::Add(@$, $1, $2);
    }
;


// NEWLINE*
// If components in a rule is empty, it means that result can match the empty string.
// For example, here is how to define a comma-separated sequence of zero or more exp groupings:
// To define a sequence of zero or more NEWLINES, we need two rules:

// newline_seq:
//    %empty
//    | newline_seq NEWLINE
//;


// *python3
// stmt: simple_stmt | compound_stmt
stmt:
    simple_stmt
    {
        $$ = $1; // stmt: simple_stmt
    }
    | compound_stmt
    {
        $$ = $1; // stmt: compound_stmt
    }
    ;

// *python simple_stmt
// simple_stmt: small_stmt (';' small_stmt)* [';'] NEWLINE

simple_stmt:
    small_stmt_seq  NEWLINE
    {
        $$ = $1;  // simple_stmt: small_stmt_seq  NEWLINE
    }
    | small_stmt_seq  ";" NEWLINE
    {
        $$ = $1; // simple_stmt:  small_stmt_seq  ";" NEWLINE
    }
;


// *python3 small_stmt
// small_stmt: (expr_stmt | del_stmt | pass_stmt | flow_stmt |
//              import_stmt | global_stmt | nonlocal_stmt | assert_stmt)
small_stmt:
    expr_stmt
    | del_stmt
    | pass_stmt
    | flow_stmt
    | import_stmt
    ;

// *python
// flow_stmt: break_stmt | continue_stmt | return_stmt | raise_stmt | yield_stmt
flow_stmt: break_stmt | continue_stmt | return_stmt | raise_stmt | yield_stmt;


// *python3
break_stmt: 
    "break"
    {
        $$ = ctx.ast->CreateBreak(@$);
    }
    ;

// *python3
continue_stmt: 
    "continue"
    {
        $$ = ctx.ast->CreateContinue(@$);
    }
    ;

// *python3
// return_stmt: 'return' [testlist]
return_stmt: 
    "return"
    {
        $$ = ctx.ast->CreateReturn(@$);
    }
    | "return" testlist
    {
        $$ = ctx.ast->CreateReturn(@$, $2);
    }
    ;

// *python3
// raise_stmt: 'raise' [test ['from' test]]
raise_stmt:
    "raise"
    {
        $$ = ctx.ast->CreateRaise(@$);
    }
    | "raise" test
    {
        $$ = ctx.ast->CreateRaise(@$, $2);
    }
    | "raise" test "from" test
    {
        $$ = ctx.ast->CreateRaise(@$, $2, $4);
    }
    ;

// *python3
yield_stmt: yield_expr;





// sequence of small statments, a single small_stmt, followed by
// an optional sequence of (';' small_stmt)
// small_stmt (';' small_stmt)*
small_stmt_seq:
    small_stmt
    {
        $$ = $1; // small_stmt_seq: small_stmt
    }
    | small_stmt_seq ";" small_stmt
    {
        // // small_stmt_seq: small_stmt_seq ";" small_stmt
        $$ = ctx.ast->CreateTuple(@$, UnknownCtx, $1, $3); 
    }
;


// *python3
// del_stmt: 'del' exprlist
del_stmt:
    "del" exprlist
    {
        $$ = ctx.ast->CreateDelete(@$, $2);
    }
    ;

// *python pass_stmt
// pass_stmt: 'pass'
pass_stmt:
    "pass"
    {
        $$ = ctx.ast->CreatePass(@$);
    }
    ;

// *python3 expr_stmt
// expr_stmt: testlist_star_expr (augassign (yield_expr|testlist) |
//                      ('=' (yield_expr|testlist_star_expr))*)
// valid examples:
// a = 1
// a,b = 1,2
// a,b = c,d = 1,2
// result of expr_stmt can be either an Expr or Assign or ???
// note, there can be zero or more rhs to an assignment statement,
// in the case of zero rhs, it reduces to a single testlist_star_expr
expr_stmt:
    testlist_star_expr augassign testlist
    {
        $$ = ctx.ast->CreateAugAssign(@$, $1, $2, $3);
    }
    | assign_expr_seq
    {
        $$ = $1;  /*foo*/
    }
    | testlist_star_expr
    {
        $$ = $1; // expr_stmt: testlist_star_expr
    }
;


// assign_expr_seq: testlist_star_expr '=' (yield_expr|testlist_star_expr))*
//
// In this recursive rule, bison scans a sequence from left to right, so
// a = b = c = 1
// first scans 'a = b', this yields the (testlist_star_expr "=" testlist_star_expr) rule,
// next the ' = c' term is scanned yielding the
// (assign_expr_seq "=" testlist_star_expr) rule.
// So, in this case, the 'Assign' node is first created in the first match,
// and for each additional term that is added, the value (what is assigned to the target)
// is shifted, and the previous value is added to the target of the Assign statement. 
assign_expr_seq:
    testlist_star_expr "=" yield_expr
    {
        std::cout << "testlist_star_expr = yield_expr" << std::endl;
    }
    | testlist_star_expr "=" testlist_star_expr
    {
        std::cout << "testlist_star_expr = testlist_star_expr" << std::endl;
        $$ = ctx.ast->CreateAssign(@$, $1, $3);
    }
    | assign_expr_seq "=" yield_expr
    {
        std::cout << "assign_expr_seq = yield_expr" << std::endl;
    }
    | assign_expr_seq "=" testlist_star_expr
    {
        Assign *a = dynamic_cast<Assign*>($1);
        assert(a);
        a->AddValue($3);
        $$ = a;
        std::cout << "assign_expr_seq = testlist_star_expr" << std::endl;
    }
    ;


// ('=' (yield_expr|testlist_star_expr))*
//assign_rhs_seq:
//    %empty
//    | assign_rhs_seq "=" yeild_expr_or_testlist_star_expr
//    ;
    
// yield_expr|testlist_star_expr
// placeholder for yeild expressions
//yeild_or_testlist_star_expr:
//    yield_expr
//    | testlist_star_expr
//;

// *python3
// yield_expr: 'yield' [yield_arg]
yield_expr:
    "yield"
    {
        $$ = ctx.ast->CreateYield(@$);
    }
    | "yield" testlist
    {
        $$ = ctx.ast->CreateYield(@$, $2);
    }
    | "yield" "from" test
    {
        $$ = ctx.ast->CreateYieldFrom(@$, $3);
    }

    ;

// *python3
// yield_arg: 'from' test | testlist
//yield_arg:
//    "from" test
//    | testlist
//    ;



// *python3
// star_expr: '*' expr
// A standard expression preceeded by a star,
// see https://www.python.org/dev/peps/pep-3132/
star_expr:
    "*" expr
    {
        $$ = ctx.ast->CreateStarred(@$, $2);
    }
    ;

// *python3
// testlist_star_expr: (test|star_expr) (',' (test|star_expr))* [',']
// the first part, "(test|star_expr) (',' (test|star_expr))*" is
// the test_or_star_seq, and followed by an optional ","
//
// if the trailing sequence, is empty, and it evaluates to
// the first rule, than the result is just whatever the first
// expression was. However, if there is a trailing commaa,
// and there is only one elment in the sequence, a length 1 tuple
// is returned. 
testlist_star_expr:
//    test_star_expr test_star_expr_seq
//    | test_star_expr test_star_expr_seq ","
//    ;


    test_star_expr_seq
    {
        AstNode *a1 = $1;
        $$ = $1; // testlist_star_expr:  test_star_expr_seq
    }
    | test_star_expr_seq ","
    {
        AstNode *a1 = $1;
        $$ = $1; // testlist_star_expr: test_star_expr_seq ","
    }
    ;



// (test|star_expr)
test_star_expr:
    test | star_expr
    ;

// testlist_star_expr_seq: (test|star_expr) (',' (test|star_expr))* 
test_star_expr_seq:
    test_star_expr
    {
        // test_star_expr_seq: test_star_expr
        $$ = $1;
    }
    | test_star_expr_seq "," test_star_expr
    {
        // test_star_expr_seq: test_star_expr_seq "," test_star_expr
        // don't know at this point what context, 
        $$ = ctx.ast->CreateTuple(@$, UnknownCtx, $1, $3);
    }
    ;

// *python3
// augassign: ('+=' | '-=' | '*=' | '/=' | '%=' | '&=' | '|=' | '^=' |
//            '<<=' | '>>=' | '**=' | '//=')
augassign:
    "+=" | "-=" | "*=" | "/=" | "%=" | "&=" | "|=" | "^=" |
    "<<=" | ">>=" | "**=" | "//="
    ;


// *python3 testlist
// testlist: test (',' test)* [',']
testlist:
    test_seq 
    | test_seq ","
;

// test_seq comma_opt

// sequence of one or more test statments
test_seq:
    test
    | test_seq "," test
    {
        $$ = ctx.ast->CreateTuple(@$, $1, $3);
    }
    ;


//comma_opt:
//    %empty
//    | COMMA
//;

// *python3 test:
// test: or_test ['if' or_test 'else' test] | lambdef
test:
    or_test
    | or_test IF or_test "else" test
;

// *python3 or_test:
// or_test: and_test ('or' and_test)*
or_test:
    and_test
    | or_test "or" and_test
    {
        // or_test: or_test "or" and_test
        $$ = ctx.ast->CreateBinOp(@$, $2, $1, $3);
    }
;

// *python3 and_test: 
// and_test: not_test ('and' not_test)*
and_test:
    not_test
    | and_test "and" not_test
    {
        // and_test: and_test "and" not_test
        $$ = ctx.ast->CreateBinOp(@$, $2, $1, $3);
    }
;

// *python3 not_test: 
// not_test: 'not' not_test | comparison
not_test:
    "not" not_test
    {
        $$ = ctx.ast->CreateUnaryOp(@$, $1, $2);
    }
   |  comparison
;


// *python3 comparison: 
// comparison: expr (comp_op expr)*
comparison:
    expr
    | comparison comp_op expr
    {
        // comparison: comparison comp_op expr
        $$ = ctx.ast->CreateCompare(@$, $1, $2, $3);
    }
;


//"*python3 comp_op"
// # <> isn't actually a valid comparison operator in Python. It's here for the
// # sake of a __future__ import described in PEP 401 (which really works :-)
// comp_op: '<'|'>'|'=='|'>='|'<='|'<>'|'!='|'in'|'not' 'in'|'is'|'is' 'not'
comp_op:
    "not" "in"
    {
        $$ = TokenAstNodes::GetAstNodeForOperatorType(py::NotIn);
    }
    | "is" "not"
    {
        $$ = TokenAstNodes::GetAstNodeForOperatorType(py::IsNot);
    }
    | "<"
    | ">"
    | "=="
    | ">="
    | "<="
    | "<>"
    | "!="
    | "in"
    | "is"
;


// *python3 expr
// expr: xor_expr ('|' xor_expr)*
expr:
    xor_expr
    | expr "|" xor_expr
    {
        $$ = ctx.ast->CreateBinOp(@$, $2, $1, $3);
    }
    ;

// *python3 xor_expr
// xor_expr: and_expr ('^' and_expr)*
xor_expr:
    and_expr
    | xor_expr "^" and_expr
    {
        $$ = ctx.ast->CreateBinOp(@$, $2, $1, $3);
    }
    ;

// *python3 and_expr
// and_expr: shift_expr ('&' shift_expr)*
and_expr:
    shift_expr
    | and_expr "&" shift_expr
    {
        $$ = ctx.ast->CreateBinOp(@$, $2, $1, $3);
    }
    ;

// *python3 shift_expr
// shift_expr: arith_expr (('<<'|'>>') arith_expr)*
shift_expr:
    arith_expr
    | shift_expr shift_op arith_expr
    {
       $$ = ctx.ast->CreateBinOp(@$, $2, $1, $3);
    }
    ;

shift_op:
    "<<" | ">>"
    ;

// *python3 arith_expr
// arith_expr: term (('+'|'-') term)*
arith_expr:
    term
    | arith_expr arith_op term
    {
        $$ = ctx.ast->CreateBinOp(@$, $2, $1, $3);
    }
    ;

arith_op:
    "+" | "-"
    ;

// *python3 term
// term: factor (('*'|'/'|'%'|'//') factor)*
term:
    factor
    | term term_op factor
    {
        $$ = ctx.ast->CreateBinOp(@$, $2, $1, $3);
    }
    ;

term_op:
    "*" | "/" | "%" | "//"
    ;


// factor: ('+'|'-'|'~') factor | power
factor:
    power
    | factor_op factor
    {
        $$ = ctx.ast->CreateUnaryOp(@$, $1, $2);
    }
    ;

factor_op:
    "+" | "-" | "~"
    ;

// *python3 power:
// power: atom_expr ['**' factor]
power:
    atom_expr
    | atom_expr "**" factor
    {
        $$ = ctx.ast->CreateBinOp(@$, $2, $1, $3);
    }
    ;

// *python3 atom_expr
// atom_expr: [AWAIT] atom trailer*
atom_expr:
    atom
    | atom trailer_seq
    {
        // atom_expr: atom trailer_seq
        AstNode *trailer = $2;
        ExprSeq *seq = dynamic_cast<ExprSeq*>(trailer);
        assert(seq);
        seq->GetTerminalExpr()->SetBaseExpr($1);
        $$ = trailer;
    }
    | "await" atom
    {
        assert(0);
    }
    | "await" atom trailer_seq
    {
        assert(0);
    }
    ;


// trailer+
trailer_seq:
    trailer
    | trailer_seq trailer
    {
        // trailer_seq: trailer_seq trailer
        AstNode *trailer = $2;
        ExprSeq *seq = dynamic_cast<ExprSeq*>(trailer);
        assert(seq);
        seq->SetBaseExpr($1);
        $$ = trailer;
    }
    ;

// *python3 trailer:
// trailer: '(' [arglist] ')' | '[' subscriptlist ']' | '.' NAME
trailer:
    "." NAME
    {
        // trailer: "." NAME
        // base type is not known at this point, filled in 
        // in trailer_seq
        $$ = ctx.ast->CreateAttribute(@$, UnknownCtx, NULL, $2);
    }
    | "(" ")"
    {
        // trailer: ()
        // name is not known until atom expr
        $$ = ctx.ast->CreateCall(@$);
    }
    | "(" arglist ")"
    {
        // trailer: ( arglist )
        // name is not known until atom expr
        $$ = ctx.ast->CreateCall(@$, $2);
    }

    ;


// *python3 arglist
// arglist: argument (',' argument)*  [',']
arglist:
    arglist_seq
    | arglist_seq ","
    {
        $$ = $1;
    }
    ;


// sequence of one or more 'argument', comma separated.
// argument (',' argument)*  
arglist_seq:
   argument
   {
       $$ = ctx.ast->CreateTuple(@$, $1);
   }
   | arglist_seq "," argument
   {
       $$ = ctx.ast->CreateTuple(@$, $1, $3);
   }
   ;

// 
// # The reason that keywords are test nodes instead of NAME is that using NAME
// # results in an ambiguity. ast.c makes sure it's a NAME.
// # "test '=' test" is really "keyword '=' test", but we have no such token.
// # These need to be in a single rule to avoid grammar that is ambiguous
// # to our LL(1) parser. Even though 'test' includes '*expr' in star_expr,
// # we explicitly match '*' here, too, to give it proper precedence.
// # Illegal combinations and orderings are blocked in ast.c:
// # multiple (test comp_for) arguements are blocked; keyword unpackings
// # that precede iterable unpackings are blocked; etc.
// argument: ( test [comp_for] |
//             test '=' test |
//             '**' test |
//             star_expr )
argument:
    test
    | star_expr
    | test "=" test
    {
        $$ = ctx.ast->CreateKeywordArg(@$, $1, $3);
    }
    | "**" test
    {
        $$ = ctx.ast->CreateDblStarred(@$, $2);
    }
    ;




//comp_iter: comp_for | comp_if
//comp_for: 'for' exprlist 'in' or_test [comp_iter]
//comp_if: 'if' test_nocond [comp_iter]




// comp_iter: comp_for | comp_if

// comp_for: 'for' exprlist 'in' or_test [comp_iter]

// comp_if: 'if' old_test [comp_iter]




// *python3 atom
// atom: ('(' [yield_expr|testlist_comp] ')' |
//        '[' [testlist_comp] ']' |
//        '{' [dictorsetmaker] '}' |
//        NAME | NUMBER | STRING+ | '...' | 'None' | 'True' | 'False')



atom:
    "(" ")"
    {
        // atom: "(" ")"
        $$ = ctx.ast->CreateTuple(@$, Load);
    }

    |"(" testlist_comp ")"
    {
        // atom: |"(" testlist_comp ")"
        AstNode *testlistComp = $2;
        testlistComp->SetAtomic(true);
        $$ = testlistComp;
    }
    | NAME       { $$ = $1; /*name*/}
    | NUMBER { $$ = $1; /*num*/} 
    | STRING { $$ = $1; /*str*/}
;


// *python3
// testlist_comp: (test|star_expr) ( comp_for | (',' (test|star_expr))* [','] )
//
// compare with testlist_star_expr;
//
// testlist_star_expr: (test|star_expr) (',' (test|star_expr))* [',']
// if the comp_for is not found, and "(',' (test|star_expr))*" branch is taken,
// it would likely result in a reduce-reduce conflict and the same sequence
// of tokens could be interpreted as either rule.
//
// solution, re-write the rule by spliting out the comp_for branch as follows
//
// testlist_comp:  (test|star_expr) ( comp_for | (',' (test|star_expr))* [','] )
// testlist_comp:  (test|star_expr) (',' (test|star_expr))* [',']
//                |(test|star_expr) (comp_for)
testlist_comp:
    testlist_star_expr
    ;






// * python compound_stmt
// compound_stmt: if_stmt | while_stmt | for_stmt | try_stmt | with_stmt | funcdef
//     | classdef | decorated
compound_stmt:
    funcdef
    | decorated
    | for_stmt
    | if_stmt
;


// *python3 
// if_stmt: 'if' test ':' suite ('elif' test ':' suite)* ['else' ':' suite]

if_stmt: 
    "if" test ":" suite elif_seq
    {
        // if_stmt:     "if" test ":" suite elif_seq
        $$ = ctx.ast->CreateIf(@$, $2, $4, $5);
    }
    | "if" test ":" suite elif_seq "else" ":" suite
    {
        // if_stmt: "if" test ":" suite elif_seq "else" ":" suite
        $$ = ctx.ast->CreateIf(@$, $2, $4, $5, $8); 
    }
    ;

// ('elif' test ':' suite)*
elif_seq:
    %empty
    {
        // elif_seq: %empty
        $$ = NULL;
    }
    | elif_seq "elif" test ":" suite
    {
        // elif_seq: elif_seq "elif" test ":" suite
        $$ = ctx.ast->CreateElif(@$, $1, $3, $5);
    }
    ;

/*

while_stmt: 'while' test ':' suite ['else' ':' suite]
for_stmt: 'for' exprlist 'in' testlist ':' suite ['else' ':' suite]
try_stmt: ('try' ':' suite
           ((except_clause ':' suite)+
            ['else' ':' suite]
            ['finally' ':' suite] |
           'finally' ':' suite))
with_stmt: 'with' with_item (',' with_item)*  ':' suite
with_item: test ['as' expr]
# NB compile.c makes sure that the default except clause is last
except_clause: 'except' [test ['as' NAME]]

*/

// stmt+
stmt_seq:
    stmt
    {
        $$ = ctx.ast->CreateTuple(@$, $1);
    }
    | stmt_seq stmt
    {
        $$ = ctx.ast->CreateTuple(@$, $1, $2);
    }
    ;

// *python3
// suite: simple_stmt | NEWLINE INDENT stmt+ DEDENT
suite:
    simple_stmt
    {
        $$ = ctx.ast->CreateTuple(@$, $1);
    }
    | NEWLINE INDENT stmt_seq DEDENT
    {
        $$ = $3;
    }
    ;

// *python3
// for_stmt: 'for' exprlist 'in' testlist ':' suite ['else' ':' suite]
for_stmt:
    "for" exprlist "in" testlist ":" suite
    {
        $$ = ctx.ast->CreateFor(@$, $2, $4, $6);
    }
    | "for" exprlist "in" testlist ":" suite "else" ":" suite
    {
        $$ = ctx.ast->CreateFor(@$, $2, $4, $6, $9);
    }
    ;

// *python3
// exprlist: (expr|star_expr) (',' (expr|star_expr))* [',']
exprlist:
    exprlist_seq
    | exprlist_seq ","
    {
        // exprlist:
        $$ = $1;
    }
    ;


// exprlist_seq: (expr|star_expr) (',' (expr|star_expr))*
exprlist_seq:
    exprlist_seq "," expr
    {
        // exprlist_seq: exprlist_seq "," expr
        $$ = ctx.ast->CreateTuple(@$, $1, $3);
    }

    | exprlist_seq "," star_expr
    {
        // exprlist_seq: exprlist_seq "," star_expr
        $$ = ctx.ast->CreateTuple(@$, $1, $3);
    }
    | expr
    | star_expr
    ;
    

// *python3    
funcdef: 
    "def" NAME parameters ":" suite
    {
        // CreateFunctionDef(loc, nm, args, returns, suite)
        $$ = ctx.ast->CreateFunctionDef(@$, $2, $3, NULL, $5);
    }
    | "def" NAME parameters "->" test ":" suite
    {
        // CreateFunctionDef(loc, nm, args, returns, suite)
        $$ = ctx.ast->CreateFunctionDef(@$, $2, $3, $5, $7);
    }
    ;


// *python3
parameters: 
    "("  ")"
    {
        $$ = ctx.ast->CreateTmpArguments(@$);
    }
    | "(" typedargslist ")"
    {
        $$ = $2;
    }
    ;

//  *python3
//  typedargslist: (tfpdef ['=' test] (',' tfpdef ['=' test])* [','
//     ['*' [tfpdef] (',' tfpdef ['=' test])* [',' '**' tfpdef] | '**' tfpdef]]
//     |  '*' [tfpdef] (',' tfpdef ['=' test])* [',' '**' tfpdef] | '**' tfpdef)
//
//  re-indent and newline to line things up better
//  typedargslist:
//  (tfpdef ['=' test] (',' tfpdef ['=' test])* 
//  [',' ['*' [tfpdef] (',' tfpdef ['=' test])* [',' '**' tfpdef] | '**' tfpdef]]
//     |  '*' [tfpdef] (',' tfpdef ['=' test])* [',' '**' tfpdef] | '**' tfpdef)
//
//  the last two lines are essentially the same, these are the python list and dictionary 
//  args, define a varargs rule for them: 
//
//  varargs: '*' [tfpdef] (',' tfpdef ['=' test])* [',' '**' tfpdef] | '**' tfpdef
//
//  note the first tfpdef is optional in the grammar, I don't think it should be, 
//  leave it for now, and catch it when the AST is built. 
// 
//  with the var_args rule, can re-write the grammar as 
//  tfpdef ['=' test] (',' tfpdef ['=' test])* 
//  [',' varargs]
//     |  varargs
// 
//  and re-align the rows:
// 
//  tfpdef ['=' test] (',' tfpdef ['=' test])*  [',' var_args]
//  |  var_args
//
//  define the first part as named_args:
//  named_args: tfpdef ['=' test] (',' tfpdef ['=' test])*  


typedargslist:
    named_args
    {
        // name_args is a tuple (list of args)
        TmpArguments *args = ctx.ast->CreateTmpArguments(@$);
        args->SetArgs($1);
        $$ = args;
    }
    | named_args "," var_args
    {
        TmpArguments *args = dynamic_cast<TmpArguments*>($3);
        assert(args);
        args->SetArgs($1);
        $$ = args;
    }
    | var_args
    {
        // var_args is already an TmpArguments type
        $$ = $1;
    }
    
// can define varargs rule for list and kw args
// '*' [tfpdef] (',' tfpdef ['=' test])* [',' '**' tfpdef] | '**' tfpdef
var_args:
    "*" tfpdef var_arglist_trailer
    {
        TmpArguments *args = ctx.ast->CreateTmpArguments(@$);
        args->SetVararg($2);
        args->SetKwOnlyArgs($3);
        $$ = args;
    }
    | "*" tfpdef var_arglist_trailer "," "**" tfpdef   
    {
        TmpArguments *args = ctx.ast->CreateTmpArguments(@$);
        args->SetVararg($2);
        args->SetKwOnlyArgs($3);
        args->SetKwArg($6);
        $$ = args;
    }
    | "**" tfpdef
    {
        TmpArguments *args = ctx.ast->CreateTmpArguments(@$);
        args->SetKwArg($2);
        $$ = args;
    };
    | "*" var_arglist_trailer      
    {
        // var_args: "*" var_arglist_trailer      
        // func(*, a) is actually valid syntax, here a is a kw only arg.
        TmpArguments *args = ctx.ast->CreateTmpArguments(@$);
        args->SetKwOnlyArgs($2);
        $$ = args;
    }
    | "*" var_arglist_trailer "," "**" tfpdef
    {
        TmpArguments *args = ctx.ast->CreateTmpArguments(@$);
        args->SetKwOnlyArgs($2);
        args->SetKwArg($5);
        $$ = args;
    }
    ;


// trailer of a list of named args
// (',' tfpdef ['=' test])* 
var_arglist_trailer:
    %empty
    {
        $$ = NULL;
    }
    | var_arglist_trailer "," tfpdef_test
    {
        $$ = ctx.ast->CreateTuple(@$, UnknownCtx, $1, $3);
    }

    ;

//  define the first part as named_args:
//  named_args: tfpdef ['=' test] (',' tfpdef ['=' test])*  
//  named_args: tfpdef ['=' test] (',' tfpdef ['=' test])*  
named_args:
    tfpdef_test
    {
        $$ = ctx.ast->CreateTuple(@$, $1);
    }
    | named_args "," tfpdef_test
    {
        $$ = ctx.ast->CreateTuple(@$, $1, $3);
    }
    ;

// tfpdef ['=' test]
tfpdef_test:
    tfpdef
    {
        $$ = $1;
    }
    | tfpdef "=" test
    {
        Arg *arg = dynamic_cast<Arg*>($1);
        assert(arg);
        arg->def = $3;
        assert(arg->def);
        $$ = arg;
    }
    ;

// *python3
// tfpdef: NAME [':' test]
tfpdef: 
    NAME 
    {
        AstNode *name = $1;
        Arg* result = ctx.ast->CreateArg(@$, name);
        ctx.ast->Free(name);
        $$ = result;
    }
    | NAME ":" test
    {
        AstNode *name = $1;
        AstNode *type = $3;
        Arg* result = ctx.ast->CreateArg(@$, name, NULL, type);
        ctx.ast->Free(name);
        ctx.ast->Free(type);
        $$ = result;
    }
    ;

// *python3
//decorator: '@' dotted_name [ '(' [arglist] ')' ] NEWLINE
decorator:
    "@" dotted_name NEWLINE
    | "@" dotted_name "(" ")" NEWLINE
    | "@" dotted_name "(" arglist ")" NEWLINE
    ;

// *python3
//decorators: decorator+
decorators:
    decorator
    | decorators decorator
    ;

// *python3
//decorated: decorators (classdef | funcdef | async_funcdef)
decorated:
    decorators funcdef
    ;

// python3
// import_stmt: import_name | import_from
import_stmt: import_name | import_from

// python3
// import_name: 'import' dotted_as_names
import_name: 
    "import" dotted_as_names
    {
        $$ = ctx.ast->CreateImport(@$, $2);
    }
    ;



// *python3 import_from:
// # note below: the ('.' | '...') is necessary because '...' is tokenized as ELLIPSIS
// import_from: ('from' (('.' | '...')* dotted_name | ('.' | '...')+)
//               'import' ('*' | '(' import_as_names ')' | import_as_names))
// 
// replace dot and tridot seq
// import_from: ('from' (dot_or_tridot_seq_opt dotted_name | dot_or_tridot_seq)
//              'import' ('*' | '(' import_as_names ')' | import_as_names))
// import_from: 
//     "from" (dot_or_tridot_seq_opt dotted_name | dot_or_tridot_seq)
//             "import" ('*' | '(' import_as_names ')' | import_as_names)
import_from: 
    "from" dotted_name "import" "*"
    {
        $$ = ctx.ast->CreateImportFrom(@$, $2, $4);
    }
    |"from" dotted_name "import" "(" import_as_names ")"
    {
        $$ = ctx.ast->CreateImportFrom(@$, $2, $5);
    }
    |"from" dotted_name "import" import_as_names
    {
        $$ = ctx.ast->CreateImportFrom(@$, $2, $4);
    }
    |"from" import_dots dotted_name "import" "*"
    {
        $$ = ctx.ast->CreateImportFrom(@$, $3, $5, $2);
    }
    |"from" import_dots dotted_name "import" "(" import_as_names ")"
    {
        $$ = ctx.ast->CreateImportFrom(@$, $3, $6, $2);
    }
    |"from" import_dots dotted_name "import" import_as_names 
    {
        $$ = ctx.ast->CreateImportFrom(@$, $3, $5, $2);
    }
    |"from" import_dots "import" "*"
    {
        $$ = ctx.ast->CreateImportFrom(@$, NULL, $4, $2);
    }
    |"from" import_dots "import" "(" import_as_names ")"
    {
        $$ = ctx.ast->CreateImportFrom(@$, NULL, $5, $2);
    }
    |"from" import_dots "import" import_as_names 
    {
        $$ = ctx.ast->CreateImportFrom(@$, NULL, $4, $2);
    }
    ;

// ('.' | '...')+
// Sets the level on a ImportFrom node. 
// this is ONLY called from the import_from rule, so its OK to 
// make a new ImportFrom node here.
import_dots:
    "."
    {
        // import_dots: "."
        $$ = ctx.ast->CreateImportFrom(@$, 1);
    }
    | "..."
    {
        // import_dots: "..."
        $$ = ctx.ast->CreateImportFrom(@$, 3);
    }
    | import_dots "."
    {
        // import_dots: import_dots "."
        $$ = ctx.ast->CreateImportFrom(@$, $1, 1);
    }
    | import_dots "..."
    {
        // import_dots: import_dots "..."
        $$ = ctx.ast->CreateImportFrom(@$, $1, 3);
    }
    ;
  

// python3
// dotted_as_name: dotted_name ['as' NAME]
// dotted_as_names: dotted_as_name (',' dotted_as_name)*
dotted_as_names:
    dotted_as_names_seq
    | dotted_as_names_seq ","
    ;

// dotted_as_name (',' dotted_as_name)*
dotted_as_names_seq: 
    dotted_name
    {
        $$ = ctx.ast->CreateAliasNodes(@$, NULL, $1, NULL);
    }
    | dotted_name "as" NAME
    {
        $$ = ctx.ast->CreateAliasNodes(@$, NULL, $1, $3);
    }
    | dotted_as_names_seq "," dotted_name
    {
        $$ = ctx.ast->CreateAliasNodes(@$, $1, $3, NULL);
    }
    | dotted_as_names_seq "," dotted_name "as" NAME
    {
        $$ = ctx.ast->CreateAliasNodes(@$, $1, $3, $5);
    }
    ;




// python3
// import_as_name: NAME ['as' NAME]
// import_as_names: import_as_name (',' import_as_name)* [',']
import_as_names: 
    import_as_names_seq
    | import_as_names_seq ","
    ;

import_as_names_seq: 
    NAME 
    {
        $$ = ctx.ast->CreateAliasNodes(@$, NULL, $1, NULL);
    }
    | NAME "as" NAME
    {
        $$ = ctx.ast->CreateAliasNodes(@$, NULL, $1, $3);
    }
    | import_as_names_seq "," NAME
    {
        $$ = ctx.ast->CreateAliasNodes(@$, $1, $3, NULL);
    }
    | import_as_names_seq "," NAME "as" NAME
    {
        $$ = ctx.ast->CreateAliasNodes(@$, $1, $3, $5);
    }
    ;




// *python3
// dotted_name: NAME ('.' NAME)*
dotted_name:
    NAME
    | dotted_name "." NAME
    {
        Name *dottedName = dynamic_cast<Name*>($1);
        assert(dottedName);
        dottedName->AppendName($3);
        ctx.ast->Free($3);
        $$ = dottedName;
    }
    ;



%%


namespace py
{

void py::py_parser::error (const location_type& l,
                          const std::string& m)
{
    ctx.Error(l, m);
}


py_parser::syntax_error syntax_error(const py_parser::location_type& l,
                                     const std::string& m)
{
    return py_parser::syntax_error(l, m);
}


}

