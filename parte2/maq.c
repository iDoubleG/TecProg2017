#include <stdio.h>
#include <stdlib.h>
#include "maq.h"

//#define DEBUG

#ifdef DEBUG
#  define D(X) X
char *CODES[] = {
  "PUSH",
  "POP",
  "DUP",
  "ADD",
  "SUB",
  "MUL",
  "DIV",
  "JMP",
  "JIT",
  "JIF",
  "CALL",
  "RET",
  "STL",/*Nova instrucao para manipulacao de variaveis locais*/
  "RCE",/*Nova instrucao para manipulacao de variaveis locais*/
  "EQ",
  "GT",
  "GE",
  "LT",
  "LE",
  "NE",
  "STO",
  "RCL",
  "END",
  "PRN",
  "ALC",/*Funcao auxiliar pala alocacao de frames na pilha de excecucao*/
  "FRE" /*Funcao auxiliar para liberacao de frames na pilha de excecucao*/
};
#else
#  define D(X)
#endif

static void Erro(char *msg) {
  fprintf(stderr, "%s\n", msg);
}

static void Fatal(char *msg, int cod) {
  Erro(msg);
  exit(cod);
}

Maquina *cria_maquina(INSTR *p) {
  Maquina *m = (Maquina*)malloc(sizeof(Maquina));
  if (!m) Fatal("Memória insuficiente",4);
  m->ip = 0;
  m->rbp = 0;
  m->prog = p;
  return m;
}

void destroi_maquina(Maquina *m) {
  free(m);
}

/* Alguns macros para facilitar a leitura do código */
#define ip (m->ip)
#define pil (&m->pil)
#define exec (&m->exec)/*Macro para referencia a pilha de excecucao*/
#define prg (m->prog)
#define rbp (m->rbp)/*Macro para referencia ao rbp*/

void exec_maquina(Maquina *m, int n) {//n vai virar o timestep
  int i;

  for (i = 0; i < n; i++) {
	OpCode   opc = prg[ip].instr;
	OPERANDO arg = prg[ip].op;

	D(printf("%3d: %-4.4s %d\n     ", ip, CODES[opc], arg));

	switch (opc) {
	  OPERANDO tmp;
	  OPERANDO op1;//this new
	  OPERANDO op2;//this new
	  OPERANDO res;//this new
	case PUSH:
	  empilha(pil, arg);
	  break;
	case POP:
	  desempilha(pil);
	  break;
	case DUP:
	  tmp = desempilha(pil);
	  empilha(pil, tmp);
	  empilha(pil, tmp);
	  break;
	case ADD://this new
	  op1 = desempilha(pil);
	  op2 = desempilha(pil);

	  if (op1.t == NUM && op2.t == NUM) {
		res.t = NUM;
		res.val.n = op1.val.n + op2.val.n;
		empilha(pil, res);
	  }
	  break;
	case SUB://this new
	  op1 = desempilha(pil);
	  op2 = desempilha(pil);

	  if (op1.t == NUM && op2.t == NUM) {
		res.t = NUM;
		res.val.n = op2.val.n - op1.val.n;
		empilha(pil, res);
	  }
	  break;
	case MUL://this new
	  op1 = desempilha(pil);
	  op2 = desempilha(pil);

	  if (op1.t == NUM && op2.t == NUM) {
		res.t = NUM;
		res.val.n = op1.val.n * op2.val.n;
		empilha(pil, res);
	  }
	  break;
	case DIV://this new
	  op1 = desempilha(pil);
	  op2 = desempilha(pil);

	  if (op1.t == NUM && op2.t == NUM) {
		res.t = NUM;
		res.val.n = op2.val.n / op1.val.n;
		empilha(pil, res);
	  }
	  break;
	case JMP:
	  ip = arg;
	  continue;
	case JIT:
	  if (desempilha(pil) != 0) {
		ip = arg;
		continue;
	  }
	  break;
	case JIF:
	  if (desempilha(pil) == 0) {
		ip = arg;
		continue;
	  }
	  break;
	case CALL:
	  empilha(exec, rbp);/*Empilha na pilha de excecucao o valor de rbp*/
	  empilha(exec, ip);
	  rbp = exec->topo -1;/*Armazena em rbp o valor do topo apos empilhar
							rbp e ip*/
	  ip = arg;
	  continue;
	case RET:
	  ip = desempilha(exec);
	  rbp = desempilha(exec);/*Retorna ao rbp a posicao do antigo topo da pilha
								de excecucao, apos desempilhar ip e rbp.*/
	  break;
	case STL:/*Instrucao armazena na pilha de excecucao a variavel local.*/
	  exec->val[arg+rbp] = desempilha(pil);
	  break;
	case RCE:/*Instrucao recupera a variavel local da pilha de excecucao.*/
	  empilha(pil, exec->val[rbp+arg]);
	  break;
	case EQ:
	  if (desempilha(pil) == desempilha(pil))
		empilha(pil, 1);
	  else
		empilha(pil, 0);
	  break;
	case GT:
	  if (desempilha(pil) < desempilha(pil))
		empilha(pil, 1);
	  else
		empilha(pil, 0);
	  break;
	case GE:
	  if (desempilha(pil) <= desempilha(pil))
		empilha(pil, 1);
	  else
		empilha(pil, 0);
	  break;
	case LT:
	  if (desempilha(pil) > desempilha(pil))
		empilha(pil, 1);
	  else
		empilha(pil, 0);
	  break;
	case LE:
	  if (desempilha(pil) >= desempilha(pil))
		empilha(pil, 1);
	  else
		empilha(pil, 0);
	  break;
	case NE:
	  if (desempilha(pil) != desempilha(pil))
		empilha(pil, 1);
	  else
		empilha(pil, 0);
	  break;
	case STO:
	  m->Mem[arg] = desempilha(pil);
	  break;
	case RCL:
	  empilha(pil,m->Mem[arg]);
	  break;
	case END:
	  return;
	case PRN:
	  printf("%d\n", desempilha(pil));
	  break;
	case ALC:/*Funcao auxiliar aloca frames na pilha de excecucao para
				armazenar a variavel local*/
	  exec->topo += arg;
	  break;
	case FRE:/*Funcao auxiliar libera frames da pilha de excecucao onde estavam
				armazenadas as variaveis locais*/
	  exec->topo -= arg;
	  break;
	case ATR:
	  tmp = desempilha(pil);
	  empilha(pil, tmp->arg);
	  break;
	case SIS:
	  empilha(arg);

	}
	D(imprime(pil,5));
	D(imprime(exec, 10));
	D(puts("\n"));
	ip++;
  }
}
