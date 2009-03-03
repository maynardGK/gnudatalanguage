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
		ARRAYDEF_CONST = 9,
		ARRAYIX = 10,
		ARRAYIX_ALL = 11,
		ARRAYIX_ORANGE = 12,
		ARRAYIX_RANGE = 13,
		ARRAYIX_ORANGE_S = 14,
		ARRAYIX_RANGE_S = 15,
		ARRAYEXPR = 16,
		ARRAYEXPR_FN = 17,
		BLOCK = 18,
		BREAK = 19,
		CONTINUE = 20,
		COMMONDECL = 21,
		COMMONDEF = 22,
		CONSTANT = 23,
		DEREF = 24,
		ELSEBLK = 25,
		EXPR = 26,
		FOR_STEP = 27,
		FCALL = 28,
		FCALL_LIB = 29,
		FCALL_LIB_RETNEW = 30,
		IF_ELSE = 31,
		KEYDECL = 32,
		KEYDEF = 33,
		KEYDEF_REF = 34,
		KEYDEF_REF_CHECK = 35,
		KEYDEF_REF_EXPR = 36,
		LABEL = 37,
		MPCALL = 38,
		MPCALL_PARENT = 39,
		MFCALL = 40,
		MFCALL_LIB = 41,
		MFCALL_LIB_RETNEW = 42,
		MFCALL_PARENT = 43,
		MFCALL_PARENT_LIB = 44,
		MFCALL_PARENT_LIB_RETNEW = 45,
		NOP = 46,
		NSTRUC = 47,
		NSTRUC_REF = 48,
		ON_IOERROR_NULL = 49,
		PCALL = 50,
		PCALL_LIB = 51,
		PARADECL = 52,
		PARAEXPR = 53,
		POSTDEC = 54,
		POSTINC = 55,
		DECSTATEMENT = 56,
		INCSTATEMENT = 57,
		REF = 58,
		REF_CHECK = 59,
		REF_EXPR = 60,
		RETURN = 61,
		RETF = 62,
		RETP = 63,
		STRUC = 64,
		SYSVAR = 65,
		UMINUS = 66,
		VAR = 67,
		VARPTR = 68,
		IDENTIFIER = 69,
		AND_OP = 70,
		BEGIN = 71,
		CASE = 72,
		COMMON = 73,
		COMPILE_OPT = 74,
		DO = 75,
		ELSE = 76,
		END = 77,
		ENDCASE = 78,
		ENDELSE = 79,
		ENDFOR = 80,
		ENDIF = 81,
		ENDREP = 82,
		ENDSWITCH = 83,
		ENDWHILE = 84,
		EQ_OP = 85,
		FOR = 86,
		FORWARD = 87,
		FUNCTION = 88,
		GE_OP = 89,
		GOTO = 90,
		GT_OP = 91,
		IF = 92,
		INHERITS = 93,
		LE_OP = 94,
		LT_OP = 95,
		MOD_OP = 96,
		NE_OP = 97,
		NOT_OP = 98,
		OF = 99,
		ON_IOERROR = 100,
		OR_OP = 101,
		PRO = 102,
		REPEAT = 103,
		SWITCH = 104,
		THEN = 105,
		UNTIL = 106,
		WHILE = 107,
		XOR_OP = 108,
		METHOD = 109,
		COMMA = 110,
		COLON = 111,
		END_U = 112,
		EQUAL = 113,
		DEC = 114,
		INC = 115,
		AND_OP_EQ = 116,
		ASTERIX_EQ = 117,
		EQ_OP_EQ = 118,
		GE_OP_EQ = 119,
		GTMARK_EQ = 120,
		GT_OP_EQ = 121,
		LE_OP_EQ = 122,
		LTMARK_EQ = 123,
		LT_OP_EQ = 124,
		MATRIX_OP1_EQ = 125,
		MATRIX_OP2_EQ = 126,
		MINUS_EQ = 127,
		MOD_OP_EQ = 128,
		NE_OP_EQ = 129,
		OR_OP_EQ = 130,
		PLUS_EQ = 131,
		POW_EQ = 132,
		SLASH_EQ = 133,
		XOR_OP_EQ = 134,
		MEMBER = 135,
		LBRACE = 136,
		RBRACE = 137,
		SLASH = 138,
		LSQUARE = 139,
		RSQUARE = 140,
		SYSVARNAME = 141,
		EXCLAMATION = 142,
		LCURLY = 143,
		RCURLY = 144,
		CONSTANT_HEX_BYTE = 145,
		CONSTANT_HEX_LONG = 146,
		CONSTANT_HEX_LONG64 = 147,
		CONSTANT_HEX_INT = 148,
		CONSTANT_HEX_I = 149,
		CONSTANT_HEX_ULONG = 150,
		CONSTANT_HEX_ULONG64 = 151,
		CONSTANT_HEX_UI = 152,
		CONSTANT_HEX_UINT = 153,
		CONSTANT_BYTE = 154,
		CONSTANT_LONG = 155,
		CONSTANT_LONG64 = 156,
		CONSTANT_INT = 157,
		CONSTANT_I = 158,
		CONSTANT_ULONG = 159,
		CONSTANT_ULONG64 = 160,
		CONSTANT_UI = 161,
		CONSTANT_UINT = 162,
		CONSTANT_OCT_BYTE = 163,
		CONSTANT_OCT_LONG = 164,
		CONSTANT_OCT_LONG64 = 165,
		CONSTANT_OCT_INT = 166,
		CONSTANT_OCT_I = 167,
		CONSTANT_OCT_ULONG = 168,
		CONSTANT_OCT_ULONG64 = 169,
		CONSTANT_OCT_UI = 170,
		CONSTANT_OCT_UINT = 171,
		CONSTANT_FLOAT = 172,
		CONSTANT_DOUBLE = 173,
		ASTERIX = 174,
		DOT = 175,
		STRING_LITERAL = 176,
		POW = 177,
		MATRIX_OP1 = 178,
		MATRIX_OP2 = 179,
		PLUS = 180,
		MINUS = 181,
		LTMARK = 182,
		GTMARK = 183,
		LOG_NEG = 184,
		LOG_AND = 185,
		LOG_OR = 186,
		QUESTION = 187,
		STRING = 188,
		INCLUDE = 189,
		EOL = 190,
		W = 191,
		D = 192,
		L = 193,
		H = 194,
		O = 195,
		EXP = 196,
		DBL_E = 197,
		DBL = 198,
		CONSTANT_OR_STRING_LITERAL = 199,
		COMMENT = 200,
		END_MARKER = 201,
		WHITESPACE = 202,
		SKIP_LINES = 203,
		CONT_STATEMENT = 204,
		END_OF_LINE = 205,
		NULL_TREE_LOOKAHEAD = 3
	};
#ifdef __cplusplus
};
#endif
#endif /*INC_GDLTokenTypes_hpp_*/
