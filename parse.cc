// parse.cc -- PREV frontend parser.

#include "prev-system.h"

#include "lex.h"
#include "parse.h"

/*
 * Class Parse
 */
Parse::Parse(Lex *lex)
	: lex_(lex), next_token_(NULL), der_tree_(NULL), parse_file_(NULL)
{

}

void
Parse::open_parse_file()
{
	parse_file_ = fopen("parse.out", "w");
	if (parse_file_ == NULL)
		fatal_error(UNKNOWN_LOCATION, "cannot open %s: %m", "parse.out");
}

void
Parse::close_parse_file()
{
	fclose(parse_file_);
}

void
Parse::print_production(const char *input)
{
	fprintf(parse_file_, "%s\n", input);
}

void
Parse::add(DerNode *node)
{
	node->add(new DerLeaf(next_token_));
	next_token_ = lex_->next_token();
}

void
Parse::parse()
{
	lex_->open_lex_file();
	open_parse_file();

	next_token_ = lex_->next_token();
	der_tree_ = parse_source();
	Token::Token_type type = next_token_->get_type();
	if (type != Token::TOKEN_EOF)
		fatal_error(next_token_->get_location(), "expected EOF token: %m");

	close_parse_file();
	lex_->close_lex_file();
}

DerTree *
Parse::parse_source()
{
	print_production("SOURCE");

	DerNode *node = new DerNode(DerNode::NONT_SOURCE);
	node->add(parse_decls());
	return node;
}

DerTree *
Parse::parse_decls()
{
	print_production("DECLS");

	DerNode *node = new DerNode(DerNode::NONT_DECLS);
	node->add(parse_decl());
	node->add(parse_decls_rest());
	return node;
}

DerTree *
Parse::parse_decls_rest()
{
	print_production("DECLS_REST");

	DerNode *node = new DerNode(DerNode::NONT_DECLSREST);
	switch (next_token_->get_type())
	{
		case Token::TOKEN_TYP:
		case Token::TOKEN_VAR:
		case Token::TOKEN_FUN:
			node->add(parse_decl());
			node->add(parse_decls_rest());
			break;
		case Token::TOKEN_RBRACE:
		case Token::TOKEN_EOF:
			break;
		default:
			fatal_error(next_token_->get_location(), "parser DECLS_REST error: %m");
	}
	return node;
}

DerTree *
Parse::parse_decl()
{
	print_production("DECL");

	DerNode *node = new DerNode(DerNode::NONT_DECL);
	switch (next_token_->get_type())
	{
		case Token::TOKEN_TYP:
			add(node);
			add(node);
			add(node);
			node->add(parse_type());
			add(node);
			break;
		case Token::TOKEN_VAR:
			add(node);
			add(node);
			add(node);
			node->add(parse_type());
			add(node);
			break;
		case Token::TOKEN_FUN:
			add(node);
			add(node);
			add(node);
			node->add(parse_par_decls_eps());
			add(node);
			add(node);
			node->add(parse_type());
			node->add(parse_body_eps());
			add(node);
			break;
		default:
			fatal_error(next_token_->get_location(), "parser DECL error: %m");
	}
}

DerTree *
Parse::parse_par_decls_eps()
{
	print_production("PAR_DECLS_EPS");

	DerNode *node = new DerNode(DerNode::NONT_PARDECLSEPS);
	switch (next_token_->get_type())
	{
		case Token::TOKEN_IDENTIFIER:
			node->add(parse_par_decls());
			break;
		case Token::TOKEN_RPARENTHESIS:
			break;
		default:
			fatal_error(next_token_->get_location(), "parser PAR_DECLS_EPS error: %m");
	}
	return node;
}

DerTree *
Parse::parse_par_decls()
{
	print_production("PAR_DECLS");

	DerNode *node = new DerNode(DerNode::NONT_PARDECLS);
	switch (next_token_->get_type())
	{
		case Token::TOKEN_IDENTIFIER:
			node->add(parse_par_decl());
			node->add(parse_par_decls_rest());
			break;
		default:
			fatal_error(next_token_->get_location(), "parser PAR_DECLS error: %m");
	}
	return node;
}

DerTree *
Parse::parse_par_decls_rest()
{
	print_production("PAR_DECLS_REST");

	DerNode *node = new DerNode(DerNode::NONT_PARDECLSREST);
	switch (next_token_->get_type())
	{
		case Token::TOKEN_RPARENTHESIS:
			break;
		case Token::TOKEN_COMMA:
			add(node);
			node->add(parse_par_decl());
			node->add(parse_par_decls_rest());
			break;
		default:
			fatal_error(next_token_->get_location(), "parser PAR_DECLS_REST error: %m");
	}
	return node;
}

DerTree *
Parse::parse_par_decl()
{
	print_production("PAR_DECL");

	DerNode *node = new DerNode(DerNode::NONT_PARDECL);
	switch (next_token_->get_type())
	{
		case Token::TOKEN_IDENTIFIER:
			add(node);
			add(node);
			node->add(parse_type());
			break;
		default:
			fatal_error(next_token_->get_location(), "parser PAR_DECL error: %m");
	}
	return node;
}

DerTree *
Parse::parse_body_eps()
{
	print_production("BODY_EPS");

	DerNode *node = new DerNode(DerNode::NONT_BODYEPS);
	switch (next_token_->get_type())
	{
		case Token::TOKEN_SEMIC:
			break;
		case Token::TOKEN_ASSIGN:
			add(node);
			node->add(parse_expr());
			break;
		default:
			fatal_error(next_token_->get_location(), "parser BODY_EPS error: %m");
	}
	return node;
}

DerTree *
Parse::parse_type()
{
	print_production("TYPE");

	DerNode *node = new DerNode(DerNode::NONT_TYPE);
	switch (next_token_->get_type())
	{
		case Token::TOKEN_IDENTIFIER:
			add(node);
			break;
		case Token::TOKEN_LPARENTHESIS:
			add(node);
			node->add(parse_type());
			add(node);
			break;
		case Token::TOKEN_VOID:
			add(node);
			break;
		case Token::TOKEN_BOOL:
			add(node);
			break;
		case Token::TOKEN_CHAR:
			add(node);
			break;
		case Token::TOKEN_INT:
			add(node);
			break;
		case Token::TOKEN_ARR:
			add(node);
			add(node);
			node->add(parse_expr());
			add(node);
			node->add(parse_type());
			break;
		case Token::TOKEN_REC:
			add(node);
			add(node);
			node->add(parse_comp_decls());
			add(node);
			break;
		case Token::TOKEN_PTR:
			add(node);
			node->add(parse_type());
			break;
		default:
			fatal_error(next_token_->get_location(), "parser TYPE error: %m");
	}
	return node;
}

DerTree *
Parse::parse_comp_decls()
{
	print_production("COMP_DECLS");

	DerNode *node = new DerNode(DerNode::NONT_COMPDECLS);
	switch (next_token_->get_type())
	{
		case Token::TOKEN_IDENTIFIER:
			node->add(parse_comp_decl());
			node->add(parse_comp_decls_rest());
			break;
		default:
			fatal_error(next_token_->get_location(), "parser COMP_DECLS error: %m");
	}
	return node;
}

DerTree *
Parse::parse_comp_decls_rest()
{
	print_production("COMP_DECLS_REST");

	DerNode *node = new DerNode(DerNode::NONT_COMPDECLSREST);
	switch (next_token_->get_type())
	{
		case Token::TOKEN_RPARENTHESIS:
			break;
		case Token::TOKEN_COMMA:
			add(node);
			node->add(parse_comp_decl());
			node->add(parse_comp_decls_rest());
			break;
		default:
			fatal_error(next_token_->get_location(), "parser COMP_DECLS_REST error: %m");
	}
	return node;
}

DerTree *
Parse::parse_comp_decl()
{
	print_production("COMP_DECL");

	DerNode *node = new DerNode(DerNode::NONT_COMPDECL);
	switch (next_token_->get_type())
	{
		case Token::TOKEN_IDENTIFIER:
			add(node);
			add(node);
			node->add(parse_type());
			break;
		default:
			fatal_error(next_token_->get_location(), "parser COMP_DECL error: %m");
	}
}

DerTree *
Parse::parse_expr()
{
	print_production("EXPR");

	DerNode *node = new DerNode(DerNode::NONT_EXPR);
	switch (next_token_->get_type())
	{
		case Token::TOKEN_IDENTIFIER:
		case Token::TOKEN_LPARENTHESIS:
		case Token::TOKEN_ADD:
		case Token::TOKEN_SUB:
		case Token::TOKEN_NOT:
		case Token::TOKEN_ADDR:
		case Token::TOKEN_DATA:
		case Token::TOKEN_VOIDCONST:
		case Token::TOKEN_BOOLCONST:
		case Token::TOKEN_CHARCONST:
		case Token::TOKEN_INTCONST:
		case Token::TOKEN_STRCONST:
		case Token::TOKEN_PTRCONST:
		case Token::TOKEN_LBRACE:
		case Token::TOKEN_NEW:
		case Token::TOKEN_DEL:
			node->add(parse_disj_expr());
			break;
		default:
			fatal_error(next_token_->get_location(), "parser EXPR error: %m");
	}
	return node;
}

DerTree *
Parse::parse_disj_expr()
{
	print_production("DISJ_EXPR");

	DerNode *node = new DerNode(DerNode::NONT_DISJEXPR);
	switch (next_token_->get_type())
	{
		case Token::TOKEN_IDENTIFIER:
		case Token::TOKEN_LPARENTHESIS:
		case Token::TOKEN_ADD:
		case Token::TOKEN_SUB:
		case Token::TOKEN_NOT:
		case Token::TOKEN_ADDR:
		case Token::TOKEN_DATA:
		case Token::TOKEN_VOIDCONST:
		case Token::TOKEN_BOOLCONST:
		case Token::TOKEN_CHARCONST:
		case Token::TOKEN_INTCONST:
		case Token::TOKEN_STRCONST:
		case Token::TOKEN_PTRCONST:
		case Token::TOKEN_LBRACE:
		case Token::TOKEN_NEW:
		case Token::TOKEN_DEL:
			node->add(parse_conj_expr());
			node->add(parse_disj_expr_rest());
			break;
		default:
			fatal_error(next_token_->get_location(), "parser DISJ_EXPR error: %m");
	}
	return node;
}

DerTree *
Parse::parse_disj_expr_rest()
{
	print_production("DISJ_EXPR_REST");

	DerNode *node = new DerNode(DerNode::NONT_DISJEXPRREST);
	switch (next_token_->get_type())
	{
		case Token::TOKEN_COLON:
		case Token::TOKEN_SEMIC:
		case Token::TOKEN_RPARENTHESIS:
		case Token::TOKEN_COMMA:
		case Token::TOKEN_ASSIGN:
		case Token::TOKEN_RBRACKET:
		case Token::TOKEN_RBRACE:
		case Token::TOKEN_WHERE:
		case Token::TOKEN_THEN:
		case Token::TOKEN_DO:
			break;
		case Token::TOKEN_IOR:
			add(node);
			node->add(parse_conj_expr());
			node->add(parse_conj_expr_rest());
			break;
		case Token::TOKEN_XOR:
			add(node);
			node->add(parse_conj_expr());
			node->add(parse_disj_expr_rest());
			break;
		default:
			fatal_error(next_token_->get_location(), "parser DISJ_EXPR_REST error: %m");
	}
	return node;
}

DerTree *
Parse::parse_conj_expr()
{
	print_production("CONJ_EXPR");

	DerNode *node = new DerNode(DerNode::NONT_CONJEXPR);
	switch (next_token_->get_type())
	{
		case Token::TOKEN_IDENTIFIER:
		case Token::TOKEN_LPARENTHESIS:
		case Token::TOKEN_ADD:
		case Token::TOKEN_SUB:
		case Token::TOKEN_NOT:
		case Token::TOKEN_ADDR:
		case Token::TOKEN_DATA:
		case Token::TOKEN_VOIDCONST:
		case Token::TOKEN_BOOLCONST:
		case Token::TOKEN_CHARCONST:
		case Token::TOKEN_INTCONST:
		case Token::TOKEN_STRCONST:
		case Token::TOKEN_PTRCONST:
		case Token::TOKEN_LBRACE:
		case Token::TOKEN_NEW:
		case Token::TOKEN_DEL:
			node->add(parse_rel_expr());
			node->add(parse_conj_expr_rest());
			break;
		default:
			fatal_error(next_token_->get_location(), "parser CONJ_EXPR error: %m");
	}
	return node;
}

DerTree *
Parse::parse_conj_expr_rest()
{
	print_production("CONJ_EXPR_REST");

	DerNode *node = new DerNode(DerNode::NONT_CONJEXPRREST);
	switch (next_token_->get_type())
	{
		case Token::TOKEN_COLON:
		case Token::TOKEN_SEMIC:
		case Token::TOKEN_RPARENTHESIS:
		case Token::TOKEN_COMMA:
		case Token::TOKEN_ASSIGN:
		case Token::TOKEN_RBRACKET:
		case Token::TOKEN_IOR:
		case Token::TOKEN_XOR:
		case Token::TOKEN_RBRACE:
		case Token::TOKEN_WHERE:
		case Token::TOKEN_THEN:
		case Token::TOKEN_DO:
			break;
		case Token::TOKEN_AND:
			add(node);
			node->add(parse_rel_expr());
			node->add(parse_conj_expr_rest());
			break;
		default:
			fatal_error(next_token_->get_location(), "parser CONJ_EXPR_REST error: %m");
	}
	return node;
}

DerTree *
Parse::parse_rel_expr()
{
	print_production("REL_EXPR");

	DerNode *node = new DerNode(DerNode::NONT_RELEXPR);
	switch (next_token_->get_type())
	{
		case Token::TOKEN_IDENTIFIER:
		case Token::TOKEN_LPARENTHESIS:
		case Token::TOKEN_ADD:
		case Token::TOKEN_SUB:
		case Token::TOKEN_NOT:
		case Token::TOKEN_ADDR:
		case Token::TOKEN_DATA:
		case Token::TOKEN_VOIDCONST:
		case Token::TOKEN_BOOLCONST:
		case Token::TOKEN_CHARCONST:
		case Token::TOKEN_INTCONST:
		case Token::TOKEN_STRCONST:
		case Token::TOKEN_PTRCONST:
		case Token::TOKEN_LBRACE:
		case Token::TOKEN_NEW:
		case Token::TOKEN_DEL:
			node->add(parse_add_expr());
			node->add(parse_rel_expr_rest());
			break;
		default:
			fatal_error(next_token_->get_location(), "parser REL_EXPR error: %m");
	}
	return node;
}

DerTree *
Parse::parse_rel_expr_rest()
{
	print_production("REL_EXPR_REST");

	DerNode *node = new DerNode(DerNode::NONT_RELEXPRREST);
	switch (next_token_->get_type())
	{
		case Token::TOKEN_COLON:
		case Token::TOKEN_SEMIC:
		case Token::TOKEN_RPARENTHESIS:
		case Token::TOKEN_COMMA:
		case Token::TOKEN_ASSIGN:
		case Token::TOKEN_RBRACKET:
		case Token::TOKEN_IOR:
		case Token::TOKEN_XOR:
		case Token::TOKEN_AND:
		case Token::TOKEN_RBRACE:
		case Token::TOKEN_WHERE:
		case Token::TOKEN_THEN:
		case Token::TOKEN_DO:
			break;
		case Token::TOKEN_EQU:
			add(node);
			node->add(parse_add_expr());
			break;
		case Token::TOKEN_NEQ:
			add(node);
			node->add(parse_add_expr());
			break;
		case Token::TOKEN_LEQ:
			add(node);
			node->add(parse_add_expr());
			break;
		case Token::TOKEN_GEQ:
			add(node);
			node->add(parse_add_expr());
			break;
		case Token::TOKEN_LTH:
			add(node);
			node->add(parse_add_expr());
			break;
		case Token::TOKEN_GTH:
			add(node);
			node->add(parse_add_expr());
			break;
		default:
			fatal_error(next_token_->get_location(), "parser REL_EXPR_REST error: %m");
	}
	return node;
}

DerTree *
Parse::parse_add_expr()
{
	print_production("ADD_EXPR");

	DerNode *node = new DerNode(DerNode::NONT_ADDEXPR);
	switch (next_token_->get_type())
	{
		case Token::TOKEN_IDENTIFIER:
		case Token::TOKEN_LPARENTHESIS:
		case Token::TOKEN_ADD:
		case Token::TOKEN_SUB:
		case Token::TOKEN_NOT:
		case Token::TOKEN_ADDR:
		case Token::TOKEN_DATA:
		case Token::TOKEN_VOIDCONST:
		case Token::TOKEN_BOOLCONST:
		case Token::TOKEN_CHARCONST:
		case Token::TOKEN_INTCONST:
		case Token::TOKEN_STRCONST:
		case Token::TOKEN_PTRCONST:
		case Token::TOKEN_LBRACE:
		case Token::TOKEN_NEW:
		case Token::TOKEN_DEL:
			node->add(parse_mul_expr());
			node->add(parse_add_expr_rest());
			break;
		default:
			fatal_error(next_token_->get_location(), "parser ADD_EXPR error: %m");
	}
	return node;
}

DerTree *
Parse::parse_add_expr_rest()
{
	print_production("ADD_EXPR_REST");

	DerNode *node = new DerNode(DerNode::NONT_ADDEXPRREST);
	switch (next_token_->get_type())
	{
		case Token::TOKEN_COLON:
		case Token::TOKEN_SEMIC:
		case Token::TOKEN_RPARENTHESIS:
		case Token::TOKEN_COMMA:
		case Token::TOKEN_ASSIGN:
		case Token::TOKEN_RBRACKET:
		case Token::TOKEN_IOR:
		case Token::TOKEN_XOR:
		case Token::TOKEN_AND:
		case Token::TOKEN_EQU:
		case Token::TOKEN_NEQ:
		case Token::TOKEN_LEQ:
		case Token::TOKEN_GEQ:
		case Token::TOKEN_LTH:
		case Token::TOKEN_GTH:
		case Token::TOKEN_RBRACE:
		case Token::TOKEN_WHERE:
		case Token::TOKEN_THEN:
		case Token::TOKEN_DO:
			break;
		case Token::TOKEN_ADD:
			add(node);
			node->add(parse_mul_expr());
			node->add(parse_add_expr_rest());
			break;
		case Token::TOKEN_SUB:
			add(node);
			node->add(parse_mul_expr());
			node->add(parse_add_expr_rest());
			break;
		default:
			fatal_error(next_token_->get_location(), "parser ADD_EXPR_REST error: %m");
	}
	return node;
}

DerTree *
Parse::parse_mul_expr()
{
	print_production("MUL_EXPR");

	DerNode *node = new DerNode(DerNode::NONT_MULEXPR);
	switch (next_token_->get_type())
	{
		case Token::TOKEN_IDENTIFIER:
		case Token::TOKEN_LPARENTHESIS:
		case Token::TOKEN_ADD:
		case Token::TOKEN_SUB:
		case Token::TOKEN_NOT:
		case Token::TOKEN_ADDR:
		case Token::TOKEN_DATA:
		case Token::TOKEN_VOIDCONST:
		case Token::TOKEN_BOOLCONST:
		case Token::TOKEN_CHARCONST:
		case Token::TOKEN_INTCONST:
		case Token::TOKEN_STRCONST:
		case Token::TOKEN_PTRCONST:
		case Token::TOKEN_LBRACE:
		case Token::TOKEN_NEW:
		case Token::TOKEN_DEL:
			node->add(parse_pref_expr());
			node->add(parse_atom_expr());
			node->add(parse_pstf_expr_rest());
			node->add(parse_mul_expr_rest());
			break;
		default:
			fatal_error(next_token_->get_location(), "parser MUL_EXPR error: %m");
	}
	return node;
}

DerTree *
Parse::parse_mul_expr_rest()
{
	print_production("MUL_EXPR_REST");

	DerNode *node = new DerNode(DerNode::NONT_MULEXPRREST);
	switch (next_token_->get_type())
	{
		case Token::TOKEN_COLON:
		case Token::TOKEN_SEMIC:
		case Token::TOKEN_RPARENTHESIS:
		case Token::TOKEN_COMMA:
		case Token::TOKEN_ASSIGN:
		case Token::TOKEN_RBRACKET:
		case Token::TOKEN_IOR:
		case Token::TOKEN_XOR:
		case Token::TOKEN_AND:
		case Token::TOKEN_EQU:
		case Token::TOKEN_NEQ:
		case Token::TOKEN_LEQ:
		case Token::TOKEN_GEQ:
		case Token::TOKEN_LTH:
		case Token::TOKEN_GTH:
		case Token::TOKEN_ADD:
		case Token::TOKEN_SUB:
		case Token::TOKEN_RBRACE:
		case Token::TOKEN_WHERE:
		case Token::TOKEN_THEN:
		case Token::TOKEN_DO:
			break;
		case Token::TOKEN_MUL:
			add(node);
			node->add(parse_pref_expr());
			node->add(parse_atom_expr());
			node->add(parse_pstf_expr_rest());
			node->add(parse_mul_expr_rest());
			break;
		case Token::TOKEN_DIV:
			add(node);
			node->add(parse_pref_expr());
			node->add(parse_atom_expr());
			node->add(parse_pstf_expr_rest());
			node->add(parse_mul_expr_rest());
			break;
		case Token::TOKEN_MOD:
			add(node);
			node->add(parse_pref_expr());
			node->add(parse_atom_expr());
			node->add(parse_pstf_expr_rest());
			node->add(parse_mul_expr_rest());
			break;
		default:
			fatal_error(next_token_->get_location(), "parser MUL_EXPR_REST error: %m");
	}
	return node;
}

DerTree *
Parse::parse_pref_expr()
{
	print_production("PREF_EXPR");

	DerNode *node = new DerNode(DerNode::NONT_PREFEXPR);
	switch (next_token_->get_type())
	{
		case Token::TOKEN_IDENTIFIER:
		case Token::TOKEN_LPARENTHESIS:
		case Token::TOKEN_VOIDCONST:
		case Token::TOKEN_BOOLCONST:
		case Token::TOKEN_CHARCONST:
		case Token::TOKEN_INTCONST:
		case Token::TOKEN_STRCONST:
		case Token::TOKEN_PTRCONST:
		case Token::TOKEN_LBRACE:
		case Token::TOKEN_NEW:
		case Token::TOKEN_DEL:
			break;
		case Token::TOKEN_ADD:
			add(node);
			node->add(parse_pref_expr());
			break;
		case Token::TOKEN_SUB:
			add(node);
			node->add(parse_pref_expr());
			break;
		case Token::TOKEN_NOT:
			add(node);
			node->add(parse_pref_expr());
			break;
		case Token::TOKEN_ADDR:
			add(node);
			node->add(parse_pref_expr());
			break;
		case Token::TOKEN_DATA:
			add(node);
			node->add(parse_pref_expr());
			break;
		default:
			fatal_error(next_token_->get_location(), "parser PREF_EXPR error: %m");
	}
	return node;
}

DerTree *
Parse::parse_pstf_expr()
{
	print_production("PSTF_EXPR");

	DerNode *node = new DerNode(DerNode::NONT_PSTFEXPR);
	switch (next_token_->get_type())
	{
		case Token::TOKEN_LBRACKET:
			add(node);
			node->add(parse_expr());
			add(node);
			break;
		case Token::TOKEN_DOT:
			add(node);
			add(node);
			break;
		default:
			fatal_error(next_token_->get_location(), "parser PSTF_EXPR error: %m");
	}
	return node;
}

DerTree *
Parse::parse_pstf_expr_rest()
{
	print_production("PSTF_EXPR_REST");

	DerNode *node = new DerNode(DerNode::NONT_PSTFEXPRREST);
	switch (next_token_->get_type())
	{
		case Token::TOKEN_COLON:
		case Token::TOKEN_SEMIC:
		case Token::TOKEN_RPARENTHESIS:
		case Token::TOKEN_COMMA:
		case Token::TOKEN_ASSIGN:
		case Token::TOKEN_RBRACKET:
		case Token::TOKEN_IOR:
		case Token::TOKEN_XOR:
		case Token::TOKEN_AND:
		case Token::TOKEN_EQU:
		case Token::TOKEN_NEQ:
		case Token::TOKEN_LEQ:
		case Token::TOKEN_GEQ:
		case Token::TOKEN_LTH:
		case Token::TOKEN_GTH:
		case Token::TOKEN_ADD:
		case Token::TOKEN_SUB:
		case Token::TOKEN_MUL:
		case Token::TOKEN_DIV:
		case Token::TOKEN_MOD:
		case Token::TOKEN_RBRACE:
		case Token::TOKEN_WHERE:
		case Token::TOKEN_THEN:
		case Token::TOKEN_DO:
			break;
		case Token::TOKEN_LBRACKET:
		case Token::TOKEN_DOT:
			node->add(parse_pstf_expr());
			node->add(parse_pstf_expr_rest());
			break;
		default:
			fatal_error(next_token_->get_location(), "parser PSTF_EXPR_REST error: %m");
	}
	return node;
}

DerTree *
Parse::parse_atom_expr()
{
	print_production("ATOM_EXPR");

	DerNode *node = new DerNode(DerNode::NONT_ATOMEXPR);
	switch (next_token_->get_type())
	{
		case Token::TOKEN_IDENTIFIER:
			add(node);
			node->add(parse_call_eps());
			break;
		case Token::TOKEN_LPARENTHESIS:
			add(node);
			node->add(parse_expr());
			node->add(parse_cast_eps());
			add(node);
			break;
		case Token::TOKEN_VOIDCONST:
			add(node);
			break;
		case Token::TOKEN_BOOLCONST:
			add(node);
			break;
		case Token::TOKEN_CHARCONST:
			add(node);
			break;
		case Token::TOKEN_INTCONST:
			add(node);
			break;
		case Token::TOKEN_STRCONST:
			add(node);
			break;
		case Token::TOKEN_PTRCONST:
			add(node);
			break;
		case Token::TOKEN_LBRACE:
			add(node);
			node->add(parse_stmts());
			add(node);
			node->add(parse_expr());
			node->add(parse_where_eps());
			add(node);
			break;
		case Token::TOKEN_NEW:
			add(node);
			add(node);
			node->add(parse_expr());
			add(node);
			break;
		case Token::TOKEN_DEL:
			add(node);
			add(node);
			node->add(parse_expr());
			add(node);
			break;
		default:
			fatal_error(next_token_->get_location(), "parser ATOM_EXPR error: %m");
	}
	return node;
}

DerTree *
Parse::parse_call_eps()
{
	print_production("CALL_EPS");

	DerNode *node = new DerNode(DerNode::NONT_CALLEPS);
	switch (next_token_->get_type())
	{
		case Token::TOKEN_COLON:
		case Token::TOKEN_SEMIC:
		case Token::TOKEN_RPARENTHESIS:
		case Token::TOKEN_COMMA:
		case Token::TOKEN_ASSIGN:
		case Token::TOKEN_LBRACKET:
		case Token::TOKEN_RBRACKET:
		case Token::TOKEN_IOR:
		case Token::TOKEN_XOR:
		case Token::TOKEN_AND:
		case Token::TOKEN_EQU:
		case Token::TOKEN_NEQ:
		case Token::TOKEN_LEQ:
		case Token::TOKEN_GEQ:
		case Token::TOKEN_LTH:
		case Token::TOKEN_GTH:
		case Token::TOKEN_ADD:
		case Token::TOKEN_SUB:
		case Token::TOKEN_MUL:
		case Token::TOKEN_DIV:
		case Token::TOKEN_MOD:
		case Token::TOKEN_DOT:
		case Token::TOKEN_RBRACE:
		case Token::TOKEN_WHERE:
		case Token::TOKEN_THEN:
		case Token::TOKEN_DO:
			break;
		case Token::TOKEN_LPARENTHESIS:
			add(node);
			node->add(parse_args_eps());
			add(node);
			break;
		default:
			fatal_error(next_token_->get_location(), "parser CALL_EPS error: %m");
	}
	return node;
}

DerTree *
Parse::parse_args_eps()
{
	print_production("ARGS_EPS");

	DerNode *node = new DerNode(DerNode::NONT_ARGSEPS);
	switch (next_token_->get_type())
	{
		case Token::TOKEN_IDENTIFIER:
		case Token::TOKEN_LPARENTHESIS:
		case Token::TOKEN_ADD:
		case Token::TOKEN_SUB:
		case Token::TOKEN_NOT:
		case Token::TOKEN_ADDR:
		case Token::TOKEN_DATA:
		case Token::TOKEN_VOIDCONST:
		case Token::TOKEN_BOOLCONST:
		case Token::TOKEN_CHARCONST:
		case Token::TOKEN_INTCONST:
		case Token::TOKEN_STRCONST:
		case Token::TOKEN_PTRCONST:
		case Token::TOKEN_LBRACE:
		case Token::TOKEN_NEW:
		case Token::TOKEN_DEL:
			node->add(parse_args());
			break;
		case Token::TOKEN_RPARENTHESIS:
			break;
		default:
			fatal_error(next_token_->get_location(), "parser ARGS_EPS error: %m");
	}
	return node;
}

DerTree *
Parse::parse_args()
{
	print_production("ARGS");

	DerNode *node = new DerNode(DerNode::NONT_ARGS);
	switch (next_token_->get_type())
	{
		case Token::TOKEN_IDENTIFIER:
		case Token::TOKEN_LPARENTHESIS:
		case Token::TOKEN_ADD:
		case Token::TOKEN_SUB:
		case Token::TOKEN_NOT:
		case Token::TOKEN_ADDR:
		case Token::TOKEN_DATA:
		case Token::TOKEN_VOIDCONST:
		case Token::TOKEN_BOOLCONST:
		case Token::TOKEN_CHARCONST:
		case Token::TOKEN_INTCONST:
		case Token::TOKEN_STRCONST:
		case Token::TOKEN_PTRCONST:
		case Token::TOKEN_LBRACE:
		case Token::TOKEN_NEW:
		case Token::TOKEN_DEL:
			node->add(parse_expr());
			node->add(parse_args_rest());
			break;
		default:
			fatal_error(next_token_->get_location(), "parser ARGS error: %m");
	}
	return node;
}

DerTree *
Parse::parse_args_rest()
{
	print_production("ARGS_REST");

	DerNode *node = new DerNode(DerNode::NONT_ARGSREST);
	switch (next_token_->get_type())
	{
		case Token::TOKEN_RPARENTHESIS:
			break;
		case Token::TOKEN_COMMA:
			add(node);
			node->add(parse_expr());
			node->add(parse_args_rest());
			break;
		default:
			fatal_error(next_token_->get_location(), "parser ARGS_REST error: %m");
	}
	return node;
}

DerTree *
Parse::parse_cast_eps()
{
	print_production("CAST_EPS");

	DerNode *node = new DerNode(DerNode::NONT_CASTEPS);
	switch (next_token_->get_type())
	{
		case Token::TOKEN_COLON:
			add(node);
			node->add(parse_type());
			break;
		case Token::TOKEN_RPARENTHESIS:
			break;
		default:
			fatal_error(next_token_->get_location(), "parser CAST_EPS error: %m");
	}
	return node;
}

DerTree *
Parse::parse_where_eps()
{
	print_production("WHERE_EPS");

	DerNode *node = new DerNode(DerNode::NONT_WHEREEPS);
	switch (next_token_->get_type())
	{
		case Token::TOKEN_RBRACE:
			break;
		case Token::TOKEN_WHERE:
			add(node);
			node->add(parse_decls());
			break;
		default:
			fatal_error(next_token_->get_location(), "parser WHERE_EPS error: %m");
	}
	return node;
}

DerTree *
Parse::parse_stmts()
{
	print_production("STMTS");

	DerNode *node = new DerNode(DerNode::NONT_STMTS);
	switch (next_token_->get_type())
	{
		case Token::TOKEN_IDENTIFIER:
		case Token::TOKEN_LPARENTHESIS:
		case Token::TOKEN_ADD:
		case Token::TOKEN_SUB:
		case Token::TOKEN_NOT:
		case Token::TOKEN_ADDR:
		case Token::TOKEN_DATA:
		case Token::TOKEN_VOIDCONST:
		case Token::TOKEN_BOOLCONST:
		case Token::TOKEN_CHARCONST:
		case Token::TOKEN_INTCONST:
		case Token::TOKEN_STRCONST:
		case Token::TOKEN_PTRCONST:
		case Token::TOKEN_LBRACE:
		case Token::TOKEN_NEW:
		case Token::TOKEN_DEL:
		case Token::TOKEN_IF:
		case Token::TOKEN_WHILE:
			node->add(parse_stmt());
			node->add(parse_stmts_rest());
			break;
		default:
			fatal_error(next_token_->get_location(), "parser STMTS error: %m");
	}
	return node;
}

DerTree *
Parse::parse_stmts_rest()
{
	print_production("STMTS_REST");

	DerNode *node = new DerNode(DerNode::NONT_STMTSREST);
	switch (next_token_->get_type())
	{
		case Token::TOKEN_IDENTIFIER:
		case Token::TOKEN_LPARENTHESIS:
		case Token::TOKEN_ADD:
		case Token::TOKEN_SUB:
		case Token::TOKEN_NOT:
		case Token::TOKEN_ADDR:
		case Token::TOKEN_DATA:
		case Token::TOKEN_VOIDCONST:
		case Token::TOKEN_BOOLCONST:
		case Token::TOKEN_CHARCONST:
		case Token::TOKEN_INTCONST:
		case Token::TOKEN_STRCONST:
		case Token::TOKEN_PTRCONST:
		case Token::TOKEN_LBRACE:
		case Token::TOKEN_NEW:
		case Token::TOKEN_DEL:
		case Token::TOKEN_IF:
		case Token::TOKEN_WHILE:
			node->add(parse_stmt());
			node->add(parse_stmts_rest());
			break;
		case Token::TOKEN_COLON:
		case Token::TOKEN_END:
		case Token::TOKEN_ELSE:
			break;
		default:
			fatal_error(next_token_->get_location(), "parser STMTS_REST error: %m");
	}
	return node;
}

DerTree *
Parse::parse_stmt()
{
	print_production("STMT");

	DerNode *node = new DerNode(DerNode::NONT_STMT);
	switch (next_token_->get_type())
	{
		case Token::TOKEN_IDENTIFIER:
		case Token::TOKEN_LPARENTHESIS:
		case Token::TOKEN_ADD:
		case Token::TOKEN_SUB:
		case Token::TOKEN_NOT:
		case Token::TOKEN_ADDR:
		case Token::TOKEN_DATA:
		case Token::TOKEN_VOIDCONST:
		case Token::TOKEN_BOOLCONST:
		case Token::TOKEN_CHARCONST:
		case Token::TOKEN_INTCONST:
		case Token::TOKEN_STRCONST:
		case Token::TOKEN_PTRCONST:
		case Token::TOKEN_LBRACE:
		case Token::TOKEN_NEW:
		case Token::TOKEN_DEL:
			node->add(parse_expr());
			node->add(parse_assign_eps());
			add(node);
			break;
		case Token::TOKEN_IF:
			add(node);
			node->add(parse_expr());
			add(node);
			node->add(parse_stmts());
			node->add(parse_else_eps());
			add(node);
			add(node);
			break;
		case Token::TOKEN_WHILE:
			add(node);
			node->add(parse_expr());
			add(node);
			node->add(parse_stmts());
			add(node);
			add(node);
			break;
		default:
			fatal_error(next_token_->get_location(), "parser STMT error: %m");
	}
	return node;
}

DerTree *
Parse::parse_assign_eps()
{
	print_production("ASSIGN_EPS");

	DerNode *node = new DerNode(DerNode::NONT_ASSIGNEPS);
	switch (next_token_->get_type())
	{
		case Token::TOKEN_SEMIC:
			break;
		case Token::TOKEN_ASSIGN:
			add(node);
			node->add(parse_expr());
			break;
		default:
			fatal_error(next_token_->get_location(), "parser ASSIGN_EPS error: %m");
	}
	return node;
}

DerTree *
Parse::parse_else_eps()
{
	print_production("ELSE_EPS");

	DerNode *node = new DerNode(DerNode::NONT_ELSEEPS);
	switch (next_token_->get_type())
	{
		case Token::TOKEN_END:
			break;
		case Token::TOKEN_ELSE:
			add(node);
			node->add(parse_stmts());
			break;
		default:
			fatal_error(next_token_->get_location(), "parser ELSE_EPS error: %m");
	}
	return node;
}

/*
 * Class DerTree
 */

/*
 * Class DerNode
 */
DerNode::DerNode(Nonterminal nont)
	: nont_(nont)
{

}

void
DerNode::add(DerTree *subtree)
{
	subtrees_.push_back(subtree);
}

/*
 * Class DerLeaf
 */
DerLeaf::DerLeaf(Token *token)
	: token_(token)
{

}
