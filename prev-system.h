// prev-system.h -- PREV frontend inclusion of gcc header files

// This file is part of GCC.

// GCC is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 3, or (at your option) any later
// version.

// GCC is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.

// You should have received a copy of the GNU General Public License
// along with GCC; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

#ifndef PREV_SYSTEM_H
#define PREV_SYSTEM_H

#include "config.h"

/* Define this so that inttypes.h defines the PRI?64 macros even
   when compiling with a C++ compiler.  Define it here so in the
   event inttypes.h gets pulled in by another header it is already
   defined.  */
#define __STDC_FORMAT_MACROS

#include <string>

// We don't really need iostream, but some versions of gmp.h include
// it when compiled with C++, which means that we need to include it
// before the macro magic of safe-ctype.h, which is included by
// system.h.
#include <iostream>

#include "system.h"
#include "ansidecl.h"
#include "coretypes.h"

#include "diagnostic-core.h"	/* For error_at and friends.  */
#include "intl.h"		/* For _().  */

// When using gcc, prev_assert is just gcc_assert.
#define prev_assert(EXPR) gcc_assert(EXPR)

// When using gcc, prev_unreachable is just gcc_unreachable.
#define prev_unreachable() gcc_unreachable()

#endif // #ifndef PREV_SYSTEM_H
