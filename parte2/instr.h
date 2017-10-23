#ifndef INSTR_H
#define INSTR_H

/* Códigos das instruções */
typedef enum {
  PUSH,
  POP,
  DUP,
  ADD,
  SUB,
  MUL,
  DIV,
  JMP,
  JIT,
  JIF,
  CALL,
  RET,
  STL,/*Declaracao da variavel STL, ver comentario em maq.c*/
  RCE,/*Declaracao da variavel RCE, ver comentario em maq.c*/
  EQ,
  GT,
  GE,
  LT,
  LE,
  NE,
  STO,
  RCL,
  END,
  PRN,
  ALC,/*Declaracao da variavel FRE, ver comentario em maq.c*/
  FRE,/*Declaracao da variavel FRE, ver comentario em maq.c*/
  ATR,//this new
  SISM,	//this new						/* chamada de sistema */
  SISA,  //this new 
  SISR,  //this new
  SISD,  //this new
} OpCode;

/* Tipos dos operandos */
typedef enum {
  NUM,
  ACAO,
  VAR,
  CELULA
} Tipo;

/* Tipo de terreno */
typedef enum {//this new
  RUELA,
  ESTRADA,
  AREIA,
  AGUA,
  ARMADILHA,
  BASE,
  NADA
} Terreno;

/* Célula */
typedef struct {
  Terreno terreno;
  short int cristais;
  short int ocupado;
  int pos[2];
  int base;//numero vai ser o time; 0 = nao tem time
} Celula;

/* Alternativamente, Celula pode ser implementada como um vetor de ints
   ou short ints, isso torna a implementação de ATR mais fácil */

/* Célula */
typedef struct {
  Terreno terreno;
  int cristais;
  int ocupado;
  int pos[2];
  int base;//numero vai ser o time; 0 = nao tem time
} Celula;

/* No caso da ação, o tipo do operando é um inteiro indicando a direção,
   mas fique livre para fazer de forma diferente */

/* Operando */
typedef struct {//this new
  Tipo t;
  union {
	int n;
	int ac;
	int v;
	Celula cel;
  } val;
} OPERANDO;

/* Instrução */
typedef struct {
  OpCode instr;
  OPERANDO op;
} INSTR;
#endif