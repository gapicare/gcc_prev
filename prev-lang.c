/* prev-lang.c -- PREV frontend gcc interface.
   Copyright (C) 2009-2020 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "target.h"
#include "tree.h"
#include "gimple-expr.h"
#include "diagnostic.h"
#include "opts.h"
#include "fold-const.h"
#include "gimplify.h"
#include "stor-layout.h"
#include "debug.h"
#include "convert.h"
#include "langhooks.h"
#include "langhooks-def.h"
#include "common/common-target.h"

/* Language-dependent contents of a type.  */

struct GTY(()) lang_type
{
  char dummy;
};

/* Language-dependent contents of a decl.  */

struct GTY(()) lang_decl
{
  char dummy;
};

/* Language-dependent contents of an identifier.  This must include a
   tree_identifier.  */

struct GTY(()) lang_identifier
{
  struct tree_identifier common;
};

/* The resulting tree type.  */

union GTY((desc ("TREE_CODE (&%h.generic) == IDENTIFIER_NODE"),
	   chain_next ("CODE_CONTAINS_STRUCT (TREE_CODE (&%h.generic), TS_COMMON) ? ((union lang_tree_node *) TREE_CHAIN (&%h.generic)) : NULL")))
lang_tree_node
{
  union tree_node GTY((tag ("0"),
		       desc ("tree_node_structure (&%h)"))) generic;
  struct lang_identifier GTY((tag ("1"))) identifier;
};

/* We don't use language_function.  */

struct GTY(()) language_function
{
  int dummy;
};

/* Language hooks.  */

static bool
prev_langhook_init (void)
{
  build_common_tree_nodes (false);

  /* I don't know why this has to be done explicitly.  */
  void_list_node = build_tree_list (NULL_TREE, void_type_node);

  build_common_builtin_nodes ();

  return true;
}

/* Run after parsing options.  */

static void
prev_langhook_parse_file (void)
{
  fprintf(stderr, "This is PREV front end!\n");
}

static tree
prev_langhook_type_for_size (unsigned int bits, int unsignedp)
{
  tree type;
  if (unsignedp)
    {
      if (bits == INT_TYPE_SIZE)
        type = unsigned_type_node;
      else if (bits == CHAR_TYPE_SIZE)
        type = unsigned_char_type_node;
      else if (bits == SHORT_TYPE_SIZE)
        type = short_unsigned_type_node;
      else if (bits == LONG_TYPE_SIZE)
        type = long_unsigned_type_node;
      else if (bits == LONG_LONG_TYPE_SIZE)
        type = long_long_unsigned_type_node;
      else
        type = make_unsigned_type(bits);
    }
  else
    {
      if (bits == INT_TYPE_SIZE)
        type = integer_type_node;
      else if (bits == CHAR_TYPE_SIZE)
        type = signed_char_type_node;
      else if (bits == SHORT_TYPE_SIZE)
        type = short_integer_type_node;
      else if (bits == LONG_TYPE_SIZE)
        type = long_integer_type_node;
      else if (bits == LONG_LONG_TYPE_SIZE)
        type = long_long_integer_type_node;
      else
        type = make_signed_type(bits);
    }
  return type;
}

static tree
prev_langhook_type_for_mode (machine_mode mode, int unsignedp)
{
  tree type;
  /* PREV has no vector types.  Build them here.  FIXME: It does not
     make sense for the middle-end to ask the frontend for a type
     which the frontend does not support.  However, at least for now
     it is required.  See PR 46805.  */
  if (GET_MODE_CLASS (mode) == MODE_VECTOR_BOOL
      && valid_vector_subparts_p (GET_MODE_NUNITS (mode)))
    {
      unsigned int elem_bits = vector_element_size (GET_MODE_BITSIZE (mode),
						    GET_MODE_NUNITS (mode));
      tree bool_type = build_nonstandard_boolean_type (elem_bits);
      return build_vector_type_for_mode (bool_type, mode);
    }
  else if (VECTOR_MODE_P (mode)
	   && valid_vector_subparts_p (GET_MODE_NUNITS (mode)))
    {
      tree inner;

      inner = prev_langhook_type_for_mode (GET_MODE_INNER (mode), unsignedp);
      if (inner != NULL_TREE)
	return build_vector_type_for_mode (inner, mode);
      return NULL_TREE;
    }

  scalar_int_mode imode;
  scalar_float_mode fmode;
  complex_mode cmode;
  if (is_int_mode (mode, &imode))
    return prev_langhook_type_for_size (GET_MODE_BITSIZE (imode), unsignedp);
  else if (is_float_mode (mode, &fmode))
    {
      switch (GET_MODE_BITSIZE (fmode))
	{
	case 32:
	  return float_type_node;
	case 64:
	  return double_type_node;
	default:
	  // We have to check for long double in order to support
	  // i386 excess precision.
	  if (fmode == TYPE_MODE(long_double_type_node))
	    return long_double_type_node;
	}
    }
  else if (is_complex_float_mode (mode, &cmode))
    {
      switch (GET_MODE_BITSIZE (cmode))
	{
	case 64:
	  return complex_float_type_node;
	case 128:
	  return complex_double_type_node;
	default:
	  // We have to check for long double in order to support
	  // i386 excess precision.
	  if (cmode == TYPE_MODE(complex_long_double_type_node))
	    return complex_long_double_type_node;
	}
    }

#if HOST_BITS_PER_WIDE_INT >= 64
  /* The middle-end and some backends rely on TImode being supported
     for 64-bit HWI.  */
  if (mode == TImode)
    {
      type = build_nonstandard_integer_type (GET_MODE_BITSIZE (TImode),
					     unsignedp);
      if (type && TYPE_MODE (type) == TImode)
	return type;
    }
#endif
  return NULL_TREE;
}

/* Record a builtin function.  We just ignore builtin functions.  */

static tree
prev_langhook_builtin_function (tree decl)
{
  return decl;
}

/* Return true if we are in the global binding level.  */

static bool
prev_langhook_global_bindings_p (void)
{
  gcc_unreachable ();
  return true;
}

/* Push a declaration into the current binding level.  We can't
   usefully implement this since we don't want to convert from tree
   back to one of our internal data structures.  I think the only way
   this is used is to record a decl which is to be returned by
   getdecls, and we could implement it for that purpose if
   necessary.  */

static tree
prev_langhook_pushdecl (tree decl ATTRIBUTE_UNUSED)
{
  gcc_unreachable ();
}

/* This hook is used to get the current list of declarations as trees.
   We don't support that; instead we use the write_globals hook.  This
   can't simply crash because it is called by -gstabs.  */

static tree
prev_langhook_getdecls (void)
{
  return NULL;
}

/* Functions called directly by the generic backend.  */

tree
convert (tree type, tree expr)
{
  if (type == error_mark_node
      || expr == error_mark_node
      || TREE_TYPE (expr) == error_mark_node)
    return error_mark_node;

  if (type == TREE_TYPE (expr))
    return expr;

  if (TYPE_MAIN_VARIANT (type) == TYPE_MAIN_VARIANT (TREE_TYPE (expr)))
    return fold_convert (type, expr);

  switch (TREE_CODE (type))
    {
    case VOID_TYPE:
    case BOOLEAN_TYPE:
      return fold_convert (type, expr);
    case INTEGER_TYPE:
      return fold (convert_to_integer (type, expr));
    case POINTER_TYPE:
      return fold (convert_to_pointer (type, expr));
    case REAL_TYPE:
      return fold (convert_to_real (type, expr));
    case COMPLEX_TYPE:
      return fold (convert_to_complex (type, expr));
    default:
      break;
    }

  gcc_unreachable ();
}

#undef LANG_HOOKS_NAME
#undef LANG_HOOKS_INIT
#undef LANG_HOOKS_PARSE_FILE
#undef LANG_HOOKS_TYPE_FOR_MODE
#undef LANG_HOOKS_TYPE_FOR_SIZE
#undef LANG_HOOKS_BUILTIN_FUNCTION
#undef LANG_HOOKS_GLOBAL_BINDINGS_P
#undef LANG_HOOKS_PUSHDECL
#undef LANG_HOOKS_GETDECLS

#define LANG_HOOKS_NAME			"PREV"
#define LANG_HOOKS_INIT			prev_langhook_init
#define LANG_HOOKS_PARSE_FILE		prev_langhook_parse_file
#define LANG_HOOKS_TYPE_FOR_MODE	prev_langhook_type_for_mode
#define LANG_HOOKS_TYPE_FOR_SIZE	prev_langhook_type_for_size
#define LANG_HOOKS_BUILTIN_FUNCTION	prev_langhook_builtin_function
#define LANG_HOOKS_GLOBAL_BINDINGS_P	prev_langhook_global_bindings_p
#define LANG_HOOKS_PUSHDECL		prev_langhook_pushdecl
#define LANG_HOOKS_GETDECLS		prev_langhook_getdecls

struct lang_hooks lang_hooks = LANG_HOOKS_INITIALIZER;

#include "gt-prev-prev-lang.h"
#include "gtype-prev.h"
