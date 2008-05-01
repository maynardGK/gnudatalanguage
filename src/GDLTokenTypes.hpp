#ifndef INC_GDLTokenTypes_hpp_
#define INC_GDLTokenTypes_hpp_

/* $ANTLR 2.7.6 (20071205): "gdlc.g" -> "GDLTokenTypes.hpp"$ */

#ifndef CUSTOM_API
# define CUSTOM_API
#endif

#ifdef __cplusplus
struct CUSTOM_API GDLTokenTypes {
#endif
	enum {
		EOF_ = 1,
		ALL = 4,
		ASSIGN = 5,
		ASSIGN_INPLACE = 6,
		ASSIGN_REPLACE = 7,
		ARRAYDEF = 8,
		ARRAYIX = 9,
		ARRAYIX_ALL = 10,
		ARRAYIX_ORANGE = 11,
		ARRAYIX_RANGE = 12,
		ARRAYIX_ORANGE_S = 13,
		ARRAYIX_RANGE_S = 14,
		ARRAYEXPR = 15,
		ARRAYEXPR_FN = 16,
		BLOCK = 17,
		BREAK = 18,
		CONTINUE = 19,
		COMMONDECL = 20,
		COMMONDEF = 21,
		CONSTANT = 22,
		DEREF = 23,
		ELSEBLK = 24,
		EXPR = 25,
		FOR_STEP = 26,
		FCALL = 27,
		FCALL_LIB = 28,
		FCALL_LIB_RETNEW = 29,
		IF_ELSE = 30,
		KEYDECL = 31,
		KEYDEF = 32,
		KEYDEF_REF = 33,
		KEYDEF_REF_CHECK = 34,
		KEYDEF_REF_EXPR = 35,
		LABEL = 36,
		MPCALL = 37,
		MPCALL_PARENT = 38,
		MFCALL = 39,
		MFCALL_LIB = 40,
		MFCALL_LIB_RETNEW = 41,
		MFCALL_PARENT = 42,
		MFCALL_PARENT_LIB = 43,
		MFCALL_PARENT_LIB_RETNEW = 44,
		NOP = 45,
		NSTRUC = 46,
		NSTRUC_REF = 47,
		ON_IOERROR_NULL = 48,
		PCALL = 49,
		PCALL_LIB = 50,
		PARADECL = 51,
		POSTDEC = 52,
		POSTINC = 53,
		DECSTATEMENT = 54,
		INCSTATEMENT = 55,
		REF = 56,
		REF_CHECK = 57,
		REF_EXPR = 58,
		RETURN = 59,
		RETF = 60,
		RETP = 61,
		STRUC = 62,
		SYSVAR = 63,
		UMINUS = 64,
		VAR = 65,
		VARPTR = 66,
		END = 67,
		FUNCTION = 68,
		PRO = 69,
		IDENTIFIER = 70,
		METHOD = 71,
		COMMA = 72,
		BEGIN = 73,
		COLON = 74,
		SWITCH = 75,
		OF = 76,
		ELSE = 77,
		CASE = 78,
		END_U = 79,
		FORWARD = 80,
		EQUAL = 81,
		COMPILE_OPT = 82,
		COMMON = 83,
		ENDIF = 84,
		ENDELSE = 85,
		ENDCASE = 86,
		ENDSWITCH = 87,
		ENDFOR = 88,
		ENDWHILE = 89,
		ENDREP = 90,
		DEC = 91,
		INC = 92,
		AND_OP_EQ = 93,
		ASTERIX_EQ = 94,
		EQ_OP_EQ = 95,
		GE_OP_EQ = 96,
		GTMARK_EQ = 97,
		GT_OP_EQ = 98,
		LE_OP_EQ = 99,
		LTMARK_EQ = 100,
		LT_OP_EQ = 101,
		MATRIX_OP1_EQ = 102,
		MATRIX_OP2_EQ = 103,
		MINUS_EQ = 104,
		MOD_OP_EQ = 105,
		NE_OP_EQ = 106,
		OR_OP_EQ = 107,
		PLUS_EQ = 108,
		POW_EQ = 109,
		SLASH_EQ = 110,
		XOR_OP_EQ = 111,
		MEMBER = 112,
		REPEAT = 113,
		UNTIL = 114,
		WHILE = 115,
		DO = 116,
		FOR = 117,
		GOTO = 118,
		ON_IOERROR = 119,
		IF = 120,
		THEN = 121,
		LBRACE = 122,
		RBRACE = 123,
		SLASH = 124,
		LSQUARE = 125,
		RSQUARE = 126,
		SYSVARNAME = 127,
		EXCLAMATION = 128,
		INHERITS = 129,
		LCURLY = 130,
		RCURLY = 131,
		CONSTANT_HEX_BYTE = 132,
		CONSTANT_HEX_LONG = 133,
		CONSTANT_HEX_LONG64 = 134,
		CONSTANT_HEX_INT = 135,
		CONSTANT_HEX_I = 136,
		CONSTANT_HEX_ULONG = 137,
		CONSTANT_HEX_ULONG64 = 138,
		CONSTANT_HEX_UI = 139,
		CONSTANT_HEX_UINT = 140,
		CONSTANT_BYTE = 141,
		CONSTANT_LONG = 142,
		CONSTANT_LONG64 = 143,
		CONSTANT_INT = 144,
		CONSTANT_I = 145,
		CONSTANT_ULONG = 146,
		CONSTANT_ULONG64 = 147,
		CONSTANT_UI = 148,
		CONSTANT_UINT = 149,
		CONSTANT_OCT_BYTE = 150,
		CONSTANT_OCT_LONG = 151,
		CONSTANT_OCT_LONG64 = 152,
		CONSTANT_OCT_INT = 153,
		CONSTANT_OCT_I = 154,
		CONSTANT_OCT_ULONG = 155,
		CONSTANT_OCT_ULONG64 = 156,
		CONSTANT_OCT_UI = 157,
		CONSTANT_OCT_UINT = 158,
		CONSTANT_FLOAT = 159,
		CONSTANT_DOUBLE = 160,
		ASTERIX = 161,
		DOT = 162,
		STRING_LITERAL = 163,
		POW = 164,
		MATRIX_OP1 = 165,
		MATRIX_OP2 = 166,
		MOD_OP = 167,
		PLUS = 168,
		MINUS = 169,
		LTMARK = 170,
		GTMARK = 171,
		NOT_OP = 172,
		LOG_NEG = 173,
		EQ_OP = 174,
		NE_OP = 175,
		LE_OP = 176,
		LT_OP = 177,
		GE_OP = 178,
		GT_OP = 179,
		AND_OP = 180,
		OR_OP = 181,
		XOR_OP = 182,
		LOG_AND = 183,
		LOG_OR = 184,
		QUESTION = 185,
		STRING = 186,
		INCLUDE = 187,
		EOL = 188,
		W = 189,
		D = 190,
		L = 191,
		H = 192,
		O = 193,
		EXP = 194,
		DBL_E = 195,
		DBL = 196,
		CONSTANT_OR_STRING_LITERAL = 197,
		COMMENT = 198,
		END_MARKER = 199,
		WHITESPACE = 200,
		SKIP_LINES = 201,
		CONT_STATEMENT = 202,
		END_OF_LINE = 203,
		NULL_TREE_LOOKAHEAD = 3
	};
#ifdef __cplusplus
};
#endif
#endif /*INC_GDLTokenTypes_hpp_*/
