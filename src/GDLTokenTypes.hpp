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
		ASSIGN_ARRAYEXPR_MFCALL = 8,
		ARRAYDEF = 9,
		ARRAYDEF_CONST = 10,
		ARRAYIX = 11,
		ARRAYIX_ALL = 12,
		ARRAYIX_ORANGE = 13,
		ARRAYIX_RANGE = 14,
		ARRAYIX_ORANGE_S = 15,
		ARRAYIX_RANGE_S = 16,
		ARRAYEXPR = 17,
		ARRAYEXPR_FN = 18,
		ARRAYEXPR_MFCALL = 19,
		BLOCK = 20,
		BREAK = 21,
		CONTINUE = 22,
		COMMONDECL = 23,
		COMMONDEF = 24,
		CONSTANT = 25,
		DEREF = 26,
		ELSEBLK = 27,
		EXPR = 28,
		FOR = 29,
		FOR_STEP = 30,
		FOREACH = 31,
		FCALL = 32,
		FCALL_LIB = 33,
		FCALL_LIB_RETNEW = 34,
		IF_ELSE = 35,
		KEYDECL = 36,
		KEYDEF = 37,
		KEYDEF_REF = 38,
		KEYDEF_REF_CHECK = 39,
		KEYDEF_REF_EXPR = 40,
		LABEL = 41,
		MPCALL = 42,
		MPCALL_PARENT = 43,
		MFCALL = 44,
		MFCALL_LIB = 45,
		MFCALL_LIB_RETNEW = 46,
		MFCALL_PARENT = 47,
		MFCALL_PARENT_LIB = 48,
		MFCALL_PARENT_LIB_RETNEW = 49,
		NOP = 50,
		NSTRUC = 51,
		NSTRUC_REF = 52,
		ON_IOERROR_NULL = 53,
		PCALL = 54,
		PCALL_LIB = 55,
		PARADECL = 56,
		PARAEXPR = 57,
		POSTDEC = 58,
		POSTINC = 59,
		DECSTATEMENT = 60,
		INCSTATEMENT = 61,
		REF = 62,
		REF_CHECK = 63,
		REF_EXPR = 64,
		RETURN = 65,
		RETF = 66,
		RETP = 67,
		STRUC = 68,
		SYSVAR = 69,
		UMINUS = 70,
		VAR = 71,
		VARPTR = 72,
		IDENTIFIER = 73,
		AND_OP = 74,
		BEGIN = 75,
		CASE = 76,
		COMMON = 77,
		COMPILE_OPT = 78,
		DO = 79,
		ELSE = 80,
		END = 81,
		ENDCASE = 82,
		ENDELSE = 83,
		ENDFOR = 84,
		ENDFOREACH = 85,
		ENDIF = 86,
		ENDREP = 87,
		ENDSWITCH = 88,
		ENDWHILE = 89,
		EQ_OP = 90,
		FORWARD = 91,
		FUNCTION = 92,
		GE_OP = 93,
		GOTO = 94,
		GT_OP = 95,
		IF = 96,
		INHERITS = 97,
		LE_OP = 98,
		LT_OP = 99,
		MOD_OP = 100,
		NE_OP = 101,
		NOT_OP = 102,
		OF = 103,
		ON_IOERROR = 104,
		OR_OP = 105,
		PRO = 106,
		REPEAT = 107,
		SWITCH = 108,
		THEN = 109,
		UNTIL = 110,
		WHILE = 111,
		XOR_OP = 112,
		METHOD = 113,
		COMMA = 114,
		COLON = 115,
		END_U = 116,
		EQUAL = 117,
		DEC = 118,
		INC = 119,
		AND_OP_EQ = 120,
		ASTERIX_EQ = 121,
		EQ_OP_EQ = 122,
		GE_OP_EQ = 123,
		GTMARK_EQ = 124,
		GT_OP_EQ = 125,
		LE_OP_EQ = 126,
		LTMARK_EQ = 127,
		LT_OP_EQ = 128,
		MATRIX_OP1_EQ = 129,
		MATRIX_OP2_EQ = 130,
		MINUS_EQ = 131,
		MOD_OP_EQ = 132,
		NE_OP_EQ = 133,
		OR_OP_EQ = 134,
		PLUS_EQ = 135,
		POW_EQ = 136,
		SLASH_EQ = 137,
		XOR_OP_EQ = 138,
		MEMBER = 139,
		LBRACE = 140,
		RBRACE = 141,
		SLASH = 142,
		LSQUARE = 143,
		RSQUARE = 144,
		SYSVARNAME = 145,
		EXCLAMATION = 146,
		LCURLY = 147,
		RCURLY = 148,
		CONSTANT_HEX_BYTE = 149,
		CONSTANT_HEX_LONG = 150,
		CONSTANT_HEX_LONG64 = 151,
		CONSTANT_HEX_INT = 152,
		CONSTANT_HEX_I = 153,
		CONSTANT_HEX_ULONG = 154,
		CONSTANT_HEX_ULONG64 = 155,
		CONSTANT_HEX_UI = 156,
		CONSTANT_HEX_UINT = 157,
		CONSTANT_BYTE = 158,
		CONSTANT_LONG = 159,
		CONSTANT_LONG64 = 160,
		CONSTANT_INT = 161,
		CONSTANT_I = 162,
		CONSTANT_ULONG = 163,
		CONSTANT_ULONG64 = 164,
		CONSTANT_UI = 165,
		CONSTANT_UINT = 166,
		CONSTANT_OCT_BYTE = 167,
		CONSTANT_OCT_LONG = 168,
		CONSTANT_OCT_LONG64 = 169,
		CONSTANT_OCT_INT = 170,
		CONSTANT_OCT_I = 171,
		CONSTANT_OCT_ULONG = 172,
		CONSTANT_OCT_ULONG64 = 173,
		CONSTANT_OCT_UI = 174,
		CONSTANT_OCT_UINT = 175,
		CONSTANT_FLOAT = 176,
		CONSTANT_DOUBLE = 177,
		CONSTANT_BIN_BYTE = 178,
		CONSTANT_BIN_LONG = 179,
		CONSTANT_BIN_LONG64 = 180,
		CONSTANT_BIN_INT = 181,
		CONSTANT_BIN_I = 182,
		CONSTANT_BIN_ULONG = 183,
		CONSTANT_BIN_ULONG64 = 184,
		CONSTANT_BIN_UI = 185,
		CONSTANT_BIN_UINT = 186,
		ASTERIX = 187,
		DOT = 188,
		STRING_LITERAL = 189,
		POW = 190,
		MATRIX_OP1 = 191,
		MATRIX_OP2 = 192,
		PLUS = 193,
		MINUS = 194,
		LTMARK = 195,
		GTMARK = 196,
		LOG_NEG = 197,
		LOG_AND = 198,
		LOG_OR = 199,
		QUESTION = 200,
		STRING = 201,
		INCLUDE = 202,
		EOL = 203,
		W = 204,
		D = 205,
		L = 206,
		H = 207,
		O = 208,
		B = 209,
		EXP = 210,
		DBL_E = 211,
		DBL = 212,
		CONSTANT_OR_STRING_LITERAL = 213,
		COMMENT = 214,
		END_MARKER = 215,
		WHITESPACE = 216,
		SKIP_LINES = 217,
		CONT_STATEMENT = 218,
		END_OF_LINE = 219,
		NULL_TREE_LOOKAHEAD = 3
	};
#ifdef __cplusplus
};
#endif
#endif /*INC_GDLTokenTypes_hpp_*/
