// parse.h -- PREV frontend parser.

#ifndef PREV_PARSE_H
#define PREV_PARSE_H

class DerTree;
class DerNode;

class Parse
{
public:
	Parse(Lex *);

	void
	add(DerNode *);

	void
	parse();

private:
	/* debugging */
	void
	open_parse_file();

	void
	close_parse_file();

	void
	print_production(const char *);
	/* debugging */
// parse.h -- PREV frontend parser.

#ifndef PREV_PARSE_H
#define PREV_PARSE_H

class Parse
{
public:
	Parse(Lex *);

	void
	parse_source();

private:
	Lex *lex_;

	Token token_;
};

#endif // #ifndef PREV_PARSE_H

	DerTree *
	parse_source();

	DerTree *
	parse_decls();

	DerTree *
	parse_decls_rest();

	DerTree *
	parse_decl();

	DerTree *
	parse_par_decls_eps();

	DerTree *
	parse_par_decls();

	DerTree *
	parse_par_decls_rest();

	DerTree *
	parse_par_decl();

	DerTree *
	parse_body_eps();

	DerTree *
	parse_type();

	DerTree *
	parse_comp_decls();

	DerTree *
	parse_comp_decls_rest();

	DerTree *
	parse_comp_decl();

	DerTree *
	parse_expr();

	DerTree *
	parse_disj_expr();

	DerTree *
	parse_disj_expr_rest();

	DerTree *
	parse_conj_expr();

	DerTree *
	parse_conj_expr_rest();

	DerTree *
	parse_rel_expr();

	DerTree *
	parse_rel_expr_rest();

	DerTree *
	parse_add_expr();

	DerTree *
	parse_add_expr_rest();

	DerTree *
	parse_mul_expr();

	DerTree *
	parse_mul_expr_rest();

	DerTree *
	parse_pref_expr();

	DerTree *
	parse_pstf_expr();

	DerTree *
	parse_pstf_expr_rest();

	DerTree *
	parse_atom_expr();

	DerTree *
	parse_call_eps();

	DerTree *
	parse_args_eps();

	DerTree *
	parse_args();

	DerTree *
	parse_args_rest();

	DerTree *
	parse_cast_eps();

	DerTree *
	parse_where_eps();

	DerTree *
	parse_stmts();

	DerTree *
	parse_stmts_rest();

	DerTree *
	parse_stmt();

	DerTree *
	parse_assign_eps();

	DerTree *
	parse_else_eps();

	Lex *lex_;
	Token *next_token_;
	DerTree *der_tree_;

	FILE *parse_file_;
};

class DerTree
{

};

class DerNode : public DerTree
{
public:
	enum Nonterminal
	{
		NONT_ADDEXPR,
		NONT_ADDEXPRREST,
		NONT_ARGS,
		NONT_ARGSEPS,
		NONT_ARGSREST,
		NONT_ASSIGNEPS,
		NONT_ATOMEXPR,
		NONT_BODYEPS,
		NONT_CALLEPS,
		NONT_CASTEPS,
		NONT_COMPDECL,
		NONT_COMPDECLS,
		NONT_COMPDECLSREST,
		NONT_CONJEXPR,
		NONT_CONJEXPRREST,
		NONT_DECL,
		NONT_DECLS,
		NONT_DECLSREST,
		NONT_DISJEXPR,
		NONT_DISJEXPRREST,
		NONT_ELSEEPS,
		NONT_EXPR,
		NONT_MULEXPR,
		NONT_MULEXPRREST,
		NONT_PARDECL,
		NONT_PARDECLS,
		NONT_PARDECLSEPS,
		NONT_PARDECLSREST,
		NONT_PREFEXPR,
		NONT_PSTFEXPR,
		NONT_PSTFEXPRREST,
		NONT_RELEXPR,
		NONT_RELEXPRREST,
		NONT_SOURCE,
		NONT_STMT,
		NONT_STMTS,
		NONT_STMTSREST,
		NONT_TYPE,
		NONT_WHEREEPS,
	};

	DerNode(Nonterminal);

	void
	add(DerTree *);

private:
	Nonterminal nont_;
	std::vector<DerTree *> subtrees_;
};

class DerLeaf : public DerTree
{
public:
	DerLeaf(Token *);

private:
	Token *token_;
};

#endif // #ifndef PREV_PARSE_H
