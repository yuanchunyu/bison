// A Bison parser, made by GNU Bison 3.0.4.

// Skeleton interface for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

/**
 ** \file py_parser.hh
 ** Define the py::parser class.
 */

// C++ LALR(1) parser skeleton written by Akim Demaille.

#ifndef YY_YY_PY_PARSER_HH_INCLUDED
# define YY_YY_PY_PARSER_HH_INCLUDED
// //                    "%code requires" blocks.
#line 10 "py_parser.yy" // lalr1.cc:392

# include <string>

    namespace py {
    class ParserContext;
    class ASTNode;
    }

#line 53 "py_parser.hh" // lalr1.cc:392

# include <cassert>
# include <cstdlib> // std::abort
# include <iostream>
# include <stdexcept>
# include <string>
# include <vector>
# include "stack.hh"
# include "location.hh"
#include <typeinfo>
#ifndef YYASSERT
# include <cassert>
# define YYASSERT assert
#endif


#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

#line 4 "py_parser.yy" // lalr1.cc:392
namespace py {
#line 130 "py_parser.hh" // lalr1.cc:392



  /// A char[S] buffer to store and retrieve objects.
  ///
  /// Sort of a variant, but does not keep track of the nature
  /// of the stored data, since that knowledge is available
  /// via the current state.
  template <size_t S>
  struct variant
  {
    /// Type of *this.
    typedef variant<S> self_type;

    /// Empty construction.
    variant ()
      : yytypeid_ (YY_NULLPTR)
    {}

    /// Construct and fill.
    template <typename T>
    variant (const T& t)
      : yytypeid_ (&typeid (T))
    {
      YYASSERT (sizeof (T) <= S);
      new (yyas_<T> ()) T (t);
    }

    /// Destruction, allowed only if empty.
    ~variant ()
    {
      YYASSERT (!yytypeid_);
    }

    /// Instantiate an empty \a T in here.
    template <typename T>
    T&
    build ()
    {
      YYASSERT (!yytypeid_);
      YYASSERT (sizeof (T) <= S);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T;
    }

    /// Instantiate a \a T in here from \a t.
    template <typename T>
    T&
    build (const T& t)
    {
      YYASSERT (!yytypeid_);
      YYASSERT (sizeof (T) <= S);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T (t);
    }

    /// Accessor to a built \a T.
    template <typename T>
    T&
    as ()
    {
      YYASSERT (*yytypeid_ == typeid (T));
      YYASSERT (sizeof (T) <= S);
      return *yyas_<T> ();
    }

    /// Const accessor to a built \a T (for %printer).
    template <typename T>
    const T&
    as () const
    {
      YYASSERT (*yytypeid_ == typeid (T));
      YYASSERT (sizeof (T) <= S);
      return *yyas_<T> ();
    }

    /// Swap the content with \a other, of same type.
    ///
    /// Both variants must be built beforehand, because swapping the actual
    /// data requires reading it (with as()), and this is not possible on
    /// unconstructed variants: it would require some dynamic testing, which
    /// should not be the variant's responsability.
    /// Swapping between built and (possibly) non-built is done with
    /// variant::move ().
    template <typename T>
    void
    swap (self_type& other)
    {
      YYASSERT (yytypeid_);
      YYASSERT (*yytypeid_ == *other.yytypeid_);
      std::swap (as<T> (), other.as<T> ());
    }

    /// Move the content of \a other to this.
    ///
    /// Destroys \a other.
    template <typename T>
    void
    move (self_type& other)
    {
      build<T> ();
      swap<T> (other);
      other.destroy<T> ();
    }

    /// Copy the content of \a other to this.
    template <typename T>
    void
    copy (const self_type& other)
    {
      build<T> (other.as<T> ());
    }

    /// Destroy the stored \a T.
    template <typename T>
    void
    destroy ()
    {
      as<T> ().~T ();
      yytypeid_ = YY_NULLPTR;
    }

  private:
    /// Prohibit blind copies.
    self_type& operator=(const self_type&);
    variant (const self_type&);

    /// Accessor to raw memory as \a T.
    template <typename T>
    T*
    yyas_ ()
    {
      void *yyp = yybuffer_.yyraw;
      return static_cast<T*> (yyp);
     }

    /// Const accessor to raw memory as \a T.
    template <typename T>
    const T*
    yyas_ () const
    {
      const void *yyp = yybuffer_.yyraw;
      return static_cast<const T*> (yyp);
     }

    union
    {
      /// Strongest alignment constraints.
      long double yyalign_me;
      /// A buffer large enough to store any of the semantic values.
      char yyraw[S];
    } yybuffer_;

    /// Whether the content is built: if defined, the name of the stored type.
    const std::type_info *yytypeid_;
  };


  /// A Bison parser.
  class py_parser
  {
  public:
#ifndef YYSTYPE
    /// An auxiliary type to compute the largest semantic type.
    union union_type
    {
      // atom
      char dummy1[sizeof(ASTNode*)];

      // "number"
      char dummy2[sizeof(int)];

      // "name"
      // "string"
      char dummy3[sizeof(std::string)];
};

    /// Symbol semantic values.
    typedef variant<sizeof(union_type)> semantic_type;
#else
    typedef YYSTYPE semantic_type;
#endif
    /// Symbol locations.
    typedef location location_type;

    /// Syntax errors thrown from user actions.
    struct syntax_error : std::runtime_error
    {
      syntax_error (const location_type& l, const std::string& m);
      location_type location;
    };

    /// Tokens.
    struct token
    {
      enum yytokentype
      {
        TOK_END = 0,
        TOK_ASSIGN = 258,
        TOK_DEF = 259,
        TOK_PASS = 260,
        TOK_IF = 261,
        TOK_ELSE = 262,
        TOK_OR = 263,
        TOK_NOT = 264,
        TOK_AND = 265,
        TOK_IN = 266,
        TOK_IS = 267,
        TOK_ENDMARKER = 268,
        TOK_NEWLINE = 269,
        TOK_INDENT = 270,
        TOK_DEDENT = 271,
        TOK_LPAR = 272,
        TOK_RPAR = 273,
        TOK_LSQB = 274,
        TOK_RSQB = 275,
        TOK_COLON = 276,
        TOK_COMMA = 277,
        TOK_SEMI = 278,
        TOK_PLUS = 279,
        TOK_MINUS = 280,
        TOK_STAR = 281,
        TOK_SLASH = 282,
        TOK_VBAR = 283,
        TOK_AMPER = 284,
        TOK_LESS = 285,
        TOK_GREATER = 286,
        TOK_EQUAL = 287,
        TOK_DOT = 288,
        TOK_PERCENT = 289,
        TOK_BACKQUOTE = 290,
        TOK_LBRACE = 291,
        TOK_RBRACE = 292,
        TOK_EQEQUAL = 293,
        TOK_NOTEQUAL = 294,
        TOK_LESSEQUAL = 295,
        TOK_GREATEREQUAL = 296,
        TOK_TILDE = 297,
        TOK_CIRCUMFLEX = 298,
        TOK_LEFTSHIFT = 299,
        TOK_RIGHTSHIFT = 300,
        TOK_DOUBLESTAR = 301,
        TOK_PLUSEQUAL = 302,
        TOK_MINEQUAL = 303,
        TOK_STAREQUAL = 304,
        TOK_SLASHEQUAL = 305,
        TOK_PERCENTEQUAL = 306,
        TOK_AMPEREQUAL = 307,
        TOK_VBAREQUAL = 308,
        TOK_CIRCUMFLEXEQUAL = 309,
        TOK_LEFTSHIFTEQUAL = 310,
        TOK_RIGHTSHIFTEQUAL = 311,
        TOK_DOUBLESTAREQUAL = 312,
        TOK_DOUBLESLASH = 313,
        TOK_DOUBLESLASHEQUAL = 314,
        TOK_AT = 315,
        TOK_AWAIT = 316,
        TOK_ASYNC = 317,
        TOK_NAME = 318,
        TOK_NUMBER = 319,
        TOK_STRING = 320
      };
    };

    /// (External) token type, as returned by yylex.
    typedef token::yytokentype token_type;

    /// Symbol type: an internal symbol number.
    typedef int symbol_number_type;

    /// The symbol type number to denote an empty symbol.
    enum { empty_symbol = -2 };

    /// Internal symbol number for tokens (subsumed by symbol_number_type).
    typedef unsigned char token_number_type;

    /// A complete symbol.
    ///
    /// Expects its Base type to provide access to the symbol type
    /// via type_get().
    ///
    /// Provide access to semantic value and location.
    template <typename Base>
    struct basic_symbol : Base
    {
      /// Alias to Base.
      typedef Base super_type;

      /// Default constructor.
      basic_symbol ();

      /// Copy constructor.
      basic_symbol (const basic_symbol& other);

      /// Constructor for valueless symbols, and symbols from each type.

  basic_symbol (typename Base::kind_type t, const location_type& l);

  basic_symbol (typename Base::kind_type t, const ASTNode* v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const int v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const std::string v, const location_type& l);


      /// Constructor for symbols with semantic value.
      basic_symbol (typename Base::kind_type t,
                    const semantic_type& v,
                    const location_type& l);

      /// Destroy the symbol.
      ~basic_symbol ();

      /// Destroy contents, and record that is empty.
      void clear ();

      /// Whether empty.
      bool empty () const;

      /// Destructive move, \a s is emptied into this.
      void move (basic_symbol& s);

      /// The semantic value.
      semantic_type value;

      /// The location.
      location_type location;

    private:
      /// Assignment operator.
      basic_symbol& operator= (const basic_symbol& other);
    };

    /// Type access provider for token (enum) based symbols.
    struct by_type
    {
      /// Default constructor.
      by_type ();

      /// Copy constructor.
      by_type (const by_type& other);

      /// The symbol type as needed by the constructor.
      typedef token_type kind_type;

      /// Constructor from (external) token numbers.
      by_type (kind_type t);

      /// Record that this symbol is empty.
      void clear ();

      /// Steal the symbol type from \a that.
      void move (by_type& that);

      /// The (internal) type number (corresponding to \a type).
      /// \a empty when empty.
      symbol_number_type type_get () const;

      /// The token.
      token_type token () const;

      /// The symbol type.
      /// \a empty_symbol when empty.
      /// An int, not token_number_type, to be able to store empty_symbol.
      int type;
    };

    /// "External" symbols: returned by the scanner.
    typedef basic_symbol<by_type> symbol_type;

    // Symbol constructors declarations.
    static inline
    symbol_type
    make_END (const location_type& l);

    static inline
    symbol_type
    make_ASSIGN (const location_type& l);

    static inline
    symbol_type
    make_DEF (const location_type& l);

    static inline
    symbol_type
    make_PASS (const location_type& l);

    static inline
    symbol_type
    make_IF (const location_type& l);

    static inline
    symbol_type
    make_ELSE (const location_type& l);

    static inline
    symbol_type
    make_OR (const location_type& l);

    static inline
    symbol_type
    make_NOT (const location_type& l);

    static inline
    symbol_type
    make_AND (const location_type& l);

    static inline
    symbol_type
    make_IN (const location_type& l);

    static inline
    symbol_type
    make_IS (const location_type& l);

    static inline
    symbol_type
    make_ENDMARKER (const location_type& l);

    static inline
    symbol_type
    make_NEWLINE (const location_type& l);

    static inline
    symbol_type
    make_INDENT (const location_type& l);

    static inline
    symbol_type
    make_DEDENT (const location_type& l);

    static inline
    symbol_type
    make_LPAR (const location_type& l);

    static inline
    symbol_type
    make_RPAR (const location_type& l);

    static inline
    symbol_type
    make_LSQB (const location_type& l);

    static inline
    symbol_type
    make_RSQB (const location_type& l);

    static inline
    symbol_type
    make_COLON (const location_type& l);

    static inline
    symbol_type
    make_COMMA (const location_type& l);

    static inline
    symbol_type
    make_SEMI (const location_type& l);

    static inline
    symbol_type
    make_PLUS (const location_type& l);

    static inline
    symbol_type
    make_MINUS (const location_type& l);

    static inline
    symbol_type
    make_STAR (const location_type& l);

    static inline
    symbol_type
    make_SLASH (const location_type& l);

    static inline
    symbol_type
    make_VBAR (const location_type& l);

    static inline
    symbol_type
    make_AMPER (const location_type& l);

    static inline
    symbol_type
    make_LESS (const location_type& l);

    static inline
    symbol_type
    make_GREATER (const location_type& l);

    static inline
    symbol_type
    make_EQUAL (const location_type& l);

    static inline
    symbol_type
    make_DOT (const location_type& l);

    static inline
    symbol_type
    make_PERCENT (const location_type& l);

    static inline
    symbol_type
    make_BACKQUOTE (const location_type& l);

    static inline
    symbol_type
    make_LBRACE (const location_type& l);

    static inline
    symbol_type
    make_RBRACE (const location_type& l);

    static inline
    symbol_type
    make_EQEQUAL (const location_type& l);

    static inline
    symbol_type
    make_NOTEQUAL (const location_type& l);

    static inline
    symbol_type
    make_LESSEQUAL (const location_type& l);

    static inline
    symbol_type
    make_GREATEREQUAL (const location_type& l);

    static inline
    symbol_type
    make_TILDE (const location_type& l);

    static inline
    symbol_type
    make_CIRCUMFLEX (const location_type& l);

    static inline
    symbol_type
    make_LEFTSHIFT (const location_type& l);

    static inline
    symbol_type
    make_RIGHTSHIFT (const location_type& l);

    static inline
    symbol_type
    make_DOUBLESTAR (const location_type& l);

    static inline
    symbol_type
    make_PLUSEQUAL (const location_type& l);

    static inline
    symbol_type
    make_MINEQUAL (const location_type& l);

    static inline
    symbol_type
    make_STAREQUAL (const location_type& l);

    static inline
    symbol_type
    make_SLASHEQUAL (const location_type& l);

    static inline
    symbol_type
    make_PERCENTEQUAL (const location_type& l);

    static inline
    symbol_type
    make_AMPEREQUAL (const location_type& l);

    static inline
    symbol_type
    make_VBAREQUAL (const location_type& l);

    static inline
    symbol_type
    make_CIRCUMFLEXEQUAL (const location_type& l);

    static inline
    symbol_type
    make_LEFTSHIFTEQUAL (const location_type& l);

    static inline
    symbol_type
    make_RIGHTSHIFTEQUAL (const location_type& l);

    static inline
    symbol_type
    make_DOUBLESTAREQUAL (const location_type& l);

    static inline
    symbol_type
    make_DOUBLESLASH (const location_type& l);

    static inline
    symbol_type
    make_DOUBLESLASHEQUAL (const location_type& l);

    static inline
    symbol_type
    make_AT (const location_type& l);

    static inline
    symbol_type
    make_AWAIT (const location_type& l);

    static inline
    symbol_type
    make_ASYNC (const location_type& l);

    static inline
    symbol_type
    make_NAME (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_NUMBER (const int& v, const location_type& l);

    static inline
    symbol_type
    make_STRING (const std::string& v, const location_type& l);


    /// Build a parser object.
    py_parser (ParserContext& context_yyarg);
    virtual ~py_parser ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

    /// Report a syntax error.
    /// \param loc    where the syntax error is found.
    /// \param msg    a description of the syntax error.
    virtual void error (const location_type& loc, const std::string& msg);

    /// Report a syntax error.
    void error (const syntax_error& err);

  private:
    /// This class is not copyable.
    py_parser (const py_parser&);
    py_parser& operator= (const py_parser&);

    /// State numbers.
    typedef int state_type;

    /// Generate an error message.
    /// \param yystate   the state where the error occurred.
    /// \param yyla      the lookahead token.
    virtual std::string yysyntax_error_ (state_type yystate,
                                         const symbol_type& yyla) const;

    /// Compute post-reduction state.
    /// \param yystate   the current state
    /// \param yysym     the nonterminal to push on the stack
    state_type yy_lr_goto_state_ (state_type yystate, int yysym);

    /// Whether the given \c yypact_ value indicates a defaulted state.
    /// \param yyvalue   the value to check
    static bool yy_pact_value_is_default_ (int yyvalue);

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue);

    static const signed char yypact_ninf_;
    static const signed char yytable_ninf_;

    /// Convert a scanner token number \a t to a symbol number.
    static token_number_type yytranslate_ (token_type t);

    // Tables.
  // YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
  // STATE-NUM.
  static const short int yypact_[];

  // YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
  // Performed when YYTABLE does not specify something else to do.  Zero
  // means the default is an error.
  static const unsigned char yydefact_[];

  // YYPGOTO[NTERM-NUM].
  static const signed char yypgoto_[];

  // YYDEFGOTO[NTERM-NUM].
  static const short int yydefgoto_[];

  // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
  // positive, shift that token.  If negative, reduce the rule whose
  // number is the opposite.  If YYTABLE_NINF, syntax error.
  static const unsigned char yytable_[];

  static const short int yycheck_[];

  // YYSTOS[STATE-NUM] -- The (internal number of the) accessing
  // symbol of state STATE-NUM.
  static const unsigned char yystos_[];

  // YYR1[YYN] -- Symbol number of symbol that rule YYN derives.
  static const unsigned char yyr1_[];

  // YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.
  static const unsigned char yyr2_[];


    /// Convert the symbol name \a n to a form suitable for a diagnostic.
    static std::string yytnamerr_ (const char *n);


    /// For a symbol, its name in clear.
    static const char* const yytname_[];
#if YYDEBUG
  // YYRLINE[YYN] -- Source line where rule number YYN was defined.
  static const unsigned short int yyrline_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r);
    /// Print the state stack on the debug stream.
    virtual void yystack_print_ ();

    // Debugging.
    int yydebug_;
    std::ostream* yycdebug_;

    /// \brief Display a symbol type, value and location.
    /// \param yyo    The output stream.
    /// \param yysym  The symbol.
    template <typename Base>
    void yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const;
#endif

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg     Why this token is reclaimed.
    ///                  If null, print nothing.
    /// \param yysym     The symbol.
    template <typename Base>
    void yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const;

  private:
    /// Type access provider for state based symbols.
    struct by_state
    {
      /// Default constructor.
      by_state ();

      /// The symbol type as needed by the constructor.
      typedef state_type kind_type;

      /// Constructor.
      by_state (kind_type s);

      /// Copy constructor.
      by_state (const by_state& other);

      /// Record that this symbol is empty.
      void clear ();

      /// Steal the symbol type from \a that.
      void move (by_state& that);

      /// The (internal) type number (corresponding to \a state).
      /// \a empty_symbol when empty.
      symbol_number_type type_get () const;

      /// The state number used to denote an empty symbol.
      enum { empty_state = -1 };

      /// The state.
      /// \a empty when empty.
      state_type state;
    };

    /// "Internal" symbol: element of the stack.
    struct stack_symbol_type : basic_symbol<by_state>
    {
      /// Superclass.
      typedef basic_symbol<by_state> super_type;
      /// Construct an empty symbol.
      stack_symbol_type ();
      /// Steal the contents from \a sym to build this.
      stack_symbol_type (state_type s, symbol_type& sym);
      /// Assignment, needed by push_back.
      stack_symbol_type& operator= (const stack_symbol_type& that);
    };

    /// Stack type.
    typedef stack<stack_symbol_type> stack_type;

    /// The stack.
    stack_type yystack_;

    /// Push a new state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the symbol
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, stack_symbol_type& s);

    /// Push a new look ahead token on the state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the state
    /// \param sym  the symbol (for its value and location).
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, state_type s, symbol_type& sym);

    /// Pop \a n symbols the three stacks.
    void yypop_ (unsigned int n = 1);

    /// Constants.
    enum
    {
      yyeof_ = 0,
      yylast_ = 256,     ///< Last index in yytable_.
      yynnts_ = 48,  ///< Number of nonterminal symbols.
      yyfinal_ = 48, ///< Termination state number.
      yyterror_ = 1,
      yyerrcode_ = 256,
      yyntokens_ = 70  ///< Number of tokens.
    };


    // User arguments.
    ParserContext& context;
  };

  // Symbol number corresponding to token number t.
  inline
  py_parser::token_number_type
  py_parser::yytranslate_ (token_type t)
  {
    static
    const token_number_type
    translate_table[] =
    {
     0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69
    };
    const unsigned int user_token_number_max_ = 324;
    const token_number_type undef_token_ = 2;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }

  inline
  py_parser::syntax_error::syntax_error (const location_type& l, const std::string& m)
    : std::runtime_error (m)
    , location (l)
  {}

  // basic_symbol.
  template <typename Base>
  inline
  py_parser::basic_symbol<Base>::basic_symbol ()
    : value ()
  {}

  template <typename Base>
  inline
  py_parser::basic_symbol<Base>::basic_symbol (const basic_symbol& other)
    : Base (other)
    , value ()
    , location (other.location)
  {
      switch (other.type_get ())
    {
      case 115: // atom
        value.copy< ASTNode* > (other.value);
        break;

      case 64: // "number"
        value.copy< int > (other.value);
        break;

      case 63: // "name"
      case 65: // "string"
        value.copy< std::string > (other.value);
        break;

      default:
        break;
    }

  }


  template <typename Base>
  inline
  py_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const semantic_type& v, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {
    (void) v;
      switch (this->type_get ())
    {
      case 115: // atom
        value.copy< ASTNode* > (v);
        break;

      case 64: // "number"
        value.copy< int > (v);
        break;

      case 63: // "name"
      case 65: // "string"
        value.copy< std::string > (v);
        break;

      default:
        break;
    }
}


  // Implementation of basic_symbol constructor for each type.

  template <typename Base>
  py_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {}

  template <typename Base>
  py_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const ASTNode* v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  py_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const int v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  py_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::string v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}


  template <typename Base>
  inline
  py_parser::basic_symbol<Base>::~basic_symbol ()
  {
    clear ();
  }

  template <typename Base>
  inline
  void
  py_parser::basic_symbol<Base>::clear ()
  {
    // User destructor.
    symbol_number_type yytype = this->type_get ();
    basic_symbol<Base>& yysym = *this;
    (void) yysym;
    switch (yytype)
    {
   default:
      break;
    }

    // Type destructor.
    switch (yytype)
    {
      case 115: // atom
        value.template destroy< ASTNode* > ();
        break;

      case 64: // "number"
        value.template destroy< int > ();
        break;

      case 63: // "name"
      case 65: // "string"
        value.template destroy< std::string > ();
        break;

      default:
        break;
    }

    Base::clear ();
  }

  template <typename Base>
  inline
  bool
  py_parser::basic_symbol<Base>::empty () const
  {
    return Base::type_get () == empty_symbol;
  }

  template <typename Base>
  inline
  void
  py_parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move(s);
      switch (this->type_get ())
    {
      case 115: // atom
        value.move< ASTNode* > (s.value);
        break;

      case 64: // "number"
        value.move< int > (s.value);
        break;

      case 63: // "name"
      case 65: // "string"
        value.move< std::string > (s.value);
        break;

      default:
        break;
    }

    location = s.location;
  }

  // by_type.
  inline
  py_parser::by_type::by_type ()
    : type (empty_symbol)
  {}

  inline
  py_parser::by_type::by_type (const by_type& other)
    : type (other.type)
  {}

  inline
  py_parser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  inline
  void
  py_parser::by_type::clear ()
  {
    type = empty_symbol;
  }

  inline
  void
  py_parser::by_type::move (by_type& that)
  {
    type = that.type;
    that.clear ();
  }

  inline
  int
  py_parser::by_type::type_get () const
  {
    return type;
  }

  inline
  py_parser::token_type
  py_parser::by_type::token () const
  {
    // YYTOKNUM[NUM] -- (External) token number corresponding to the
    // (internal) symbol number NUM (which must be that of a token).  */
    static
    const unsigned short int
    yytoken_number_[] =
    {
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324
    };
    return static_cast<token_type> (yytoken_number_[type]);
  }
  // Implementation of make_symbol for each symbol type.
  py_parser::symbol_type
  py_parser::make_END (const location_type& l)
  {
    return symbol_type (token::TOK_END, l);
  }

  py_parser::symbol_type
  py_parser::make_ASSIGN (const location_type& l)
  {
    return symbol_type (token::TOK_ASSIGN, l);
  }

  py_parser::symbol_type
  py_parser::make_DEF (const location_type& l)
  {
    return symbol_type (token::TOK_DEF, l);
  }

  py_parser::symbol_type
  py_parser::make_PASS (const location_type& l)
  {
    return symbol_type (token::TOK_PASS, l);
  }

  py_parser::symbol_type
  py_parser::make_IF (const location_type& l)
  {
    return symbol_type (token::TOK_IF, l);
  }

  py_parser::symbol_type
  py_parser::make_ELSE (const location_type& l)
  {
    return symbol_type (token::TOK_ELSE, l);
  }

  py_parser::symbol_type
  py_parser::make_OR (const location_type& l)
  {
    return symbol_type (token::TOK_OR, l);
  }

  py_parser::symbol_type
  py_parser::make_NOT (const location_type& l)
  {
    return symbol_type (token::TOK_NOT, l);
  }

  py_parser::symbol_type
  py_parser::make_AND (const location_type& l)
  {
    return symbol_type (token::TOK_AND, l);
  }

  py_parser::symbol_type
  py_parser::make_IN (const location_type& l)
  {
    return symbol_type (token::TOK_IN, l);
  }

  py_parser::symbol_type
  py_parser::make_IS (const location_type& l)
  {
    return symbol_type (token::TOK_IS, l);
  }

  py_parser::symbol_type
  py_parser::make_ENDMARKER (const location_type& l)
  {
    return symbol_type (token::TOK_ENDMARKER, l);
  }

  py_parser::symbol_type
  py_parser::make_NEWLINE (const location_type& l)
  {
    return symbol_type (token::TOK_NEWLINE, l);
  }

  py_parser::symbol_type
  py_parser::make_INDENT (const location_type& l)
  {
    return symbol_type (token::TOK_INDENT, l);
  }

  py_parser::symbol_type
  py_parser::make_DEDENT (const location_type& l)
  {
    return symbol_type (token::TOK_DEDENT, l);
  }

  py_parser::symbol_type
  py_parser::make_LPAR (const location_type& l)
  {
    return symbol_type (token::TOK_LPAR, l);
  }

  py_parser::symbol_type
  py_parser::make_RPAR (const location_type& l)
  {
    return symbol_type (token::TOK_RPAR, l);
  }

  py_parser::symbol_type
  py_parser::make_LSQB (const location_type& l)
  {
    return symbol_type (token::TOK_LSQB, l);
  }

  py_parser::symbol_type
  py_parser::make_RSQB (const location_type& l)
  {
    return symbol_type (token::TOK_RSQB, l);
  }

  py_parser::symbol_type
  py_parser::make_COLON (const location_type& l)
  {
    return symbol_type (token::TOK_COLON, l);
  }

  py_parser::symbol_type
  py_parser::make_COMMA (const location_type& l)
  {
    return symbol_type (token::TOK_COMMA, l);
  }

  py_parser::symbol_type
  py_parser::make_SEMI (const location_type& l)
  {
    return symbol_type (token::TOK_SEMI, l);
  }

  py_parser::symbol_type
  py_parser::make_PLUS (const location_type& l)
  {
    return symbol_type (token::TOK_PLUS, l);
  }

  py_parser::symbol_type
  py_parser::make_MINUS (const location_type& l)
  {
    return symbol_type (token::TOK_MINUS, l);
  }

  py_parser::symbol_type
  py_parser::make_STAR (const location_type& l)
  {
    return symbol_type (token::TOK_STAR, l);
  }

  py_parser::symbol_type
  py_parser::make_SLASH (const location_type& l)
  {
    return symbol_type (token::TOK_SLASH, l);
  }

  py_parser::symbol_type
  py_parser::make_VBAR (const location_type& l)
  {
    return symbol_type (token::TOK_VBAR, l);
  }

  py_parser::symbol_type
  py_parser::make_AMPER (const location_type& l)
  {
    return symbol_type (token::TOK_AMPER, l);
  }

  py_parser::symbol_type
  py_parser::make_LESS (const location_type& l)
  {
    return symbol_type (token::TOK_LESS, l);
  }

  py_parser::symbol_type
  py_parser::make_GREATER (const location_type& l)
  {
    return symbol_type (token::TOK_GREATER, l);
  }

  py_parser::symbol_type
  py_parser::make_EQUAL (const location_type& l)
  {
    return symbol_type (token::TOK_EQUAL, l);
  }

  py_parser::symbol_type
  py_parser::make_DOT (const location_type& l)
  {
    return symbol_type (token::TOK_DOT, l);
  }

  py_parser::symbol_type
  py_parser::make_PERCENT (const location_type& l)
  {
    return symbol_type (token::TOK_PERCENT, l);
  }

  py_parser::symbol_type
  py_parser::make_BACKQUOTE (const location_type& l)
  {
    return symbol_type (token::TOK_BACKQUOTE, l);
  }

  py_parser::symbol_type
  py_parser::make_LBRACE (const location_type& l)
  {
    return symbol_type (token::TOK_LBRACE, l);
  }

  py_parser::symbol_type
  py_parser::make_RBRACE (const location_type& l)
  {
    return symbol_type (token::TOK_RBRACE, l);
  }

  py_parser::symbol_type
  py_parser::make_EQEQUAL (const location_type& l)
  {
    return symbol_type (token::TOK_EQEQUAL, l);
  }

  py_parser::symbol_type
  py_parser::make_NOTEQUAL (const location_type& l)
  {
    return symbol_type (token::TOK_NOTEQUAL, l);
  }

  py_parser::symbol_type
  py_parser::make_LESSEQUAL (const location_type& l)
  {
    return symbol_type (token::TOK_LESSEQUAL, l);
  }

  py_parser::symbol_type
  py_parser::make_GREATEREQUAL (const location_type& l)
  {
    return symbol_type (token::TOK_GREATEREQUAL, l);
  }

  py_parser::symbol_type
  py_parser::make_TILDE (const location_type& l)
  {
    return symbol_type (token::TOK_TILDE, l);
  }

  py_parser::symbol_type
  py_parser::make_CIRCUMFLEX (const location_type& l)
  {
    return symbol_type (token::TOK_CIRCUMFLEX, l);
  }

  py_parser::symbol_type
  py_parser::make_LEFTSHIFT (const location_type& l)
  {
    return symbol_type (token::TOK_LEFTSHIFT, l);
  }

  py_parser::symbol_type
  py_parser::make_RIGHTSHIFT (const location_type& l)
  {
    return symbol_type (token::TOK_RIGHTSHIFT, l);
  }

  py_parser::symbol_type
  py_parser::make_DOUBLESTAR (const location_type& l)
  {
    return symbol_type (token::TOK_DOUBLESTAR, l);
  }

  py_parser::symbol_type
  py_parser::make_PLUSEQUAL (const location_type& l)
  {
    return symbol_type (token::TOK_PLUSEQUAL, l);
  }

  py_parser::symbol_type
  py_parser::make_MINEQUAL (const location_type& l)
  {
    return symbol_type (token::TOK_MINEQUAL, l);
  }

  py_parser::symbol_type
  py_parser::make_STAREQUAL (const location_type& l)
  {
    return symbol_type (token::TOK_STAREQUAL, l);
  }

  py_parser::symbol_type
  py_parser::make_SLASHEQUAL (const location_type& l)
  {
    return symbol_type (token::TOK_SLASHEQUAL, l);
  }

  py_parser::symbol_type
  py_parser::make_PERCENTEQUAL (const location_type& l)
  {
    return symbol_type (token::TOK_PERCENTEQUAL, l);
  }

  py_parser::symbol_type
  py_parser::make_AMPEREQUAL (const location_type& l)
  {
    return symbol_type (token::TOK_AMPEREQUAL, l);
  }

  py_parser::symbol_type
  py_parser::make_VBAREQUAL (const location_type& l)
  {
    return symbol_type (token::TOK_VBAREQUAL, l);
  }

  py_parser::symbol_type
  py_parser::make_CIRCUMFLEXEQUAL (const location_type& l)
  {
    return symbol_type (token::TOK_CIRCUMFLEXEQUAL, l);
  }

  py_parser::symbol_type
  py_parser::make_LEFTSHIFTEQUAL (const location_type& l)
  {
    return symbol_type (token::TOK_LEFTSHIFTEQUAL, l);
  }

  py_parser::symbol_type
  py_parser::make_RIGHTSHIFTEQUAL (const location_type& l)
  {
    return symbol_type (token::TOK_RIGHTSHIFTEQUAL, l);
  }

  py_parser::symbol_type
  py_parser::make_DOUBLESTAREQUAL (const location_type& l)
  {
    return symbol_type (token::TOK_DOUBLESTAREQUAL, l);
  }

  py_parser::symbol_type
  py_parser::make_DOUBLESLASH (const location_type& l)
  {
    return symbol_type (token::TOK_DOUBLESLASH, l);
  }

  py_parser::symbol_type
  py_parser::make_DOUBLESLASHEQUAL (const location_type& l)
  {
    return symbol_type (token::TOK_DOUBLESLASHEQUAL, l);
  }

  py_parser::symbol_type
  py_parser::make_AT (const location_type& l)
  {
    return symbol_type (token::TOK_AT, l);
  }

  py_parser::symbol_type
  py_parser::make_AWAIT (const location_type& l)
  {
    return symbol_type (token::TOK_AWAIT, l);
  }

  py_parser::symbol_type
  py_parser::make_ASYNC (const location_type& l)
  {
    return symbol_type (token::TOK_ASYNC, l);
  }

  py_parser::symbol_type
  py_parser::make_NAME (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_NAME, v, l);
  }

  py_parser::symbol_type
  py_parser::make_NUMBER (const int& v, const location_type& l)
  {
    return symbol_type (token::TOK_NUMBER, v, l);
  }

  py_parser::symbol_type
  py_parser::make_STRING (const std::string& v, const location_type& l)
  {
    return symbol_type (token::TOK_STRING, v, l);
  }


#line 4 "py_parser.yy" // lalr1.cc:392
} // py
#line 1659 "py_parser.hh" // lalr1.cc:392




#endif // !YY_YY_PY_PARSER_HH_INCLUDED