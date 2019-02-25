/* -------------------------------------------------------------------
 *            Arquivo: compilaodr.h
 * -------------------------------------------------------------------
 *              Autor: Bruno Muller Junior
 *               Data: 08/2007
 *      Atualizado em: [15/03/2012, 08h:22m]
 *
 * -------------------------------------------------------------------
 *
 * Tipos, protótipos e vaiáveis globais do compilador
 *
 * ------------------------------------------------------------------- */

#define TAM_TOKEN 16

typedef enum simbolos { 
  // Palavras reservadas em ordem alfabetica
  simb_and, simb_array, simb_asm, simb_begin, simb_break,
  simb_case, simb_const, simb_continue, simb_div, simb_do,
  simb_downto, simb_else, simb_end, simb_false, simb_file, simb_for,
  simb_function, simb_goto, simb_if, simb_label, simb_mod, simb_nil,
  simb_not, simb_object, simb_of, simb_or, simb_packed, simb_procedure,
  simb_program, simb_record, simb_repeat, simb_set, simb_shl, simb_shr,
  simb_string, simb_then, simb_to, simb_true, simb_type, simb_unit,
  simb_until, simb_uses, simb_var, simb_while, simb_xor,

  // Simbolos especiais e expressoes regulares
  simb_identificador, simb_numero,
  simb_ponto, simb_virgula, simb_ponto_e_virgula, simb_dois_pontos,
  simb_atribuicao, simb_abre_parenteses, simb_fecha_parenteses,
  simb_soma,
} simbolos;



/* -------------------------------------------------------------------
 * variáveis globais
 * ------------------------------------------------------------------- */

extern simbolos simbolo, relacao;
extern char token[TAM_TOKEN];
extern int nivel_lexico;
extern int desloc;
extern int nl;


simbolos simbolo, relacao;
char token[TAM_TOKEN];



