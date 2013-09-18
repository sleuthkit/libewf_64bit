/* A Bison parser, made by GNU Bison 2.6.1.  */

/* Bison interface for Yacc-like parsers in C

      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef CUE_SCANNER_LIBODRAW_CUE_PARSER_H
# define CUE_SCANNER_LIBODRAW_CUE_PARSER_H
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int cue_scanner_debug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     CUE_END_OF_LINE = 258,
     CUE_SEMI_COLON = 259,
     CUE_2DIGIT = 260,
     CUE_CATALOG_NUMBER = 261,
     CUE_ISRC_CODE = 262,
     CUE_KEYWORD_STRING = 263,
     CUE_MSF = 264,
     CUE_STRING = 265,
     CUE_CATALOG = 266,
     CUE_CDTEXTFILE = 267,
     CUE_FLAGS = 268,
     CUE_FILE = 269,
     CUE_INDEX = 270,
     CUE_ISRC = 271,
     CUE_POSTGAP = 272,
     CUE_PREGAP = 273,
     CUE_REMARK = 274,
     CUE_TRACK = 275,
     CUE_CDTEXT_ARRANGER = 276,
     CUE_CDTEXT_COMPOSER = 277,
     CUE_CDTEXT_DISC_ID = 278,
     CUE_CDTEXT_GENRE = 279,
     CUE_CDTEXT_MESSAGE = 280,
     CUE_CDTEXT_PERFORMER = 281,
     CUE_CDTEXT_SIZE_INFO = 282,
     CUE_CDTEXT_SONGWRITER = 283,
     CUE_CDTEXT_TITLE = 284,
     CUE_CDTEXT_TOC_INFO1 = 285,
     CUE_CDTEXT_TOC_INFO2 = 286,
     CUE_CDTEXT_UPC_EAN = 287,
     CUE_REMARK_LEAD_OUT = 288,
     CUE_REMARK_ORIGINAL_MEDIA_TYPE = 289,
     CUE_REMARK_RUN_OUT = 290,
     CUE_REMARK_SESSION = 291
   };
#endif
/* Tokens.  */
#define CUE_END_OF_LINE 258
#define CUE_SEMI_COLON 259
#define CUE_2DIGIT 260
#define CUE_CATALOG_NUMBER 261
#define CUE_ISRC_CODE 262
#define CUE_KEYWORD_STRING 263
#define CUE_MSF 264
#define CUE_STRING 265
#define CUE_CATALOG 266
#define CUE_CDTEXTFILE 267
#define CUE_FLAGS 268
#define CUE_FILE 269
#define CUE_INDEX 270
#define CUE_ISRC 271
#define CUE_POSTGAP 272
#define CUE_PREGAP 273
#define CUE_REMARK 274
#define CUE_TRACK 275
#define CUE_CDTEXT_ARRANGER 276
#define CUE_CDTEXT_COMPOSER 277
#define CUE_CDTEXT_DISC_ID 278
#define CUE_CDTEXT_GENRE 279
#define CUE_CDTEXT_MESSAGE 280
#define CUE_CDTEXT_PERFORMER 281
#define CUE_CDTEXT_SIZE_INFO 282
#define CUE_CDTEXT_SONGWRITER 283
#define CUE_CDTEXT_TITLE 284
#define CUE_CDTEXT_TOC_INFO1 285
#define CUE_CDTEXT_TOC_INFO2 286
#define CUE_CDTEXT_UPC_EAN 287
#define CUE_REMARK_LEAD_OUT 288
#define CUE_REMARK_ORIGINAL_MEDIA_TYPE 289
#define CUE_REMARK_RUN_OUT 290
#define CUE_REMARK_SESSION 291



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{


        /* The numeric value
         */
        uint32_t numeric_value;

        /* The string value
         */
	struct cue_string_value
	{
		/* The string data
		 */
	        const char *data;

		/* The string length
		 */
		size_t length;

	} string_value;



} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE cue_scanner_lval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int cue_scanner_parse (void *YYPARSE_PARAM);
#else
int cue_scanner_parse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int cue_scanner_parse (void);
#else
int cue_scanner_parse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !CUE_SCANNER_LIBODRAW_CUE_PARSER_H  */
