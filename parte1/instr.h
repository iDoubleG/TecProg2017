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
  STL,//this new
  RCE,//this new
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
  ALC,//this new5
  FRE,//this new5
} OpCode;

/* Tipos dos operandos */
/* no momento, são todos inteiros */
typedef enum {
  NUM,
  ACAO,
  VAR
} Tipo;

/* Operando */
/* typedef struct { */
/*   Tipo t; */
/*   union { */
/* 	int n; */
/* 	int ac; */
/* 	int v; */
/*   }; */
/* } OPERANDO; */
typedef int OPERANDO;

/* Instrução */
typedef struct {
  OpCode instr;
  OPERANDO op;
} INSTR;
