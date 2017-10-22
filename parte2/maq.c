	#include <stdio.h>
#include <stdlib.h>
#include "maq.h"
#include "arena.h"

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
  "FRE",/*Funcao auxiliar para liberacao de frames na pilha de excecucao*/
  "ATR",//this new
  "SISM" //this new
  "SISA"
  "SISR"
  "SISD"
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
  m->ncristais = 0;//this new
  m->vida = 1;
  m->matou = 0;
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
#define maqi (m->pos[0])/*Macro para referencia a posição i da maquina*/
#define maqj (m->pos[1])/*Macro para referencia a posição j da maquina*/
#define contador_cristais (m->ncristais)

void exec_maquina(Maquina *m, int timestep) {//n vai virar o timestep
  int i;

  for (i = 0; i < timestep; i++) {
	OpCode   opc = prg[ip].instr;-
	OPERANDO arg = prg[ip].op;

	D(printf("%3d: %-4.4s %d\n     ", ip, CODES[opc], arg));

	switch (opc) {
	  OPERANDO tmp;
	  OPERANDO op1;//this new
	  OPERANDO op2;//this new
	  OPERANDO res;//this new
	  Celula cel;
	case PUSH:/*Nao necessita verificacao de tipo; empilha o obejto do topo*/
	  empilha(pil, arg);
	  break;
	case POP:/*Nao necessita verificacao de tipo; retira o objeto do topo*/
	  desempilha(pil);
	  break;
	case DUP:/*Nao necessita de verificacao de tipo; duplica o objeto do topo*/
	  tmp = desempilha(pil);
	  empilha(pil, tmp);
	  empilha(pil, tmp);
	  break;
	case ADD:/*Verifica o tipo dos operandos e os soma na NPR*/
	  op1 = desempilha(pil);
	  op2 = desempilha(pil);

	  if (op1.t == NUM && op2.t == NUM) {
		res.t = NUM;
		res.val.n = op1.val.n + op2.val.n;
		empilha(pil, res);
	  }
	  break;
	case SUB:/*Verifica o tipo dos operandos e os subtrai na NPR*/
	  op1 = desempilha(pil);
	  op2 = desempilha(pil);

	  if (op1.t == NUM && op2.t == NUM) {
		res.t = NUM;
		res.val.n = op2.val.n - op1.val.n;
		empilha(pil, res);
	  }
	  break;
	case MUL:/*Verifica o tipo dos operandos e os multiplica na NPR*/
	  op1 = desempilha(pil);
	  op2 = desempilha(pil);

	  if (op1.t == NUM && op2.t == NUM) {
		res.t = NUM;
		res.val.n = op1.val.n * op2.val.n;
		empilha(pil, res);
	  }
	  break;
	case DIV:/*Verifica o tipo dos operandos e os divide na NPR*/
	  op1 = desempilha(pil);
	  op2 = desempilha(pil);

	  if (op1.t == NUM && op2.t == NUM) {
		res.t = NUM;
		res.val.n = op2.val.n / op1.val.n;
		empilha(pil, res);
	  }
	  break;
	case JMP:/*Verifica o tipo do argumento e da o jump para o argumento*/
	  if (arg.t == NUM)
	  	ip = arg.val.n;
	  continue;
	case JIT:/*Verifica o tipo do topo da pilha e da o jump para o argumento 
				caso verdadeiro*/
	  tmp = desempilha(pil);
	  if (tmp.t == && arg.t == NUM) {//perguntar qual o tipo de tmp nesse caso
		if (tmp != 0) {
		  ip = arg.val.n;
		  continue;
		}
	  }
	  break;
	case JIF:/*Verifica o tipo do topo da pilha e da o jump para o argumento 
				caso falso*/
	  tmp = desempilha(pil);
	  if (tmp.t == && arg.t == NUM) {//perguntar tambem
	    if (desempilha(pil) == 0) {
		  ip = arg;
		  continue;
		}
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
	  tmp = desempilha(pil);
	  if (tmp.t == VAR && arg.t == NUM)
	    exec->val[arg.val.n+rbp] = tmp.var.v;
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
	case ATR://this new
	  cel = desempilha(pil);
	  switch(arg) {
	  	case 0:
	  		empilha(pil, cel.terreno)
	  		break;
	  	case 1:
	  		empilha(pil, cel.cristais)
	  		break;
	  	case 2:
	  		empilha(pil, cel.ocupado)
	  		break;
	  	case 3:
	  		empilha(pil, cel.pos[0])
	  		break;
	  	case 4:
	  		empilha(pil, cel.pos[1])
	  		break;
	  	case 5:
	  		empilha(pil, cel.base)
	  		break;
	  }
	  break;
	case SISM://System call para mover
	  tmp = Sistema(arg, *m);
	  if(tmp == 1) {
	  	switch(arg) {
			case 0:
				maqi += -2;
			case 1:
				maqi += -1;
				maqj += 1;
			case 2:
				maqi += 1;
				maqj += 1;
			case 3:
				maqi += 2;
			case 4:
				maqi += 1;
				maqj += -1;
			case 5:
				maqi += -1;
				maqj += -1;
		}
	  break;
	case SISA://System call para atacar
	  tmp = Sistema(arg+10, *m);
	  if(tmp == 1){
		m->matou += 1;
	  }
	  break;
	case SISR://System call para recolher os cristais 
	  tmp = Sistema(arg+20, *m);
	  if(tmp > 0){
		contador_cristais += tmp;
	  }
	  break;
	case SISD://System call para depositar os cristais
	  tmp = Sistema(arg+30, *m);
	  if(tmp == 1 && contador_cristais > 0) {
		contador_cristais = 0;
	  }
	  break;
	}
	D(imprime(pil,5));
	D(imprime(exec, 10));
	D(puts("\n"));
	ip++;
  }
}
