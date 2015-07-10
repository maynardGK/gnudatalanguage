/* *************************************************************************
                          format.g  -  parser for GDL format strings
                             -------------------
    begin                : July 22 2002
    copyright            : (C) 2002 by Marc Schellens
    email                : m_schellens@hotmail.com
 ***************************************************************************/

/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

header "pre_include_cpp" {
#include "includefirst.hpp"
}

header {

#include <fstream>
#include <sstream>

#include "fmtnode.hpp"

#include "CFMTLexer.hpp"

#include "antlr/TokenStreamSelector.hpp"

//using namespace antlr;
}

options {
	language="Cpp";
	genHashLines = false;
	namespaceStd="std";         // cosmetic option to get rid of long defines
	namespaceAntlr="antlr";     // cosmetic option to get rid of long defines
}	

// the format Parser *********************************************
class FMTParser extends Parser;

options {
	importVocab = CFMT;	// use vocab generated by clexer
	exportVocab = FMT;	// use vocab generated by lexer
	buildAST = true;
  	ASTLabelType = "RefFMTNode";
	k=1;
//    defaultErrorHandler = true;
    defaultErrorHandler = false;
}

tokens {
    FORMAT;
}

{
// class extensions
}

format [ int repeat] // mark last format for format reversion 
    : LBRACE! qfq (COMMA! qfq)* RBRACE!
        {
            #format = #( [FORMAT,"FORMAT"], #format);
            #format->setRep( repeat);
        }
    ;

qfq
    : q (f q)?
    ;

q!
{
    int n1 = 0;
}
    : (SLASH { n1++;})*
        {
            if( n1 > 0) 
            {
                #q = #([SLASH,"/"]);
                #q->setRep( n1);
            }           
        }
    ;

f_csubcode // note: IDL doesn't allow hollerith strings inside C()
{
    int n1;
}
    : STRING // just write it out (H strings are handled in the scanner)
    | cstring
    | tl:TL n1=nn { #tl->setW( n1);}
    | tr:TR n1=nn { #tr->setW( n1);}
    ;

cstring
    : (s:CSTR { #s->setType( STRING);} | cformat)+
//    : (CSTR | cformat)+
    ;

// for S code this is not 100% compatible 
// (eg. %"%10.4s" puts out the '4' also in the original)
// but as this is wrong syntax anyway, we go with this implementation
cformat
{
    int w = 0;
    int d = -1;
    char f = ' ';
}
    : (w=cnnf { if (w<0) { w *= -1; f = '0'; } } (CDOT! d=cnn)?)?
        (
           c:CD {  #c->setW( w);  #c->setD( d);  #c->setType( I);  #c->setFill( f); }
        |  e:CE {  #e->setW( w);  #e->setD( d);  #e->setType( E);  #e->setFill( f); }
        |  i:CI {  #i->setW( w);  #i->setD( d);  #i->setType( I);  #i->setFill( f); }
        | ff:CF { #ff->setW( w); #ff->setD( d); #ff->setType( F); #ff->setFill( f); }
        |  g:CG {  #g->setW( w);  #g->setD( d);  #g->setType( G);  #g->setFill( f); }
        |  o:CO {  #o->setW( w);  #o->setD( d);  #o->setType( O);  #o->setFill( f); }
        |  b:CB {  #b->setW( w);  #b->setD( d);  #b->setType( B);  #b->setFill( f); }
        |  x:CX {  #x->setW( w);  #x->setD( d);  #x->setType( Z);  #x->setFill( f); }
        |  z:CZ {  #z->setW( w);  #z->setD( d);  #z->setType( Z);  #z->setFill( f); }
        |  s:CS {  #s->setW( w);  #s->setType( A);}
        )
    ;

// no nodes for cnumbers
cnn! returns[ int n]
    : num:CNUMBER 
        { 
            std::istringstream s(#num->getText());
            s >> n;
        }
    ;

// no nodes for cnumbers with zero padding
cnnf! returns[ int n]
    : num:CNUMBER 
        { 
            std::istringstream s(#num->getText());
            char c = s.get();
            s.putback(c);
            s >> n;
            if (c == '0') n *= -1;
        }
    ;

f
{
    int n1;
}
    : TERM
    | NONL
    | Q
    | t:T n1=nn { #t->setW( n1);}
    | f_csubcode
    | rep_fmt[ 1]
    | n1=nn (rep_fmt[ n1] | x:X { #x->setW( n1);})
    | xx:X { #xx->setW( 1);}
    ;

rep_fmt [ int repeat]
{
    int n1;
}
    : format[ repeat] 
    | a:A (n1=nn { #a->setW( n1);})? { #a->setRep( repeat);}
    | ff:F w_d  [ #ff] { #ff->setRep( repeat);} // F and D are the same -> D->F
    | d:D w_d  [ #d] { #d->setRep( repeat); #d->setText("f"); #d->setType(F);}
    | e:E w_d_e[ #e] { #e->setRep( repeat);}
    | g:G w_d_e[ #g] { #g->setRep( repeat);}
    | i:I w_d  [ #i] { #i->setRep( repeat);}
    | o:O w_d  [ #o] { #o->setRep( repeat);}
    | b:B w_d  [ #b] { #b->setRep( repeat);}
    | z:Z w_d  [ #z] { #z->setRep( repeat);}
    | zz:ZZ w_d  [ #zz] { #zz->setRep( repeat);}
    | c:C^ LBRACE! csub RBRACE! { #c->setRep( repeat);}
    ;   

csub
    : csubcode (COMMA! csubcode)*
    ;

csubcode
{
    int n1;
}
    : c1:CMOA (n1=nn { #c1->setW( n1);})?
    | c2:CMoA (n1=nn { #c2->setW( n1);})? 
    | c3:CmoA (n1=nn { #c3->setW( n1);})?
    | c4:CHI w_d[ #c4]
    | c5:ChI w_d[ #c5]
    | c6:CDWA (n1=nn { #c6->setW( n1);})?
    | c7:CDwA (n1=nn { #c7->setW( n1);})?
    | c8:CdwA (n1=nn { #c8->setW( n1);})?
    | c9:CAPA (n1=nn { #c9->setW( n1);})?
    | c10:CApA (n1=nn { #c10->setW( n1);})?
    | c11:CapA (n1=nn { #c11->setW( n1);})?
    | c12:CMOI w_d[ #c12]
    | c13:CDI w_d[ #c13]
    | c14:CYI w_d[ #c14]
    | c15:CMI w_d[ #c15]
    | c16:CSI w_d[ #c16]
    | c17:CSF w_d[ #c17]
    | n1=nn x:X { #x->setW( n1);}
    | f_csubcode
    ;

// no nodes for numbers
nn! returns[ int n]
    : num:NUMBER 
        { 
            std::istringstream s(#num->getText());
            s >> n;
        }
    ;

// no nodes for numbers with zero padding
nnf! [ RefFMTNode fNode] returns[ int n]
    : num:NUMBER 
        { 
            std::istringstream s(#num->getText());
            char c = s.get();
            char next = s.peek();
            s.putback(c);
            s >> n;
            if (c == '0') 
               fNode->setFill('0');
            if (c == '+') { //test if 0 is following, I.e.:+0 something
               if (next == '0') fNode->setFill('@'); else fNode->setFill('+');
            }
        }
    ;

w_d! [ RefFMTNode fNode]
{
    int n1, n2;
    fNode->setW( -1);
    fNode->setD( -1);
}
    // : (n1=nnf { if (n1<0) { n1 *= -1; fNode->setFill('0'); } fNode->setW( n1);} (DOT n2=nn { fNode->setD( n2);} )?)?
    : (n1=nnf[ fNode] { if (n1<0) { n1 = 0;} fNode->setW( n1);} (DOT n2=nn { fNode->setD( n2);} )?)?
    ;

w_d_e! [ RefFMTNode fNode]
    : (options { greedy=true;}: w_d[ fNode] (E NUMBER)?)? 
    ;

// the Format Lexer *********************************************
class FMTLexer extends Lexer;

options {
	charVocabulary = '\3'..'\377';
	caseSensitive=true ;
	testLiterals =true;
	caseSensitiveLiterals=false;
//	importVocab = CFMT;	// use vocab generated by clexer
	exportVocab = FMT;
	k=4;
    defaultErrorHandler = false;
//    defaultErrorHandler = true;
// 	analyzerDebug=true;
}

// the reserved words
tokens {
	TL="tl";
	TR="tr";
}
{
    private:
    antlr::TokenStreamSelector*  selector; 
    CFMTLexer*            cLexer;

    public:
    void SetSelector( antlr::TokenStreamSelector& s)
    {
        selector = &s;
    }
    void SetCLexer( CFMTLexer& l)
    {
        cLexer = &l;
    }
}

STRING
// 	: '\"'! (~('\"'|'\r'|'\n')| '\"' '\"'! )* '\"'!
// 	| '\''! (~('\''|'\r'|'\n')| '\'' '\''! )* '\''!
	: '\"'! (~('\"')| '\"' '\"'! )* '\"'!
	| '\''! (~('\'')| '\'' '\''! )* '\''!
	;	

CSTRING!
	: '%' '\"' 
        { cLexer->DoubleQuotes( true); selector->push( cLexer); selector->retry();}
    | '%' '\'' 
        { cLexer->DoubleQuotes( false); selector->push( cLexer); selector->retry();}
	;	


LBRACE: '(';
RBRACE: ')';

SLASH: '/';

COMMA: ',';

A:('A'|'a');
TERM:':';
NONL:'$';
F:('f'|'F');
D:('d'|'D');
E:('e'|'E');
G:('g'|'G');

I:('i'|'I');
O:('o'|'O');
B:('b'|'B');
Z:('Z');
ZZ:('z'); // lower case output

Q:('q'|'Q');

H:('h'|'H');

T:('t'|'T');

L:('l'|'L');
R:('r'|'R');

X:('x'|'X');

C:('c'|'C');

CMOA: ( 'C' 'M' 'O' 'A');
CMoA: ( 'C' 'M' 'o' 'A');
CmoA: ( 'C' 'm' 'o' 'A');
CMOI: ( 'C' 'M' 'O' 'I');

CDI: ( 'C' 'D' 'I');
CMI: ( 'C' 'M' 'I');
CYI: ( 'C' 'Y' 'I');
CSI: ( 'C' 'S' 'I');
CSF: ( 'C' 'S' 'F');
CHI: ( 'C' 'h' 'I');
ChI: ( 'C' 'H' 'I');

CDWA: ( 'C' 'D' 'W' 'A');
CDwA: ( 'C' 'D' 'w' 'A');
CdwA: ( 'C' 'd' 'w' 'A');

CAPA: ( 'C' 'A' 'P' 'A');
CApA: ( 'C' 'A' 'p' 'A');
CapA: ( 'C' 'a' 'p' 'A');

PERCENT:'%';

DOT:'.';

protected     
W
//	: ( '\003'..'\010' | '\t' | '\r' | '\013' | '\f' | '\016'.. '\037' | ' ' )
	: (' '| '\t') 
	;

WHITESPACE
    : (W)+
        { _ttype=antlr::Token::SKIP; }
    ;

protected
DIGITS
	: ('0'..'9')+
	;

// NUMBER // handles hollerith strings also
// 	: DIGITS
//         (H! 
//             { 
//                 string text=$getText;
//                 int len=atoi(text.c_str());
//                 $setText("");
//                     for( int i=0; i<len; i++)
//                 {
//                     consume(); // appends char
//                 }   
//                 $setType(STRING);
//             }   
//         )?
//     ;

protected
CHAR: ('\003'..'\377');

NUMBER // handles hollerith strings also
{ 
    SizeT n;
    SizeT i = 0;
    bool uPlus = false;
    bool uMinus = false;
} 
    : 
      ('-' 
       {
       uMinus = true;
       }
      |'+' 
       {
       uPlus = true;
       }
      )? 
      num:DIGITS
      {
      if( uMinus) num->setText( "-" + num->getText());
      if( uPlus) num->setText( "+" + num->getText());
      }
        (   
            { 
                $setType(STRING); 
                std::istringstream s(num->getText());
                s >> n;
                $setText(""); // clear string (remove number)
            }
            'H'! 
            (
                { // init action gets executed even in guessing mode
                    if( i == n )
                    break;
                    i++; // count chars here so that guessing mode works
                }: // ":" makes it an init action
                CHAR
            )+
        )?
    ;