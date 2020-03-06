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
