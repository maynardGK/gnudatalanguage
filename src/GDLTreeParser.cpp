/* $ANTLR 2.7.4: "gdlc.tree.g" -> "GDLTreeParser.cpp"$ */
#include "GDLTreeParser.hpp"
#include <antlr/Token.hpp>
#include <antlr/AST.hpp>
#include <antlr/NoViableAltException.hpp>
#include <antlr/MismatchedTokenException.hpp>
#include <antlr/SemanticException.hpp>
#include <antlr/BitSet.hpp>
GDLTreeParser::GDLTreeParser()
	: antlr::TreeParser() {
}

void GDLTreeParser::translation_unit(RefDNode _t) {
	RefDNode translation_unit_AST_in = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	returnAST = RefDNode(antlr::nullAST);
	antlr::ASTPair currentAST;
	RefDNode translation_unit_AST = RefDNode(antlr::nullAST);
	
	try {      // for error handling
		{ // ( ... )*
		for (;;) {
			if (_t == RefDNode(antlr::nullAST) )
				_t = ASTNULL;
			switch ( _t->getType()) {
			case PRO:
			{
				procedure_def(_t);
				_t = _retTree;
				astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
				break;
			}
			case FUNCTION:
			{
				function_def(_t);
				_t = _retTree;
				astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
				break;
			}
			case FORWARD:
			{
				forward_function(_t);
				_t = _retTree;
				astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
				break;
			}
			default:
			{
				goto _loop3;
			}
			}
		}
		_loop3:;
		} // ( ... )*
		translation_unit_AST = RefDNode(currentAST.root);
	}
	catch ( GDLException& e) {
		
		throw;
		
	}
	catch ( antlr::NoViableAltException& e) {
		
		// SYNTAX ERROR
		throw GDLException( e.getLine(), e.getColumn(), "Compiler syntax error: "+e.getMessage());
		
	}
	catch ( antlr::RecognitionException& e) {
		
		// SYNTAX ERROR
		throw GDLException( e.getLine(), e.getColumn(), "General syntax error: "+e.getMessage());
		
	}
	returnAST = translation_unit_AST;
	_retTree = _t;
}

void GDLTreeParser::procedure_def(RefDNode _t) {
	RefDNode procedure_def_AST_in = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	returnAST = RefDNode(antlr::nullAST);
	antlr::ASTPair currentAST;
	RefDNode procedure_def_AST = RefDNode(antlr::nullAST);
	RefDNode name = RefDNode(antlr::nullAST);
	RefDNode name_AST = RefDNode(antlr::nullAST);
	RefDNode obj = RefDNode(antlr::nullAST);
	RefDNode obj_AST = RefDNode(antlr::nullAST);
	
	RefDNode __t18 = _t;
	RefDNode tmp1_AST = RefDNode(antlr::nullAST);
	RefDNode tmp1_AST_in = RefDNode(antlr::nullAST);
	tmp1_AST = astFactory->create(antlr::RefAST(_t));
	tmp1_AST_in = _t;
	antlr::ASTPair __currentAST18 = currentAST;
	currentAST.root = currentAST.child;
	currentAST.child = RefDNode(antlr::nullAST);
	match(antlr::RefAST(_t),PRO);
	_t = _t->getFirstChild();
	name = _t;
	RefDNode name_AST_in = RefDNode(antlr::nullAST);
	name_AST = astFactory->create(antlr::RefAST(name));
	match(antlr::RefAST(_t),IDENTIFIER);
	_t = _t->getNextSibling();
	{
	if (_t == RefDNode(antlr::nullAST) )
		_t = ASTNULL;
	switch ( _t->getType()) {
	case METHOD:
	{
		RefDNode tmp2_AST = RefDNode(antlr::nullAST);
		RefDNode tmp2_AST_in = RefDNode(antlr::nullAST);
		tmp2_AST = astFactory->create(antlr::RefAST(_t));
		tmp2_AST_in = _t;
		match(antlr::RefAST(_t),METHOD);
		_t = _t->getNextSibling();
		obj = _t;
		RefDNode obj_AST_in = RefDNode(antlr::nullAST);
		obj_AST = astFactory->create(antlr::RefAST(obj));
		match(antlr::RefAST(_t),IDENTIFIER);
		_t = _t->getNextSibling();
		
		comp.StartPro(name->getText(),obj->getText());
		
		break;
	}
	case 3:
	case ASSIGN:
	case BLOCK:
	case COMMONDECL:
	case COMMONDEF:
	case MPCALL:
	case MPCALL_PARENT:
	case PCALL:
	case PARADECL:
	case RETURN:
	case IDENTIFIER:
	case SWITCH:
	case CASE:
	case FORWARD:
	case DEC:
	case INC:
	case AND_OP_EQ:
	case ASTERIX_EQ:
	case EQ_OP_EQ:
	case GE_OP_EQ:
	case GTMARK_EQ:
	case GT_OP_EQ:
	case LE_OP_EQ:
	case LTMARK_EQ:
	case LT_OP_EQ:
	case MATRIX_OP1_EQ:
	case MATRIX_OP2_EQ:
	case MINUS_EQ:
	case MOD_OP_EQ:
	case NE_OP_EQ:
	case OR_OP_EQ:
	case PLUS_EQ:
	case POW_EQ:
	case SLASH_EQ:
	case XOR_OP_EQ:
	case BREAK:
	case CONTINUE:
	case REPEAT:
	case WHILE:
	case FOR:
	case GOTO:
	case ON_IOERROR:
	case IF:
	{
		
		comp.StartPro(name->getText());
		
		break;
	}
	default:
	{
		throw antlr::NoViableAltException(antlr::RefAST(_t));
	}
	}
	}
	{
	if (_t == RefDNode(antlr::nullAST) )
		_t = ASTNULL;
	switch ( _t->getType()) {
	case PARADECL:
	{
		parameter_declaration(_t);
		_t = _retTree;
		break;
	}
	case 3:
	case ASSIGN:
	case BLOCK:
	case COMMONDECL:
	case COMMONDEF:
	case MPCALL:
	case MPCALL_PARENT:
	case PCALL:
	case RETURN:
	case IDENTIFIER:
	case SWITCH:
	case CASE:
	case FORWARD:
	case DEC:
	case INC:
	case AND_OP_EQ:
	case ASTERIX_EQ:
	case EQ_OP_EQ:
	case GE_OP_EQ:
	case GTMARK_EQ:
	case GT_OP_EQ:
	case LE_OP_EQ:
	case LTMARK_EQ:
	case LT_OP_EQ:
	case MATRIX_OP1_EQ:
	case MATRIX_OP2_EQ:
	case MINUS_EQ:
	case MOD_OP_EQ:
	case NE_OP_EQ:
	case OR_OP_EQ:
	case PLUS_EQ:
	case POW_EQ:
	case SLASH_EQ:
	case XOR_OP_EQ:
	case BREAK:
	case CONTINUE:
	case REPEAT:
	case WHILE:
	case FOR:
	case GOTO:
	case ON_IOERROR:
	case IF:
	{
		break;
	}
	default:
	{
		throw antlr::NoViableAltException(antlr::RefAST(_t));
	}
	}
	}
	{
	if (_t == RefDNode(antlr::nullAST) )
		_t = ASTNULL;
	switch ( _t->getType()) {
	case ASSIGN:
	case BLOCK:
	case COMMONDECL:
	case COMMONDEF:
	case MPCALL:
	case MPCALL_PARENT:
	case PCALL:
	case RETURN:
	case IDENTIFIER:
	case SWITCH:
	case CASE:
	case FORWARD:
	case DEC:
	case INC:
	case AND_OP_EQ:
	case ASTERIX_EQ:
	case EQ_OP_EQ:
	case GE_OP_EQ:
	case GTMARK_EQ:
	case GT_OP_EQ:
	case LE_OP_EQ:
	case LTMARK_EQ:
	case LT_OP_EQ:
	case MATRIX_OP1_EQ:
	case MATRIX_OP2_EQ:
	case MINUS_EQ:
	case MOD_OP_EQ:
	case NE_OP_EQ:
	case OR_OP_EQ:
	case PLUS_EQ:
	case POW_EQ:
	case SLASH_EQ:
	case XOR_OP_EQ:
	case BREAK:
	case CONTINUE:
	case REPEAT:
	case WHILE:
	case FOR:
	case GOTO:
	case ON_IOERROR:
	case IF:
	{
		statement_list(_t);
		_t = _retTree;
		
		comp.SetTree( returnAST);
		
		break;
	}
	case 3:
	{
		break;
	}
	default:
	{
		throw antlr::NoViableAltException(antlr::RefAST(_t));
	}
	}
	}
	
	comp.EndPro();
	
	currentAST = __currentAST18;
	_t = __t18;
	_t = _t->getNextSibling();
	returnAST = procedure_def_AST;
	_retTree = _t;
}

void GDLTreeParser::function_def(RefDNode _t) {
	RefDNode function_def_AST_in = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	returnAST = RefDNode(antlr::nullAST);
	antlr::ASTPair currentAST;
	RefDNode function_def_AST = RefDNode(antlr::nullAST);
	RefDNode name = RefDNode(antlr::nullAST);
	RefDNode name_AST = RefDNode(antlr::nullAST);
	RefDNode obj = RefDNode(antlr::nullAST);
	RefDNode obj_AST = RefDNode(antlr::nullAST);
	
	RefDNode __t23 = _t;
	RefDNode tmp3_AST = RefDNode(antlr::nullAST);
	RefDNode tmp3_AST_in = RefDNode(antlr::nullAST);
	tmp3_AST = astFactory->create(antlr::RefAST(_t));
	tmp3_AST_in = _t;
	antlr::ASTPair __currentAST23 = currentAST;
	currentAST.root = currentAST.child;
	currentAST.child = RefDNode(antlr::nullAST);
	match(antlr::RefAST(_t),FUNCTION);
	_t = _t->getFirstChild();
	name = _t;
	RefDNode name_AST_in = RefDNode(antlr::nullAST);
	name_AST = astFactory->create(antlr::RefAST(name));
	match(antlr::RefAST(_t),IDENTIFIER);
	_t = _t->getNextSibling();
	{
	if (_t == RefDNode(antlr::nullAST) )
		_t = ASTNULL;
	switch ( _t->getType()) {
	case METHOD:
	{
		RefDNode tmp4_AST = RefDNode(antlr::nullAST);
		RefDNode tmp4_AST_in = RefDNode(antlr::nullAST);
		tmp4_AST = astFactory->create(antlr::RefAST(_t));
		tmp4_AST_in = _t;
		match(antlr::RefAST(_t),METHOD);
		_t = _t->getNextSibling();
		obj = _t;
		RefDNode obj_AST_in = RefDNode(antlr::nullAST);
		obj_AST = astFactory->create(antlr::RefAST(obj));
		match(antlr::RefAST(_t),IDENTIFIER);
		_t = _t->getNextSibling();
		
		comp.StartFun(name->getText(),obj->getText());
		
		break;
	}
	case 3:
	case ASSIGN:
	case BLOCK:
	case COMMONDECL:
	case COMMONDEF:
	case MPCALL:
	case MPCALL_PARENT:
	case PCALL:
	case PARADECL:
	case RETURN:
	case IDENTIFIER:
	case SWITCH:
	case CASE:
	case FORWARD:
	case DEC:
	case INC:
	case AND_OP_EQ:
	case ASTERIX_EQ:
	case EQ_OP_EQ:
	case GE_OP_EQ:
	case GTMARK_EQ:
	case GT_OP_EQ:
	case LE_OP_EQ:
	case LTMARK_EQ:
	case LT_OP_EQ:
	case MATRIX_OP1_EQ:
	case MATRIX_OP2_EQ:
	case MINUS_EQ:
	case MOD_OP_EQ:
	case NE_OP_EQ:
	case OR_OP_EQ:
	case PLUS_EQ:
	case POW_EQ:
	case SLASH_EQ:
	case XOR_OP_EQ:
	case BREAK:
	case CONTINUE:
	case REPEAT:
	case WHILE:
	case FOR:
	case GOTO:
	case ON_IOERROR:
	case IF:
	{
		
		comp.StartFun(name->getText());
		
		break;
	}
	default:
	{
		throw antlr::NoViableAltException(antlr::RefAST(_t));
	}
	}
	}
	{
	if (_t == RefDNode(antlr::nullAST) )
		_t = ASTNULL;
	switch ( _t->getType()) {
	case PARADECL:
	{
		parameter_declaration(_t);
		_t = _retTree;
		break;
	}
	case 3:
	case ASSIGN:
	case BLOCK:
	case COMMONDECL:
	case COMMONDEF:
	case MPCALL:
	case MPCALL_PARENT:
	case PCALL:
	case RETURN:
	case IDENTIFIER:
	case SWITCH:
	case CASE:
	case FORWARD:
	case DEC:
	case INC:
	case AND_OP_EQ:
	case ASTERIX_EQ:
	case EQ_OP_EQ:
	case GE_OP_EQ:
	case GTMARK_EQ:
	case GT_OP_EQ:
	case LE_OP_EQ:
	case LTMARK_EQ:
	case LT_OP_EQ:
	case MATRIX_OP1_EQ:
	case MATRIX_OP2_EQ:
	case MINUS_EQ:
	case MOD_OP_EQ:
	case NE_OP_EQ:
	case OR_OP_EQ:
	case PLUS_EQ:
	case POW_EQ:
	case SLASH_EQ:
	case XOR_OP_EQ:
	case BREAK:
	case CONTINUE:
	case REPEAT:
	case WHILE:
	case FOR:
	case GOTO:
	case ON_IOERROR:
	case IF:
	{
		break;
	}
	default:
	{
		throw antlr::NoViableAltException(antlr::RefAST(_t));
	}
	}
	}
	{
	if (_t == RefDNode(antlr::nullAST) )
		_t = ASTNULL;
	switch ( _t->getType()) {
	case ASSIGN:
	case BLOCK:
	case COMMONDECL:
	case COMMONDEF:
	case MPCALL:
	case MPCALL_PARENT:
	case PCALL:
	case RETURN:
	case IDENTIFIER:
	case SWITCH:
	case CASE:
	case FORWARD:
	case DEC:
	case INC:
	case AND_OP_EQ:
	case ASTERIX_EQ:
	case EQ_OP_EQ:
	case GE_OP_EQ:
	case GTMARK_EQ:
	case GT_OP_EQ:
	case LE_OP_EQ:
	case LTMARK_EQ:
	case LT_OP_EQ:
	case MATRIX_OP1_EQ:
	case MATRIX_OP2_EQ:
	case MINUS_EQ:
	case MOD_OP_EQ:
	case NE_OP_EQ:
	case OR_OP_EQ:
	case PLUS_EQ:
	case POW_EQ:
	case SLASH_EQ:
	case XOR_OP_EQ:
	case BREAK:
	case CONTINUE:
	case REPEAT:
	case WHILE:
	case FOR:
	case GOTO:
	case ON_IOERROR:
	case IF:
	{
		statement_list(_t);
		_t = _retTree;
		
		comp.SetTree( returnAST);
		
		break;
	}
	case 3:
	{
		break;
	}
	default:
	{
		throw antlr::NoViableAltException(antlr::RefAST(_t));
	}
	}
	}
	
	comp.EndFun();
	
	currentAST = __currentAST23;
	_t = __t23;
	_t = _t->getNextSibling();
	returnAST = function_def_AST;
	_retTree = _t;
}

void GDLTreeParser::forward_function(RefDNode _t) {
	RefDNode forward_function_AST_in = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	returnAST = RefDNode(antlr::nullAST);
	antlr::ASTPair currentAST;
	RefDNode forward_function_AST = RefDNode(antlr::nullAST);
	RefDNode id = RefDNode(antlr::nullAST);
	RefDNode id_AST = RefDNode(antlr::nullAST);
	
	RefDNode __t8 = _t;
	RefDNode tmp5_AST = RefDNode(antlr::nullAST);
	RefDNode tmp5_AST_in = RefDNode(antlr::nullAST);
	tmp5_AST = astFactory->create(antlr::RefAST(_t));
	tmp5_AST_in = _t;
	antlr::ASTPair __currentAST8 = currentAST;
	currentAST.root = currentAST.child;
	currentAST.child = RefDNode(antlr::nullAST);
	match(antlr::RefAST(_t),FORWARD);
	_t = _t->getFirstChild();
	{ // ( ... )+
	int _cnt10=0;
	for (;;) {
		if (_t == RefDNode(antlr::nullAST) )
			_t = ASTNULL;
		if ((_t->getType() == IDENTIFIER)) {
			id = _t;
			RefDNode id_AST_in = RefDNode(antlr::nullAST);
			id_AST = astFactory->create(antlr::RefAST(id));
			match(antlr::RefAST(_t),IDENTIFIER);
			_t = _t->getNextSibling();
			
					  comp.ForwardFunction(id->getText());
					
		}
		else {
			if ( _cnt10>=1 ) { goto _loop10; } else {throw antlr::NoViableAltException(antlr::RefAST(_t));}
		}
		
		_cnt10++;
	}
	_loop10:;
	}  // ( ... )+
	currentAST = __currentAST8;
	_t = __t8;
	_t = _t->getNextSibling();
	returnAST = forward_function_AST;
	_retTree = _t;
}

void GDLTreeParser::interactive(RefDNode _t) {
	RefDNode interactive_AST_in = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	returnAST = RefDNode(antlr::nullAST);
	antlr::ASTPair currentAST;
	RefDNode interactive_AST = RefDNode(antlr::nullAST);
	
	try {      // for error handling
		{ // ( ... )+
		int _cnt6=0;
		for (;;) {
			if (_t == RefDNode(antlr::nullAST) )
				_t = ASTNULL;
			if ((_tokenSet_0.member(_t->getType()))) {
				statement(_t);
				_t = _retTree;
				astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
			}
			else {
				if ( _cnt6>=1 ) { goto _loop6; } else {throw antlr::NoViableAltException(antlr::RefAST(_t));}
			}
			
			_cnt6++;
		}
		_loop6:;
		}  // ( ... )+
		interactive_AST = RefDNode(currentAST.root);
	}
	catch ( GDLException& e) {
		
		throw;
		
	}
	catch ( antlr::NoViableAltException& e) {
		
		// SYNTAX ERROR
		throw GDLException( e.getLine(), e.getColumn(), "Compiler syntax error: "+e.getMessage());
		
	}
	catch ( antlr::RecognitionException& e) {
		
		// SYNTAX ERROR
		throw GDLException( e.getLine(), e.getColumn(), "General syntax error: "+e.getMessage());
		
	}
	returnAST = interactive_AST;
	_retTree = _t;
}

void GDLTreeParser::statement(RefDNode _t) {
	RefDNode statement_AST_in = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	returnAST = RefDNode(antlr::nullAST);
	antlr::ASTPair currentAST;
	RefDNode statement_AST = RefDNode(antlr::nullAST);
	
	if (_t == RefDNode(antlr::nullAST) )
		_t = ASTNULL;
	switch ( _t->getType()) {
	case ASSIGN:
	{
		assign_expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		statement_AST = RefDNode(currentAST.root);
		break;
	}
	case AND_OP_EQ:
	case ASTERIX_EQ:
	case EQ_OP_EQ:
	case GE_OP_EQ:
	case GTMARK_EQ:
	case GT_OP_EQ:
	case LE_OP_EQ:
	case LTMARK_EQ:
	case LT_OP_EQ:
	case MATRIX_OP1_EQ:
	case MATRIX_OP2_EQ:
	case MINUS_EQ:
	case MOD_OP_EQ:
	case NE_OP_EQ:
	case OR_OP_EQ:
	case PLUS_EQ:
	case POW_EQ:
	case SLASH_EQ:
	case XOR_OP_EQ:
	{
		comp_assign_expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		statement_AST = RefDNode(currentAST.root);
		break;
	}
	case MPCALL:
	case MPCALL_PARENT:
	case PCALL:
	{
		procedure_call(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		statement_AST = RefDNode(currentAST.root);
		break;
	}
	case FOR:
	{
		for_statement(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		statement_AST = RefDNode(currentAST.root);
		break;
	}
	case REPEAT:
	{
		repeat_statement(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		statement_AST = RefDNode(currentAST.root);
		break;
	}
	case WHILE:
	{
		while_statement(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		statement_AST = RefDNode(currentAST.root);
		break;
	}
	case RETURN:
	case GOTO:
	case ON_IOERROR:
	{
		jump_statement(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		statement_AST = RefDNode(currentAST.root);
		break;
	}
	case IF:
	{
		if_statement(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		statement_AST = RefDNode(currentAST.root);
		break;
	}
	case CASE:
	{
		case_statement(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		statement_AST = RefDNode(currentAST.root);
		break;
	}
	case SWITCH:
	{
		switch_statement(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		statement_AST = RefDNode(currentAST.root);
		break;
	}
	case FORWARD:
	{
		forward_function(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		statement_AST = RefDNode(currentAST.root);
		break;
	}
	case COMMONDECL:
	case COMMONDEF:
	{
		common_block(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		statement_AST = RefDNode(currentAST.root);
		break;
	}
	case BLOCK:
	{
		block(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		statement_AST = RefDNode(currentAST.root);
		break;
	}
	case DEC:
	{
		RefDNode __t54 = _t;
		RefDNode tmp6_AST = RefDNode(antlr::nullAST);
		RefDNode tmp6_AST_in = RefDNode(antlr::nullAST);
		tmp6_AST = astFactory->create(antlr::RefAST(_t));
		tmp6_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp6_AST));
		antlr::ASTPair __currentAST54 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),DEC);
		_t = _t->getFirstChild();
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		currentAST = __currentAST54;
		_t = __t54;
		_t = _t->getNextSibling();
		statement_AST = RefDNode(currentAST.root);
		break;
	}
	case INC:
	{
		RefDNode __t55 = _t;
		RefDNode tmp7_AST = RefDNode(antlr::nullAST);
		RefDNode tmp7_AST_in = RefDNode(antlr::nullAST);
		tmp7_AST = astFactory->create(antlr::RefAST(_t));
		tmp7_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp7_AST));
		antlr::ASTPair __currentAST55 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),INC);
		_t = _t->getFirstChild();
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		currentAST = __currentAST55;
		_t = __t55;
		_t = _t->getNextSibling();
		statement_AST = RefDNode(currentAST.root);
		break;
	}
	case BREAK:
	{
		RefDNode tmp8_AST = RefDNode(antlr::nullAST);
		RefDNode tmp8_AST_in = RefDNode(antlr::nullAST);
		tmp8_AST = astFactory->create(antlr::RefAST(_t));
		tmp8_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp8_AST));
		match(antlr::RefAST(_t),BREAK);
		_t = _t->getNextSibling();
		statement_AST = RefDNode(currentAST.root);
		break;
	}
	case CONTINUE:
	{
		RefDNode tmp9_AST = RefDNode(antlr::nullAST);
		RefDNode tmp9_AST_in = RefDNode(antlr::nullAST);
		tmp9_AST = astFactory->create(antlr::RefAST(_t));
		tmp9_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp9_AST));
		match(antlr::RefAST(_t),CONTINUE);
		_t = _t->getNextSibling();
		statement_AST = RefDNode(currentAST.root);
		break;
	}
	default:
	{
		throw antlr::NoViableAltException(antlr::RefAST(_t));
	}
	}
	returnAST = statement_AST;
	_retTree = _t;
}

void GDLTreeParser::parameter_declaration(RefDNode _t) {
	RefDNode parameter_declaration_AST_in = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	returnAST = RefDNode(antlr::nullAST);
	antlr::ASTPair currentAST;
	RefDNode parameter_declaration_AST = RefDNode(antlr::nullAST);
	RefDNode id = RefDNode(antlr::nullAST);
	RefDNode id_AST = RefDNode(antlr::nullAST);
	
	RefDNode __t12 = _t;
	RefDNode tmp10_AST = RefDNode(antlr::nullAST);
	RefDNode tmp10_AST_in = RefDNode(antlr::nullAST);
	tmp10_AST = astFactory->create(antlr::RefAST(_t));
	tmp10_AST_in = _t;
	antlr::ASTPair __currentAST12 = currentAST;
	currentAST.root = currentAST.child;
	currentAST.child = RefDNode(antlr::nullAST);
	match(antlr::RefAST(_t),PARADECL);
	_t = _t->getFirstChild();
	{ // ( ... )+
	int _cnt14=0;
	for (;;) {
		if (_t == RefDNode(antlr::nullAST) )
			_t = ASTNULL;
		switch ( _t->getType()) {
		case IDENTIFIER:
		{
			id = _t;
			RefDNode id_AST_in = RefDNode(antlr::nullAST);
			id_AST = astFactory->create(antlr::RefAST(id));
			match(antlr::RefAST(_t),IDENTIFIER);
			_t = _t->getNextSibling();
			
					  comp.AddPar(id->getText());
					
			break;
		}
		case KEYDECL:
		{
			keyword_declaration(_t);
			_t = _retTree;
			break;
		}
		default:
		{
			if ( _cnt14>=1 ) { goto _loop14; } else {throw antlr::NoViableAltException(antlr::RefAST(_t));}
		}
		}
		_cnt14++;
	}
	_loop14:;
	}  // ( ... )+
	currentAST = __currentAST12;
	_t = __t12;
	_t = _t->getNextSibling();
	returnAST = parameter_declaration_AST;
	_retTree = _t;
}

void GDLTreeParser::keyword_declaration(RefDNode _t) {
	RefDNode keyword_declaration_AST_in = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	returnAST = RefDNode(antlr::nullAST);
	antlr::ASTPair currentAST;
	RefDNode keyword_declaration_AST = RefDNode(antlr::nullAST);
	RefDNode key = RefDNode(antlr::nullAST);
	RefDNode key_AST = RefDNode(antlr::nullAST);
	RefDNode val = RefDNode(antlr::nullAST);
	RefDNode val_AST = RefDNode(antlr::nullAST);
	
	RefDNode __t16 = _t;
	RefDNode tmp11_AST = RefDNode(antlr::nullAST);
	RefDNode tmp11_AST_in = RefDNode(antlr::nullAST);
	tmp11_AST = astFactory->create(antlr::RefAST(_t));
	tmp11_AST_in = _t;
	antlr::ASTPair __currentAST16 = currentAST;
	currentAST.root = currentAST.child;
	currentAST.child = RefDNode(antlr::nullAST);
	match(antlr::RefAST(_t),KEYDECL);
	_t = _t->getFirstChild();
	key = _t;
	RefDNode key_AST_in = RefDNode(antlr::nullAST);
	key_AST = astFactory->create(antlr::RefAST(key));
	match(antlr::RefAST(_t),IDENTIFIER);
	_t = _t->getNextSibling();
	val = _t;
	RefDNode val_AST_in = RefDNode(antlr::nullAST);
	val_AST = astFactory->create(antlr::RefAST(val));
	match(antlr::RefAST(_t),IDENTIFIER);
	_t = _t->getNextSibling();
	currentAST = __currentAST16;
	_t = __t16;
	_t = _t->getNextSibling();
	
		  comp.AddKey(key->getText(),val->getText());
		
	returnAST = keyword_declaration_AST;
	_retTree = _t;
}

void GDLTreeParser::statement_list(RefDNode _t) {
	RefDNode statement_list_AST_in = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	returnAST = RefDNode(antlr::nullAST);
	antlr::ASTPair currentAST;
	RefDNode statement_list_AST = RefDNode(antlr::nullAST);
	
	{ // ( ... )+
	int _cnt52=0;
	for (;;) {
		if (_t == RefDNode(antlr::nullAST) )
			_t = ASTNULL;
		switch ( _t->getType()) {
		case ASSIGN:
		case BLOCK:
		case COMMONDECL:
		case COMMONDEF:
		case MPCALL:
		case MPCALL_PARENT:
		case PCALL:
		case RETURN:
		case SWITCH:
		case CASE:
		case FORWARD:
		case DEC:
		case INC:
		case AND_OP_EQ:
		case ASTERIX_EQ:
		case EQ_OP_EQ:
		case GE_OP_EQ:
		case GTMARK_EQ:
		case GT_OP_EQ:
		case LE_OP_EQ:
		case LTMARK_EQ:
		case LT_OP_EQ:
		case MATRIX_OP1_EQ:
		case MATRIX_OP2_EQ:
		case MINUS_EQ:
		case MOD_OP_EQ:
		case NE_OP_EQ:
		case OR_OP_EQ:
		case PLUS_EQ:
		case POW_EQ:
		case SLASH_EQ:
		case XOR_OP_EQ:
		case BREAK:
		case CONTINUE:
		case REPEAT:
		case WHILE:
		case FOR:
		case GOTO:
		case ON_IOERROR:
		case IF:
		{
			statement(_t);
			_t = _retTree;
			astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
			break;
		}
		case IDENTIFIER:
		{
			label(_t);
			_t = _retTree;
			astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
			break;
		}
		default:
		{
			if ( _cnt52>=1 ) { goto _loop52; } else {throw antlr::NoViableAltException(antlr::RefAST(_t));}
		}
		}
		_cnt52++;
	}
	_loop52:;
	}  // ( ... )+
	statement_list_AST = RefDNode(currentAST.root);
	returnAST = statement_list_AST;
	_retTree = _t;
}

void GDLTreeParser::common_block(RefDNode _t) {
	RefDNode common_block_AST_in = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	returnAST = RefDNode(antlr::nullAST);
	antlr::ASTPair currentAST;
	RefDNode common_block_AST = RefDNode(antlr::nullAST);
	RefDNode id = RefDNode(antlr::nullAST);
	RefDNode id_AST = RefDNode(antlr::nullAST);
	RefDNode cv = RefDNode(antlr::nullAST);
	RefDNode cv_AST = RefDNode(antlr::nullAST);
	RefDNode id2 = RefDNode(antlr::nullAST);
	RefDNode id2_AST = RefDNode(antlr::nullAST);
	
	if (_t == RefDNode(antlr::nullAST) )
		_t = ASTNULL;
	switch ( _t->getType()) {
	case COMMONDEF:
	{
		RefDNode __t28 = _t;
		RefDNode tmp12_AST = RefDNode(antlr::nullAST);
		RefDNode tmp12_AST_in = RefDNode(antlr::nullAST);
		tmp12_AST = astFactory->create(antlr::RefAST(_t));
		tmp12_AST_in = _t;
		antlr::ASTPair __currentAST28 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),COMMONDEF);
		_t = _t->getFirstChild();
		id = _t;
		RefDNode id_AST_in = RefDNode(antlr::nullAST);
		id_AST = astFactory->create(antlr::RefAST(id));
		match(antlr::RefAST(_t),IDENTIFIER);
		_t = _t->getNextSibling();
		
				DCommonBase* actCommon=comp.CommonDef(id->getText());
			
		{ // ( ... )+
		int _cnt30=0;
		for (;;) {
			if (_t == RefDNode(antlr::nullAST) )
				_t = ASTNULL;
			if ((_t->getType() == IDENTIFIER)) {
				cv = _t;
				RefDNode cv_AST_in = RefDNode(antlr::nullAST);
				cv_AST = astFactory->create(antlr::RefAST(cv));
				match(antlr::RefAST(_t),IDENTIFIER);
				_t = _t->getNextSibling();
				
						  comp.CommonVar(actCommon,cv->getText());
						
			}
			else {
				if ( _cnt30>=1 ) { goto _loop30; } else {throw antlr::NoViableAltException(antlr::RefAST(_t));}
			}
			
			_cnt30++;
		}
		_loop30:;
		}  // ( ... )+
		currentAST = __currentAST28;
		_t = __t28;
		_t = _t->getNextSibling();
		break;
	}
	case COMMONDECL:
	{
		RefDNode __t31 = _t;
		RefDNode tmp13_AST = RefDNode(antlr::nullAST);
		RefDNode tmp13_AST_in = RefDNode(antlr::nullAST);
		tmp13_AST = astFactory->create(antlr::RefAST(_t));
		tmp13_AST_in = _t;
		antlr::ASTPair __currentAST31 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),COMMONDECL);
		_t = _t->getFirstChild();
		id2 = _t;
		RefDNode id2_AST_in = RefDNode(antlr::nullAST);
		id2_AST = astFactory->create(antlr::RefAST(id2));
		match(antlr::RefAST(_t),IDENTIFIER);
		_t = _t->getNextSibling();
		
				comp.CommonDecl(id2->getText());
			
		currentAST = __currentAST31;
		_t = __t31;
		_t = _t->getNextSibling();
		break;
	}
	default:
	{
		throw antlr::NoViableAltException(antlr::RefAST(_t));
	}
	}
	returnAST = common_block_AST;
	_retTree = _t;
}

void GDLTreeParser::unbrace_expr(RefDNode _t) {
	RefDNode unbrace_expr_AST_in = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	returnAST = RefDNode(antlr::nullAST);
	antlr::ASTPair currentAST;
	RefDNode unbrace_expr_AST = RefDNode(antlr::nullAST);
	RefDNode ex_AST = RefDNode(antlr::nullAST);
	RefDNode ex = RefDNode(antlr::nullAST);
	
	ex = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	expr(_t);
	_t = _retTree;
	ex_AST = returnAST;
	unbrace_expr_AST = RefDNode(currentAST.root);
	
	// remove last pair of braces
				if( ex_AST->getType()==EXPR) 
	unbrace_expr_AST=RefDNode(astFactory->make((new antlr::ASTArray(1))->add(antlr::RefAST(ex->getFirstChild()))));
	else
	unbrace_expr_AST=RefDNode(astFactory->make((new antlr::ASTArray(1))->add(antlr::RefAST(ex_AST))));
			
	currentAST.root = unbrace_expr_AST;
	if ( unbrace_expr_AST!=RefDNode(antlr::nullAST) &&
		unbrace_expr_AST->getFirstChild() != RefDNode(antlr::nullAST) )
		  currentAST.child = unbrace_expr_AST->getFirstChild();
	else
		currentAST.child = unbrace_expr_AST;
	currentAST.advanceChildToEnd();
	returnAST = unbrace_expr_AST;
	_retTree = _t;
}

void GDLTreeParser::expr(RefDNode _t) {
	RefDNode expr_AST_in = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	returnAST = RefDNode(antlr::nullAST);
	antlr::ASTPair currentAST;
	RefDNode expr_AST = RefDNode(antlr::nullAST);
	
	if (_t == RefDNode(antlr::nullAST) )
		_t = ASTNULL;
	switch ( _t->getType()) {
	case ARRAYEXPR:
	case EXPR:
	case SYSVAR:
	case VAR:
	{
		array_expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		expr_AST = RefDNode(currentAST.root);
		break;
	}
	case DOT:
	{
		RefDNode __t190 = _t;
		RefDNode tmp14_AST = RefDNode(antlr::nullAST);
		RefDNode tmp14_AST_in = RefDNode(antlr::nullAST);
		tmp14_AST = astFactory->create(antlr::RefAST(_t));
		tmp14_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp14_AST));
		antlr::ASTPair __currentAST190 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),DOT);
		_t = _t->getFirstChild();
		array_expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		{ // ( ... )+
		int _cnt192=0;
		for (;;) {
			if (_t == RefDNode(antlr::nullAST) )
				_t = ASTNULL;
			if ((_t->getType() == ARRAYEXPR || _t->getType() == EXPR || _t->getType() == IDENTIFIER)) {
				tag_array_expr(_t);
				_t = _retTree;
				astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
			}
			else {
				if ( _cnt192>=1 ) { goto _loop192; } else {throw antlr::NoViableAltException(antlr::RefAST(_t));}
			}
			
			_cnt192++;
		}
		_loop192:;
		}  // ( ... )+
		currentAST = __currentAST190;
		_t = __t190;
		_t = _t->getNextSibling();
		expr_AST = RefDNode(currentAST.root);
		break;
	}
	case DEREF:
	{
		RefDNode __t193 = _t;
		RefDNode tmp15_AST = RefDNode(antlr::nullAST);
		RefDNode tmp15_AST_in = RefDNode(antlr::nullAST);
		tmp15_AST = astFactory->create(antlr::RefAST(_t));
		tmp15_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp15_AST));
		antlr::ASTPair __currentAST193 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),DEREF);
		_t = _t->getFirstChild();
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		currentAST = __currentAST193;
		_t = __t193;
		_t = _t->getNextSibling();
		expr_AST = RefDNode(currentAST.root);
		break;
	}
	case ASSIGN:
	case ARRAYDEF:
	case ARRAYEXPR_FN:
	case CONSTANT:
	case FCALL:
	case MFCALL:
	case MFCALL_PARENT:
	case NSTRUC_REF:
	case POSTDEC:
	case POSTINC:
	case STRUC:
	case UMINUS:
	case DEC:
	case INC:
	case AND_OP_EQ:
	case ASTERIX_EQ:
	case EQ_OP_EQ:
	case GE_OP_EQ:
	case GTMARK_EQ:
	case GT_OP_EQ:
	case LE_OP_EQ:
	case LTMARK_EQ:
	case LT_OP_EQ:
	case MATRIX_OP1_EQ:
	case MATRIX_OP2_EQ:
	case MINUS_EQ:
	case MOD_OP_EQ:
	case NE_OP_EQ:
	case OR_OP_EQ:
	case PLUS_EQ:
	case POW_EQ:
	case SLASH_EQ:
	case XOR_OP_EQ:
	case SLASH:
	case ASTERIX:
	case POW:
	case MATRIX_OP1:
	case MATRIX_OP2:
	case MOD_OP:
	case PLUS:
	case MINUS:
	case LTMARK:
	case GTMARK:
	case NOT_OP:
	case EQ_OP:
	case NE_OP:
	case LE_OP:
	case LT_OP:
	case GE_OP:
	case GT_OP:
	case AND_OP:
	case OR_OP:
	case XOR_OP:
	case LOG_AND:
	case LOG_OR:
	case LOG_NEG:
	case QUESTION:
	{
		op_expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		expr_AST = RefDNode(currentAST.root);
		break;
	}
	default:
	{
		throw antlr::NoViableAltException(antlr::RefAST(_t));
	}
	}
	returnAST = expr_AST;
	_retTree = _t;
}

void GDLTreeParser::labeled_expr(RefDNode _t) {
	RefDNode labeled_expr_AST_in = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	returnAST = RefDNode(antlr::nullAST);
	antlr::ASTPair currentAST;
	RefDNode labeled_expr_AST = RefDNode(antlr::nullAST);
	RefDNode ex_AST = RefDNode(antlr::nullAST);
	RefDNode ex = RefDNode(antlr::nullAST);
	
	ex = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	expr(_t);
	_t = _retTree;
	ex_AST = returnAST;
	astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
	labeled_expr_AST = RefDNode(currentAST.root);
	
	if( ex_AST->getType() != EXPR)   
	labeled_expr_AST = RefDNode(astFactory->make((new antlr::ASTArray(2))->add(antlr::RefAST(astFactory->create(EXPR,"expr")))->add(antlr::RefAST(labeled_expr_AST))));
	
	currentAST.root = labeled_expr_AST;
	if ( labeled_expr_AST!=RefDNode(antlr::nullAST) &&
		labeled_expr_AST->getFirstChild() != RefDNode(antlr::nullAST) )
		  currentAST.child = labeled_expr_AST->getFirstChild();
	else
		currentAST.child = labeled_expr_AST;
	currentAST.advanceChildToEnd();
	labeled_expr_AST = RefDNode(currentAST.root);
	returnAST = labeled_expr_AST;
	_retTree = _t;
}

void GDLTreeParser::caseswitch_body(RefDNode _t) {
	RefDNode caseswitch_body_AST_in = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	returnAST = RefDNode(antlr::nullAST);
	antlr::ASTPair currentAST;
	RefDNode caseswitch_body_AST = RefDNode(antlr::nullAST);
	
	if (_t == RefDNode(antlr::nullAST) )
		_t = ASTNULL;
	switch ( _t->getType()) {
	case BLOCK:
	{
		RefDNode __t35 = _t;
		RefDNode tmp16_AST = RefDNode(antlr::nullAST);
		RefDNode tmp16_AST_in = RefDNode(antlr::nullAST);
		tmp16_AST = astFactory->create(antlr::RefAST(_t));
		tmp16_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp16_AST));
		antlr::ASTPair __currentAST35 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),BLOCK);
		_t = _t->getFirstChild();
		labeled_expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		{
		if (_t == RefDNode(antlr::nullAST) )
			_t = ASTNULL;
		switch ( _t->getType()) {
		case ASSIGN:
		case BLOCK:
		case COMMONDECL:
		case COMMONDEF:
		case MPCALL:
		case MPCALL_PARENT:
		case PCALL:
		case RETURN:
		case IDENTIFIER:
		case SWITCH:
		case CASE:
		case FORWARD:
		case DEC:
		case INC:
		case AND_OP_EQ:
		case ASTERIX_EQ:
		case EQ_OP_EQ:
		case GE_OP_EQ:
		case GTMARK_EQ:
		case GT_OP_EQ:
		case LE_OP_EQ:
		case LTMARK_EQ:
		case LT_OP_EQ:
		case MATRIX_OP1_EQ:
		case MATRIX_OP2_EQ:
		case MINUS_EQ:
		case MOD_OP_EQ:
		case NE_OP_EQ:
		case OR_OP_EQ:
		case PLUS_EQ:
		case POW_EQ:
		case SLASH_EQ:
		case XOR_OP_EQ:
		case BREAK:
		case CONTINUE:
		case REPEAT:
		case WHILE:
		case FOR:
		case GOTO:
		case ON_IOERROR:
		case IF:
		{
			statement_list(_t);
			_t = _retTree;
			astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
			break;
		}
		case 3:
		{
			break;
		}
		default:
		{
			throw antlr::NoViableAltException(antlr::RefAST(_t));
		}
		}
		}
		currentAST = __currentAST35;
		_t = __t35;
		_t = _t->getNextSibling();
		caseswitch_body_AST = RefDNode(currentAST.root);
		break;
	}
	case ELSEBLK:
	{
		RefDNode __t37 = _t;
		RefDNode tmp17_AST = RefDNode(antlr::nullAST);
		RefDNode tmp17_AST_in = RefDNode(antlr::nullAST);
		tmp17_AST = astFactory->create(antlr::RefAST(_t));
		tmp17_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp17_AST));
		antlr::ASTPair __currentAST37 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),ELSEBLK);
		_t = _t->getFirstChild();
		{
		if (_t == RefDNode(antlr::nullAST) )
			_t = ASTNULL;
		switch ( _t->getType()) {
		case ASSIGN:
		case BLOCK:
		case COMMONDECL:
		case COMMONDEF:
		case MPCALL:
		case MPCALL_PARENT:
		case PCALL:
		case RETURN:
		case IDENTIFIER:
		case SWITCH:
		case CASE:
		case FORWARD:
		case DEC:
		case INC:
		case AND_OP_EQ:
		case ASTERIX_EQ:
		case EQ_OP_EQ:
		case GE_OP_EQ:
		case GTMARK_EQ:
		case GT_OP_EQ:
		case LE_OP_EQ:
		case LTMARK_EQ:
		case LT_OP_EQ:
		case MATRIX_OP1_EQ:
		case MATRIX_OP2_EQ:
		case MINUS_EQ:
		case MOD_OP_EQ:
		case NE_OP_EQ:
		case OR_OP_EQ:
		case PLUS_EQ:
		case POW_EQ:
		case SLASH_EQ:
		case XOR_OP_EQ:
		case BREAK:
		case CONTINUE:
		case REPEAT:
		case WHILE:
		case FOR:
		case GOTO:
		case ON_IOERROR:
		case IF:
		{
			statement_list(_t);
			_t = _retTree;
			astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
			break;
		}
		case 3:
		{
			break;
		}
		default:
		{
			throw antlr::NoViableAltException(antlr::RefAST(_t));
		}
		}
		}
		currentAST = __currentAST37;
		_t = __t37;
		_t = _t->getNextSibling();
		caseswitch_body_AST = RefDNode(currentAST.root);
		break;
	}
	default:
	{
		throw antlr::NoViableAltException(antlr::RefAST(_t));
	}
	}
	returnAST = caseswitch_body_AST;
	_retTree = _t;
}

void GDLTreeParser::switch_statement(RefDNode _t) {
	RefDNode switch_statement_AST_in = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	returnAST = RefDNode(antlr::nullAST);
	antlr::ASTPair currentAST;
	RefDNode switch_statement_AST = RefDNode(antlr::nullAST);
	RefDNode s = RefDNode(antlr::nullAST);
	RefDNode s_AST = RefDNode(antlr::nullAST);
	
	int labelStart = comp.NDefLabel();
	
	
	RefDNode __t40 = _t;
	s = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	RefDNode s_AST_in = RefDNode(antlr::nullAST);
	s_AST = astFactory->create(antlr::RefAST(s));
	astFactory->addASTChild(currentAST, antlr::RefAST(s_AST));
	antlr::ASTPair __currentAST40 = currentAST;
	currentAST.root = currentAST.child;
	currentAST.child = RefDNode(antlr::nullAST);
	match(antlr::RefAST(_t),SWITCH);
	_t = _t->getFirstChild();
	expr(_t);
	_t = _retTree;
	astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
	{ // ( ... )*
	for (;;) {
		if (_t == RefDNode(antlr::nullAST) )
			_t = ASTNULL;
		if ((_t->getType() == BLOCK || _t->getType() == ELSEBLK)) {
			caseswitch_body(_t);
			_t = _retTree;
			astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		}
		else {
			goto _loop42;
		}
		
	}
	_loop42:;
	} // ( ... )*
	currentAST = __currentAST40;
	_t = __t40;
	_t = _t->getNextSibling();
	
	s_AST->SetLabelRange( labelStart, comp.NDefLabel());
	
	switch_statement_AST = RefDNode(currentAST.root);
	returnAST = switch_statement_AST;
	_retTree = _t;
}

void GDLTreeParser::case_statement(RefDNode _t) {
	RefDNode case_statement_AST_in = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	returnAST = RefDNode(antlr::nullAST);
	antlr::ASTPair currentAST;
	RefDNode case_statement_AST = RefDNode(antlr::nullAST);
	RefDNode c = RefDNode(antlr::nullAST);
	RefDNode c_AST = RefDNode(antlr::nullAST);
	
	int labelStart = comp.NDefLabel();
	
	
	RefDNode __t44 = _t;
	c = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	RefDNode c_AST_in = RefDNode(antlr::nullAST);
	c_AST = astFactory->create(antlr::RefAST(c));
	astFactory->addASTChild(currentAST, antlr::RefAST(c_AST));
	antlr::ASTPair __currentAST44 = currentAST;
	currentAST.root = currentAST.child;
	currentAST.child = RefDNode(antlr::nullAST);
	match(antlr::RefAST(_t),CASE);
	_t = _t->getFirstChild();
	expr(_t);
	_t = _retTree;
	astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
	{ // ( ... )*
	for (;;) {
		if (_t == RefDNode(antlr::nullAST) )
			_t = ASTNULL;
		if ((_t->getType() == BLOCK || _t->getType() == ELSEBLK)) {
			caseswitch_body(_t);
			_t = _retTree;
			astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		}
		else {
			goto _loop46;
		}
		
	}
	_loop46:;
	} // ( ... )*
	currentAST = __currentAST44;
	_t = __t44;
	_t = _t->getNextSibling();
	
	c_AST->SetLabelRange( labelStart, comp.NDefLabel());
	
	case_statement_AST = RefDNode(currentAST.root);
	returnAST = case_statement_AST;
	_retTree = _t;
}

void GDLTreeParser::block(RefDNode _t) {
	RefDNode block_AST_in = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	returnAST = RefDNode(antlr::nullAST);
	antlr::ASTPair currentAST;
	RefDNode block_AST = RefDNode(antlr::nullAST);
	
	RefDNode __t48 = _t;
	RefDNode tmp18_AST = RefDNode(antlr::nullAST);
	RefDNode tmp18_AST_in = RefDNode(antlr::nullAST);
	tmp18_AST = astFactory->create(antlr::RefAST(_t));
	tmp18_AST_in = _t;
	astFactory->addASTChild(currentAST, antlr::RefAST(tmp18_AST));
	antlr::ASTPair __currentAST48 = currentAST;
	currentAST.root = currentAST.child;
	currentAST.child = RefDNode(antlr::nullAST);
	match(antlr::RefAST(_t),BLOCK);
	_t = _t->getFirstChild();
	{
	if (_t == RefDNode(antlr::nullAST) )
		_t = ASTNULL;
	switch ( _t->getType()) {
	case ASSIGN:
	case BLOCK:
	case COMMONDECL:
	case COMMONDEF:
	case MPCALL:
	case MPCALL_PARENT:
	case PCALL:
	case RETURN:
	case IDENTIFIER:
	case SWITCH:
	case CASE:
	case FORWARD:
	case DEC:
	case INC:
	case AND_OP_EQ:
	case ASTERIX_EQ:
	case EQ_OP_EQ:
	case GE_OP_EQ:
	case GTMARK_EQ:
	case GT_OP_EQ:
	case LE_OP_EQ:
	case LTMARK_EQ:
	case LT_OP_EQ:
	case MATRIX_OP1_EQ:
	case MATRIX_OP2_EQ:
	case MINUS_EQ:
	case MOD_OP_EQ:
	case NE_OP_EQ:
	case OR_OP_EQ:
	case PLUS_EQ:
	case POW_EQ:
	case SLASH_EQ:
	case XOR_OP_EQ:
	case BREAK:
	case CONTINUE:
	case REPEAT:
	case WHILE:
	case FOR:
	case GOTO:
	case ON_IOERROR:
	case IF:
	{
		statement_list(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		break;
	}
	case 3:
	{
		break;
	}
	default:
	{
		throw antlr::NoViableAltException(antlr::RefAST(_t));
	}
	}
	}
	currentAST = __currentAST48;
	_t = __t48;
	_t = _t->getNextSibling();
	block_AST = RefDNode(currentAST.root);
	returnAST = block_AST;
	_retTree = _t;
}

void GDLTreeParser::label(RefDNode _t) {
	RefDNode label_AST_in = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	returnAST = RefDNode(antlr::nullAST);
	antlr::ASTPair currentAST;
	RefDNode label_AST = RefDNode(antlr::nullAST);
	RefDNode i = RefDNode(antlr::nullAST);
	RefDNode i_AST = RefDNode(antlr::nullAST);
	
	RefDNode __t64 = _t;
	i = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	RefDNode i_AST_in = RefDNode(antlr::nullAST);
	i_AST = astFactory->create(antlr::RefAST(i));
	antlr::ASTPair __currentAST64 = currentAST;
	currentAST.root = currentAST.child;
	currentAST.child = RefDNode(antlr::nullAST);
	match(antlr::RefAST(_t),IDENTIFIER);
	_t = _t->getFirstChild();
	RefDNode tmp19_AST = RefDNode(antlr::nullAST);
	RefDNode tmp19_AST_in = RefDNode(antlr::nullAST);
	tmp19_AST = astFactory->create(antlr::RefAST(_t));
	tmp19_AST_in = _t;
	match(antlr::RefAST(_t),COLON);
	_t = _t->getNextSibling();
	currentAST = __currentAST64;
	_t = __t64;
	_t = _t->getNextSibling();
	label_AST = RefDNode(currentAST.root);
	
		  label_AST=astFactory->create(LABEL,i->getText());
		  comp.Label(label_AST); 
		
	currentAST.root = label_AST;
	if ( label_AST!=RefDNode(antlr::nullAST) &&
		label_AST->getFirstChild() != RefDNode(antlr::nullAST) )
		  currentAST.child = label_AST->getFirstChild();
	else
		currentAST.child = label_AST;
	currentAST.advanceChildToEnd();
	returnAST = label_AST;
	_retTree = _t;
}

void GDLTreeParser::assign_expr(RefDNode _t) {
	RefDNode assign_expr_AST_in = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	returnAST = RefDNode(antlr::nullAST);
	antlr::ASTPair currentAST;
	RefDNode assign_expr_AST = RefDNode(antlr::nullAST);
	RefDNode a = RefDNode(antlr::nullAST);
	RefDNode a_AST = RefDNode(antlr::nullAST);
	RefDNode l_AST = RefDNode(antlr::nullAST);
	RefDNode l = RefDNode(antlr::nullAST);
	RefDNode r_AST = RefDNode(antlr::nullAST);
	RefDNode r = RefDNode(antlr::nullAST);
	
	RefDNode __t108 = _t;
	a = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	RefDNode a_AST_in = RefDNode(antlr::nullAST);
	a_AST = astFactory->create(antlr::RefAST(a));
	antlr::ASTPair __currentAST108 = currentAST;
	currentAST.root = currentAST.child;
	currentAST.child = RefDNode(antlr::nullAST);
	match(antlr::RefAST(_t),ASSIGN);
	_t = _t->getFirstChild();
	l = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	lassign_expr(_t);
	_t = _retTree;
	l_AST = returnAST;
	r = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	expr(_t);
	_t = _retTree;
	r_AST = returnAST;
	currentAST = __currentAST108;
	_t = __t108;
	_t = _t->getNextSibling();
	assign_expr_AST = RefDNode(currentAST.root);
	assign_expr_AST=RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(a_AST))->add(antlr::RefAST(r_AST))->add(antlr::RefAST(l_AST))));
	currentAST.root = assign_expr_AST;
	if ( assign_expr_AST!=RefDNode(antlr::nullAST) &&
		assign_expr_AST->getFirstChild() != RefDNode(antlr::nullAST) )
		  currentAST.child = assign_expr_AST->getFirstChild();
	else
		currentAST.child = assign_expr_AST;
	currentAST.advanceChildToEnd();
	returnAST = assign_expr_AST;
	_retTree = _t;
}

void GDLTreeParser::comp_assign_expr(RefDNode _t) {
	RefDNode comp_assign_expr_AST_in = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	returnAST = RefDNode(antlr::nullAST);
	antlr::ASTPair currentAST;
	RefDNode comp_assign_expr_AST = RefDNode(antlr::nullAST);
	RefDNode a1 = RefDNode(antlr::nullAST);
	RefDNode a1_AST = RefDNode(antlr::nullAST);
	RefDNode l1_AST = RefDNode(antlr::nullAST);
	RefDNode l1 = RefDNode(antlr::nullAST);
	RefDNode r1_AST = RefDNode(antlr::nullAST);
	RefDNode r1 = RefDNode(antlr::nullAST);
	RefDNode a2 = RefDNode(antlr::nullAST);
	RefDNode a2_AST = RefDNode(antlr::nullAST);
	RefDNode l2_AST = RefDNode(antlr::nullAST);
	RefDNode l2 = RefDNode(antlr::nullAST);
	RefDNode r2_AST = RefDNode(antlr::nullAST);
	RefDNode r2 = RefDNode(antlr::nullAST);
	RefDNode a3 = RefDNode(antlr::nullAST);
	RefDNode a3_AST = RefDNode(antlr::nullAST);
	RefDNode l3_AST = RefDNode(antlr::nullAST);
	RefDNode l3 = RefDNode(antlr::nullAST);
	RefDNode r3_AST = RefDNode(antlr::nullAST);
	RefDNode r3 = RefDNode(antlr::nullAST);
	RefDNode a4 = RefDNode(antlr::nullAST);
	RefDNode a4_AST = RefDNode(antlr::nullAST);
	RefDNode l4_AST = RefDNode(antlr::nullAST);
	RefDNode l4 = RefDNode(antlr::nullAST);
	RefDNode r4_AST = RefDNode(antlr::nullAST);
	RefDNode r4 = RefDNode(antlr::nullAST);
	RefDNode a5 = RefDNode(antlr::nullAST);
	RefDNode a5_AST = RefDNode(antlr::nullAST);
	RefDNode l5_AST = RefDNode(antlr::nullAST);
	RefDNode l5 = RefDNode(antlr::nullAST);
	RefDNode r5_AST = RefDNode(antlr::nullAST);
	RefDNode r5 = RefDNode(antlr::nullAST);
	RefDNode a6 = RefDNode(antlr::nullAST);
	RefDNode a6_AST = RefDNode(antlr::nullAST);
	RefDNode l6_AST = RefDNode(antlr::nullAST);
	RefDNode l6 = RefDNode(antlr::nullAST);
	RefDNode r6_AST = RefDNode(antlr::nullAST);
	RefDNode r6 = RefDNode(antlr::nullAST);
	RefDNode a7 = RefDNode(antlr::nullAST);
	RefDNode a7_AST = RefDNode(antlr::nullAST);
	RefDNode l7_AST = RefDNode(antlr::nullAST);
	RefDNode l7 = RefDNode(antlr::nullAST);
	RefDNode r7_AST = RefDNode(antlr::nullAST);
	RefDNode r7 = RefDNode(antlr::nullAST);
	RefDNode a8 = RefDNode(antlr::nullAST);
	RefDNode a8_AST = RefDNode(antlr::nullAST);
	RefDNode l8_AST = RefDNode(antlr::nullAST);
	RefDNode l8 = RefDNode(antlr::nullAST);
	RefDNode r8_AST = RefDNode(antlr::nullAST);
	RefDNode r8 = RefDNode(antlr::nullAST);
	RefDNode a9 = RefDNode(antlr::nullAST);
	RefDNode a9_AST = RefDNode(antlr::nullAST);
	RefDNode l9_AST = RefDNode(antlr::nullAST);
	RefDNode l9 = RefDNode(antlr::nullAST);
	RefDNode r9_AST = RefDNode(antlr::nullAST);
	RefDNode r9 = RefDNode(antlr::nullAST);
	RefDNode a10 = RefDNode(antlr::nullAST);
	RefDNode a10_AST = RefDNode(antlr::nullAST);
	RefDNode l10_AST = RefDNode(antlr::nullAST);
	RefDNode l10 = RefDNode(antlr::nullAST);
	RefDNode r10_AST = RefDNode(antlr::nullAST);
	RefDNode r10 = RefDNode(antlr::nullAST);
	RefDNode a11 = RefDNode(antlr::nullAST);
	RefDNode a11_AST = RefDNode(antlr::nullAST);
	RefDNode l11_AST = RefDNode(antlr::nullAST);
	RefDNode l11 = RefDNode(antlr::nullAST);
	RefDNode r11_AST = RefDNode(antlr::nullAST);
	RefDNode r11 = RefDNode(antlr::nullAST);
	RefDNode a12 = RefDNode(antlr::nullAST);
	RefDNode a12_AST = RefDNode(antlr::nullAST);
	RefDNode l12_AST = RefDNode(antlr::nullAST);
	RefDNode l12 = RefDNode(antlr::nullAST);
	RefDNode r12_AST = RefDNode(antlr::nullAST);
	RefDNode r12 = RefDNode(antlr::nullAST);
	RefDNode a13 = RefDNode(antlr::nullAST);
	RefDNode a13_AST = RefDNode(antlr::nullAST);
	RefDNode l13_AST = RefDNode(antlr::nullAST);
	RefDNode l13 = RefDNode(antlr::nullAST);
	RefDNode r13_AST = RefDNode(antlr::nullAST);
	RefDNode r13 = RefDNode(antlr::nullAST);
	RefDNode a14 = RefDNode(antlr::nullAST);
	RefDNode a14_AST = RefDNode(antlr::nullAST);
	RefDNode l14_AST = RefDNode(antlr::nullAST);
	RefDNode l14 = RefDNode(antlr::nullAST);
	RefDNode r14_AST = RefDNode(antlr::nullAST);
	RefDNode r14 = RefDNode(antlr::nullAST);
	RefDNode a15 = RefDNode(antlr::nullAST);
	RefDNode a15_AST = RefDNode(antlr::nullAST);
	RefDNode l15_AST = RefDNode(antlr::nullAST);
	RefDNode l15 = RefDNode(antlr::nullAST);
	RefDNode r15_AST = RefDNode(antlr::nullAST);
	RefDNode r15 = RefDNode(antlr::nullAST);
	RefDNode a16 = RefDNode(antlr::nullAST);
	RefDNode a16_AST = RefDNode(antlr::nullAST);
	RefDNode l16_AST = RefDNode(antlr::nullAST);
	RefDNode l16 = RefDNode(antlr::nullAST);
	RefDNode r16_AST = RefDNode(antlr::nullAST);
	RefDNode r16 = RefDNode(antlr::nullAST);
	RefDNode a17 = RefDNode(antlr::nullAST);
	RefDNode a17_AST = RefDNode(antlr::nullAST);
	RefDNode l17_AST = RefDNode(antlr::nullAST);
	RefDNode l17 = RefDNode(antlr::nullAST);
	RefDNode r17_AST = RefDNode(antlr::nullAST);
	RefDNode r17 = RefDNode(antlr::nullAST);
	RefDNode a18 = RefDNode(antlr::nullAST);
	RefDNode a18_AST = RefDNode(antlr::nullAST);
	RefDNode l18_AST = RefDNode(antlr::nullAST);
	RefDNode l18 = RefDNode(antlr::nullAST);
	RefDNode r18_AST = RefDNode(antlr::nullAST);
	RefDNode r18 = RefDNode(antlr::nullAST);
	RefDNode a19 = RefDNode(antlr::nullAST);
	RefDNode a19_AST = RefDNode(antlr::nullAST);
	RefDNode l19_AST = RefDNode(antlr::nullAST);
	RefDNode l19 = RefDNode(antlr::nullAST);
	RefDNode r19_AST = RefDNode(antlr::nullAST);
	RefDNode r19 = RefDNode(antlr::nullAST);
	
	if (_t == RefDNode(antlr::nullAST) )
		_t = ASTNULL;
	switch ( _t->getType()) {
	case AND_OP_EQ:
	{
		RefDNode __t110 = _t;
		a1 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		RefDNode a1_AST_in = RefDNode(antlr::nullAST);
		a1_AST = astFactory->create(antlr::RefAST(a1));
		antlr::ASTPair __currentAST110 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),AND_OP_EQ);
		_t = _t->getFirstChild();
		l1 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		lassign_expr(_t);
		_t = _retTree;
		l1_AST = returnAST;
		r1 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		expr(_t);
		_t = _retTree;
		r1_AST = returnAST;
		currentAST = __currentAST110;
		_t = __t110;
		_t = _t->getNextSibling();
		comp_assign_expr_AST = RefDNode(currentAST.root);
		comp_assign_expr_AST=RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(astFactory->create(ASSIGN,":=")))->add(antlr::RefAST(RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(astFactory->create(AND_OP,"and")))->add(antlr::RefAST(l1_AST))->add(antlr::RefAST(r1_AST))))))->add(antlr::RefAST(l1_AST))));
		currentAST.root = comp_assign_expr_AST;
		if ( comp_assign_expr_AST!=RefDNode(antlr::nullAST) &&
			comp_assign_expr_AST->getFirstChild() != RefDNode(antlr::nullAST) )
			  currentAST.child = comp_assign_expr_AST->getFirstChild();
		else
			currentAST.child = comp_assign_expr_AST;
		currentAST.advanceChildToEnd();
		break;
	}
	case ASTERIX_EQ:
	{
		RefDNode __t111 = _t;
		a2 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		RefDNode a2_AST_in = RefDNode(antlr::nullAST);
		a2_AST = astFactory->create(antlr::RefAST(a2));
		antlr::ASTPair __currentAST111 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),ASTERIX_EQ);
		_t = _t->getFirstChild();
		l2 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		lassign_expr(_t);
		_t = _retTree;
		l2_AST = returnAST;
		r2 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		expr(_t);
		_t = _retTree;
		r2_AST = returnAST;
		currentAST = __currentAST111;
		_t = __t111;
		_t = _t->getNextSibling();
		comp_assign_expr_AST = RefDNode(currentAST.root);
		comp_assign_expr_AST=RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(astFactory->create(ASSIGN,":=")))->add(antlr::RefAST(RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(astFactory->create(ASTERIX,"*")))->add(antlr::RefAST(l2_AST))->add(antlr::RefAST(r2_AST))))))->add(antlr::RefAST(l2_AST))));
		currentAST.root = comp_assign_expr_AST;
		if ( comp_assign_expr_AST!=RefDNode(antlr::nullAST) &&
			comp_assign_expr_AST->getFirstChild() != RefDNode(antlr::nullAST) )
			  currentAST.child = comp_assign_expr_AST->getFirstChild();
		else
			currentAST.child = comp_assign_expr_AST;
		currentAST.advanceChildToEnd();
		break;
	}
	case EQ_OP_EQ:
	{
		RefDNode __t112 = _t;
		a3 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		RefDNode a3_AST_in = RefDNode(antlr::nullAST);
		a3_AST = astFactory->create(antlr::RefAST(a3));
		antlr::ASTPair __currentAST112 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),EQ_OP_EQ);
		_t = _t->getFirstChild();
		l3 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		lassign_expr(_t);
		_t = _retTree;
		l3_AST = returnAST;
		r3 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		expr(_t);
		_t = _retTree;
		r3_AST = returnAST;
		currentAST = __currentAST112;
		_t = __t112;
		_t = _t->getNextSibling();
		comp_assign_expr_AST = RefDNode(currentAST.root);
		comp_assign_expr_AST=RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(astFactory->create(ASSIGN,":=")))->add(antlr::RefAST(RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(astFactory->create(EQ_OP,"eq")))->add(antlr::RefAST(l3_AST))->add(antlr::RefAST(r3_AST))))))->add(antlr::RefAST(l3_AST))));
		currentAST.root = comp_assign_expr_AST;
		if ( comp_assign_expr_AST!=RefDNode(antlr::nullAST) &&
			comp_assign_expr_AST->getFirstChild() != RefDNode(antlr::nullAST) )
			  currentAST.child = comp_assign_expr_AST->getFirstChild();
		else
			currentAST.child = comp_assign_expr_AST;
		currentAST.advanceChildToEnd();
		break;
	}
	case GE_OP_EQ:
	{
		RefDNode __t113 = _t;
		a4 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		RefDNode a4_AST_in = RefDNode(antlr::nullAST);
		a4_AST = astFactory->create(antlr::RefAST(a4));
		antlr::ASTPair __currentAST113 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),GE_OP_EQ);
		_t = _t->getFirstChild();
		l4 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		lassign_expr(_t);
		_t = _retTree;
		l4_AST = returnAST;
		r4 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		expr(_t);
		_t = _retTree;
		r4_AST = returnAST;
		currentAST = __currentAST113;
		_t = __t113;
		_t = _t->getNextSibling();
		comp_assign_expr_AST = RefDNode(currentAST.root);
		comp_assign_expr_AST=RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(astFactory->create(ASSIGN,":=")))->add(antlr::RefAST(RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(astFactory->create(GE_OP,"ge")))->add(antlr::RefAST(l4_AST))->add(antlr::RefAST(r4_AST))))))->add(antlr::RefAST(l4_AST))));
		currentAST.root = comp_assign_expr_AST;
		if ( comp_assign_expr_AST!=RefDNode(antlr::nullAST) &&
			comp_assign_expr_AST->getFirstChild() != RefDNode(antlr::nullAST) )
			  currentAST.child = comp_assign_expr_AST->getFirstChild();
		else
			currentAST.child = comp_assign_expr_AST;
		currentAST.advanceChildToEnd();
		break;
	}
	case GTMARK_EQ:
	{
		RefDNode __t114 = _t;
		a5 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		RefDNode a5_AST_in = RefDNode(antlr::nullAST);
		a5_AST = astFactory->create(antlr::RefAST(a5));
		antlr::ASTPair __currentAST114 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),GTMARK_EQ);
		_t = _t->getFirstChild();
		l5 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		lassign_expr(_t);
		_t = _retTree;
		l5_AST = returnAST;
		r5 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		expr(_t);
		_t = _retTree;
		r5_AST = returnAST;
		currentAST = __currentAST114;
		_t = __t114;
		_t = _t->getNextSibling();
		comp_assign_expr_AST = RefDNode(currentAST.root);
		comp_assign_expr_AST=RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(astFactory->create(ASSIGN,":=")))->add(antlr::RefAST(RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(astFactory->create(GTMARK,">")))->add(antlr::RefAST(l5_AST))->add(antlr::RefAST(r5_AST))))))->add(antlr::RefAST(l5_AST))));
		currentAST.root = comp_assign_expr_AST;
		if ( comp_assign_expr_AST!=RefDNode(antlr::nullAST) &&
			comp_assign_expr_AST->getFirstChild() != RefDNode(antlr::nullAST) )
			  currentAST.child = comp_assign_expr_AST->getFirstChild();
		else
			currentAST.child = comp_assign_expr_AST;
		currentAST.advanceChildToEnd();
		break;
	}
	case GT_OP_EQ:
	{
		RefDNode __t115 = _t;
		a6 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		RefDNode a6_AST_in = RefDNode(antlr::nullAST);
		a6_AST = astFactory->create(antlr::RefAST(a6));
		antlr::ASTPair __currentAST115 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),GT_OP_EQ);
		_t = _t->getFirstChild();
		l6 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		lassign_expr(_t);
		_t = _retTree;
		l6_AST = returnAST;
		r6 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		expr(_t);
		_t = _retTree;
		r6_AST = returnAST;
		currentAST = __currentAST115;
		_t = __t115;
		_t = _t->getNextSibling();
		comp_assign_expr_AST = RefDNode(currentAST.root);
		comp_assign_expr_AST=RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(astFactory->create(ASSIGN,":=")))->add(antlr::RefAST(RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(astFactory->create(GT_OP,"gt")))->add(antlr::RefAST(l6_AST))->add(antlr::RefAST(r6_AST))))))->add(antlr::RefAST(l6_AST))));
		currentAST.root = comp_assign_expr_AST;
		if ( comp_assign_expr_AST!=RefDNode(antlr::nullAST) &&
			comp_assign_expr_AST->getFirstChild() != RefDNode(antlr::nullAST) )
			  currentAST.child = comp_assign_expr_AST->getFirstChild();
		else
			currentAST.child = comp_assign_expr_AST;
		currentAST.advanceChildToEnd();
		break;
	}
	case LE_OP_EQ:
	{
		RefDNode __t116 = _t;
		a7 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		RefDNode a7_AST_in = RefDNode(antlr::nullAST);
		a7_AST = astFactory->create(antlr::RefAST(a7));
		antlr::ASTPair __currentAST116 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),LE_OP_EQ);
		_t = _t->getFirstChild();
		l7 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		lassign_expr(_t);
		_t = _retTree;
		l7_AST = returnAST;
		r7 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		expr(_t);
		_t = _retTree;
		r7_AST = returnAST;
		currentAST = __currentAST116;
		_t = __t116;
		_t = _t->getNextSibling();
		comp_assign_expr_AST = RefDNode(currentAST.root);
		comp_assign_expr_AST=RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(astFactory->create(ASSIGN,":=")))->add(antlr::RefAST(RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(astFactory->create(LE_OP,"le")))->add(antlr::RefAST(l7_AST))->add(antlr::RefAST(r7_AST))))))->add(antlr::RefAST(l7_AST))));
		currentAST.root = comp_assign_expr_AST;
		if ( comp_assign_expr_AST!=RefDNode(antlr::nullAST) &&
			comp_assign_expr_AST->getFirstChild() != RefDNode(antlr::nullAST) )
			  currentAST.child = comp_assign_expr_AST->getFirstChild();
		else
			currentAST.child = comp_assign_expr_AST;
		currentAST.advanceChildToEnd();
		break;
	}
	case LTMARK_EQ:
	{
		RefDNode __t117 = _t;
		a8 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		RefDNode a8_AST_in = RefDNode(antlr::nullAST);
		a8_AST = astFactory->create(antlr::RefAST(a8));
		antlr::ASTPair __currentAST117 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),LTMARK_EQ);
		_t = _t->getFirstChild();
		l8 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		lassign_expr(_t);
		_t = _retTree;
		l8_AST = returnAST;
		r8 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		expr(_t);
		_t = _retTree;
		r8_AST = returnAST;
		currentAST = __currentAST117;
		_t = __t117;
		_t = _t->getNextSibling();
		comp_assign_expr_AST = RefDNode(currentAST.root);
		comp_assign_expr_AST=RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(astFactory->create(ASSIGN,":=")))->add(antlr::RefAST(RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(astFactory->create(LTMARK,"<")))->add(antlr::RefAST(l8_AST))->add(antlr::RefAST(r8_AST))))))->add(antlr::RefAST(l8_AST))));
		currentAST.root = comp_assign_expr_AST;
		if ( comp_assign_expr_AST!=RefDNode(antlr::nullAST) &&
			comp_assign_expr_AST->getFirstChild() != RefDNode(antlr::nullAST) )
			  currentAST.child = comp_assign_expr_AST->getFirstChild();
		else
			currentAST.child = comp_assign_expr_AST;
		currentAST.advanceChildToEnd();
		break;
	}
	case LT_OP_EQ:
	{
		RefDNode __t118 = _t;
		a9 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		RefDNode a9_AST_in = RefDNode(antlr::nullAST);
		a9_AST = astFactory->create(antlr::RefAST(a9));
		antlr::ASTPair __currentAST118 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),LT_OP_EQ);
		_t = _t->getFirstChild();
		l9 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		lassign_expr(_t);
		_t = _retTree;
		l9_AST = returnAST;
		r9 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		expr(_t);
		_t = _retTree;
		r9_AST = returnAST;
		currentAST = __currentAST118;
		_t = __t118;
		_t = _t->getNextSibling();
		comp_assign_expr_AST = RefDNode(currentAST.root);
		comp_assign_expr_AST=RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(astFactory->create(ASSIGN,":=")))->add(antlr::RefAST(RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(astFactory->create(LT_OP,"lt")))->add(antlr::RefAST(l9_AST))->add(antlr::RefAST(r9_AST))))))->add(antlr::RefAST(l9_AST))));
		currentAST.root = comp_assign_expr_AST;
		if ( comp_assign_expr_AST!=RefDNode(antlr::nullAST) &&
			comp_assign_expr_AST->getFirstChild() != RefDNode(antlr::nullAST) )
			  currentAST.child = comp_assign_expr_AST->getFirstChild();
		else
			currentAST.child = comp_assign_expr_AST;
		currentAST.advanceChildToEnd();
		break;
	}
	case MATRIX_OP1_EQ:
	{
		RefDNode __t119 = _t;
		a10 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		RefDNode a10_AST_in = RefDNode(antlr::nullAST);
		a10_AST = astFactory->create(antlr::RefAST(a10));
		antlr::ASTPair __currentAST119 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),MATRIX_OP1_EQ);
		_t = _t->getFirstChild();
		l10 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		lassign_expr(_t);
		_t = _retTree;
		l10_AST = returnAST;
		r10 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		expr(_t);
		_t = _retTree;
		r10_AST = returnAST;
		currentAST = __currentAST119;
		_t = __t119;
		_t = _t->getNextSibling();
		comp_assign_expr_AST = RefDNode(currentAST.root);
		comp_assign_expr_AST=RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(astFactory->create(ASSIGN,":=")))->add(antlr::RefAST(RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(astFactory->create(MATRIX_OP1,"#")))->add(antlr::RefAST(l10_AST))->add(antlr::RefAST(r10_AST))))))->add(antlr::RefAST(l10_AST))));
		currentAST.root = comp_assign_expr_AST;
		if ( comp_assign_expr_AST!=RefDNode(antlr::nullAST) &&
			comp_assign_expr_AST->getFirstChild() != RefDNode(antlr::nullAST) )
			  currentAST.child = comp_assign_expr_AST->getFirstChild();
		else
			currentAST.child = comp_assign_expr_AST;
		currentAST.advanceChildToEnd();
		break;
	}
	case MATRIX_OP2_EQ:
	{
		RefDNode __t120 = _t;
		a11 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		RefDNode a11_AST_in = RefDNode(antlr::nullAST);
		a11_AST = astFactory->create(antlr::RefAST(a11));
		antlr::ASTPair __currentAST120 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),MATRIX_OP2_EQ);
		_t = _t->getFirstChild();
		l11 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		lassign_expr(_t);
		_t = _retTree;
		l11_AST = returnAST;
		r11 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		expr(_t);
		_t = _retTree;
		r11_AST = returnAST;
		currentAST = __currentAST120;
		_t = __t120;
		_t = _t->getNextSibling();
		comp_assign_expr_AST = RefDNode(currentAST.root);
		comp_assign_expr_AST=RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(astFactory->create(ASSIGN,":=")))->add(antlr::RefAST(RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(astFactory->create(MATRIX_OP2,"##")))->add(antlr::RefAST(l11_AST))->add(antlr::RefAST(r11_AST))))))->add(antlr::RefAST(l11_AST))));
		currentAST.root = comp_assign_expr_AST;
		if ( comp_assign_expr_AST!=RefDNode(antlr::nullAST) &&
			comp_assign_expr_AST->getFirstChild() != RefDNode(antlr::nullAST) )
			  currentAST.child = comp_assign_expr_AST->getFirstChild();
		else
			currentAST.child = comp_assign_expr_AST;
		currentAST.advanceChildToEnd();
		break;
	}
	case MINUS_EQ:
	{
		RefDNode __t121 = _t;
		a12 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		RefDNode a12_AST_in = RefDNode(antlr::nullAST);
		a12_AST = astFactory->create(antlr::RefAST(a12));
		antlr::ASTPair __currentAST121 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),MINUS_EQ);
		_t = _t->getFirstChild();
		l12 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		lassign_expr(_t);
		_t = _retTree;
		l12_AST = returnAST;
		r12 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		expr(_t);
		_t = _retTree;
		r12_AST = returnAST;
		currentAST = __currentAST121;
		_t = __t121;
		_t = _t->getNextSibling();
		comp_assign_expr_AST = RefDNode(currentAST.root);
		comp_assign_expr_AST=RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(astFactory->create(ASSIGN,":=")))->add(antlr::RefAST(RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(astFactory->create(MINUS,"-")))->add(antlr::RefAST(l12_AST))->add(antlr::RefAST(r12_AST))))))->add(antlr::RefAST(l12_AST))));
		currentAST.root = comp_assign_expr_AST;
		if ( comp_assign_expr_AST!=RefDNode(antlr::nullAST) &&
			comp_assign_expr_AST->getFirstChild() != RefDNode(antlr::nullAST) )
			  currentAST.child = comp_assign_expr_AST->getFirstChild();
		else
			currentAST.child = comp_assign_expr_AST;
		currentAST.advanceChildToEnd();
		break;
	}
	case MOD_OP_EQ:
	{
		RefDNode __t122 = _t;
		a13 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		RefDNode a13_AST_in = RefDNode(antlr::nullAST);
		a13_AST = astFactory->create(antlr::RefAST(a13));
		antlr::ASTPair __currentAST122 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),MOD_OP_EQ);
		_t = _t->getFirstChild();
		l13 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		lassign_expr(_t);
		_t = _retTree;
		l13_AST = returnAST;
		r13 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		expr(_t);
		_t = _retTree;
		r13_AST = returnAST;
		currentAST = __currentAST122;
		_t = __t122;
		_t = _t->getNextSibling();
		comp_assign_expr_AST = RefDNode(currentAST.root);
		comp_assign_expr_AST=RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(astFactory->create(ASSIGN,":=")))->add(antlr::RefAST(RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(astFactory->create(MOD_OP,"mod")))->add(antlr::RefAST(l13_AST))->add(antlr::RefAST(r13_AST))))))->add(antlr::RefAST(l13_AST))));
		currentAST.root = comp_assign_expr_AST;
		if ( comp_assign_expr_AST!=RefDNode(antlr::nullAST) &&
			comp_assign_expr_AST->getFirstChild() != RefDNode(antlr::nullAST) )
			  currentAST.child = comp_assign_expr_AST->getFirstChild();
		else
			currentAST.child = comp_assign_expr_AST;
		currentAST.advanceChildToEnd();
		break;
	}
	case NE_OP_EQ:
	{
		RefDNode __t123 = _t;
		a14 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		RefDNode a14_AST_in = RefDNode(antlr::nullAST);
		a14_AST = astFactory->create(antlr::RefAST(a14));
		antlr::ASTPair __currentAST123 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),NE_OP_EQ);
		_t = _t->getFirstChild();
		l14 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		lassign_expr(_t);
		_t = _retTree;
		l14_AST = returnAST;
		r14 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		expr(_t);
		_t = _retTree;
		r14_AST = returnAST;
		currentAST = __currentAST123;
		_t = __t123;
		_t = _t->getNextSibling();
		comp_assign_expr_AST = RefDNode(currentAST.root);
		comp_assign_expr_AST=RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(astFactory->create(ASSIGN,":=")))->add(antlr::RefAST(RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(astFactory->create(NE_OP,"ne")))->add(antlr::RefAST(l14_AST))->add(antlr::RefAST(r14_AST))))))->add(antlr::RefAST(l14_AST))));
		currentAST.root = comp_assign_expr_AST;
		if ( comp_assign_expr_AST!=RefDNode(antlr::nullAST) &&
			comp_assign_expr_AST->getFirstChild() != RefDNode(antlr::nullAST) )
			  currentAST.child = comp_assign_expr_AST->getFirstChild();
		else
			currentAST.child = comp_assign_expr_AST;
		currentAST.advanceChildToEnd();
		break;
	}
	case OR_OP_EQ:
	{
		RefDNode __t124 = _t;
		a15 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		RefDNode a15_AST_in = RefDNode(antlr::nullAST);
		a15_AST = astFactory->create(antlr::RefAST(a15));
		antlr::ASTPair __currentAST124 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),OR_OP_EQ);
		_t = _t->getFirstChild();
		l15 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		lassign_expr(_t);
		_t = _retTree;
		l15_AST = returnAST;
		r15 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		expr(_t);
		_t = _retTree;
		r15_AST = returnAST;
		currentAST = __currentAST124;
		_t = __t124;
		_t = _t->getNextSibling();
		comp_assign_expr_AST = RefDNode(currentAST.root);
		comp_assign_expr_AST=RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(astFactory->create(ASSIGN,":=")))->add(antlr::RefAST(RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(astFactory->create(OR_OP,"or")))->add(antlr::RefAST(l15_AST))->add(antlr::RefAST(r15_AST))))))->add(antlr::RefAST(l15_AST))));
		currentAST.root = comp_assign_expr_AST;
		if ( comp_assign_expr_AST!=RefDNode(antlr::nullAST) &&
			comp_assign_expr_AST->getFirstChild() != RefDNode(antlr::nullAST) )
			  currentAST.child = comp_assign_expr_AST->getFirstChild();
		else
			currentAST.child = comp_assign_expr_AST;
		currentAST.advanceChildToEnd();
		break;
	}
	case PLUS_EQ:
	{
		RefDNode __t125 = _t;
		a16 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		RefDNode a16_AST_in = RefDNode(antlr::nullAST);
		a16_AST = astFactory->create(antlr::RefAST(a16));
		antlr::ASTPair __currentAST125 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),PLUS_EQ);
		_t = _t->getFirstChild();
		l16 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		lassign_expr(_t);
		_t = _retTree;
		l16_AST = returnAST;
		r16 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		expr(_t);
		_t = _retTree;
		r16_AST = returnAST;
		currentAST = __currentAST125;
		_t = __t125;
		_t = _t->getNextSibling();
		comp_assign_expr_AST = RefDNode(currentAST.root);
		comp_assign_expr_AST=RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(astFactory->create(ASSIGN,":=")))->add(antlr::RefAST(RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(astFactory->create(PLUS,"+")))->add(antlr::RefAST(l16_AST))->add(antlr::RefAST(r16_AST))))))->add(antlr::RefAST(l16_AST))));
		currentAST.root = comp_assign_expr_AST;
		if ( comp_assign_expr_AST!=RefDNode(antlr::nullAST) &&
			comp_assign_expr_AST->getFirstChild() != RefDNode(antlr::nullAST) )
			  currentAST.child = comp_assign_expr_AST->getFirstChild();
		else
			currentAST.child = comp_assign_expr_AST;
		currentAST.advanceChildToEnd();
		break;
	}
	case POW_EQ:
	{
		RefDNode __t126 = _t;
		a17 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		RefDNode a17_AST_in = RefDNode(antlr::nullAST);
		a17_AST = astFactory->create(antlr::RefAST(a17));
		antlr::ASTPair __currentAST126 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),POW_EQ);
		_t = _t->getFirstChild();
		l17 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		lassign_expr(_t);
		_t = _retTree;
		l17_AST = returnAST;
		r17 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		expr(_t);
		_t = _retTree;
		r17_AST = returnAST;
		currentAST = __currentAST126;
		_t = __t126;
		_t = _t->getNextSibling();
		comp_assign_expr_AST = RefDNode(currentAST.root);
		comp_assign_expr_AST=RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(astFactory->create(ASSIGN,":=")))->add(antlr::RefAST(RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(astFactory->create(POW,"^")))->add(antlr::RefAST(l17_AST))->add(antlr::RefAST(r17_AST))))))->add(antlr::RefAST(l17_AST))));
		currentAST.root = comp_assign_expr_AST;
		if ( comp_assign_expr_AST!=RefDNode(antlr::nullAST) &&
			comp_assign_expr_AST->getFirstChild() != RefDNode(antlr::nullAST) )
			  currentAST.child = comp_assign_expr_AST->getFirstChild();
		else
			currentAST.child = comp_assign_expr_AST;
		currentAST.advanceChildToEnd();
		break;
	}
	case SLASH_EQ:
	{
		RefDNode __t127 = _t;
		a18 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		RefDNode a18_AST_in = RefDNode(antlr::nullAST);
		a18_AST = astFactory->create(antlr::RefAST(a18));
		antlr::ASTPair __currentAST127 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),SLASH_EQ);
		_t = _t->getFirstChild();
		l18 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		lassign_expr(_t);
		_t = _retTree;
		l18_AST = returnAST;
		r18 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		expr(_t);
		_t = _retTree;
		r18_AST = returnAST;
		currentAST = __currentAST127;
		_t = __t127;
		_t = _t->getNextSibling();
		comp_assign_expr_AST = RefDNode(currentAST.root);
		comp_assign_expr_AST=RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(astFactory->create(ASSIGN,":=")))->add(antlr::RefAST(RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(astFactory->create(SLASH,"/")))->add(antlr::RefAST(l18_AST))->add(antlr::RefAST(r18_AST))))))->add(antlr::RefAST(l18_AST))));
		currentAST.root = comp_assign_expr_AST;
		if ( comp_assign_expr_AST!=RefDNode(antlr::nullAST) &&
			comp_assign_expr_AST->getFirstChild() != RefDNode(antlr::nullAST) )
			  currentAST.child = comp_assign_expr_AST->getFirstChild();
		else
			currentAST.child = comp_assign_expr_AST;
		currentAST.advanceChildToEnd();
		break;
	}
	case XOR_OP_EQ:
	{
		RefDNode __t128 = _t;
		a19 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		RefDNode a19_AST_in = RefDNode(antlr::nullAST);
		a19_AST = astFactory->create(antlr::RefAST(a19));
		antlr::ASTPair __currentAST128 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),XOR_OP_EQ);
		_t = _t->getFirstChild();
		l19 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		lassign_expr(_t);
		_t = _retTree;
		l19_AST = returnAST;
		r19 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		expr(_t);
		_t = _retTree;
		r19_AST = returnAST;
		currentAST = __currentAST128;
		_t = __t128;
		_t = _t->getNextSibling();
		comp_assign_expr_AST = RefDNode(currentAST.root);
		comp_assign_expr_AST=RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(astFactory->create(ASSIGN,":=")))->add(antlr::RefAST(RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(astFactory->create(XOR_OP,"xor")))->add(antlr::RefAST(l19_AST))->add(antlr::RefAST(r19_AST))))))->add(antlr::RefAST(l19_AST))));
		currentAST.root = comp_assign_expr_AST;
		if ( comp_assign_expr_AST!=RefDNode(antlr::nullAST) &&
			comp_assign_expr_AST->getFirstChild() != RefDNode(antlr::nullAST) )
			  currentAST.child = comp_assign_expr_AST->getFirstChild();
		else
			currentAST.child = comp_assign_expr_AST;
		currentAST.advanceChildToEnd();
		break;
	}
	default:
	{
		throw antlr::NoViableAltException(antlr::RefAST(_t));
	}
	}
	returnAST = comp_assign_expr_AST;
	_retTree = _t;
}

void GDLTreeParser::procedure_call(RefDNode _t) {
	RefDNode procedure_call_AST_in = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	returnAST = RefDNode(antlr::nullAST);
	antlr::ASTPair currentAST;
	RefDNode procedure_call_AST = RefDNode(antlr::nullAST);
	RefDNode p = RefDNode(antlr::nullAST);
	RefDNode p_AST = RefDNode(antlr::nullAST);
	RefDNode id = RefDNode(antlr::nullAST);
	RefDNode id_AST = RefDNode(antlr::nullAST);
	
	if (_t == RefDNode(antlr::nullAST) )
		_t = ASTNULL;
	switch ( _t->getType()) {
	case MPCALL:
	{
		RefDNode __t74 = _t;
		RefDNode tmp20_AST = RefDNode(antlr::nullAST);
		RefDNode tmp20_AST_in = RefDNode(antlr::nullAST);
		tmp20_AST = astFactory->create(antlr::RefAST(_t));
		tmp20_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp20_AST));
		antlr::ASTPair __currentAST74 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),MPCALL);
		_t = _t->getFirstChild();
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		RefDNode tmp21_AST = RefDNode(antlr::nullAST);
		RefDNode tmp21_AST_in = RefDNode(antlr::nullAST);
		tmp21_AST = astFactory->create(antlr::RefAST(_t));
		tmp21_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp21_AST));
		match(antlr::RefAST(_t),IDENTIFIER);
		_t = _t->getNextSibling();
		{ // ( ... )*
		for (;;) {
			if (_t == RefDNode(antlr::nullAST) )
				_t = ASTNULL;
			if ((_tokenSet_1.member(_t->getType()))) {
				parameter_def(_t);
				_t = _retTree;
				astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
			}
			else {
				goto _loop76;
			}
			
		}
		_loop76:;
		} // ( ... )*
		currentAST = __currentAST74;
		_t = __t74;
		_t = _t->getNextSibling();
		procedure_call_AST = RefDNode(currentAST.root);
		break;
	}
	case MPCALL_PARENT:
	{
		RefDNode __t77 = _t;
		RefDNode tmp22_AST = RefDNode(antlr::nullAST);
		RefDNode tmp22_AST_in = RefDNode(antlr::nullAST);
		tmp22_AST = astFactory->create(antlr::RefAST(_t));
		tmp22_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp22_AST));
		antlr::ASTPair __currentAST77 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),MPCALL_PARENT);
		_t = _t->getFirstChild();
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		RefDNode tmp23_AST = RefDNode(antlr::nullAST);
		RefDNode tmp23_AST_in = RefDNode(antlr::nullAST);
		tmp23_AST = astFactory->create(antlr::RefAST(_t));
		tmp23_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp23_AST));
		match(antlr::RefAST(_t),IDENTIFIER);
		_t = _t->getNextSibling();
		RefDNode tmp24_AST = RefDNode(antlr::nullAST);
		RefDNode tmp24_AST_in = RefDNode(antlr::nullAST);
		tmp24_AST = astFactory->create(antlr::RefAST(_t));
		tmp24_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp24_AST));
		match(antlr::RefAST(_t),IDENTIFIER);
		_t = _t->getNextSibling();
		{ // ( ... )*
		for (;;) {
			if (_t == RefDNode(antlr::nullAST) )
				_t = ASTNULL;
			if ((_tokenSet_1.member(_t->getType()))) {
				parameter_def(_t);
				_t = _retTree;
				astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
			}
			else {
				goto _loop79;
			}
			
		}
		_loop79:;
		} // ( ... )*
		currentAST = __currentAST77;
		_t = __t77;
		_t = _t->getNextSibling();
		procedure_call_AST = RefDNode(currentAST.root);
		break;
	}
	case PCALL:
	{
		RefDNode __t80 = _t;
		p = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		RefDNode p_AST_in = RefDNode(antlr::nullAST);
		p_AST = astFactory->create(antlr::RefAST(p));
		astFactory->addASTChild(currentAST, antlr::RefAST(p_AST));
		antlr::ASTPair __currentAST80 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),PCALL);
		_t = _t->getFirstChild();
		id = _t;
		RefDNode id_AST_in = RefDNode(antlr::nullAST);
		id_AST = astFactory->create(antlr::RefAST(id));
		astFactory->addASTChild(currentAST, antlr::RefAST(id_AST));
		match(antlr::RefAST(_t),IDENTIFIER);
		_t = _t->getNextSibling();
		{ // ( ... )*
		for (;;) {
			if (_t == RefDNode(antlr::nullAST) )
				_t = ASTNULL;
			if ((_tokenSet_1.member(_t->getType()))) {
				parameter_def(_t);
				_t = _retTree;
				astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
			}
			else {
				goto _loop82;
			}
			
		}
		_loop82:;
		} // ( ... )*
		
		// first search library procedures
		int i=LibProIx(id_AST->getText());
		if( i != -1)
		{
		p_AST->setType(PCALL_LIB);
		p_AST->setText("pcall_lib");
		id_AST->SetProIx(i);
		}
		else
		{
		// then search user defined procedures
		i=ProIx(id_AST->getText());
		id_AST->SetProIx(i);
		}
		
		currentAST = __currentAST80;
		_t = __t80;
		_t = _t->getNextSibling();
		procedure_call_AST = RefDNode(currentAST.root);
		break;
	}
	default:
	{
		throw antlr::NoViableAltException(antlr::RefAST(_t));
	}
	}
	returnAST = procedure_call_AST;
	_retTree = _t;
}

void GDLTreeParser::for_statement(RefDNode _t) {
	RefDNode for_statement_AST_in = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	returnAST = RefDNode(antlr::nullAST);
	antlr::ASTPair currentAST;
	RefDNode for_statement_AST = RefDNode(antlr::nullAST);
	RefDNode f = RefDNode(antlr::nullAST);
	RefDNode f_AST = RefDNode(antlr::nullAST);
	RefDNode i = RefDNode(antlr::nullAST);
	RefDNode i_AST = RefDNode(antlr::nullAST);
	
	int labelStart = comp.NDefLabel();
	
	
	RefDNode __t61 = _t;
	f = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	RefDNode f_AST_in = RefDNode(antlr::nullAST);
	f_AST = astFactory->create(antlr::RefAST(f));
	astFactory->addASTChild(currentAST, antlr::RefAST(f_AST));
	antlr::ASTPair __currentAST61 = currentAST;
	currentAST.root = currentAST.child;
	currentAST.child = RefDNode(antlr::nullAST);
	match(antlr::RefAST(_t),FOR);
	_t = _t->getFirstChild();
	i = _t;
	RefDNode i_AST_in = RefDNode(antlr::nullAST);
	i_AST = astFactory->create(antlr::RefAST(i));
	astFactory->addASTChild(currentAST, antlr::RefAST(i_AST));
	match(antlr::RefAST(_t),IDENTIFIER);
	_t = _t->getNextSibling();
	
	i_AST->setType(VAR);
	comp.Var(i_AST);	
	
	expr(_t);
	_t = _retTree;
	astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
	expr(_t);
	_t = _retTree;
	astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
	{
	if (_t == RefDNode(antlr::nullAST) )
		_t = ASTNULL;
	switch ( _t->getType()) {
	case ASSIGN:
	case ARRAYDEF:
	case ARRAYEXPR:
	case ARRAYEXPR_FN:
	case CONSTANT:
	case DEREF:
	case EXPR:
	case FCALL:
	case MFCALL:
	case MFCALL_PARENT:
	case NSTRUC_REF:
	case POSTDEC:
	case POSTINC:
	case STRUC:
	case SYSVAR:
	case UMINUS:
	case VAR:
	case DEC:
	case INC:
	case AND_OP_EQ:
	case ASTERIX_EQ:
	case EQ_OP_EQ:
	case GE_OP_EQ:
	case GTMARK_EQ:
	case GT_OP_EQ:
	case LE_OP_EQ:
	case LTMARK_EQ:
	case LT_OP_EQ:
	case MATRIX_OP1_EQ:
	case MATRIX_OP2_EQ:
	case MINUS_EQ:
	case MOD_OP_EQ:
	case NE_OP_EQ:
	case OR_OP_EQ:
	case PLUS_EQ:
	case POW_EQ:
	case SLASH_EQ:
	case XOR_OP_EQ:
	case SLASH:
	case ASTERIX:
	case DOT:
	case POW:
	case MATRIX_OP1:
	case MATRIX_OP2:
	case MOD_OP:
	case PLUS:
	case MINUS:
	case LTMARK:
	case GTMARK:
	case NOT_OP:
	case EQ_OP:
	case NE_OP:
	case LE_OP:
	case LT_OP:
	case GE_OP:
	case GT_OP:
	case AND_OP:
	case OR_OP:
	case XOR_OP:
	case LOG_AND:
	case LOG_OR:
	case LOG_NEG:
	case QUESTION:
	{
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		
		f_AST->setType(FOR_STEP);
		f_AST->setText("for_step");
		
		break;
	}
	case BLOCK:
	{
		break;
	}
	default:
	{
		throw antlr::NoViableAltException(antlr::RefAST(_t));
	}
	}
	}
	block(_t);
	_t = _retTree;
	astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
	currentAST = __currentAST61;
	_t = __t61;
	_t = _t->getNextSibling();
	
	f_AST->SetLabelRange( labelStart, comp.NDefLabel());
	
	for_statement_AST = RefDNode(currentAST.root);
	returnAST = for_statement_AST;
	_retTree = _t;
}

void GDLTreeParser::repeat_statement(RefDNode _t) {
	RefDNode repeat_statement_AST_in = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	returnAST = RefDNode(antlr::nullAST);
	antlr::ASTPair currentAST;
	RefDNode repeat_statement_AST = RefDNode(antlr::nullAST);
	RefDNode r = RefDNode(antlr::nullAST);
	RefDNode r_AST = RefDNode(antlr::nullAST);
	
	int labelStart = comp.NDefLabel();
	
	
	RefDNode __t57 = _t;
	r = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	RefDNode r_AST_in = RefDNode(antlr::nullAST);
	r_AST = astFactory->create(antlr::RefAST(r));
	astFactory->addASTChild(currentAST, antlr::RefAST(r_AST));
	antlr::ASTPair __currentAST57 = currentAST;
	currentAST.root = currentAST.child;
	currentAST.child = RefDNode(antlr::nullAST);
	match(antlr::RefAST(_t),REPEAT);
	_t = _t->getFirstChild();
	block(_t);
	_t = _retTree;
	astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
	expr(_t);
	_t = _retTree;
	astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
	currentAST = __currentAST57;
	_t = __t57;
	_t = _t->getNextSibling();
	
	r_AST->SetLabelRange( labelStart, comp.NDefLabel());
	
	repeat_statement_AST = RefDNode(currentAST.root);
	returnAST = repeat_statement_AST;
	_retTree = _t;
}

void GDLTreeParser::while_statement(RefDNode _t) {
	RefDNode while_statement_AST_in = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	returnAST = RefDNode(antlr::nullAST);
	antlr::ASTPair currentAST;
	RefDNode while_statement_AST = RefDNode(antlr::nullAST);
	RefDNode w = RefDNode(antlr::nullAST);
	RefDNode w_AST = RefDNode(antlr::nullAST);
	RefDNode e_AST = RefDNode(antlr::nullAST);
	RefDNode e = RefDNode(antlr::nullAST);
	RefDNode s_AST = RefDNode(antlr::nullAST);
	RefDNode s = RefDNode(antlr::nullAST);
	
	int labelStart = comp.NDefLabel();
	
	
	RefDNode __t59 = _t;
	w = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	RefDNode w_AST_in = RefDNode(antlr::nullAST);
	w_AST = astFactory->create(antlr::RefAST(w));
	antlr::ASTPair __currentAST59 = currentAST;
	currentAST.root = currentAST.child;
	currentAST.child = RefDNode(antlr::nullAST);
	match(antlr::RefAST(_t),WHILE);
	_t = _t->getFirstChild();
	e = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	expr(_t);
	_t = _retTree;
	e_AST = returnAST;
	s = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	statement(_t);
	_t = _retTree;
	s_AST = returnAST;
	currentAST = __currentAST59;
	_t = __t59;
	_t = _t->getNextSibling();
	while_statement_AST = RefDNode(currentAST.root);
	
	w_AST->SetLabelRange( labelStart, comp.NDefLabel());
	
	// swap e <-> s for easier access in interpreter
	while_statement_AST=RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(w_AST))->add(antlr::RefAST(s_AST))->add(antlr::RefAST(e_AST))));
	
	currentAST.root = while_statement_AST;
	if ( while_statement_AST!=RefDNode(antlr::nullAST) &&
		while_statement_AST->getFirstChild() != RefDNode(antlr::nullAST) )
		  currentAST.child = while_statement_AST->getFirstChild();
	else
		currentAST.child = while_statement_AST;
	currentAST.advanceChildToEnd();
	returnAST = while_statement_AST;
	_retTree = _t;
}

void GDLTreeParser::jump_statement(RefDNode _t) {
	RefDNode jump_statement_AST_in = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	returnAST = RefDNode(antlr::nullAST);
	antlr::ASTPair currentAST;
	RefDNode jump_statement_AST = RefDNode(antlr::nullAST);
	RefDNode i1 = RefDNode(antlr::nullAST);
	RefDNode i1_AST = RefDNode(antlr::nullAST);
	RefDNode e_AST = RefDNode(antlr::nullAST);
	RefDNode e = RefDNode(antlr::nullAST);
	RefDNode i2 = RefDNode(antlr::nullAST);
	RefDNode i2_AST = RefDNode(antlr::nullAST);
	
	if (_t == RefDNode(antlr::nullAST) )
		_t = ASTNULL;
	switch ( _t->getType()) {
	case GOTO:
	{
		RefDNode __t66 = _t;
		RefDNode tmp25_AST = RefDNode(antlr::nullAST);
		RefDNode tmp25_AST_in = RefDNode(antlr::nullAST);
		tmp25_AST = astFactory->create(antlr::RefAST(_t));
		tmp25_AST_in = _t;
		antlr::ASTPair __currentAST66 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),GOTO);
		_t = _t->getFirstChild();
		i1 = _t;
		RefDNode i1_AST_in = RefDNode(antlr::nullAST);
		i1_AST = astFactory->create(antlr::RefAST(i1));
		match(antlr::RefAST(_t),IDENTIFIER);
		_t = _t->getNextSibling();
		currentAST = __currentAST66;
		_t = __t66;
		_t = _t->getNextSibling();
		jump_statement_AST = RefDNode(currentAST.root);
		
			  jump_statement_AST=astFactory->create(GOTO,i1->getText());
		//	  #jump_statement=#[GOTO,i1->getText()]; // doesn't work
			  comp.Goto(jump_statement_AST); 
			
		currentAST.root = jump_statement_AST;
		if ( jump_statement_AST!=RefDNode(antlr::nullAST) &&
			jump_statement_AST->getFirstChild() != RefDNode(antlr::nullAST) )
			  currentAST.child = jump_statement_AST->getFirstChild();
		else
			currentAST.child = jump_statement_AST;
		currentAST.advanceChildToEnd();
		break;
	}
	case RETURN:
	{
		RefDNode __t67 = _t;
		RefDNode tmp26_AST = RefDNode(antlr::nullAST);
		RefDNode tmp26_AST_in = RefDNode(antlr::nullAST);
		tmp26_AST = astFactory->create(antlr::RefAST(_t));
		tmp26_AST_in = _t;
		antlr::ASTPair __currentAST67 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),RETURN);
		_t = _t->getFirstChild();
		bool exprThere=false;
		{
		if (_t == RefDNode(antlr::nullAST) )
			_t = ASTNULL;
		switch ( _t->getType()) {
		case ASSIGN:
		case ARRAYDEF:
		case ARRAYEXPR:
		case ARRAYEXPR_FN:
		case CONSTANT:
		case DEREF:
		case EXPR:
		case FCALL:
		case MFCALL:
		case MFCALL_PARENT:
		case NSTRUC_REF:
		case POSTDEC:
		case POSTINC:
		case STRUC:
		case SYSVAR:
		case UMINUS:
		case VAR:
		case DEC:
		case INC:
		case AND_OP_EQ:
		case ASTERIX_EQ:
		case EQ_OP_EQ:
		case GE_OP_EQ:
		case GTMARK_EQ:
		case GT_OP_EQ:
		case LE_OP_EQ:
		case LTMARK_EQ:
		case LT_OP_EQ:
		case MATRIX_OP1_EQ:
		case MATRIX_OP2_EQ:
		case MINUS_EQ:
		case MOD_OP_EQ:
		case NE_OP_EQ:
		case OR_OP_EQ:
		case PLUS_EQ:
		case POW_EQ:
		case SLASH_EQ:
		case XOR_OP_EQ:
		case SLASH:
		case ASTERIX:
		case DOT:
		case POW:
		case MATRIX_OP1:
		case MATRIX_OP2:
		case MOD_OP:
		case PLUS:
		case MINUS:
		case LTMARK:
		case GTMARK:
		case NOT_OP:
		case EQ_OP:
		case NE_OP:
		case LE_OP:
		case LT_OP:
		case GE_OP:
		case GT_OP:
		case AND_OP:
		case OR_OP:
		case XOR_OP:
		case LOG_AND:
		case LOG_OR:
		case LOG_NEG:
		case QUESTION:
		{
			e = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
			expr(_t);
			_t = _retTree;
			e_AST = returnAST;
			exprThere=true;
			break;
		}
		case 3:
		{
			break;
		}
		default:
		{
			throw antlr::NoViableAltException(antlr::RefAST(_t));
		}
		}
		}
		currentAST = __currentAST67;
		_t = __t67;
		_t = _t->getNextSibling();
		jump_statement_AST = RefDNode(currentAST.root);
		
			  if( comp.IsFun())
			  	{
				if( !exprThere)	throw GDLException(	_t, 
		"Return statement in functions "
		"must have 1 value.");
				jump_statement_AST=RefDNode(astFactory->make((new antlr::ASTArray(2))->add(antlr::RefAST(astFactory->create(RETF,"retf")))->add(antlr::RefAST(e_AST))));
				}
			  else
			  	{
				if( exprThere) throw GDLException(	_t, 
		"Return statement in "
		"procedures cannot have values.");
				jump_statement_AST=astFactory->create(RETP,"retp"); // astFactory.create(RETP,"retp");
			  	}
			
		currentAST.root = jump_statement_AST;
		if ( jump_statement_AST!=RefDNode(antlr::nullAST) &&
			jump_statement_AST->getFirstChild() != RefDNode(antlr::nullAST) )
			  currentAST.child = jump_statement_AST->getFirstChild();
		else
			currentAST.child = jump_statement_AST;
		currentAST.advanceChildToEnd();
		break;
	}
	case ON_IOERROR:
	{
		RefDNode __t69 = _t;
		RefDNode tmp27_AST = RefDNode(antlr::nullAST);
		RefDNode tmp27_AST_in = RefDNode(antlr::nullAST);
		tmp27_AST = astFactory->create(antlr::RefAST(_t));
		tmp27_AST_in = _t;
		antlr::ASTPair __currentAST69 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),ON_IOERROR);
		_t = _t->getFirstChild();
		i2 = _t;
		RefDNode i2_AST_in = RefDNode(antlr::nullAST);
		i2_AST = astFactory->create(antlr::RefAST(i2));
		match(antlr::RefAST(_t),IDENTIFIER);
		_t = _t->getNextSibling();
		currentAST = __currentAST69;
		_t = __t69;
		_t = _t->getNextSibling();
		jump_statement_AST = RefDNode(currentAST.root);
		
		if( i2->getText() == "NULL")
		{
		jump_statement_AST=astFactory->create(ON_IOERROR_NULL,
		"on_ioerror_null");
		}
		else
		{
		jump_statement_AST=astFactory->create(ON_IOERROR,i2->getText());
		//	            #jump_statement=#[ON_IOERROR,i2->getText()];
		comp.Goto(jump_statement_AST); // same handling		 
		}
			
		currentAST.root = jump_statement_AST;
		if ( jump_statement_AST!=RefDNode(antlr::nullAST) &&
			jump_statement_AST->getFirstChild() != RefDNode(antlr::nullAST) )
			  currentAST.child = jump_statement_AST->getFirstChild();
		else
			currentAST.child = jump_statement_AST;
		currentAST.advanceChildToEnd();
		break;
	}
	default:
	{
		throw antlr::NoViableAltException(antlr::RefAST(_t));
	}
	}
	returnAST = jump_statement_AST;
	_retTree = _t;
}

void GDLTreeParser::if_statement(RefDNode _t) {
	RefDNode if_statement_AST_in = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	returnAST = RefDNode(antlr::nullAST);
	antlr::ASTPair currentAST;
	RefDNode if_statement_AST = RefDNode(antlr::nullAST);
	RefDNode i = RefDNode(antlr::nullAST);
	RefDNode i_AST = RefDNode(antlr::nullAST);
	RefDNode e_AST = RefDNode(antlr::nullAST);
	RefDNode e = RefDNode(antlr::nullAST);
	RefDNode s1_AST = RefDNode(antlr::nullAST);
	RefDNode s1 = RefDNode(antlr::nullAST);
	RefDNode s2_AST = RefDNode(antlr::nullAST);
	RefDNode s2 = RefDNode(antlr::nullAST);
	
	int labelStart = comp.NDefLabel();
	
	
	RefDNode __t71 = _t;
	i = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	RefDNode i_AST_in = RefDNode(antlr::nullAST);
	i_AST = astFactory->create(antlr::RefAST(i));
	antlr::ASTPair __currentAST71 = currentAST;
	currentAST.root = currentAST.child;
	currentAST.child = RefDNode(antlr::nullAST);
	match(antlr::RefAST(_t),IF);
	_t = _t->getFirstChild();
	e = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	expr(_t);
	_t = _retTree;
	e_AST = returnAST;
	s1 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	statement(_t);
	_t = _retTree;
	s1_AST = returnAST;
	{
	if (_t == RefDNode(antlr::nullAST) )
		_t = ASTNULL;
	switch ( _t->getType()) {
	case 3:
	{
		if_statement_AST = RefDNode(currentAST.root);
		
		if_statement_AST=RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(i_AST))->add(antlr::RefAST(e_AST))->add(antlr::RefAST(s1_AST))));
		
		currentAST.root = if_statement_AST;
		if ( if_statement_AST!=RefDNode(antlr::nullAST) &&
			if_statement_AST->getFirstChild() != RefDNode(antlr::nullAST) )
			  currentAST.child = if_statement_AST->getFirstChild();
		else
			currentAST.child = if_statement_AST;
		currentAST.advanceChildToEnd();
		break;
	}
	case ASSIGN:
	case BLOCK:
	case COMMONDECL:
	case COMMONDEF:
	case MPCALL:
	case MPCALL_PARENT:
	case PCALL:
	case RETURN:
	case SWITCH:
	case CASE:
	case FORWARD:
	case DEC:
	case INC:
	case AND_OP_EQ:
	case ASTERIX_EQ:
	case EQ_OP_EQ:
	case GE_OP_EQ:
	case GTMARK_EQ:
	case GT_OP_EQ:
	case LE_OP_EQ:
	case LTMARK_EQ:
	case LT_OP_EQ:
	case MATRIX_OP1_EQ:
	case MATRIX_OP2_EQ:
	case MINUS_EQ:
	case MOD_OP_EQ:
	case NE_OP_EQ:
	case OR_OP_EQ:
	case PLUS_EQ:
	case POW_EQ:
	case SLASH_EQ:
	case XOR_OP_EQ:
	case BREAK:
	case CONTINUE:
	case REPEAT:
	case WHILE:
	case FOR:
	case GOTO:
	case ON_IOERROR:
	case IF:
	{
		s2 = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		statement(_t);
		_t = _retTree;
		s2_AST = returnAST;
		if_statement_AST = RefDNode(currentAST.root);
		
		i_AST->setText( "if_else");
		i_AST->setType( IF_ELSE);
		if_statement_AST=RefDNode(astFactory->make((new antlr::ASTArray(4))->add(antlr::RefAST(i_AST))->add(antlr::RefAST(e_AST))->add(antlr::RefAST(s1_AST))->add(antlr::RefAST(s2_AST))));
		
		currentAST.root = if_statement_AST;
		if ( if_statement_AST!=RefDNode(antlr::nullAST) &&
			if_statement_AST->getFirstChild() != RefDNode(antlr::nullAST) )
			  currentAST.child = if_statement_AST->getFirstChild();
		else
			currentAST.child = if_statement_AST;
		currentAST.advanceChildToEnd();
		break;
	}
	default:
	{
		throw antlr::NoViableAltException(antlr::RefAST(_t));
	}
	}
	}
	currentAST = __currentAST71;
	_t = __t71;
	_t = _t->getNextSibling();
	
	i_AST->SetLabelRange( labelStart, comp.NDefLabel());
	
	returnAST = if_statement_AST;
	_retTree = _t;
}

void GDLTreeParser::parameter_def(RefDNode _t) {
	RefDNode parameter_def_AST_in = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	returnAST = RefDNode(antlr::nullAST);
	antlr::ASTPair currentAST;
	RefDNode parameter_def_AST = RefDNode(antlr::nullAST);
	RefDNode d = RefDNode(antlr::nullAST);
	RefDNode d_AST = RefDNode(antlr::nullAST);
	RefDNode i = RefDNode(antlr::nullAST);
	RefDNode i_AST = RefDNode(antlr::nullAST);
	RefDNode k_AST = RefDNode(antlr::nullAST);
	RefDNode k = RefDNode(antlr::nullAST);
	RefDNode e_AST = RefDNode(antlr::nullAST);
	RefDNode e = RefDNode(antlr::nullAST);
	
	RefDNode variable;
	RefDNode varCp;
	
	
	if (_t == RefDNode(antlr::nullAST) )
		_t = ASTNULL;
	switch ( _t->getType()) {
	case KEYDEF:
	{
		RefDNode __t84 = _t;
		d = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		RefDNode d_AST_in = RefDNode(antlr::nullAST);
		d_AST = astFactory->create(antlr::RefAST(d));
		antlr::ASTPair __currentAST84 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),KEYDEF);
		_t = _t->getFirstChild();
		i = _t;
		RefDNode i_AST_in = RefDNode(antlr::nullAST);
		i_AST = astFactory->create(antlr::RefAST(i));
		match(antlr::RefAST(_t),IDENTIFIER);
		_t = _t->getNextSibling();
		k = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		unbrace_expr(_t);
		_t = _retTree;
		k_AST = returnAST;
		parameter_def_AST = RefDNode(currentAST.root);
		
		variable=comp.ByReference(k_AST);
		if( variable != static_cast<RefDNode>(antlr::nullAST))
		{
		if( variable == k_AST)
		{
		d_AST=astFactory->create(KEYDEF_REF,"keydef_ref");
		parameter_def_AST=RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(d_AST))->add(antlr::RefAST(i_AST))->add(antlr::RefAST(variable))));
		}
		else
		{
		d_AST=astFactory->create(KEYDEF_REF_EXPR,"keydef_ref_expr");
		varCp = RefDNode(astFactory->make((new antlr::ASTArray(1))->add(antlr::RefAST(variable))));
		parameter_def_AST=RefDNode(astFactory->make((new antlr::ASTArray(4))->add(antlr::RefAST(d_AST))->add(antlr::RefAST(i_AST))->add(antlr::RefAST(k_AST))->add(antlr::RefAST(varCp))));
		}
		}
		else 
		{
		int t = k_AST->getType();
		if( t == FCALL_LIB || t == MFCALL_LIB || 
		t == MFCALL_PARENT_LIB)
		{
		d_AST=astFactory->create(KEYDEF_REF_CHECK,"keydef_ref_check");
		parameter_def_AST=RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(d_AST))->add(antlr::RefAST(i_AST))->add(antlr::RefAST(k_AST))));
		}
		else
		{
		parameter_def_AST=RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(d_AST))->add(antlr::RefAST(i_AST))->add(antlr::RefAST(k_AST))));
		}
		}
		
		currentAST.root = parameter_def_AST;
		if ( parameter_def_AST!=RefDNode(antlr::nullAST) &&
			parameter_def_AST->getFirstChild() != RefDNode(antlr::nullAST) )
			  currentAST.child = parameter_def_AST->getFirstChild();
		else
			currentAST.child = parameter_def_AST;
		currentAST.advanceChildToEnd();
		currentAST = __currentAST84;
		_t = __t84;
		_t = _t->getNextSibling();
		break;
	}
	case ASSIGN:
	case ARRAYDEF:
	case ARRAYEXPR:
	case ARRAYEXPR_FN:
	case CONSTANT:
	case DEREF:
	case EXPR:
	case FCALL:
	case MFCALL:
	case MFCALL_PARENT:
	case NSTRUC_REF:
	case POSTDEC:
	case POSTINC:
	case STRUC:
	case SYSVAR:
	case UMINUS:
	case VAR:
	case DEC:
	case INC:
	case AND_OP_EQ:
	case ASTERIX_EQ:
	case EQ_OP_EQ:
	case GE_OP_EQ:
	case GTMARK_EQ:
	case GT_OP_EQ:
	case LE_OP_EQ:
	case LTMARK_EQ:
	case LT_OP_EQ:
	case MATRIX_OP1_EQ:
	case MATRIX_OP2_EQ:
	case MINUS_EQ:
	case MOD_OP_EQ:
	case NE_OP_EQ:
	case OR_OP_EQ:
	case PLUS_EQ:
	case POW_EQ:
	case SLASH_EQ:
	case XOR_OP_EQ:
	case SLASH:
	case ASTERIX:
	case DOT:
	case POW:
	case MATRIX_OP1:
	case MATRIX_OP2:
	case MOD_OP:
	case PLUS:
	case MINUS:
	case LTMARK:
	case GTMARK:
	case NOT_OP:
	case EQ_OP:
	case NE_OP:
	case LE_OP:
	case LT_OP:
	case GE_OP:
	case GT_OP:
	case AND_OP:
	case OR_OP:
	case XOR_OP:
	case LOG_AND:
	case LOG_OR:
	case LOG_NEG:
	case QUESTION:
	{
		e = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		unbrace_expr(_t);
		_t = _retTree;
		e_AST = returnAST;
		parameter_def_AST = RefDNode(currentAST.root);
		
		variable=comp.ByReference(e_AST);
		if( variable != static_cast<RefDNode>(antlr::nullAST))
		{
		if( variable == e_AST)
		{
		d_AST=astFactory->create(REF,"ref");
		parameter_def_AST=RefDNode(astFactory->make((new antlr::ASTArray(2))->add(antlr::RefAST(d_AST))->add(antlr::RefAST(variable))));
		}
		else
		{
		d_AST=astFactory->create(REF_EXPR,"ref_expr");
		varCp = RefDNode(astFactory->make((new antlr::ASTArray(1))->add(antlr::RefAST(variable))));
		parameter_def_AST=RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(d_AST))->add(antlr::RefAST(e_AST))->add(antlr::RefAST(varCp))));
		}
		}
		else 
		{
		int t = e_AST->getType();
		if( t == FCALL_LIB || t == MFCALL_LIB || 
		t == MFCALL_PARENT_LIB)
		{
		// something like: CALLAPRO,reform(a,/OVERWRITE)
		parameter_def_AST=RefDNode(astFactory->make((new antlr::ASTArray(2))->add(antlr::RefAST(astFactory->create(REF_CHECK,"ref_check")))->add(antlr::RefAST(e_AST))));
		}
		else
		{
		parameter_def_AST=RefDNode(astFactory->make((new antlr::ASTArray(1))->add(antlr::RefAST(e_AST))));
		}
		}
		
		currentAST.root = parameter_def_AST;
		if ( parameter_def_AST!=RefDNode(antlr::nullAST) &&
			parameter_def_AST->getFirstChild() != RefDNode(antlr::nullAST) )
			  currentAST.child = parameter_def_AST->getFirstChild();
		else
			currentAST.child = parameter_def_AST;
		currentAST.advanceChildToEnd();
		break;
	}
	default:
	{
		throw antlr::NoViableAltException(antlr::RefAST(_t));
	}
	}
	returnAST = parameter_def_AST;
	_retTree = _t;
}

int  GDLTreeParser::array_def(RefDNode _t) {
	int depth;
	RefDNode array_def_AST_in = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	returnAST = RefDNode(antlr::nullAST);
	antlr::ASTPair currentAST;
	RefDNode array_def_AST = RefDNode(antlr::nullAST);
	RefDNode a = RefDNode(antlr::nullAST);
	RefDNode a_AST = RefDNode(antlr::nullAST);
	
	RefDNode sPos;
	
	
	RefDNode __t86 = _t;
	a = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	RefDNode a_AST_in = RefDNode(antlr::nullAST);
	a_AST = astFactory->create(antlr::RefAST(a));
	astFactory->addASTChild(currentAST, antlr::RefAST(a_AST));
	antlr::ASTPair __currentAST86 = currentAST;
	currentAST.root = currentAST.child;
	currentAST.child = RefDNode(antlr::nullAST);
	match(antlr::RefAST(_t),ARRAYDEF);
	_t = _t->getFirstChild();
	sPos=_t;
	{ // ( ... )*
	for (;;) {
		if (_t == RefDNode(antlr::nullAST) )
			_t = ASTNULL;
		if ((_tokenSet_2.member(_t->getType()))) {
			expr(_t);
			_t = _retTree;
			astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		}
		else {
			goto _loop88;
		}
		
	}
	_loop88:;
	} // ( ... )*
	currentAST = __currentAST86;
	_t = __t86;
	_t = _t->getNextSibling();
	
	depth=0;
	for( RefDNode e=sPos; 
	e != static_cast<RefDNode>(antlr::nullAST);
	e=e->getNextSibling())
	{
	if( e->getType() != ARRAYDEF)
	{
	depth=0;
	break;
	}
	else
	{
	int act=array_def(e); // recursive call
	act=act+1;
	if( depth == 0)
	{
	depth=act;
	}
	else
	{
	if( depth > act) depth=act;
	}
	}   
	}
	a_AST->SetArrayDepth(depth);
	
	array_def_AST = RefDNode(currentAST.root);
	returnAST = array_def_AST;
	_retTree = _t;
	return depth;
}

void GDLTreeParser::struct_def(RefDNode _t) {
	RefDNode struct_def_AST_in = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	returnAST = RefDNode(antlr::nullAST);
	antlr::ASTPair currentAST;
	RefDNode struct_def_AST = RefDNode(antlr::nullAST);
	RefDNode n = RefDNode(antlr::nullAST);
	RefDNode n_AST = RefDNode(antlr::nullAST);
	
	bool noTagName = false;
	
	
	if (_t == RefDNode(antlr::nullAST) )
		_t = ASTNULL;
	switch ( _t->getType()) {
	case NSTRUC_REF:
	{
		RefDNode __t90 = _t;
		n = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		RefDNode n_AST_in = RefDNode(antlr::nullAST);
		n_AST = astFactory->create(antlr::RefAST(n));
		astFactory->addASTChild(currentAST, antlr::RefAST(n_AST));
		antlr::ASTPair __currentAST90 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),NSTRUC_REF);
		_t = _t->getFirstChild();
		RefDNode tmp28_AST = RefDNode(antlr::nullAST);
		RefDNode tmp28_AST_in = RefDNode(antlr::nullAST);
		tmp28_AST = astFactory->create(antlr::RefAST(_t));
		tmp28_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp28_AST));
		match(antlr::RefAST(_t),IDENTIFIER);
		_t = _t->getNextSibling();
		{
		if (_t == RefDNode(antlr::nullAST) )
			_t = ASTNULL;
		switch ( _t->getType()) {
		case ASSIGN:
		case ARRAYDEF:
		case ARRAYEXPR:
		case ARRAYEXPR_FN:
		case CONSTANT:
		case DEREF:
		case EXPR:
		case FCALL:
		case MFCALL:
		case MFCALL_PARENT:
		case NSTRUC_REF:
		case POSTDEC:
		case POSTINC:
		case STRUC:
		case SYSVAR:
		case UMINUS:
		case VAR:
		case IDENTIFIER:
		case DEC:
		case INC:
		case AND_OP_EQ:
		case ASTERIX_EQ:
		case EQ_OP_EQ:
		case GE_OP_EQ:
		case GTMARK_EQ:
		case GT_OP_EQ:
		case LE_OP_EQ:
		case LTMARK_EQ:
		case LT_OP_EQ:
		case MATRIX_OP1_EQ:
		case MATRIX_OP2_EQ:
		case MINUS_EQ:
		case MOD_OP_EQ:
		case NE_OP_EQ:
		case OR_OP_EQ:
		case PLUS_EQ:
		case POW_EQ:
		case SLASH_EQ:
		case XOR_OP_EQ:
		case SLASH:
		case INHERITS:
		case ASTERIX:
		case DOT:
		case POW:
		case MATRIX_OP1:
		case MATRIX_OP2:
		case MOD_OP:
		case PLUS:
		case MINUS:
		case LTMARK:
		case GTMARK:
		case NOT_OP:
		case EQ_OP:
		case NE_OP:
		case LE_OP:
		case LT_OP:
		case GE_OP:
		case GT_OP:
		case AND_OP:
		case OR_OP:
		case XOR_OP:
		case LOG_AND:
		case LOG_OR:
		case LOG_NEG:
		case QUESTION:
		{
			{ // ( ... )+
			int _cnt93=0;
			for (;;) {
				if (_t == RefDNode(antlr::nullAST) )
					_t = ASTNULL;
				switch ( _t->getType()) {
				case ASSIGN:
				case ARRAYDEF:
				case ARRAYEXPR:
				case ARRAYEXPR_FN:
				case CONSTANT:
				case DEREF:
				case EXPR:
				case FCALL:
				case MFCALL:
				case MFCALL_PARENT:
				case NSTRUC_REF:
				case POSTDEC:
				case POSTINC:
				case STRUC:
				case SYSVAR:
				case UMINUS:
				case VAR:
				case DEC:
				case INC:
				case AND_OP_EQ:
				case ASTERIX_EQ:
				case EQ_OP_EQ:
				case GE_OP_EQ:
				case GTMARK_EQ:
				case GT_OP_EQ:
				case LE_OP_EQ:
				case LTMARK_EQ:
				case LT_OP_EQ:
				case MATRIX_OP1_EQ:
				case MATRIX_OP2_EQ:
				case MINUS_EQ:
				case MOD_OP_EQ:
				case NE_OP_EQ:
				case OR_OP_EQ:
				case PLUS_EQ:
				case POW_EQ:
				case SLASH_EQ:
				case XOR_OP_EQ:
				case SLASH:
				case ASTERIX:
				case DOT:
				case POW:
				case MATRIX_OP1:
				case MATRIX_OP2:
				case MOD_OP:
				case PLUS:
				case MINUS:
				case LTMARK:
				case GTMARK:
				case NOT_OP:
				case EQ_OP:
				case NE_OP:
				case LE_OP:
				case LT_OP:
				case GE_OP:
				case GT_OP:
				case AND_OP:
				case OR_OP:
				case XOR_OP:
				case LOG_AND:
				case LOG_OR:
				case LOG_NEG:
				case QUESTION:
				{
					expr(_t);
					_t = _retTree;
					astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
					noTagName = true;
					break;
				}
				case IDENTIFIER:
				{
					RefDNode tmp29_AST = RefDNode(antlr::nullAST);
					RefDNode tmp29_AST_in = RefDNode(antlr::nullAST);
					tmp29_AST = astFactory->create(antlr::RefAST(_t));
					tmp29_AST_in = _t;
					astFactory->addASTChild(currentAST, antlr::RefAST(tmp29_AST));
					match(antlr::RefAST(_t),IDENTIFIER);
					_t = _t->getNextSibling();
					expr(_t);
					_t = _retTree;
					astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
					break;
				}
				case INHERITS:
				{
					RefDNode tmp30_AST = RefDNode(antlr::nullAST);
					RefDNode tmp30_AST_in = RefDNode(antlr::nullAST);
					tmp30_AST = astFactory->create(antlr::RefAST(_t));
					tmp30_AST_in = _t;
					astFactory->addASTChild(currentAST, antlr::RefAST(tmp30_AST));
					match(antlr::RefAST(_t),INHERITS);
					_t = _t->getNextSibling();
					RefDNode tmp31_AST = RefDNode(antlr::nullAST);
					RefDNode tmp31_AST_in = RefDNode(antlr::nullAST);
					tmp31_AST = astFactory->create(antlr::RefAST(_t));
					tmp31_AST_in = _t;
					astFactory->addASTChild(currentAST, antlr::RefAST(tmp31_AST));
					match(antlr::RefAST(_t),IDENTIFIER);
					_t = _t->getNextSibling();
					break;
				}
				default:
				{
					if ( _cnt93>=1 ) { goto _loop93; } else {throw antlr::NoViableAltException(antlr::RefAST(_t));}
				}
				}
				_cnt93++;
			}
			_loop93:;
			}  // ( ... )+
			
			// set to nstruct if defined here
			n_AST->setType(NSTRUC); 
			n_AST->setText("nstruct");
			n_AST->DefinedStruct( noTagName);
			
			break;
		}
		case 3:
		{
			break;
		}
		default:
		{
			throw antlr::NoViableAltException(antlr::RefAST(_t));
		}
		}
		}
		currentAST = __currentAST90;
		_t = __t90;
		_t = _t->getNextSibling();
		struct_def_AST = RefDNode(currentAST.root);
		break;
	}
	case STRUC:
	{
		RefDNode __t94 = _t;
		RefDNode tmp32_AST = RefDNode(antlr::nullAST);
		RefDNode tmp32_AST_in = RefDNode(antlr::nullAST);
		tmp32_AST = astFactory->create(antlr::RefAST(_t));
		tmp32_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp32_AST));
		antlr::ASTPair __currentAST94 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),STRUC);
		_t = _t->getFirstChild();
		{ // ( ... )+
		int _cnt96=0;
		for (;;) {
			if (_t == RefDNode(antlr::nullAST) )
				_t = ASTNULL;
			if ((_t->getType() == IDENTIFIER)) {
				tag_def(_t);
				_t = _retTree;
				astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
			}
			else {
				if ( _cnt96>=1 ) { goto _loop96; } else {throw antlr::NoViableAltException(antlr::RefAST(_t));}
			}
			
			_cnt96++;
		}
		_loop96:;
		}  // ( ... )+
		currentAST = __currentAST94;
		_t = __t94;
		_t = _t->getNextSibling();
		struct_def_AST = RefDNode(currentAST.root);
		break;
	}
	default:
	{
		throw antlr::NoViableAltException(antlr::RefAST(_t));
	}
	}
	returnAST = struct_def_AST;
	_retTree = _t;
}

void GDLTreeParser::tag_def(RefDNode _t) {
	RefDNode tag_def_AST_in = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	returnAST = RefDNode(antlr::nullAST);
	antlr::ASTPair currentAST;
	RefDNode tag_def_AST = RefDNode(antlr::nullAST);
	
	RefDNode tmp33_AST = RefDNode(antlr::nullAST);
	RefDNode tmp33_AST_in = RefDNode(antlr::nullAST);
	tmp33_AST = astFactory->create(antlr::RefAST(_t));
	tmp33_AST_in = _t;
	astFactory->addASTChild(currentAST, antlr::RefAST(tmp33_AST));
	match(antlr::RefAST(_t),IDENTIFIER);
	_t = _t->getNextSibling();
	expr(_t);
	_t = _retTree;
	astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
	tag_def_AST = RefDNode(currentAST.root);
	returnAST = tag_def_AST;
	_retTree = _t;
}

void GDLTreeParser::arrayindex_list(RefDNode _t) {
	RefDNode arrayindex_list_AST_in = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	returnAST = RefDNode(antlr::nullAST);
	antlr::ASTPair currentAST;
	RefDNode arrayindex_list_AST = RefDNode(antlr::nullAST);
	
	{ // ( ... )+
	int _cnt100=0;
	for (;;) {
		if (_t == RefDNode(antlr::nullAST) )
			_t = ASTNULL;
		if ((_t->getType() == ARRAYIX)) {
			arrayindex(_t);
			_t = _retTree;
			astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		}
		else {
			if ( _cnt100>=1 ) { goto _loop100; } else {throw antlr::NoViableAltException(antlr::RefAST(_t));}
		}
		
		_cnt100++;
	}
	_loop100:;
	}  // ( ... )+
	arrayindex_list_AST = RefDNode(currentAST.root);
	returnAST = arrayindex_list_AST;
	_retTree = _t;
}

void GDLTreeParser::arrayindex(RefDNode _t) {
	RefDNode arrayindex_AST_in = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	returnAST = RefDNode(antlr::nullAST);
	antlr::ASTPair currentAST;
	RefDNode arrayindex_AST = RefDNode(antlr::nullAST);
	RefDNode ax = RefDNode(antlr::nullAST);
	RefDNode ax_AST = RefDNode(antlr::nullAST);
	
	RefDNode __t102 = _t;
	ax = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	RefDNode ax_AST_in = RefDNode(antlr::nullAST);
	ax_AST = astFactory->create(antlr::RefAST(ax));
	astFactory->addASTChild(currentAST, antlr::RefAST(ax_AST));
	antlr::ASTPair __currentAST102 = currentAST;
	currentAST.root = currentAST.child;
	currentAST.child = RefDNode(antlr::nullAST);
	match(antlr::RefAST(_t),ARRAYIX);
	_t = _t->getFirstChild();
	{
	if (_t == RefDNode(antlr::nullAST) )
		_t = ASTNULL;
	switch ( _t->getType()) {
	case ALL:
	{
		RefDNode tmp34_AST = RefDNode(antlr::nullAST);
		RefDNode tmp34_AST_in = RefDNode(antlr::nullAST);
		tmp34_AST = astFactory->create(antlr::RefAST(_t));
		tmp34_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp34_AST));
		match(antlr::RefAST(_t),ALL);
		_t = _t->getNextSibling();
		
		ax_AST->setType(ARRAYIX_ALL); // 1
		ax_AST->setText("*");
		
		break;
	}
	case ASSIGN:
	case ARRAYDEF:
	case ARRAYEXPR:
	case ARRAYEXPR_FN:
	case CONSTANT:
	case DEREF:
	case EXPR:
	case FCALL:
	case MFCALL:
	case MFCALL_PARENT:
	case NSTRUC_REF:
	case POSTDEC:
	case POSTINC:
	case STRUC:
	case SYSVAR:
	case UMINUS:
	case VAR:
	case DEC:
	case INC:
	case AND_OP_EQ:
	case ASTERIX_EQ:
	case EQ_OP_EQ:
	case GE_OP_EQ:
	case GTMARK_EQ:
	case GT_OP_EQ:
	case LE_OP_EQ:
	case LTMARK_EQ:
	case LT_OP_EQ:
	case MATRIX_OP1_EQ:
	case MATRIX_OP2_EQ:
	case MINUS_EQ:
	case MOD_OP_EQ:
	case NE_OP_EQ:
	case OR_OP_EQ:
	case PLUS_EQ:
	case POW_EQ:
	case SLASH_EQ:
	case XOR_OP_EQ:
	case SLASH:
	case ASTERIX:
	case DOT:
	case POW:
	case MATRIX_OP1:
	case MATRIX_OP2:
	case MOD_OP:
	case PLUS:
	case MINUS:
	case LTMARK:
	case GTMARK:
	case NOT_OP:
	case EQ_OP:
	case NE_OP:
	case LE_OP:
	case LT_OP:
	case GE_OP:
	case GT_OP:
	case AND_OP:
	case OR_OP:
	case XOR_OP:
	case LOG_AND:
	case LOG_OR:
	case LOG_NEG:
	case QUESTION:
	{
		{
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		{
		if (_t == RefDNode(antlr::nullAST) )
			_t = ASTNULL;
		switch ( _t->getType()) {
		case ALL:
		{
			RefDNode tmp35_AST = RefDNode(antlr::nullAST);
			RefDNode tmp35_AST_in = RefDNode(antlr::nullAST);
			tmp35_AST = astFactory->create(antlr::RefAST(_t));
			tmp35_AST_in = _t;
			match(antlr::RefAST(_t),ALL);
			_t = _t->getNextSibling();
			
			ax_AST->setType(ARRAYIX_ORANGE); // 3
			ax_AST->setText("s:*");
			
			break;
		}
		case ASSIGN:
		case ARRAYDEF:
		case ARRAYEXPR:
		case ARRAYEXPR_FN:
		case CONSTANT:
		case DEREF:
		case EXPR:
		case FCALL:
		case MFCALL:
		case MFCALL_PARENT:
		case NSTRUC_REF:
		case POSTDEC:
		case POSTINC:
		case STRUC:
		case SYSVAR:
		case UMINUS:
		case VAR:
		case DEC:
		case INC:
		case AND_OP_EQ:
		case ASTERIX_EQ:
		case EQ_OP_EQ:
		case GE_OP_EQ:
		case GTMARK_EQ:
		case GT_OP_EQ:
		case LE_OP_EQ:
		case LTMARK_EQ:
		case LT_OP_EQ:
		case MATRIX_OP1_EQ:
		case MATRIX_OP2_EQ:
		case MINUS_EQ:
		case MOD_OP_EQ:
		case NE_OP_EQ:
		case OR_OP_EQ:
		case PLUS_EQ:
		case POW_EQ:
		case SLASH_EQ:
		case XOR_OP_EQ:
		case SLASH:
		case ASTERIX:
		case DOT:
		case POW:
		case MATRIX_OP1:
		case MATRIX_OP2:
		case MOD_OP:
		case PLUS:
		case MINUS:
		case LTMARK:
		case GTMARK:
		case NOT_OP:
		case EQ_OP:
		case NE_OP:
		case LE_OP:
		case LT_OP:
		case GE_OP:
		case GT_OP:
		case AND_OP:
		case OR_OP:
		case XOR_OP:
		case LOG_AND:
		case LOG_OR:
		case LOG_NEG:
		case QUESTION:
		{
			expr(_t);
			_t = _retTree;
			astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
			
			ax_AST->setType(ARRAYIX_RANGE); // 4
			ax_AST->setText("s:e");
			
			break;
		}
		case 3:
		{
			break;
		}
		default:
		{
			throw antlr::NoViableAltException(antlr::RefAST(_t));
		}
		}
		}
		}
		break;
	}
	default:
	{
		throw antlr::NoViableAltException(antlr::RefAST(_t));
	}
	}
	}
	currentAST = __currentAST102;
	_t = __t102;
	_t = _t->getNextSibling();
	arrayindex_AST = RefDNode(currentAST.root);
	returnAST = arrayindex_AST;
	_retTree = _t;
}

void GDLTreeParser::lassign_expr(RefDNode _t) {
	RefDNode lassign_expr_AST_in = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	returnAST = RefDNode(antlr::nullAST);
	antlr::ASTPair currentAST;
	RefDNode lassign_expr_AST = RefDNode(antlr::nullAST);
	RefDNode ex_AST = RefDNode(antlr::nullAST);
	RefDNode ex = RefDNode(antlr::nullAST);
	
	ex = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	expr(_t);
	_t = _retTree;
	ex_AST = returnAST;
	lassign_expr_AST = RefDNode(currentAST.root);
	
	// remove last pair of braces
				if( ex_AST->getType()==EXPR)
	{
	int cT = ex_AST->getFirstChild()->getType();
	if( cT != FCALL && 
	cT != MFCALL && 
	cT != MFCALL_PARENT &&
	cT != FCALL_LIB && 
	cT != MFCALL_LIB && 
	cT != MFCALL_PARENT_LIB)
	ex_AST=ex_AST->getFirstChild();
	}
	
	if( ex_AST->getType()==ASSIGN)
	throw GDLException(	_t, "Assign expression is not allowed as "
	"l-expression in assignment");
	
	lassign_expr_AST=RefDNode(astFactory->make((new antlr::ASTArray(1))->add(antlr::RefAST(ex_AST))));
			
	currentAST.root = lassign_expr_AST;
	if ( lassign_expr_AST!=RefDNode(antlr::nullAST) &&
		lassign_expr_AST->getFirstChild() != RefDNode(antlr::nullAST) )
		  currentAST.child = lassign_expr_AST->getFirstChild();
	else
		currentAST.child = lassign_expr_AST;
	currentAST.advanceChildToEnd();
	returnAST = lassign_expr_AST;
	_retTree = _t;
}

void GDLTreeParser::sysvar(RefDNode _t) {
	RefDNode sysvar_AST_in = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	returnAST = RefDNode(antlr::nullAST);
	antlr::ASTPair currentAST;
	RefDNode sysvar_AST = RefDNode(antlr::nullAST);
	RefDNode i = RefDNode(antlr::nullAST);
	RefDNode i_AST = RefDNode(antlr::nullAST);
	
	RefDNode __t130 = _t;
	RefDNode tmp36_AST = RefDNode(antlr::nullAST);
	RefDNode tmp36_AST_in = RefDNode(antlr::nullAST);
	tmp36_AST = astFactory->create(antlr::RefAST(_t));
	tmp36_AST_in = _t;
	antlr::ASTPair __currentAST130 = currentAST;
	currentAST.root = currentAST.child;
	currentAST.child = RefDNode(antlr::nullAST);
	match(antlr::RefAST(_t),SYSVAR);
	_t = _t->getFirstChild();
	i = _t;
	RefDNode i_AST_in = RefDNode(antlr::nullAST);
	i_AST = astFactory->create(antlr::RefAST(i));
	match(antlr::RefAST(_t),SYSVARNAME);
	_t = _t->getNextSibling();
	currentAST = __currentAST130;
	_t = __t130;
	_t = _t->getNextSibling();
	sysvar_AST = RefDNode(currentAST.root);
	
	std::string sysVarName = i->getText();
	// here we create the real sysvar node      
		  sysvar_AST=astFactory->create(SYSVAR, sysVarName.substr(1));
	//	  #sysvar=#[SYSVAR,i->getText()];
		  comp.SysVar(sysvar_AST); // sets var to NULL
		
	currentAST.root = sysvar_AST;
	if ( sysvar_AST!=RefDNode(antlr::nullAST) &&
		sysvar_AST->getFirstChild() != RefDNode(antlr::nullAST) )
		  currentAST.child = sysvar_AST->getFirstChild();
	else
		currentAST.child = sysvar_AST;
	currentAST.advanceChildToEnd();
	returnAST = sysvar_AST;
	_retTree = _t;
}

void GDLTreeParser::var(RefDNode _t) {
	RefDNode var_AST_in = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	returnAST = RefDNode(antlr::nullAST);
	antlr::ASTPair currentAST;
	RefDNode var_AST = RefDNode(antlr::nullAST);
	RefDNode i = RefDNode(antlr::nullAST);
	RefDNode i_AST = RefDNode(antlr::nullAST);
	
	RefDNode __t132 = _t;
	RefDNode tmp37_AST = RefDNode(antlr::nullAST);
	RefDNode tmp37_AST_in = RefDNode(antlr::nullAST);
	tmp37_AST = astFactory->create(antlr::RefAST(_t));
	tmp37_AST_in = _t;
	antlr::ASTPair __currentAST132 = currentAST;
	currentAST.root = currentAST.child;
	currentAST.child = RefDNode(antlr::nullAST);
	match(antlr::RefAST(_t),VAR);
	_t = _t->getFirstChild();
	i = _t;
	RefDNode i_AST_in = RefDNode(antlr::nullAST);
	i_AST = astFactory->create(antlr::RefAST(i));
	match(antlr::RefAST(_t),IDENTIFIER);
	_t = _t->getNextSibling();
	currentAST = __currentAST132;
	_t = __t132;
	_t = _t->getNextSibling();
	var_AST = RefDNode(currentAST.root);
	
		  var_AST=astFactory->create(VAR,i->getText());
	//	  #var=#[VAR,i->getText()];
		  comp.Var(var_AST);	
		
	currentAST.root = var_AST;
	if ( var_AST!=RefDNode(antlr::nullAST) &&
		var_AST->getFirstChild() != RefDNode(antlr::nullAST) )
		  currentAST.child = var_AST->getFirstChild();
	else
		currentAST.child = var_AST;
	currentAST.advanceChildToEnd();
	returnAST = var_AST;
	_retTree = _t;
}

void GDLTreeParser::brace_expr(RefDNode _t) {
	RefDNode brace_expr_AST_in = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	returnAST = RefDNode(antlr::nullAST);
	antlr::ASTPair currentAST;
	RefDNode brace_expr_AST = RefDNode(antlr::nullAST);
	RefDNode e = RefDNode(antlr::nullAST);
	RefDNode e_AST = RefDNode(antlr::nullAST);
	RefDNode ex_AST = RefDNode(antlr::nullAST);
	RefDNode ex = RefDNode(antlr::nullAST);
	
	RefDNode __t134 = _t;
	e = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	RefDNode e_AST_in = RefDNode(antlr::nullAST);
	e_AST = astFactory->create(antlr::RefAST(e));
	antlr::ASTPair __currentAST134 = currentAST;
	currentAST.root = currentAST.child;
	currentAST.child = RefDNode(antlr::nullAST);
	match(antlr::RefAST(_t),EXPR);
	_t = _t->getFirstChild();
	ex = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	expr(_t);
	_t = _retTree;
	ex_AST = returnAST;
	currentAST = __currentAST134;
	_t = __t134;
	_t = _t->getNextSibling();
	brace_expr_AST = RefDNode(currentAST.root);
	
	// remove multiple braces
				while( ex_AST->getType()==EXPR) ex_AST=ex_AST->getFirstChild();
		  		brace_expr_AST=RefDNode(astFactory->make((new antlr::ASTArray(2))->add(antlr::RefAST(e_AST))->add(antlr::RefAST(ex_AST))));
			
	currentAST.root = brace_expr_AST;
	if ( brace_expr_AST!=RefDNode(antlr::nullAST) &&
		brace_expr_AST->getFirstChild() != RefDNode(antlr::nullAST) )
		  currentAST.child = brace_expr_AST->getFirstChild();
	else
		currentAST.child = brace_expr_AST;
	currentAST.advanceChildToEnd();
	returnAST = brace_expr_AST;
	_retTree = _t;
}

void GDLTreeParser::arrayindex_list_to_expression_list(RefDNode _t) {
	RefDNode arrayindex_list_to_expression_list_AST_in = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	returnAST = RefDNode(antlr::nullAST);
	antlr::ASTPair currentAST;
	RefDNode arrayindex_list_to_expression_list_AST = RefDNode(antlr::nullAST);
	RefDNode e_AST = RefDNode(antlr::nullAST);
	RefDNode e = RefDNode(antlr::nullAST);
	
	RefDNode variable;
	
	
	{ // ( ... )+
	int _cnt138=0;
	for (;;) {
		if (_t == RefDNode(antlr::nullAST) )
			_t = ASTNULL;
		if ((_t->getType() == ARRAYIX)) {
			RefDNode __t137 = _t;
			RefDNode tmp38_AST = RefDNode(antlr::nullAST);
			RefDNode tmp38_AST_in = RefDNode(antlr::nullAST);
			tmp38_AST = astFactory->create(antlr::RefAST(_t));
			tmp38_AST_in = _t;
			antlr::ASTPair __currentAST137 = currentAST;
			currentAST.root = currentAST.child;
			currentAST.child = RefDNode(antlr::nullAST);
			match(antlr::RefAST(_t),ARRAYIX);
			_t = _t->getFirstChild();
			e = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
			expr(_t);
			_t = _retTree;
			e_AST = returnAST;
			currentAST = __currentAST137;
			_t = __t137;
			_t = _t->getNextSibling();
			arrayindex_list_to_expression_list_AST = RefDNode(currentAST.root);
			
			variable = comp.ByReference(e_AST);
			if( variable != static_cast<RefDNode>(antlr::nullAST))
			{ 
			e_AST=RefDNode(astFactory->make((new antlr::ASTArray(2))->add(antlr::RefAST(astFactory->create(REF,"ref")))->add(antlr::RefAST(variable))));
			}
			
			arrayindex_list_to_expression_list_AST=
			RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(NULL))->add(antlr::RefAST(arrayindex_list_to_expression_list_AST))->add(antlr::RefAST(e_AST))));
			
			currentAST.root = arrayindex_list_to_expression_list_AST;
			if ( arrayindex_list_to_expression_list_AST!=RefDNode(antlr::nullAST) &&
				arrayindex_list_to_expression_list_AST->getFirstChild() != RefDNode(antlr::nullAST) )
				  currentAST.child = arrayindex_list_to_expression_list_AST->getFirstChild();
			else
				currentAST.child = arrayindex_list_to_expression_list_AST;
			currentAST.advanceChildToEnd();
		}
		else {
			if ( _cnt138>=1 ) { goto _loop138; } else {throw antlr::NoViableAltException(antlr::RefAST(_t));}
		}
		
		_cnt138++;
	}
	_loop138:;
	}  // ( ... )+
	returnAST = arrayindex_list_to_expression_list_AST;
	_retTree = _t;
}

void GDLTreeParser::arrayexpr_fn(RefDNode _t) {
	RefDNode arrayexpr_fn_AST_in = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	returnAST = RefDNode(antlr::nullAST);
	antlr::ASTPair currentAST;
	RefDNode arrayexpr_fn_AST = RefDNode(antlr::nullAST);
	RefDNode va = RefDNode(antlr::nullAST);
	RefDNode va_AST = RefDNode(antlr::nullAST);
	RefDNode id = RefDNode(antlr::nullAST);
	RefDNode id_AST = RefDNode(antlr::nullAST);
	RefDNode al_AST = RefDNode(antlr::nullAST);
	RefDNode al = RefDNode(antlr::nullAST);
	RefDNode el_AST = RefDNode(antlr::nullAST);
	RefDNode el = RefDNode(antlr::nullAST);
	
	std::string id_text;
	bool isVar;
	
	
	RefDNode __t140 = _t;
	RefDNode tmp39_AST = RefDNode(antlr::nullAST);
	RefDNode tmp39_AST_in = RefDNode(antlr::nullAST);
	tmp39_AST = astFactory->create(antlr::RefAST(_t));
	tmp39_AST_in = _t;
	antlr::ASTPair __currentAST140 = currentAST;
	currentAST.root = currentAST.child;
	currentAST.child = RefDNode(antlr::nullAST);
	match(antlr::RefAST(_t),ARRAYEXPR_FN);
	_t = _t->getFirstChild();
	RefDNode __t141 = _t;
	va = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	RefDNode va_AST_in = RefDNode(antlr::nullAST);
	va_AST = astFactory->create(antlr::RefAST(va));
	antlr::ASTPair __currentAST141 = currentAST;
	currentAST.root = currentAST.child;
	currentAST.child = RefDNode(antlr::nullAST);
	match(antlr::RefAST(_t),VAR);
	_t = _t->getFirstChild();
	id = _t;
	RefDNode id_AST_in = RefDNode(antlr::nullAST);
	id_AST = astFactory->create(antlr::RefAST(id));
	match(antlr::RefAST(_t),IDENTIFIER);
	_t = _t->getNextSibling();
	currentAST = __currentAST141;
	_t = __t141;
	_t = _t->getNextSibling();
	
	id_text=id_AST->getText(); 
	isVar = comp.IsVar( id_text);
	
	{
	if (_t == RefDNode(antlr::nullAST) )
		_t = ASTNULL;
	if (((_t->getType() == ARRAYIX))&&( isVar)) {
		al = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		arrayindex_list(_t);
		_t = _retTree;
		al_AST = returnAST;
	}
	else if ((_t->getType() == ARRAYIX)) {
		el = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		arrayindex_list_to_expression_list(_t);
		_t = _retTree;
		el_AST = returnAST;
	}
	else {
		throw antlr::NoViableAltException(antlr::RefAST(_t));
	}
	
	}
	arrayexpr_fn_AST = RefDNode(currentAST.root);
	
	if( !isVar)
	{   // no variable -> function call
	
	// first search library functions
	int i=LibFunIx(id_text);
	if( i != -1)
	{
	id_AST->SetFunIx(i);
	arrayexpr_fn_AST=
	RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(astFactory->create(FCALL_LIB,"fcall_lib")))->add(antlr::RefAST(id_AST))->add(antlr::RefAST(el_AST))));
	}
	else
	{
	// then search user defined functions
	i=FunIx(id_text);
	id_AST->SetFunIx(i);
	
	arrayexpr_fn_AST=
	RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(astFactory->create(FCALL,"fcall")))->add(antlr::RefAST(id_AST))->add(antlr::RefAST(el_AST))));
	}
	}
	else
	{   // variable -> arrayexpr
	
	// make var
	va_AST=astFactory->create(VAR,id_AST->getText());
	//                    #va=#[VAR,id->getText()];
	comp.Var(va_AST);	
	
	arrayexpr_fn_AST=
	RefDNode(astFactory->make((new antlr::ASTArray(3))->add(antlr::RefAST(astFactory->create(ARRAYEXPR,"arrayexpr")))->add(antlr::RefAST(al_AST))->add(antlr::RefAST(va_AST))));
	}
	
	currentAST.root = arrayexpr_fn_AST;
	if ( arrayexpr_fn_AST!=RefDNode(antlr::nullAST) &&
		arrayexpr_fn_AST->getFirstChild() != RefDNode(antlr::nullAST) )
		  currentAST.child = arrayexpr_fn_AST->getFirstChild();
	else
		currentAST.child = arrayexpr_fn_AST;
	currentAST.advanceChildToEnd();
	currentAST = __currentAST140;
	_t = __t140;
	_t = _t->getNextSibling();
	returnAST = arrayexpr_fn_AST;
	_retTree = _t;
}

void GDLTreeParser::primary_expr(RefDNode _t) {
	RefDNode primary_expr_AST_in = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	returnAST = RefDNode(antlr::nullAST);
	antlr::ASTPair currentAST;
	RefDNode primary_expr_AST = RefDNode(antlr::nullAST);
	RefDNode f = RefDNode(antlr::nullAST);
	RefDNode f_AST = RefDNode(antlr::nullAST);
	RefDNode id = RefDNode(antlr::nullAST);
	RefDNode id_AST = RefDNode(antlr::nullAST);
	
	int dummy;
	
	
	if (_t == RefDNode(antlr::nullAST) )
		_t = ASTNULL;
	switch ( _t->getType()) {
	case ASSIGN:
	{
		assign_expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		primary_expr_AST = RefDNode(currentAST.root);
		break;
	}
	case AND_OP_EQ:
	case ASTERIX_EQ:
	case EQ_OP_EQ:
	case GE_OP_EQ:
	case GTMARK_EQ:
	case GT_OP_EQ:
	case LE_OP_EQ:
	case LTMARK_EQ:
	case LT_OP_EQ:
	case MATRIX_OP1_EQ:
	case MATRIX_OP2_EQ:
	case MINUS_EQ:
	case MOD_OP_EQ:
	case NE_OP_EQ:
	case OR_OP_EQ:
	case PLUS_EQ:
	case POW_EQ:
	case SLASH_EQ:
	case XOR_OP_EQ:
	{
		comp_assign_expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		primary_expr_AST = RefDNode(currentAST.root);
		break;
	}
	case MFCALL:
	{
		RefDNode __t144 = _t;
		RefDNode tmp40_AST = RefDNode(antlr::nullAST);
		RefDNode tmp40_AST_in = RefDNode(antlr::nullAST);
		tmp40_AST = astFactory->create(antlr::RefAST(_t));
		tmp40_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp40_AST));
		antlr::ASTPair __currentAST144 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),MFCALL);
		_t = _t->getFirstChild();
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		RefDNode tmp41_AST = RefDNode(antlr::nullAST);
		RefDNode tmp41_AST_in = RefDNode(antlr::nullAST);
		tmp41_AST = astFactory->create(antlr::RefAST(_t));
		tmp41_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp41_AST));
		match(antlr::RefAST(_t),IDENTIFIER);
		_t = _t->getNextSibling();
		{ // ( ... )*
		for (;;) {
			if (_t == RefDNode(antlr::nullAST) )
				_t = ASTNULL;
			if ((_tokenSet_1.member(_t->getType()))) {
				parameter_def(_t);
				_t = _retTree;
				astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
			}
			else {
				goto _loop146;
			}
			
		}
		_loop146:;
		} // ( ... )*
		currentAST = __currentAST144;
		_t = __t144;
		_t = _t->getNextSibling();
		primary_expr_AST = RefDNode(currentAST.root);
		break;
	}
	case MFCALL_PARENT:
	{
		RefDNode __t147 = _t;
		RefDNode tmp42_AST = RefDNode(antlr::nullAST);
		RefDNode tmp42_AST_in = RefDNode(antlr::nullAST);
		tmp42_AST = astFactory->create(antlr::RefAST(_t));
		tmp42_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp42_AST));
		antlr::ASTPair __currentAST147 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),MFCALL_PARENT);
		_t = _t->getFirstChild();
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		RefDNode tmp43_AST = RefDNode(antlr::nullAST);
		RefDNode tmp43_AST_in = RefDNode(antlr::nullAST);
		tmp43_AST = astFactory->create(antlr::RefAST(_t));
		tmp43_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp43_AST));
		match(antlr::RefAST(_t),IDENTIFIER);
		_t = _t->getNextSibling();
		RefDNode tmp44_AST = RefDNode(antlr::nullAST);
		RefDNode tmp44_AST_in = RefDNode(antlr::nullAST);
		tmp44_AST = astFactory->create(antlr::RefAST(_t));
		tmp44_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp44_AST));
		match(antlr::RefAST(_t),IDENTIFIER);
		_t = _t->getNextSibling();
		{ // ( ... )*
		for (;;) {
			if (_t == RefDNode(antlr::nullAST) )
				_t = ASTNULL;
			if ((_tokenSet_1.member(_t->getType()))) {
				parameter_def(_t);
				_t = _retTree;
				astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
			}
			else {
				goto _loop149;
			}
			
		}
		_loop149:;
		} // ( ... )*
		currentAST = __currentAST147;
		_t = __t147;
		_t = _t->getNextSibling();
		primary_expr_AST = RefDNode(currentAST.root);
		break;
	}
	case FCALL:
	{
		RefDNode __t150 = _t;
		f = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
		RefDNode f_AST_in = RefDNode(antlr::nullAST);
		f_AST = astFactory->create(antlr::RefAST(f));
		astFactory->addASTChild(currentAST, antlr::RefAST(f_AST));
		antlr::ASTPair __currentAST150 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),FCALL);
		_t = _t->getFirstChild();
		id = _t;
		RefDNode id_AST_in = RefDNode(antlr::nullAST);
		id_AST = astFactory->create(antlr::RefAST(id));
		astFactory->addASTChild(currentAST, antlr::RefAST(id_AST));
		match(antlr::RefAST(_t),IDENTIFIER);
		_t = _t->getNextSibling();
		{ // ( ... )*
		for (;;) {
			if (_t == RefDNode(antlr::nullAST) )
				_t = ASTNULL;
			if ((_tokenSet_1.member(_t->getType()))) {
				parameter_def(_t);
				_t = _retTree;
				astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
			}
			else {
				goto _loop152;
			}
			
		}
		_loop152:;
		} // ( ... )*
		
		// first search library functions
		int i=LibFunIx(id->getText());
		if( i != -1)
		{
		f_AST->setType(FCALL_LIB);
		f_AST->setText("fcall_lib");
		id_AST->SetFunIx(i);
		}
		else
		{
		// then search user defined functions
		i=FunIx(id_AST->getText());
		id_AST->SetFunIx(i);
		}
		
		currentAST = __currentAST150;
		_t = __t150;
		_t = _t->getNextSibling();
		primary_expr_AST = RefDNode(currentAST.root);
		break;
	}
	case ARRAYEXPR_FN:
	{
		arrayexpr_fn(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		primary_expr_AST = RefDNode(currentAST.root);
		break;
	}
	case CONSTANT:
	{
		RefDNode tmp45_AST = RefDNode(antlr::nullAST);
		RefDNode tmp45_AST_in = RefDNode(antlr::nullAST);
		tmp45_AST = astFactory->create(antlr::RefAST(_t));
		tmp45_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp45_AST));
		match(antlr::RefAST(_t),CONSTANT);
		_t = _t->getNextSibling();
		primary_expr_AST = RefDNode(currentAST.root);
		break;
	}
	case ARRAYDEF:
	{
		dummy=array_def(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		primary_expr_AST = RefDNode(currentAST.root);
		break;
	}
	case NSTRUC_REF:
	case STRUC:
	{
		struct_def(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		primary_expr_AST = RefDNode(currentAST.root);
		break;
	}
	default:
	{
		throw antlr::NoViableAltException(antlr::RefAST(_t));
	}
	}
	returnAST = primary_expr_AST;
	_retTree = _t;
}

void GDLTreeParser::op_expr(RefDNode _t) {
	RefDNode op_expr_AST_in = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	returnAST = RefDNode(antlr::nullAST);
	antlr::ASTPair currentAST;
	RefDNode op_expr_AST = RefDNode(antlr::nullAST);
	
	if (_t == RefDNode(antlr::nullAST) )
		_t = ASTNULL;
	switch ( _t->getType()) {
	case QUESTION:
	{
		RefDNode __t154 = _t;
		RefDNode tmp46_AST = RefDNode(antlr::nullAST);
		RefDNode tmp46_AST_in = RefDNode(antlr::nullAST);
		tmp46_AST = astFactory->create(antlr::RefAST(_t));
		tmp46_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp46_AST));
		antlr::ASTPair __currentAST154 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),QUESTION);
		_t = _t->getFirstChild();
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		currentAST = __currentAST154;
		_t = __t154;
		_t = _t->getNextSibling();
		op_expr_AST = RefDNode(currentAST.root);
		break;
	}
	case AND_OP:
	{
		RefDNode __t155 = _t;
		RefDNode tmp47_AST = RefDNode(antlr::nullAST);
		RefDNode tmp47_AST_in = RefDNode(antlr::nullAST);
		tmp47_AST = astFactory->create(antlr::RefAST(_t));
		tmp47_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp47_AST));
		antlr::ASTPair __currentAST155 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),AND_OP);
		_t = _t->getFirstChild();
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		currentAST = __currentAST155;
		_t = __t155;
		_t = _t->getNextSibling();
		op_expr_AST = RefDNode(currentAST.root);
		break;
	}
	case OR_OP:
	{
		RefDNode __t156 = _t;
		RefDNode tmp48_AST = RefDNode(antlr::nullAST);
		RefDNode tmp48_AST_in = RefDNode(antlr::nullAST);
		tmp48_AST = astFactory->create(antlr::RefAST(_t));
		tmp48_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp48_AST));
		antlr::ASTPair __currentAST156 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),OR_OP);
		_t = _t->getFirstChild();
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		currentAST = __currentAST156;
		_t = __t156;
		_t = _t->getNextSibling();
		op_expr_AST = RefDNode(currentAST.root);
		break;
	}
	case XOR_OP:
	{
		RefDNode __t157 = _t;
		RefDNode tmp49_AST = RefDNode(antlr::nullAST);
		RefDNode tmp49_AST_in = RefDNode(antlr::nullAST);
		tmp49_AST = astFactory->create(antlr::RefAST(_t));
		tmp49_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp49_AST));
		antlr::ASTPair __currentAST157 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),XOR_OP);
		_t = _t->getFirstChild();
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		currentAST = __currentAST157;
		_t = __t157;
		_t = _t->getNextSibling();
		op_expr_AST = RefDNode(currentAST.root);
		break;
	}
	case LOG_AND:
	{
		RefDNode __t158 = _t;
		RefDNode tmp50_AST = RefDNode(antlr::nullAST);
		RefDNode tmp50_AST_in = RefDNode(antlr::nullAST);
		tmp50_AST = astFactory->create(antlr::RefAST(_t));
		tmp50_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp50_AST));
		antlr::ASTPair __currentAST158 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),LOG_AND);
		_t = _t->getFirstChild();
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		currentAST = __currentAST158;
		_t = __t158;
		_t = _t->getNextSibling();
		op_expr_AST = RefDNode(currentAST.root);
		break;
	}
	case LOG_OR:
	{
		RefDNode __t159 = _t;
		RefDNode tmp51_AST = RefDNode(antlr::nullAST);
		RefDNode tmp51_AST_in = RefDNode(antlr::nullAST);
		tmp51_AST = astFactory->create(antlr::RefAST(_t));
		tmp51_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp51_AST));
		antlr::ASTPair __currentAST159 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),LOG_OR);
		_t = _t->getFirstChild();
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		currentAST = __currentAST159;
		_t = __t159;
		_t = _t->getNextSibling();
		op_expr_AST = RefDNode(currentAST.root);
		break;
	}
	case EQ_OP:
	{
		RefDNode __t160 = _t;
		RefDNode tmp52_AST = RefDNode(antlr::nullAST);
		RefDNode tmp52_AST_in = RefDNode(antlr::nullAST);
		tmp52_AST = astFactory->create(antlr::RefAST(_t));
		tmp52_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp52_AST));
		antlr::ASTPair __currentAST160 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),EQ_OP);
		_t = _t->getFirstChild();
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		currentAST = __currentAST160;
		_t = __t160;
		_t = _t->getNextSibling();
		op_expr_AST = RefDNode(currentAST.root);
		break;
	}
	case NE_OP:
	{
		RefDNode __t161 = _t;
		RefDNode tmp53_AST = RefDNode(antlr::nullAST);
		RefDNode tmp53_AST_in = RefDNode(antlr::nullAST);
		tmp53_AST = astFactory->create(antlr::RefAST(_t));
		tmp53_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp53_AST));
		antlr::ASTPair __currentAST161 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),NE_OP);
		_t = _t->getFirstChild();
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		currentAST = __currentAST161;
		_t = __t161;
		_t = _t->getNextSibling();
		op_expr_AST = RefDNode(currentAST.root);
		break;
	}
	case LE_OP:
	{
		RefDNode __t162 = _t;
		RefDNode tmp54_AST = RefDNode(antlr::nullAST);
		RefDNode tmp54_AST_in = RefDNode(antlr::nullAST);
		tmp54_AST = astFactory->create(antlr::RefAST(_t));
		tmp54_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp54_AST));
		antlr::ASTPair __currentAST162 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),LE_OP);
		_t = _t->getFirstChild();
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		currentAST = __currentAST162;
		_t = __t162;
		_t = _t->getNextSibling();
		op_expr_AST = RefDNode(currentAST.root);
		break;
	}
	case LT_OP:
	{
		RefDNode __t163 = _t;
		RefDNode tmp55_AST = RefDNode(antlr::nullAST);
		RefDNode tmp55_AST_in = RefDNode(antlr::nullAST);
		tmp55_AST = astFactory->create(antlr::RefAST(_t));
		tmp55_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp55_AST));
		antlr::ASTPair __currentAST163 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),LT_OP);
		_t = _t->getFirstChild();
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		currentAST = __currentAST163;
		_t = __t163;
		_t = _t->getNextSibling();
		op_expr_AST = RefDNode(currentAST.root);
		break;
	}
	case GE_OP:
	{
		RefDNode __t164 = _t;
		RefDNode tmp56_AST = RefDNode(antlr::nullAST);
		RefDNode tmp56_AST_in = RefDNode(antlr::nullAST);
		tmp56_AST = astFactory->create(antlr::RefAST(_t));
		tmp56_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp56_AST));
		antlr::ASTPair __currentAST164 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),GE_OP);
		_t = _t->getFirstChild();
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		currentAST = __currentAST164;
		_t = __t164;
		_t = _t->getNextSibling();
		op_expr_AST = RefDNode(currentAST.root);
		break;
	}
	case GT_OP:
	{
		RefDNode __t165 = _t;
		RefDNode tmp57_AST = RefDNode(antlr::nullAST);
		RefDNode tmp57_AST_in = RefDNode(antlr::nullAST);
		tmp57_AST = astFactory->create(antlr::RefAST(_t));
		tmp57_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp57_AST));
		antlr::ASTPair __currentAST165 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),GT_OP);
		_t = _t->getFirstChild();
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		currentAST = __currentAST165;
		_t = __t165;
		_t = _t->getNextSibling();
		op_expr_AST = RefDNode(currentAST.root);
		break;
	}
	case NOT_OP:
	{
		RefDNode __t166 = _t;
		RefDNode tmp58_AST = RefDNode(antlr::nullAST);
		RefDNode tmp58_AST_in = RefDNode(antlr::nullAST);
		tmp58_AST = astFactory->create(antlr::RefAST(_t));
		tmp58_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp58_AST));
		antlr::ASTPair __currentAST166 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),NOT_OP);
		_t = _t->getFirstChild();
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		currentAST = __currentAST166;
		_t = __t166;
		_t = _t->getNextSibling();
		op_expr_AST = RefDNode(currentAST.root);
		break;
	}
	case PLUS:
	{
		RefDNode __t167 = _t;
		RefDNode tmp59_AST = RefDNode(antlr::nullAST);
		RefDNode tmp59_AST_in = RefDNode(antlr::nullAST);
		tmp59_AST = astFactory->create(antlr::RefAST(_t));
		tmp59_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp59_AST));
		antlr::ASTPair __currentAST167 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),PLUS);
		_t = _t->getFirstChild();
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		currentAST = __currentAST167;
		_t = __t167;
		_t = _t->getNextSibling();
		op_expr_AST = RefDNode(currentAST.root);
		break;
	}
	case MINUS:
	{
		RefDNode __t168 = _t;
		RefDNode tmp60_AST = RefDNode(antlr::nullAST);
		RefDNode tmp60_AST_in = RefDNode(antlr::nullAST);
		tmp60_AST = astFactory->create(antlr::RefAST(_t));
		tmp60_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp60_AST));
		antlr::ASTPair __currentAST168 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),MINUS);
		_t = _t->getFirstChild();
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		currentAST = __currentAST168;
		_t = __t168;
		_t = _t->getNextSibling();
		op_expr_AST = RefDNode(currentAST.root);
		break;
	}
	case LTMARK:
	{
		RefDNode __t169 = _t;
		RefDNode tmp61_AST = RefDNode(antlr::nullAST);
		RefDNode tmp61_AST_in = RefDNode(antlr::nullAST);
		tmp61_AST = astFactory->create(antlr::RefAST(_t));
		tmp61_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp61_AST));
		antlr::ASTPair __currentAST169 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),LTMARK);
		_t = _t->getFirstChild();
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		currentAST = __currentAST169;
		_t = __t169;
		_t = _t->getNextSibling();
		op_expr_AST = RefDNode(currentAST.root);
		break;
	}
	case GTMARK:
	{
		RefDNode __t170 = _t;
		RefDNode tmp62_AST = RefDNode(antlr::nullAST);
		RefDNode tmp62_AST_in = RefDNode(antlr::nullAST);
		tmp62_AST = astFactory->create(antlr::RefAST(_t));
		tmp62_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp62_AST));
		antlr::ASTPair __currentAST170 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),GTMARK);
		_t = _t->getFirstChild();
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		currentAST = __currentAST170;
		_t = __t170;
		_t = _t->getNextSibling();
		op_expr_AST = RefDNode(currentAST.root);
		break;
	}
	case UMINUS:
	{
		RefDNode __t171 = _t;
		RefDNode tmp63_AST = RefDNode(antlr::nullAST);
		RefDNode tmp63_AST_in = RefDNode(antlr::nullAST);
		tmp63_AST = astFactory->create(antlr::RefAST(_t));
		tmp63_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp63_AST));
		antlr::ASTPair __currentAST171 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),UMINUS);
		_t = _t->getFirstChild();
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		currentAST = __currentAST171;
		_t = __t171;
		_t = _t->getNextSibling();
		op_expr_AST = RefDNode(currentAST.root);
		break;
	}
	case LOG_NEG:
	{
		RefDNode __t172 = _t;
		RefDNode tmp64_AST = RefDNode(antlr::nullAST);
		RefDNode tmp64_AST_in = RefDNode(antlr::nullAST);
		tmp64_AST = astFactory->create(antlr::RefAST(_t));
		tmp64_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp64_AST));
		antlr::ASTPair __currentAST172 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),LOG_NEG);
		_t = _t->getFirstChild();
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		currentAST = __currentAST172;
		_t = __t172;
		_t = _t->getNextSibling();
		op_expr_AST = RefDNode(currentAST.root);
		break;
	}
	case ASTERIX:
	{
		RefDNode __t173 = _t;
		RefDNode tmp65_AST = RefDNode(antlr::nullAST);
		RefDNode tmp65_AST_in = RefDNode(antlr::nullAST);
		tmp65_AST = astFactory->create(antlr::RefAST(_t));
		tmp65_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp65_AST));
		antlr::ASTPair __currentAST173 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),ASTERIX);
		_t = _t->getFirstChild();
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		currentAST = __currentAST173;
		_t = __t173;
		_t = _t->getNextSibling();
		op_expr_AST = RefDNode(currentAST.root);
		break;
	}
	case MATRIX_OP1:
	{
		RefDNode __t174 = _t;
		RefDNode tmp66_AST = RefDNode(antlr::nullAST);
		RefDNode tmp66_AST_in = RefDNode(antlr::nullAST);
		tmp66_AST = astFactory->create(antlr::RefAST(_t));
		tmp66_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp66_AST));
		antlr::ASTPair __currentAST174 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),MATRIX_OP1);
		_t = _t->getFirstChild();
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		currentAST = __currentAST174;
		_t = __t174;
		_t = _t->getNextSibling();
		op_expr_AST = RefDNode(currentAST.root);
		break;
	}
	case MATRIX_OP2:
	{
		RefDNode __t175 = _t;
		RefDNode tmp67_AST = RefDNode(antlr::nullAST);
		RefDNode tmp67_AST_in = RefDNode(antlr::nullAST);
		tmp67_AST = astFactory->create(antlr::RefAST(_t));
		tmp67_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp67_AST));
		antlr::ASTPair __currentAST175 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),MATRIX_OP2);
		_t = _t->getFirstChild();
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		currentAST = __currentAST175;
		_t = __t175;
		_t = _t->getNextSibling();
		op_expr_AST = RefDNode(currentAST.root);
		break;
	}
	case SLASH:
	{
		RefDNode __t176 = _t;
		RefDNode tmp68_AST = RefDNode(antlr::nullAST);
		RefDNode tmp68_AST_in = RefDNode(antlr::nullAST);
		tmp68_AST = astFactory->create(antlr::RefAST(_t));
		tmp68_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp68_AST));
		antlr::ASTPair __currentAST176 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),SLASH);
		_t = _t->getFirstChild();
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		currentAST = __currentAST176;
		_t = __t176;
		_t = _t->getNextSibling();
		op_expr_AST = RefDNode(currentAST.root);
		break;
	}
	case MOD_OP:
	{
		RefDNode __t177 = _t;
		RefDNode tmp69_AST = RefDNode(antlr::nullAST);
		RefDNode tmp69_AST_in = RefDNode(antlr::nullAST);
		tmp69_AST = astFactory->create(antlr::RefAST(_t));
		tmp69_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp69_AST));
		antlr::ASTPair __currentAST177 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),MOD_OP);
		_t = _t->getFirstChild();
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		currentAST = __currentAST177;
		_t = __t177;
		_t = _t->getNextSibling();
		op_expr_AST = RefDNode(currentAST.root);
		break;
	}
	case POW:
	{
		RefDNode __t178 = _t;
		RefDNode tmp70_AST = RefDNode(antlr::nullAST);
		RefDNode tmp70_AST_in = RefDNode(antlr::nullAST);
		tmp70_AST = astFactory->create(antlr::RefAST(_t));
		tmp70_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp70_AST));
		antlr::ASTPair __currentAST178 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),POW);
		_t = _t->getFirstChild();
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		currentAST = __currentAST178;
		_t = __t178;
		_t = _t->getNextSibling();
		op_expr_AST = RefDNode(currentAST.root);
		break;
	}
	case DEC:
	{
		RefDNode __t179 = _t;
		RefDNode tmp71_AST = RefDNode(antlr::nullAST);
		RefDNode tmp71_AST_in = RefDNode(antlr::nullAST);
		tmp71_AST = astFactory->create(antlr::RefAST(_t));
		tmp71_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp71_AST));
		antlr::ASTPair __currentAST179 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),DEC);
		_t = _t->getFirstChild();
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		currentAST = __currentAST179;
		_t = __t179;
		_t = _t->getNextSibling();
		op_expr_AST = RefDNode(currentAST.root);
		break;
	}
	case INC:
	{
		RefDNode __t180 = _t;
		RefDNode tmp72_AST = RefDNode(antlr::nullAST);
		RefDNode tmp72_AST_in = RefDNode(antlr::nullAST);
		tmp72_AST = astFactory->create(antlr::RefAST(_t));
		tmp72_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp72_AST));
		antlr::ASTPair __currentAST180 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),INC);
		_t = _t->getFirstChild();
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		currentAST = __currentAST180;
		_t = __t180;
		_t = _t->getNextSibling();
		op_expr_AST = RefDNode(currentAST.root);
		break;
	}
	case POSTDEC:
	{
		RefDNode __t181 = _t;
		RefDNode tmp73_AST = RefDNode(antlr::nullAST);
		RefDNode tmp73_AST_in = RefDNode(antlr::nullAST);
		tmp73_AST = astFactory->create(antlr::RefAST(_t));
		tmp73_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp73_AST));
		antlr::ASTPair __currentAST181 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),POSTDEC);
		_t = _t->getFirstChild();
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		currentAST = __currentAST181;
		_t = __t181;
		_t = _t->getNextSibling();
		op_expr_AST = RefDNode(currentAST.root);
		break;
	}
	case POSTINC:
	{
		RefDNode __t182 = _t;
		RefDNode tmp74_AST = RefDNode(antlr::nullAST);
		RefDNode tmp74_AST_in = RefDNode(antlr::nullAST);
		tmp74_AST = astFactory->create(antlr::RefAST(_t));
		tmp74_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp74_AST));
		antlr::ASTPair __currentAST182 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),POSTINC);
		_t = _t->getFirstChild();
		expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		currentAST = __currentAST182;
		_t = __t182;
		_t = _t->getNextSibling();
		op_expr_AST = RefDNode(currentAST.root);
		break;
	}
	case ASSIGN:
	case ARRAYDEF:
	case ARRAYEXPR_FN:
	case CONSTANT:
	case FCALL:
	case MFCALL:
	case MFCALL_PARENT:
	case NSTRUC_REF:
	case STRUC:
	case AND_OP_EQ:
	case ASTERIX_EQ:
	case EQ_OP_EQ:
	case GE_OP_EQ:
	case GTMARK_EQ:
	case GT_OP_EQ:
	case LE_OP_EQ:
	case LTMARK_EQ:
	case LT_OP_EQ:
	case MATRIX_OP1_EQ:
	case MATRIX_OP2_EQ:
	case MINUS_EQ:
	case MOD_OP_EQ:
	case NE_OP_EQ:
	case OR_OP_EQ:
	case PLUS_EQ:
	case POW_EQ:
	case SLASH_EQ:
	case XOR_OP_EQ:
	{
		primary_expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		op_expr_AST = RefDNode(currentAST.root);
		break;
	}
	default:
	{
		throw antlr::NoViableAltException(antlr::RefAST(_t));
	}
	}
	returnAST = op_expr_AST;
	_retTree = _t;
}

void GDLTreeParser::indexable_expr(RefDNode _t) {
	RefDNode indexable_expr_AST_in = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	returnAST = RefDNode(antlr::nullAST);
	antlr::ASTPair currentAST;
	RefDNode indexable_expr_AST = RefDNode(antlr::nullAST);
	
	if (_t == RefDNode(antlr::nullAST) )
		_t = ASTNULL;
	switch ( _t->getType()) {
	case VAR:
	{
		var(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		indexable_expr_AST = RefDNode(currentAST.root);
		break;
	}
	case SYSVAR:
	{
		sysvar(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		indexable_expr_AST = RefDNode(currentAST.root);
		break;
	}
	case EXPR:
	{
		brace_expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		indexable_expr_AST = RefDNode(currentAST.root);
		break;
	}
	default:
	{
		throw antlr::NoViableAltException(antlr::RefAST(_t));
	}
	}
	returnAST = indexable_expr_AST;
	_retTree = _t;
}

void GDLTreeParser::array_expr(RefDNode _t) {
	RefDNode array_expr_AST_in = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	returnAST = RefDNode(antlr::nullAST);
	antlr::ASTPair currentAST;
	RefDNode array_expr_AST = RefDNode(antlr::nullAST);
	
	if (_t == RefDNode(antlr::nullAST) )
		_t = ASTNULL;
	switch ( _t->getType()) {
	case ARRAYEXPR:
	{
		RefDNode __t185 = _t;
		RefDNode tmp75_AST = RefDNode(antlr::nullAST);
		RefDNode tmp75_AST_in = RefDNode(antlr::nullAST);
		tmp75_AST = astFactory->create(antlr::RefAST(_t));
		tmp75_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp75_AST));
		antlr::ASTPair __currentAST185 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),ARRAYEXPR);
		_t = _t->getFirstChild();
		arrayindex_list(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		indexable_expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		currentAST = __currentAST185;
		_t = __t185;
		_t = _t->getNextSibling();
		array_expr_AST = RefDNode(currentAST.root);
		break;
	}
	case EXPR:
	case SYSVAR:
	case VAR:
	{
		indexable_expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		array_expr_AST = RefDNode(currentAST.root);
		break;
	}
	default:
	{
		throw antlr::NoViableAltException(antlr::RefAST(_t));
	}
	}
	returnAST = array_expr_AST;
	_retTree = _t;
}

void GDLTreeParser::tag_expr(RefDNode _t) {
	RefDNode tag_expr_AST_in = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	returnAST = RefDNode(antlr::nullAST);
	antlr::ASTPair currentAST;
	RefDNode tag_expr_AST = RefDNode(antlr::nullAST);
	
	if (_t == RefDNode(antlr::nullAST) )
		_t = ASTNULL;
	switch ( _t->getType()) {
	case EXPR:
	{
		brace_expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		tag_expr_AST = RefDNode(currentAST.root);
		break;
	}
	case IDENTIFIER:
	{
		RefDNode tmp76_AST = RefDNode(antlr::nullAST);
		RefDNode tmp76_AST_in = RefDNode(antlr::nullAST);
		tmp76_AST = astFactory->create(antlr::RefAST(_t));
		tmp76_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp76_AST));
		match(antlr::RefAST(_t),IDENTIFIER);
		_t = _t->getNextSibling();
		tag_expr_AST = RefDNode(currentAST.root);
		break;
	}
	default:
	{
		throw antlr::NoViableAltException(antlr::RefAST(_t));
	}
	}
	returnAST = tag_expr_AST;
	_retTree = _t;
}

void GDLTreeParser::tag_array_expr(RefDNode _t) {
	RefDNode tag_array_expr_AST_in = (_t == ASTNULL) ? RefDNode(antlr::nullAST) : _t;
	returnAST = RefDNode(antlr::nullAST);
	antlr::ASTPair currentAST;
	RefDNode tag_array_expr_AST = RefDNode(antlr::nullAST);
	
	if (_t == RefDNode(antlr::nullAST) )
		_t = ASTNULL;
	switch ( _t->getType()) {
	case ARRAYEXPR:
	{
		RefDNode __t188 = _t;
		RefDNode tmp77_AST = RefDNode(antlr::nullAST);
		RefDNode tmp77_AST_in = RefDNode(antlr::nullAST);
		tmp77_AST = astFactory->create(antlr::RefAST(_t));
		tmp77_AST_in = _t;
		astFactory->addASTChild(currentAST, antlr::RefAST(tmp77_AST));
		antlr::ASTPair __currentAST188 = currentAST;
		currentAST.root = currentAST.child;
		currentAST.child = RefDNode(antlr::nullAST);
		match(antlr::RefAST(_t),ARRAYEXPR);
		_t = _t->getFirstChild();
		arrayindex_list(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		tag_expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		currentAST = __currentAST188;
		_t = __t188;
		_t = _t->getNextSibling();
		tag_array_expr_AST = RefDNode(currentAST.root);
		break;
	}
	case EXPR:
	case IDENTIFIER:
	{
		tag_expr(_t);
		_t = _retTree;
		astFactory->addASTChild(currentAST, antlr::RefAST(returnAST));
		tag_array_expr_AST = RefDNode(currentAST.root);
		break;
	}
	default:
	{
		throw antlr::NoViableAltException(antlr::RefAST(_t));
	}
	}
	returnAST = tag_array_expr_AST;
	_retTree = _t;
}

void GDLTreeParser::initializeASTFactory( antlr::ASTFactory& factory )
{
	factory.setMaxNodeType(196);
}
const char* GDLTreeParser::tokenNames[] = {
	"<0>",
	"EOF",
	"<2>",
	"NULL_TREE_LOOKAHEAD",
	"ALL",
	"ASSIGN",
	"ARRAYDEF",
	"ARRAYIX",
	"ARRAYIX_ALL",
	"ARRAYIX_ORANGE",
	"ARRAYIX_RANGE",
	"ARRAYEXPR",
	"ARRAYEXPR_FN",
	"BLOCK",
	"COMMONDECL",
	"COMMONDEF",
	"CONSTANT",
	"DEREF",
	"ELSEBLK",
	"EXPR",
	"FOR_STEP",
	"FCALL",
	"FCALL_LIB",
	"IF_ELSE",
	"KEYDECL",
	"KEYDEF",
	"KEYDEF_REF",
	"KEYDEF_REF_CHECK",
	"KEYDEF_REF_EXPR",
	"LABEL",
	"MPCALL",
	"MPCALL_PARENT",
	"MFCALL",
	"MFCALL_LIB",
	"MFCALL_PARENT",
	"MFCALL_PARENT_LIB",
	"NOP",
	"NSTRUC",
	"NSTRUC_REF",
	"ON_IOERROR_NULL",
	"PCALL",
	"PCALL_LIB",
	"PARADECL",
	"POSTDEC",
	"POSTINC",
	"DECSTATEMENT",
	"INCSTATEMENT",
	"REF",
	"REF_CHECK",
	"REF_EXPR",
	"RETURN",
	"RETF",
	"RETP",
	"STRUC",
	"SYSVAR",
	"UMINUS",
	"VAR",
	"VARPTR",
	"\"function\"",
	"\"pro\"",
	"IDENTIFIER",
	"METHOD",
	"COMMA",
	"\"begin\"",
	"COLON",
	"\"switch\"",
	"\"of\"",
	"\"else\"",
	"\"case\"",
	"END_U",
	"\"forward_function\"",
	"EQUAL",
	"\"end\"",
	"\"compile_opt\"",
	"\"common\"",
	"\"endif\"",
	"\"endelse\"",
	"\"endcase\"",
	"\"endswitch\"",
	"\"endfor\"",
	"\"endwhile\"",
	"\"endrep\"",
	"DEC",
	"INC",
	"AND_OP_EQ",
	"ASTERIX_EQ",
	"EQ_OP_EQ",
	"GE_OP_EQ",
	"GTMARK_EQ",
	"GT_OP_EQ",
	"LE_OP_EQ",
	"LTMARK_EQ",
	"LT_OP_EQ",
	"MATRIX_OP1_EQ",
	"MATRIX_OP2_EQ",
	"MINUS_EQ",
	"MOD_OP_EQ",
	"NE_OP_EQ",
	"OR_OP_EQ",
	"PLUS_EQ",
	"POW_EQ",
	"SLASH_EQ",
	"XOR_OP_EQ",
	"MEMBER",
	"\"break\"",
	"\"continue\"",
	"\"repeat\"",
	"\"until\"",
	"\"while\"",
	"\"do\"",
	"\"for\"",
	"\"goto\"",
	"\"on_ioerror\"",
	"\"if\"",
	"\"then\"",
	"LBRACE",
	"RBRACE",
	"SLASH",
	"LSQUARE",
	"RSQUARE",
	"SYSVARNAME",
	"EXCLAMATION",
	"LCURLY",
	"RCURLY",
	"\"inherits\"",
	"CONSTANT_HEX_BYTE",
	"CONSTANT_HEX_LONG",
	"CONSTANT_HEX_LONG64",
	"CONSTANT_HEX_INT",
	"CONSTANT_HEX_I",
	"CONSTANT_HEX_ULONG",
	"CONSTANT_HEX_ULONG64",
	"CONSTANT_HEX_UI",
	"CONSTANT_HEX_UINT",
	"CONSTANT_BYTE",
	"CONSTANT_LONG",
	"CONSTANT_LONG64",
	"CONSTANT_INT",
	"CONSTANT_I",
	"CONSTANT_ULONG",
	"CONSTANT_ULONG64",
	"CONSTANT_UI",
	"CONSTANT_UINT",
	"CONSTANT_OCT_BYTE",
	"CONSTANT_OCT_LONG",
	"CONSTANT_OCT_LONG64",
	"CONSTANT_OCT_INT",
	"CONSTANT_OCT_I",
	"CONSTANT_OCT_ULONG",
	"CONSTANT_OCT_ULONG64",
	"CONSTANT_OCT_UI",
	"CONSTANT_OCT_UINT",
	"CONSTANT_FLOAT",
	"CONSTANT_DOUBLE",
	"ASTERIX",
	"DOT",
	"STRING_LITERAL",
	"POW",
	"MATRIX_OP1",
	"MATRIX_OP2",
	"\"mod\"",
	"PLUS",
	"MINUS",
	"LTMARK",
	"GTMARK",
	"\"not\"",
	"\"eq\"",
	"\"ne\"",
	"\"le\"",
	"\"lt\"",
	"\"ge\"",
	"\"gt\"",
	"\"and\"",
	"\"or\"",
	"\"xor\"",
	"LOG_AND",
	"LOG_OR",
	"LOG_NEG",
	"QUESTION",
	"STRING",
	"INCLUDE",
	"EOL",
	"W",
	"D",
	"L",
	"H",
	"O",
	"EXP",
	"DBL_E",
	"DBL",
	"CONSTANT_OR_STRING_LITERAL",
	"COMMENT",
	"END_MARKER",
	"WHITESPACE",
	"SKIP_LINES",
	"CONT_STATEMENT",
	"END_OF_LINE",
	0
};

const unsigned long GDLTreeParser::_tokenSet_0_data_[] = { 3221282848UL, 262400UL, 4294705234UL, 251775UL, 0UL, 0UL, 0UL, 0UL };
// ASSIGN BLOCK COMMONDECL COMMONDEF MPCALL MPCALL_PARENT PCALL RETURN 
// "switch" "case" "forward_function" DEC INC AND_OP_EQ ASTERIX_EQ EQ_OP_EQ 
// GE_OP_EQ GTMARK_EQ GT_OP_EQ LE_OP_EQ LTMARK_EQ LT_OP_EQ MATRIX_OP1_EQ 
// MATRIX_OP2_EQ MINUS_EQ MOD_OP_EQ NE_OP_EQ OR_OP_EQ PLUS_EQ POW_EQ SLASH_EQ 
// XOR_OP_EQ "break" "continue" "repeat" "while" "for" "goto" "on_ioerror" 
// "if" 
const antlr::BitSet GDLTreeParser::_tokenSet_0(_tokenSet_0_data_,8);
const unsigned long GDLTreeParser::_tokenSet_1_data_[] = { 36378720UL, 31463493UL, 4294705152UL, 2097279UL, 3959422976UL, 524287UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// ASSIGN ARRAYDEF ARRAYEXPR ARRAYEXPR_FN CONSTANT DEREF EXPR FCALL KEYDEF 
// MFCALL MFCALL_PARENT NSTRUC_REF POSTDEC POSTINC STRUC SYSVAR UMINUS 
// VAR DEC INC AND_OP_EQ ASTERIX_EQ EQ_OP_EQ GE_OP_EQ GTMARK_EQ GT_OP_EQ 
// LE_OP_EQ LTMARK_EQ LT_OP_EQ MATRIX_OP1_EQ MATRIX_OP2_EQ MINUS_EQ MOD_OP_EQ 
// NE_OP_EQ OR_OP_EQ PLUS_EQ POW_EQ SLASH_EQ XOR_OP_EQ SLASH ASTERIX DOT 
// POW MATRIX_OP1 MATRIX_OP2 "mod" PLUS MINUS LTMARK GTMARK "not" "eq" 
// "ne" "le" "lt" "ge" "gt" "and" "or" "xor" LOG_AND LOG_OR LOG_NEG QUESTION 
const antlr::BitSet GDLTreeParser::_tokenSet_1(_tokenSet_1_data_,12);
const unsigned long GDLTreeParser::_tokenSet_2_data_[] = { 2824288UL, 31463493UL, 4294705152UL, 2097279UL, 3959422976UL, 524287UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// ASSIGN ARRAYDEF ARRAYEXPR ARRAYEXPR_FN CONSTANT DEREF EXPR FCALL MFCALL 
// MFCALL_PARENT NSTRUC_REF POSTDEC POSTINC STRUC SYSVAR UMINUS VAR DEC 
// INC AND_OP_EQ ASTERIX_EQ EQ_OP_EQ GE_OP_EQ GTMARK_EQ GT_OP_EQ LE_OP_EQ 
// LTMARK_EQ LT_OP_EQ MATRIX_OP1_EQ MATRIX_OP2_EQ MINUS_EQ MOD_OP_EQ NE_OP_EQ 
// OR_OP_EQ PLUS_EQ POW_EQ SLASH_EQ XOR_OP_EQ SLASH ASTERIX DOT POW MATRIX_OP1 
// MATRIX_OP2 "mod" PLUS MINUS LTMARK GTMARK "not" "eq" "ne" "le" "lt" 
// "ge" "gt" "and" "or" "xor" LOG_AND LOG_OR LOG_NEG QUESTION 
const antlr::BitSet GDLTreeParser::_tokenSet_2(_tokenSet_2_data_,12);


