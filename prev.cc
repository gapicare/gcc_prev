// prev.cc -- PREV frontend main file for gcc.

#include "prev-system.h"

#include "prev-c.h"

#include "lex.h"

void
prev_parse_input_files(const char** filenames, unsigned int filename_count)
{
	for (unsigned int i = 0; i < filename_count; ++i)
	{
		const char* filename = filenames[i];
		FILE* file;
		if (strcmp(filename, "-") == 0)
			file = stdin;
		else
		{
			file = fopen(filename, "r");
			if (file == NULL)
				fatal_error(UNKNOWN_LOCATION, "cannot open %s: %m", filename);
		}

		Lex lex(filename, file);

		FILE *lex_out = fopen("lex.out", "w");
		if (lex_out == NULL)
			fatal_error(UNKNOWN_LOCATION, "cannot open lex.out: %m");

		Token *t = lex.next_token();
		while (t->get_type() != Token::TOKEN_EOF)
		{
			t->print(lex_out);
			t = lex.next_token();
		}
		t->print(lex_out);

		fclose(lex_out);

		if (strcmp(filename, "-") != 0)
			fclose(file);
	}
}
