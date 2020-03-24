// prev.cc -- PREV frontend main file for gcc.

#include "prev-system.h"

#include "prev-c.h"

#include "lex.h"
#include "parse.h"

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

		Lex lexer(filename, file);

		Parse parse(&lexer);
		parse.parse();

		if (strcmp(filename, "-") != 0)
			fclose(file);
	}
}
