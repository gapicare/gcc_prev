// lex.cc -- PREV frontend lexer.

#include "prev-system.h"

#include "lex.h"

/*
 * class Token
 */

// Token constructor.
Token::Token(Token_type type, location_t location, std::string lexeme)
	: type_(type), location_(location), lexeme_(new std::string (lexeme))
{

}

// Get token type.
Token::Token_type
Token::get_type()
{
	return this->type_;
}

// Get token location.
location_t
Token::get_location()
{
	return this->location_;
}

// For debugging.
void
Token::print(FILE *file)
{
	switch(this->type_)
	{
		case TOKEN_INVALID:
			fprintf(file, "<INVALID \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_EOF:
			fprintf(file, "<EOF \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_ADD:
			fprintf(file, "<ADD \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_ADDR:
			fprintf(file, "<ADDR \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_AND:
			fprintf(file, "<AND \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_ARR:
			fprintf(file, "<ARR \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_ASSIGN:
			fprintf(file, "<ASSIGN \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_BOOL:
			fprintf(file, "<BOOL \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_BOOLCONST:
			fprintf(file, "<BOOLCONST \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_CHAR:
			fprintf(file, "<CHAR \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_CHARCONST:
			fprintf(file, "<CHARCONST \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_COLON:
			fprintf(file, "<COLON \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_COMMA:
			fprintf(file, "<COMMA \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_DATA:
			fprintf(file, "<DATA \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_DEL:
			fprintf(file, "<DEL \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_DIV:
			fprintf(file, "<DIV \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_DO:
			fprintf(file, "<DO \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_DOT:
			fprintf(file, "<DOT \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_ELSE:
			fprintf(file, "<ELSE \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_END:
			fprintf(file, "<END \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_EQU:
			fprintf(file, "<EQU \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_FUN:
			fprintf(file, "<FUN \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_GEQ:
			fprintf(file, "<GEQ \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_GTH:
			fprintf(file, "<GTH \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_IDENTIFIER:
			fprintf(file, "<IDENTIFIER \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_IF:
			fprintf(file, "<IF \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_INT:
			fprintf(file, "<INT \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_INTCONST:
			fprintf(file, "<INTCONST \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_IOR:
			fprintf(file, "<IOR \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_LBRACE:
			fprintf(file, "<LBRACE \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_LBRACKET:
			fprintf(file, "<LBRACKET \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_LEQ:
			fprintf(file, "<LEQ \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_LPARENTHESIS:
			fprintf(file, "<LPARENTHESIS \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_LTH:
			fprintf(file, "<LTH \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_MOD:
			fprintf(file, "<MOD \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_MUL:
			fprintf(file, "<MUL \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_NEQ:
			fprintf(file, "<NEQ \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_NEW:
			fprintf(file, "<NEW \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_NOT:
			fprintf(file, "<NOT \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_PTR:
			fprintf(file, "<PTR \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_PTRCONST:
			fprintf(file, "<PTRCONST \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_RBRACE:
			fprintf(file, "<RBRACE \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_RBRACKET:
			fprintf(file, "<RBRACKET \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_REC:
			fprintf(file, "<REC \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_RPARENTHESIS:
			fprintf(file, "<RPARENTHESIS \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_SEMIC:
			fprintf(file, "<SEMIC \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_STRCONST:
			fprintf(file, "<STRCONST \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_SUB:
			fprintf(file, "<SUB \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_THEN:
			fprintf(file, "<THEN \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_TYP:
			fprintf(file, "<TYP \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_VAR:
			fprintf(file, "<VAR \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_VOID:
			fprintf(file, "<VOID \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_VOIDCONST:
			fprintf(file, "<VOIDCONST \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_WHERE:
			fprintf(file, "<WHERE \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_WHILE:
			fprintf(file, "<WHILE \"%s\">\n", (*this->lexeme_).c_str());
			break;
		case TOKEN_XOR:
			fprintf(file, "<XOR \"%s\">\n", (*this->lexeme_).c_str());
			break;
		default:
			prev_unreachable();
	}
}

/*
 * class Lex
 */

// Lex constructor.
Lex::Lex(const char *filename, FILE *file)
	: filename_(filename), file_(file), line_(1), column_(0),
	  prev_column_(0), line_map_(0), lex_file_(NULL)
{
	line_map_ = ::linemap_add(::line_table, ::LC_ENTER,
							 /* sysp */ 0, filename_,
							 /* current_line */ 1);
}

// Lex destructor.
Lex::~Lex()
{
	::linemap_add (::line_table, ::LC_LEAVE,
		 		   /* sysp */ 0,
				   /* filename */ NULL,
				   /* to_line */ 0);
}

// Returns next character from the input file.
int
Lex::get_char()
{
	int c;

	c = fgetc(this->file_);

	this->prev_column_ = this->column_;
	if (c == '\n')
	{
		(this->line_)++;
		this->column_ = 0;
		linemap_line_start(::line_table, line_, 80);
	}
	else
		(this->column_)++;

	return c;
}

// Pushes last read character back to the input stream.
void
Lex::unget_char(int c)
{
	this->column_ = this->prev_column_;
	if (c == '\n')
	{
		(this->line_)--;
		linemap_line_start(::line_table, line_, 80);
	}

	ungetc(c, file_);
}

// Returns next token from the input file.
Token*
Lex::next_token ()
{
	int c;
	Token::Token_type type;
	location_t location;
	std::string lexeme;
	Token *token;

	// Ignore whitespace and comments.
	while (1)
	{
		if ((c = get_char()) == '\t' || c == '\n' || c == '\r' || c == ' ')
			continue;
		else if (c == '#')
		{
			while ((c = get_char()) != EOF || c == '\n')
				continue;
			unget_char(c);
		}
		else
		{
			unget_char(c);
			break;
		}
	}

	c = get_char();

	location = ::linemap_position_for_column(::line_table, this->column_);
	lexeme.push_back(c);

	if (c == EOF)
		type = Token::TOKEN_EOF;
	else if (c == '!')
	{
		if ((c = get_char()) == '=')
		{
			type = Token::TOKEN_NEQ;
		}
		else
		{
			type = Token::TOKEN_NOT;
		}
	}
	else if (c == '"')
	{
		while (1)
		{
			if ((c = get_char()) == '"')
			{
				lexeme.push_back(c);
				type = Token::TOKEN_STRCONST;
				break;
			}
			else if (c >= ' ' && c <= '~')
			{
				lexeme.push_back(c);
				continue;
			}
			else
			{
				unget_char(c);
				error_at(location, "missing terminating \" character");
				type = Token::TOKEN_INVALID;
				break;
			}
		}
	}
	else if (c == '$')
		type = Token::TOKEN_ADDR;
	else if (c == '%')
		type = Token::TOKEN_MOD;
	else if (c == '&')
		type = Token::TOKEN_AND;
	else if (c == '\'')
	{
		c = get_char();
		lexeme.push_back(c);
		if ((c = get_char()) == '\'')
		{
			lexeme.push_back(c);
			type = Token::TOKEN_CHARCONST;
		}
		else
		{
			unget_char(c);
			error_at(location, "missing terminating ' character");
			type = Token::TOKEN_INVALID;
		}
	}
	else if (c == '(')
		type = Token::TOKEN_LPARENTHESIS;
	else if (c == ')')
		type = Token::TOKEN_RPARENTHESIS;
	else if (c == '*')
		type = Token::TOKEN_MUL;
	else if (c == '+')
		type = Token::TOKEN_ADD;
	else if (c == ',')
		type = Token::TOKEN_COMMA;
	else if (c == '-')
		type = Token::TOKEN_SUB;
	else if (c == '.')
		type = Token::TOKEN_DOT;
	else if (c == '/')
		type = Token::TOKEN_DIV;
	else if (c >= '0' && c <= '9')
	{
		while ((c = get_char()) >= '0' && c <= '9')
			lexeme.push_back(c);
		unget_char(c);
		type = Token::TOKEN_INTCONST;
	}
	else if (c == ':')
		type = Token::TOKEN_COLON;
	else if (c == ';')
		type = Token::TOKEN_SEMIC;
	else if (c == '<')
	{
		if ((c = get_char()) == '=')
		{
			lexeme.push_back(c);
			type = Token::TOKEN_LEQ;
		}
		else
		{
			unget_char(c);
			type = Token::TOKEN_LTH;
		}
	}
	else if (c == '=')
	{
		if ((c = get_char()) == '=')
		{
			lexeme.push_back(c);
			type = Token::TOKEN_EQU;
		}
		else
		{
			unget_char(c);
			type = Token::TOKEN_ASSIGN;
		}
	}
	else if (c == '>')
	{
		if ((c = get_char()) == '=')
		{
			lexeme.push_back(c);
			type = Token::TOKEN_GEQ;
		}
		else
		{
			unget_char(c);
			type = Token::TOKEN_GTH;
		}
	}
	else if (c == '@')
		type = Token::TOKEN_DATA;
	else if ((c >= 'A' && c <= 'Z') || c == '_' || (c >= 'a' && c <= 'z'))
	{
		while (((c = get_char()) >= '0' && c <= '9') ||
			   (c >= 'A' && c <= 'Z') || c == '_' || (c >= 'a' && c <= 'z'))
		    lexeme.push_back(c);
		unget_char(c);

		if (lexeme.compare("arr") == 0)
			type = Token::TOKEN_ARR;
		else if (lexeme.compare("bool") == 0)
			type = Token::TOKEN_BOOL;
		else if (lexeme.compare("char") == 0)
			type = Token::TOKEN_CHAR;
		else if (lexeme.compare("del") == 0)
			type = Token::TOKEN_DEL;
		else if (lexeme.compare("do") == 0)
			type = Token::TOKEN_DO;
		else if (lexeme.compare("else") == 0)
			type = Token::TOKEN_ELSE;
		else if (lexeme.compare("end") == 0)
			type = Token::TOKEN_END;
		else if (lexeme.compare("false") == 0 ||
				 lexeme.compare("true") == 0)
			type = Token::TOKEN_BOOLCONST;
		else if (lexeme.compare("fun") == 0)
			type = Token::TOKEN_FUN;
		else if (lexeme.compare("if") == 0)
			type = Token::TOKEN_IF;
		else if (lexeme.compare("int") == 0)
			type = Token::TOKEN_INT;
		else if (lexeme.compare("new") == 0)
			type = Token::TOKEN_NEW;
		else if (lexeme.compare("none") == 0)
			type = Token::TOKEN_VOIDCONST;
		else if (lexeme.compare("null") == 0)
			type = Token::TOKEN_PTRCONST;
		else if (lexeme.compare("ptr") == 0)
			type = Token::TOKEN_PTR;
		else if (lexeme.compare("rec") == 0)
			type = Token::TOKEN_REC;
		else if (lexeme.compare("then") == 0)
			type = Token::TOKEN_THEN;
		else if (lexeme.compare("typ") == 0)
			type = Token::TOKEN_TYP;
		else if (lexeme.compare("var") == 0)
			type = Token::TOKEN_VAR;
		else if (lexeme.compare("void") == 0)
			type = Token::TOKEN_VOID;
		else if (lexeme.compare("where") == 0)
			type = Token::TOKEN_WHERE;
		else if (lexeme.compare("while") == 0)
			type = Token::TOKEN_WHILE;
		else
			type = Token::TOKEN_IDENTIFIER;
	}
	else if (c == '[')
		type = Token::TOKEN_LBRACKET;
	else if (c == ']')
		type = Token::TOKEN_RBRACKET;
	else if (c == '^')
		type = Token::TOKEN_XOR;
	else if (c == '{')
		type = Token::TOKEN_LBRACE;
	else if (c == '|')
		type = Token::TOKEN_IOR;
	else if (c == '}')
		type = Token::TOKEN_RBRACE;
	else
	{
		error_at(location, "unexpected character '%x'", c);
		type = Token::TOKEN_INVALID;
	}

	token = new Token(type, location, lexeme);

	token->print(lex_file_);

	return token;
}

void
Lex::open_lex_file()
{
	lex_file_ = fopen("lex.out", "w");
	if (lex_file_ == NULL)
		fatal_error(UNKNOWN_LOCATION, "cannot open %s: %m", "lex.out");
}

void
Lex::close_lex_file()
{
	fclose(lex_file_);
}
