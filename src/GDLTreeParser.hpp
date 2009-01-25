#ifndef INC_GDLTreeParser_hpp_
#define INC_GDLTreeParser_hpp_

#include <antlr/config.hpp>
#include "GDLTreeParserTokenTypes.hpp"
/* $ANTLR 2.7.6 (20071205): "gdlc.tree.g" -> "GDLTreeParser.hpp"$ */
#include <antlr/TreeParser.hpp>


#include "objects.hpp"
#include "dcompiler.hpp"
#include "dnodefactory.hpp"
#include "arrayindexlistt.hpp"

class CUSTOM_API GDLTreeParser : public antlr::TreeParser, public GDLTreeParserTokenTypes
{

    private:
    DCompiler       comp; // each tree parser has its own compiler
    
    IDList          loopVarStack;
    
//     // called after structure is fixed
//     ArrayIndexListT* MakeArrayIndex( ArrayIndexVectorT* ixList)
//     {
//         assert( ixList->size() != 0); // must be, from compiler
        
//         if( ixList->size() == 1)
//         {
//             if( dynamic_cast< CArrayIndexScalar*>((*ixList)[0]))
//                 return new ArrayIndexListOneConstScalarT( ixList);

//             if( dynamic_cast< ArrayIndexScalar*>((*ixList)[0]))
//                 return new ArrayIndexListOneScalarT( ixList);

//             if( dynamic_cast< ArrayIndexScalarVP*>((*ixList)[0]))
//                 return new ArrayIndexListOneScalarVPT( ixList);

//             return new ArrayIndexListOneT( ixList);
//         }

//         SizeT nScalar  = 0;
//         for( SizeT i=0; i<ixList->size(); ++i)
//         {
//             if( dynamic_cast< ArrayIndexScalar*>((*ixList)[i]) ||
//                 dynamic_cast< ArrayIndexScalarVP*>((*ixList)[i]) ||
//                 dynamic_cast< CArrayIndexScalar*>((*ixList)[i])) ++nScalar;
//         }
//         if( nScalar == ixList->size())
//         return new ArrayIndexListScalarT( ixList);
        
//         return new ArrayIndexListMultiT( ixList);
//     }

    bool LoopVar( RefDNode& lN)
    {
        int lT = lN->getType();
        if( lT == VAR || lT == VARPTR)
        return (FindInIDList( loopVarStack, lN->getText()) != -1);
        return false;
    }
    
    // Replaces ASSIGN with ASSIGN_REPLACE if appropiate
    void AssignReplace( RefDNode& lN, RefDNode& aN)
    {
        if( LoopVar( lN))
        Warning( "Warning: Assignment to FOR loop variable detected.");
        
        int lT = lN->getType();
        if( lT == FCALL || lT == MFCALL || lT == MFCALL_PARENT ||
            lT == FCALL_LIB || 
            lT == FCALL_LIB_RETNEW || 
            lT == MFCALL_LIB || 
            lT == MFCALL_LIB_RETNEW || 
            lT == MFCALL_PARENT_LIB ||
            lT == MFCALL_PARENT_LIB_RETNEW ||
            lT == DEREF || lT == VAR || lT == VARPTR)
        {
            aN->setType( ASSIGN_REPLACE);
            aN->setText( "r=");
        }
    }
    
    bool SelfAssignment( RefDNode& lN, RefDNode& rN)
    {
        int lT = lN->getType();
        int rT = rN->getType();
        if( 
            rT == VARPTR && lT == VARPTR)
        {
            return  (lN->GetVar() == rN->GetVar());
        }
        if( 
            rT == VAR && lT == VAR)
        {
            return  (lN->GetVarIx() == rN->GetVarIx());
        }
        return false;
        
    }

  RefDNode RemoveNextSibling( RefDNode l)
  {
    RefDNode newNode = RefDNode(astFactory->dupTree( antlr::RefAST(l)));
    newNode->SetLine( l->getLine());
    return newNode;
//    newNode->setNextSibling( RefDNode(antlr::nullAST));
  }
  public:
  // constructor with processed file
  GDLTreeParser(const std::string& f, const std::string& sub)
    : antlr::TreeParser(), comp(f, NULL, sub)
    {
        //       setTokenNames(_tokenNames);
        //       setASTNodeFactory( DNode::factory );
        initializeASTFactory( DNodeFactory);
        setASTFactory( &DNodeFactory );
    }
  // constructor for command line/execute
  GDLTreeParser( EnvBaseT* env)
    : antlr::TreeParser(), comp( "", env, "")
    {
        initializeASTFactory( DNodeFactory);
        setASTFactory( &DNodeFactory );
    }

  bool ActiveProCompiled() const { return comp.ActiveProCompiled();} 
public:
	GDLTreeParser();
	static void initializeASTFactory( antlr::ASTFactory& factory );
	int getNumTokens() const
	{
		return GDLTreeParser::NUM_TOKENS;
	}
	const char* getTokenName( int type ) const
	{
		if( type > getNumTokens() ) return 0;
		return GDLTreeParser::tokenNames[type];
	}
	const char* const* getTokenNames() const
	{
		return GDLTreeParser::tokenNames;
	}
	public: void translation_unit(RefDNode _t);
	public: void procedure_def(RefDNode _t);
	public: void function_def(RefDNode _t);
	public: void forward_function(RefDNode _t);
	public: void common_block(RefDNode _t);
	public: void statement_list(RefDNode _t);
	public: void interactive(RefDNode _t);
	public: void statement(RefDNode _t);
	public: void parameter_declaration(RefDNode _t);
	public: void keyword_declaration(RefDNode _t);
	public: void caseswitch_body(RefDNode _t);
	public: void expr(RefDNode _t);
	public: void switch_statement(RefDNode _t);
	public: void case_statement(RefDNode _t);
	public: void block(RefDNode _t);
	public: void unblock(RefDNode _t);
	public: void label(RefDNode _t);
	public: void assign_expr_statement(RefDNode _t);
	public: void comp_assign_expr(RefDNode _t);
	public: void procedure_call(RefDNode _t);
	public: void for_statement(RefDNode _t);
	public: void repeat_statement(RefDNode _t);
	public: void while_statement(RefDNode _t);
	public: void jump_statement(RefDNode _t);
	public: void if_statement(RefDNode _t);
	public: void parameter_def(RefDNode _t);
	public: void key_parameter(RefDNode _t);
	public: void pos_parameter(RefDNode _t);
	public: int  array_def(RefDNode _t);
	public: void struct_def(RefDNode _t);
	public: void tag_def(RefDNode _t);
	public: void arrayindex(RefDNode _t,
		ArrayIndexVectorT* ixList
	);
	public: void arrayindex_list(RefDNode _t);
	public: void lassign_expr(RefDNode _t);
	public: void assign_expr(RefDNode _t);
	public: void sysvar(RefDNode _t);
	public: void var(RefDNode _t);
	public: void arrayindex_list_to_expression_list(RefDNode _t);
	public: void arrayexpr_fn(RefDNode _t);
	public: void primary_expr(RefDNode _t);
	public: void op_expr(RefDNode _t);
	public: void uminus(RefDNode _t);
	public: void brace_expr(RefDNode _t);
	public: void unbrace_expr(RefDNode _t);
	public: void indexable_expr(RefDNode _t);
	public: void array_expr(RefDNode _t);
	public: void tag_indexable_expr(RefDNode _t);
	public: void tag_array_expr_1st(RefDNode _t);
	public: void tag_expr(RefDNode _t);
	public: void tag_array_expr(RefDNode _t);
public:
	antlr::RefAST getAST()
	{
		return antlr::RefAST(returnAST);
	}
	
protected:
	RefDNode returnAST;
	RefDNode _retTree;
private:
	static const char* tokenNames[];
#ifndef NO_STATIC_CONSTS
	static const int NUM_TOKENS = 205;
#else
	enum {
		NUM_TOKENS = 205
	};
#endif
	
	static const unsigned long _tokenSet_0_data_[];
	static const antlr::BitSet _tokenSet_0;
	static const unsigned long _tokenSet_1_data_[];
	static const antlr::BitSet _tokenSet_1;
	static const unsigned long _tokenSet_2_data_[];
	static const antlr::BitSet _tokenSet_2;
};

#endif /*INC_GDLTreeParser_hpp_*/
