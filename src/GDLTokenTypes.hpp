#ifndef INC_GDLTokenTypes_hpp_
#define INC_GDLTokenTypes_hpp_

/* $ANTLR 2.7.4: "gdlc.g" -> "GDLTokenTypes.hpp"$ */

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
		ASSIGN_REPLACE = 6,
		ARRAYDEF = 7,
		ARRAYIX = 8,
		ARRAYIX_ALL = 9,
		ARRAYIX_ORANGE = 10,
		ARRAYIX_RANGE = 11,
		ARRAYIX_ORANGE_S = 12,
		ARRAYIX_RANGE_S = 13,
		ARRAYEXPR = 14,
		ARRAYEXPR_FN = 15,
		BLOCK = 16,
		BREAK = 17,
		CONTINUE = 18,
		COMMONDECL = 19,
		COMMONDEF = 20,
		CONSTANT = 21,
		DEREF = 22,
		ELSEBLK = 23,
		EXPR = 24,
		FOR_STEP = 25,
		FCALL = 26,
		FCALL_LIB = 27,
		IF_ELSE = 28,
		KEYDECL = 29,
		KEYDEF = 30,
		KEYDEF_REF = 31,
		KEYDEF_REF_CHECK = 32,
		KEYDEF_REF_EXPR = 33,
		LABEL = 34,
		MPCALL = 35,
		MPCALL_PARENT = 36,
		MFCALL = 37,
		MFCALL_LIB = 38,
		MFCALL_PARENT = 39,
		MFCALL_PARENT_LIB = 40,
		NOP = 41,
		NSTRUC = 42,
		NSTRUC_REF = 43,
		ON_IOERROR_NULL = 44,
		PCALL = 45,
		PCALL_LIB = 46,
		PARADECL = 47,
		POSTDEC = 48,
		POSTINC = 49,
		DECSTATEMENT = 50,
		INCSTATEMENT = 51,
		REF = 52,
		REF_CHECK = 53,
		REF_EXPR = 54,
		RETURN = 55,
		RETF = 56,
		RETP = 57,
		STRUC = 58,
		SYSVAR = 59,
		UMINUS = 60,
		VAR = 61,
		VARPTR = 62,
		END = 63,
		FUNCTION = 64,
		PRO = 65,
		IDENTIFIER = 66,
		METHOD = 67,
		COMMA = 68,
		BEGIN = 69,
		COLON = 70,
		SWITCH = 71,
		OF = 72,
		ELSE = 73,
		CASE = 74,
		END_U = 75,
		FORWARD = 76,
		EQUAL = 77,
		COMPILE_OPT = 78,
		COMMON = 79,
		ENDIF = 80,
		ENDELSE = 81,
		ENDCASE = 82,
		ENDSWITCH = 83,
		ENDFOR = 84,
		ENDWHILE = 85,
		ENDREP = 86,
		DEC = 87,
		INC = 88,
		AND_OP_EQ = 89,
		ASTERIX_EQ = 90,
		EQ_OP_EQ = 91,
		GE_OP_EQ = 92,
		GTMARK_EQ = 93,
		GT_OP_EQ = 94,
		LE_OP_EQ = 95,
		LTMARK_EQ = 96,
		LT_OP_EQ = 97,
		MATRIX_OP1_EQ = 98,
		MATRIX_OP2_EQ = 99,
		MINUS_EQ = 100,
		MOD_OP_EQ = 101,
		NE_OP_EQ = 102,
		OR_OP_EQ = 103,
		PLUS_EQ = 104,
		POW_EQ = 105,
		SLASH_EQ = 106,
		XOR_OP_EQ = 107,
		MEMBER = 108,
		REPEAT = 109,
		UNTIL = 110,
		WHILE = 111,
		DO = 112,
		FOR = 113,
		GOTO = 114,
		ON_IOERROR = 115,
		IF = 116,
		THEN = 117,
		LBRACE = 118,
		RBRACE = 119,
		SLASH = 120,
		LSQUARE = 121,
		RSQUARE = 122,
		SYSVARNAME = 123,
		EXCLAMATION = 124,
		LCURLY = 125,
		RCURLY = 126,
		INHERITS = 127,
		CONSTANT_HEX_BYTE = 128,
		CONSTANT_HEX_LONG = 129,
		CONSTANT_HEX_LONG64 = 130,
		CONSTANT_HEX_INT = 131,
		CONSTANT_HEX_I = 132,
		CONSTANT_HEX_ULONG = 133,
		CONSTANT_HEX_ULONG64 = 134,
		CONSTANT_HEX_UI = 135,
		CONSTANT_HEX_UINT = 136,
		CONSTANT_BYTE = 137,
		CONSTANT_LONG = 138,
		CONSTANT_LONG64 = 139,
		CONSTANT_INT = 140,
		CONSTANT_I = 141,
		CONSTANT_ULONG = 142,
		CONSTANT_ULONG64 = 143,
		CONSTANT_UI = 144,
		CONSTANT_UINT = 145,
		CONSTANT_OCT_BYTE = 146,
		CONSTANT_OCT_LONG = 147,
		CONSTANT_OCT_LONG64 = 148,
		CONSTANT_OCT_INT = 149,
		CONSTANT_OCT_I = 150,
		CONSTANT_OCT_ULONG = 151,
		CONSTANT_OCT_ULONG64 = 152,
		CONSTANT_OCT_UI = 153,
		CONSTANT_OCT_UINT = 154,
		CONSTANT_FLOAT = 155,
		CONSTANT_DOUBLE = 156,
		ASTERIX = 157,
		DOT = 158,
		STRING_LITERAL = 159,
		POW = 160,
		MATRIX_OP1 = 161,
		MATRIX_OP2 = 162,
		MOD_OP = 163,
		PLUS = 164,
		MINUS = 165,
		LTMARK = 166,
		GTMARK = 167,
		NOT_OP = 168,
		EQ_OP = 169,
		NE_OP = 170,
		LE_OP = 171,
		LT_OP = 172,
		GE_OP = 173,
		GT_OP = 174,
		AND_OP = 175,
		OR_OP = 176,
		XOR_OP = 177,
		LOG_NEG = 178,
		LOG_AND = 179,
		LOG_OR = 180,
		QUESTION = 181,
		STRING = 182,
		INCLUDE = 183,
		EOL = 184,
		W = 185,
		D = 186,
		L = 187,
		H = 188,
		O = 189,
		EXP = 190,
		DBL_E = 191,
		DBL = 192,
		CONSTANT_OR_STRING_LITERAL = 193,
		COMMENT = 194,
		END_MARKER = 195,
		WHITESPACE = 196,
		SKIP_LINES = 197,
		CONT_STATEMENT = 198,
		END_OF_LINE = 199,
		NULL_TREE_LOOKAHEAD = 3
	};
#ifdef __cplusplus
};
#endif
#endif /*INC_GDLTokenTypes_hpp_*/
