#define MAP_SIZE 1000

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
  SIS	//this new						/* chamada de sistema */
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

/* Alternativamente, Celula pode ser implementada como um vetor de ints
   ou short ints, isso torna a implementação de ATR mais fácil */

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
