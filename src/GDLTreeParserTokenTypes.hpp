#ifndef INC_GDLTreeParserTokenTypes_hpp_
#define INC_GDLTreeParserTokenTypes_hpp_

/* $ANTLR 2.7.4: "gdlc.tree.g" -> "GDLTreeParserTokenTypes.hpp"$ */

#ifndef CUSTOM_API
# define CUSTOM_API
#endif

#ifdef __cplusplus
struct CUSTOM_API GDLTreeParserTokenTypes {
#endif
	enum {
		EOF_ = 1,
		ALL = 4,
		ASSIGN = 5,
		ARRAYDEF = 6,
		ARRAYIX = 7,
		ARRAYIX_ALL = 8,
		ARRAYIX_ORANGE = 9,
		ARRAYIX_RANGE = 10,
		ARRAYEXPR = 11,
		ARRAYEXPR_FN = 12,
		BLOCK = 13,
		COMMONDECL = 14,
		COMMONDEF = 15,
		CONSTANT = 16,
		DEREF = 17,
		ELSEBLK = 18,
		EXPR = 19,
		FOR_STEP = 20,
		FCALL = 21,
		FCALL_LIB = 22,
		IF_ELSE = 23,
		KEYDECL = 24,
		KEYDEF = 25,
		KEYDEF_REF = 26,
		KEYDEF_REF_CHECK = 27,
		LABEL = 28,
		MPCALL = 29,
		MPCALL_PARENT = 30,
		MFCALL = 31,
		MFCALL_LIB = 32,
		MFCALL_PARENT = 33,
		MFCALL_PARENT_LIB = 34,
		NOP = 35,
		NSTRUC = 36,
		NSTRUC_REF = 37,
		ON_IOERROR_NULL = 38,
		PCALL = 39,
		PCALL_LIB = 40,
		PARADECL = 41,
		POSTDEC = 42,
		POSTINC = 43,
		DECSTATEMENT = 44,
		INCSTATEMENT = 45,
		REF = 46,
		REF_CHECK = 47,
		REF_ASSIGN = 48,
		RETURN = 49,
		RETF = 50,
		RETP = 51,
		STRUC = 52,
		SYSVAR = 53,
		UMINUS = 54,
		VAR = 55,
		VARPTR = 56,
		FUNCTION = 57,
		PRO = 58,
		IDENTIFIER = 59,
		METHOD = 60,
		COMMA = 61,
		BEGIN = 62,
		COLON = 63,
		SWITCH = 64,
		OF = 65,
		ELSE = 66,
		CASE = 67,
		END_U = 68,
		FORWARD = 69,
		EQUAL = 70,
		END = 71,
		COMPILE_OPT = 72,
		COMMON = 73,
		ENDIF = 74,
		ENDELSE = 75,
		ENDCASE = 76,
		ENDSWITCH = 77,
		ENDFOR = 78,
		ENDWHILE = 79,
		ENDREP = 80,
		DEC = 81,
		INC = 82,
		AND_OP_EQ = 83,
		ASTERIX_EQ = 84,
		EQ_OP_EQ = 85,
		GE_OP_EQ = 86,
		GTMARK_EQ = 87,
		GT_OP_EQ = 88,
		LE_OP_EQ = 89,
		LTMARK_EQ = 90,
		LT_OP_EQ = 91,
		MATRIX_OP1_EQ = 92,
		MATRIX_OP2_EQ = 93,
		MINUS_EQ = 94,
		MOD_OP_EQ = 95,
		NE_OP_EQ = 96,
		OR_OP_EQ = 97,
		PLUS_EQ = 98,
		POW_EQ = 99,
		SLASH_EQ = 100,
		XOR_OP_EQ = 101,
		MEMBER = 102,
		BREAK = 103,
		CONTINUE = 104,
		REPEAT = 105,
		UNTIL = 106,
		WHILE = 107,
		DO = 108,
		FOR = 109,
		GOTO = 110,
		ON_IOERROR = 111,
		IF = 112,
		THEN = 113,
		LBRACE = 114,
		RBRACE = 115,
		SLASH = 116,
		LSQUARE = 117,
		RSQUARE = 118,
		SYSVARNAME = 119,
		EXCLAMATION = 120,
		LCURLY = 121,
		RCURLY = 122,
		INHERITS = 123,
		CONSTANT_HEX_BYTE = 124,
		CONSTANT_HEX_LONG = 125,
		CONSTANT_HEX_LONG64 = 126,
		CONSTANT_HEX_INT = 127,
		CONSTANT_HEX_I = 128,
		CONSTANT_HEX_ULONG = 129,
		CONSTANT_HEX_ULONG64 = 130,
		CONSTANT_HEX_UI = 131,
		CONSTANT_HEX_UINT = 132,
		CONSTANT_BYTE = 133,
		CONSTANT_LONG = 134,
		CONSTANT_LONG64 = 135,
		CONSTANT_INT = 136,
		CONSTANT_I = 137,
		CONSTANT_ULONG = 138,
		CONSTANT_ULONG64 = 139,
		CONSTANT_UI = 140,
		CONSTANT_UINT = 141,
		CONSTANT_OCT_BYTE = 142,
		CONSTANT_OCT_LONG = 143,
		CONSTANT_OCT_LONG64 = 144,
		CONSTANT_OCT_INT = 145,
		CONSTANT_OCT_I = 146,
		CONSTANT_OCT_ULONG = 147,
		CONSTANT_OCT_ULONG64 = 148,
		CONSTANT_OCT_UI = 149,
		CONSTANT_OCT_UINT = 150,
		CONSTANT_FLOAT = 151,
		CONSTANT_DOUBLE = 152,
		ASTERIX = 153,
		DOT = 154,
		STRING_LITERAL = 155,
		POW = 156,
		MATRIX_OP1 = 157,
		MATRIX_OP2 = 158,
		MOD_OP = 159,
		PLUS = 160,
		MINUS = 161,
		LTMARK = 162,
		GTMARK = 163,
		NOT_OP = 164,
		EQ_OP = 165,
		NE_OP = 166,
		LE_OP = 167,
		LT_OP = 168,
		GE_OP = 169,
		GT_OP = 170,
		AND_OP = 171,
		OR_OP = 172,
		XOR_OP = 173,
		LOG_AND = 174,
		LOG_OR = 175,
		LOG_NEG = 176,
		QUESTION = 177,
		STRING = 178,
		INCLUDE = 179,
		EOL = 180,
		W = 181,
		D = 182,
		L = 183,
		H = 184,
		O = 185,
		EXP = 186,
		DBL_E = 187,
		DBL = 188,
		CONSTANT_OR_STRING_LITERAL = 189,
		COMMENT = 190,
		END_MARKER = 191,
		WHITESPACE = 192,
		SKIP_LINES = 193,
		CONT_STATEMENT = 194,
		END_OF_LINE = 195,
		NULL_TREE_LOOKAHEAD = 3
	};
#ifdef __cplusplus
};
#endif
#endif /*INC_GDLTreeParserTokenTypes_hpp_*/
