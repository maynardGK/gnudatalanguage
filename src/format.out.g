/* *************************************************************************
                          format.out.g  -  interpreter for formatted output
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

header "post_include_cpp" {
    // gets inserted after the antlr generated includes in the cpp file
}

header {
#include <fstream>

//***test
#include "print_tree.hpp"

#include "envt.hpp"

#include "fmtnode.hpp"
//#include "getfmtast.hpp"    // for FMTNodeFactory;    

#include "gdljournal.hpp"
}

options {
	language="Cpp";
//	genHashLines = true;
	genHashLines = false;
	namespaceStd="std";         // cosmetic option to get rid of long defines
	namespaceAntlr="antlr";     // cosmetic option to get rid of long defines
}	

// the format Parser *********************************************
class FMTOut extends TreeParser;

options {
	importVocab = FMT;	// use vocab generated by format lexer
	buildAST = false;
  	ASTLabelType = "RefFMTNode";
    defaultErrorHandler = false;
//    defaultErrorHandler = true;
//    codeGenBitsetTestThreshold=999;
//    codeGenMakeSwitchThreshold=1;
}

{
public:
    FMTOut( RefFMTNode fmt, std::ostream* os_, EnvT* e_, int parOffset)
    : antlr::TreeParser(), os(os_), e( e_), nextParIx( parOffset),
	valIx(0), termFlag(false), nonlFlag(false), nElements(0)
    {
        nParam = e->NParam();

        NextPar();
    
        GDLStream* j = lib::get_journal();

        if( j != NULL && j->OStream() == (*os)) 
            (*os) << lib::JOURNALCOMMENT;

        format( fmt);
        
        SizeT nextParIxComp = nextParIx;
        SizeT valIxComp = valIx;

        // format reversion
        while( actPar != NULL)
        {
            (*os) << '\n';
            
            if( j != NULL && j->OStream() == (*os)) 
                (*os) << lib::JOURNALCOMMENT;

            format_reversion( reversionAnker);
            
            if( (nextParIx == nextParIxComp) && (valIx == valIxComp))   
                throw GDLException("Infinite format loop detected.");
        }
        
        if( !nonlFlag) (*os) << '\n';

        (*os) << std::flush;
    }
    
private:
    void NextPar()
    {
        valIx = 0;
        if( nextParIx < nParam)
        {
            actPar = e->GetPar( nextParIx);
            if( actPar != NULL)
            nElements = actPar->ToTransfer();
            else
            nElements = 0;
        } 
        else 
        {
            actPar = NULL;
            nElements = 0;
        }
        nextParIx++;
    }

    void NextVal( SizeT n=1)
    {
        valIx += n;
        if( valIx >= nElements)
            NextPar();
    }
    
    std::ostream* os;
    EnvT*    e;
    SizeT   nextParIx;
    SizeT   valIx;

    bool termFlag;
    bool nonlFlag;

    SizeT   nParam;
    BaseGDL* actPar;
    SizeT nElements;

    RefFMTNode reversionAnker;
}

format
    : #(fmt:FORMAT 
            { goto realCode; } // fool ANTLR 
            q (f q)+ // this gets never executed
            {
                realCode:

                reversionAnker = #fmt;
                
                RefFMTNode blk = _t; // q (f q)+

                for( int r = #fmt->getRep(); r > 0; r--)
                {
                    q( blk);
                    _t = _retTree;

                    for (;;) 
                    {
                        if( _t == static_cast<RefFMTNode>(antlr::nullAST))
                            _t = ASTNULL;

                        switch ( _t->getType()) {
                        case FORMAT:
                        case STRING:
                        case CSTRING:
                        case TL:
                        case TR:
                        case TERM:
                        case NONL:
                        case Q: case T: case X: case A:
                        case F: case D: case E: case G:
                        case I: case O: case Z: case C:
                            {
                                f(_t);
                                if( actPar == NULL && termFlag) goto endFMT;
                                _t = _retTree;
                                q(_t);
                                _t = _retTree;
                                break; // out of switch
                            }
                        default:
                            goto endFMT;
                        }
                    }
                    
                    endFMT: // end of one repetition
                    if( actPar == NULL && termFlag) break;
                }
            }
        )
    ;

format_reversion
    : format 
        { goto realCode;}
        q (f q)*
        {
            realCode:
            
            q( _t);
            _t = _retTree;
            
            for (;;) 
            {
                if( _t == static_cast<RefFMTNode>(antlr::nullAST))
                _t = ASTNULL;
                
                switch ( _t->getType()) {
                case FORMAT:
                case STRING:
                case CSTRING:
                case TL:
                case TR:
                case TERM:
                case NONL:
                case Q: case T: case X: case A:
                case F: case D: case E: case G:
                case I: case O: case Z: case C:
                    {
                        f(_t);
                        if( actPar == NULL) goto endFMT;
                        _t = _retTree;
                        q(_t);
                        _t = _retTree;
                        break; // out of switch
                    }
                default:
                    goto endFMT;
                }
            }
            endFMT: // end of one repetition
        }
    ;

q
    : (s:SLASH 
            {
                // only one newline to journal file
                GDLStream* j = lib::get_journal();
                if( j != NULL && j->OStream() == (*os))
                    (*os) << '\n' << lib::JOURNALCOMMENT;
                else
                    for( int r=s->getRep(); r > 0; r--) (*os) << '\n';
            }
        )?
    ;

f_csubcode // note: IDL doesn't allow hollerith strings inside C()
    : s:STRING { (*os) << s->getText(); }
//    | CSTRING // *** requires special handling
    | tl:TL 
        { 
            SizeT actP  = os->tellp(); 
            int    tlVal = tl->getW();
            if( tlVal > actP)
                os->seekp( 0);
            else
                os->seekp( actP - tlVal);
        }
    | tr:TR 
        { 
            int    tlVal = tl->getW();
            os->seekp( tlVal, std::ios_base::cur);
        }
    ;

f
    : TERM { termFlag = true; }
    | NONL { nonlFlag = true; }
    | Q // ignored on output
    | t:T
        { 
            int    tVal = t->getW();
            os->seekp( tVal, std::ios_base::beg);
        }
    | f_csubcode
    | x
    | format // following are repeatable formats
    | a:A 
        {
            if( actPar == NULL) break;

            int r = a->getRep();
            int w = a->getW();
            do {
                SizeT tCount = actPar->OFmtA( os, valIx, r, w);
                r -= tCount;
                NextVal( tCount);
                if( actPar == NULL) break;
            } while( r>0);
        }
    | f:F
        {
            if( actPar == NULL) break;
            
            int r = f->getRep();
            int w = f->getW();
            int d = f->getD();
            do {
                SizeT tCount = actPar->OFmtF( os, valIx, r, w, d);
                r -= tCount;
                NextVal( tCount);
                if( actPar == NULL) break;
            } while( r>0);
        }
//  | d:D // D is transformed to F
    | e:E
        {
            if( actPar == NULL) break;
            
            int r = e->getRep();
            int w = e->getW();
            int d = e->getD();
            do {
                SizeT tCount = actPar->OFmtF( os, valIx, r, w, d, 
                                               BaseGDL::SCIENTIFIC);
                r -= tCount;
                NextVal( tCount);
                if( actPar == NULL) break;
            } while( r>0);
        }
    | g:G
        {
            if( actPar == NULL) break;
            
            int r = g->getRep();
            int w = g->getW();
            int d = g->getD();
            do {
                SizeT tCount = actPar->OFmtF( os, valIx, r, w, d, 
                                               BaseGDL::AUTO);
                r -= tCount;
                NextVal( tCount);
                if( actPar == NULL) break;
            } while( r>0);
        }
    | i:I
        {
            if( actPar == NULL) break;
            
            int r = i->getRep();
            int w = i->getW();
            int d = i->getD();
            do {
                SizeT tCount = actPar->OFmtI( os, valIx, r, w, d, 
                                               BaseGDL::DEC);
                r -= tCount;
                NextVal( tCount);
                if( actPar == NULL) break;
            } while( r>0);
        }
    | o:O
        {
            if( actPar == NULL) break;
            
            int r = o->getRep();
            int w = o->getW();
            int d = o->getD();
            do {
                SizeT tCount = actPar->OFmtI( os, valIx, r, w, d, 
                                               BaseGDL::OCT);
                r -= tCount;
                NextVal( tCount);
                if( actPar == NULL) break;
            } while( r>0);
        }
    | z:Z
        {
            if( actPar == NULL) break;
            
            int r = z->getRep();
            int w = z->getW();
            int d = z->getD();
            do {
                SizeT tCount = actPar->OFmtI( os, valIx, r, w, d, 
                                               BaseGDL::HEX);
                r -= tCount;
                NextVal( tCount);
                if( actPar == NULL) break;
            } while( r>0);
        }
    | zz:ZZ
        {
            if( actPar == NULL) break;
            
            int r = zz->getRep();
            int w = zz->getW();
            int d = zz->getD();
            do {
                SizeT tCount = actPar->OFmtI( os, valIx, r, w, d, 
                                               BaseGDL::HEXL);
                r -= tCount;
                NextVal( tCount);
                if( actPar == NULL) break;
            } while( r>0);
        }
    | #(c:C (csubcode)+) 
    ;   

csubcode
    : c1:CMOA
    | c2:CMoA
    | c3:CmoA
    | c4:CHI
    | c5:ChI
    | c6:CDWA
    | c7:CDwA
    | c8:CdwA
    | c9:CAPA
    | c10:CApA
    | c11:CapA
    | c12:CMOI
    | c13:CDI 
    | c14:CYI
    | c15:CMI
    | c16:CSI
    | c17:CSF
    | x
    | f_csubcode
    ;

x
    : tl:X 
        {
            if( _t != static_cast<RefFMTNode>(antlr::nullAST))
            {
                int    tlVal = #tl->getW();
                os->seekp( tlVal, std::ios_base::cur);
            }
            // for( int r=x->getW(); r > 0; r--) (*os) << ' ';
        }
    ;
