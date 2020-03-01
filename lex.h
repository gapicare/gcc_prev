// lex.h -- PREV frontend lexer.

#ifndef PREV_LEX_H
#define PREV_LEX_H

/*
 * PREV token.
 */
class Token
{
public:
	// All possible PREV token types.
	enum Token_type
	{
		TOKEN_INVALID,
		TOKEN_EOF,
		TOKEN_ADD,
		TOKEN_ADDR,
		TOKEN_AND,
		TOKEN_ARR,
		TOKEN_ASSIGN,
		TOKEN_BOOL,
		TOKEN_BOOLCONST,
		TOKEN_CHAR,
		TOKEN_CHARCONST,
		TOKEN_COLON,
		TOKEN_COMMA,
		TOKEN_DATA,
		TOKEN_DEL,
		TOKEN_DIV,
		TOKEN_DO,
		TOKEN_DOT,
		TOKEN_ELSE,
		TOKEN_END,
		TOKEN_EQU,
		TOKEN_FUN,
		TOKEN_GEQ,
		TOKEN_GTH,
		TOKEN_IDENTIFIER,
		TOKEN_IF,
		TOKEN_INT,
		TOKEN_INTCONST,
		TOKEN_IOR,
		TOKEN_LBRACE,
		TOKEN_LBRACKET,
		TOKEN_LEQ,
		TOKEN_LPARENTHESIS,
		TOKEN_LTH,
		TOKEN_MOD,
		TOKEN_MUL,
		TOKEN_NEQ,
		TOKEN_NEW,
		TOKEN_NOT,
		TOKEN_PTR,
		TOKEN_PTRCONST,
		TOKEN_RBRACE,
		TOKEN_RBRACKET,
		TOKEN_REC,
		TOKEN_RPARENTHESIS,
		TOKEN_SEMIC,
		TOKEN_STRCONST,
		TOKEN_SUB,
		TOKEN_THEN,
		TOKEN_TYP,
		TOKEN_VAR,
		TOKEN_VOID,
		TOKEN_VOIDCONST,
		TOKEN_WHERE,
		TOKEN_WHILE,
		TOKEN_XOR
	};

	Token(Token_type type, location_t location, std::string lexeme);

	Token_type
	get_type();

	void
	print(FILE *file);

private:
	Token_type type_;
	location_t location_;
	std::string *lexeme_;
};

/*
 * PREV lexer.
 */
class Lex
{
public:
	Lex(const char *filename, FILE *file);
	~Lex();

	Token*
	next_token();

private:

	int
	get_char();

	void
	unget_char(int c);

	const char *filename_;
	FILE *file_;

	int line_;
	int column_;
	int prev_column_;
	const struct line_map *line_map_;
};

#endif // #ifndef PREV_LEX_H
