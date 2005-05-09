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
		BREAK = 14,
		CONTINUE = 15,
		COMMONDECL = 16,
		COMMONDEF = 17,
		CONSTANT = 18,
		DEREF = 19,
		ELSEBLK = 20,
		EXPR = 21,
		FOR_STEP = 22,
		FCALL = 23,
		FCALL_LIB = 24,
		IF_ELSE = 25,
		KEYDECL = 26,
		KEYDEF = 27,
		KEYDEF_REF = 28,
		KEYDEF_REF_CHECK = 29,
		KEYDEF_REF_EXPR = 30,
		LABEL = 31,
		MPCALL = 32,
		MPCALL_PARENT = 33,
		MFCALL = 34,
		MFCALL_LIB = 35,
		MFCALL_PARENT = 36,
		MFCALL_PARENT_LIB = 37,
		NOP = 38,
		NSTRUC = 39,
		NSTRUC_REF = 40,
		ON_IOERROR_NULL = 41,
		PCALL = 42,
		PCALL_LIB = 43,
		PARADECL = 44,
		POSTDEC = 45,
		POSTINC = 46,
		DECSTATEMENT = 47,
		INCSTATEMENT = 48,
		REF = 49,
		REF_CHECK = 50,
		REF_EXPR = 51,
		RETURN = 52,
		RETF = 53,
		RETP = 54,
		STRUC = 55,
		SYSVAR = 56,
		UMINUS = 57,
		VAR = 58,
		VARPTR = 59,
		END = 60,
		FUNCTION = 61,
		PRO = 62,
		IDENTIFIER = 63,
		METHOD = 64,
		COMMA = 65,
		BEGIN = 66,
		COLON = 67,
		SWITCH = 68,
		OF = 69,
		ELSE = 70,
		CASE = 71,
		END_U = 72,
		FORWARD = 73,
		EQUAL = 74,
		COMPILE_OPT = 75,
		COMMON = 76,
		ENDIF = 77,
		ENDELSE = 78,
		ENDCASE = 79,
		ENDSWITCH = 80,
		ENDFOR = 81,
		ENDWHILE = 82,
		ENDREP = 83,
		DEC = 84,
		INC = 85,
		AND_OP_EQ = 86,
		ASTERIX_EQ = 87,
		EQ_OP_EQ = 88,
		GE_OP_EQ = 89,
		GTMARK_EQ = 90,
		GT_OP_EQ = 91,
		LE_OP_EQ = 92,
		LTMARK_EQ = 93,
		LT_OP_EQ = 94,
		MATRIX_OP1_EQ = 95,
		MATRIX_OP2_EQ = 96,
		MINUS_EQ = 97,
		MOD_OP_EQ = 98,
		NE_OP_EQ = 99,
		OR_OP_EQ = 100,
		PLUS_EQ = 101,
		POW_EQ = 102,
		SLASH_EQ = 103,
		XOR_OP_EQ = 104,
		MEMBER = 105,
		REPEAT = 106,
		UNTIL = 107,
		WHILE = 108,
		DO = 109,
		FOR = 110,
		GOTO = 111,
		ON_IOERROR = 112,
		IF = 113,
		THEN = 114,
		LBRACE = 115,
		RBRACE = 116,
		SLASH = 117,
		LSQUARE = 118,
		RSQUARE = 119,
		SYSVARNAME = 120,
		EXCLAMATION = 121,
		LCURLY = 122,
		RCURLY = 123,
		INHERITS = 124,
		CONSTANT_HEX_BYTE = 125,
		CONSTANT_HEX_LONG = 126,
		CONSTANT_HEX_LONG64 = 127,
		CONSTANT_HEX_INT = 128,
		CONSTANT_HEX_I = 129,
		CONSTANT_HEX_ULONG = 130,
		CONSTANT_HEX_ULONG64 = 131,
		CONSTANT_HEX_UI = 132,
		CONSTANT_HEX_UINT = 133,
		CONSTANT_BYTE = 134,
		CONSTANT_LONG = 135,
		CONSTANT_LONG64 = 136,
		CONSTANT_INT = 137,
		CONSTANT_I = 138,
		CONSTANT_ULONG = 139,
		CONSTANT_ULONG64 = 140,
		CONSTANT_UI = 141,
		CONSTANT_UINT = 142,
		CONSTANT_OCT_BYTE = 143,
		CONSTANT_OCT_LONG = 144,
		CONSTANT_OCT_LONG64 = 145,
		CONSTANT_OCT_INT = 146,
		CONSTANT_OCT_I = 147,
		CONSTANT_OCT_ULONG = 148,
		CONSTANT_OCT_ULONG64 = 149,
		CONSTANT_OCT_UI = 150,
		CONSTANT_OCT_UINT = 151,
		CONSTANT_FLOAT = 152,
		CONSTANT_DOUBLE = 153,
		ASTERIX = 154,
		DOT = 155,
		STRING_LITERAL = 156,
		POW = 157,
		MATRIX_OP1 = 158,
		MATRIX_OP2 = 159,
		MOD_OP = 160,
		PLUS = 161,
		MINUS = 162,
		LTMARK = 163,
		GTMARK = 164,
		NOT_OP = 165,
		EQ_OP = 166,
		NE_OP = 167,
		LE_OP = 168,
		LT_OP = 169,
		GE_OP = 170,
		GT_OP = 171,
		AND_OP = 172,
		OR_OP = 173,
		XOR_OP = 174,
		LOG_AND = 175,
		LOG_OR = 176,
		LOG_NEG = 177,
		QUESTION = 178,
		STRING = 179,
		INCLUDE = 180,
		EOL = 181,
		W = 182,
		D = 183,
		L = 184,
		H = 185,
		O = 186,
		EXP = 187,
		DBL_E = 188,
		DBL = 189,
		CONSTANT_OR_STRING_LITERAL = 190,
		COMMENT = 191,
		END_MARKER = 192,
		WHITESPACE = 193,
		SKIP_LINES = 194,
		CONT_STATEMENT = 195,
		END_OF_LINE = 196,
		NULL_TREE_LOOKAHEAD = 3
	};
#ifdef __cplusplus
};
#endif
#endif /*INC_GDLTreeParserTokenTypes_hpp_*/
