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
  "ATR",/*Nova instrucao para receber um atributo desejado de uma celula*/
  "SISM",/*Nova instrucao que faz uma systemcall para mover a maquina*/
  "SISA",/*Nova instrucao que faz uma systemcall para a maquina atacar*/
  "SISR",/*Nova instrucao que faz uma systemcall para recolher cristais*/
  "SISD", /*Nova instrucao que faz uma systemcall para depositar cristais*/
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

Maquina *cria_maquina(INSTR *p) {/*Inicializa a maquina e seus atributos*/
  Maquina *m = (Maquina*)malloc(sizeof(Maquina));
  if (!m) Fatal("Memória insuficiente",4);
  m->ip = 0;
  m->rbp = 0;
  m->ncristais = 0;
  m->vida = 3;
  m->matou = 0;
  m->prog = p;
  return m;
}

void destroi_maquina(Maquina *m) {/*Quando morrer, destroi a maquina*/
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

void exec_maquina(Maquina *m, int timestep) {/*Excecuta as instrucoes de uma
												maquina*/
  int i;

  for (i = 0; i < timestep; i++) {
	OpCode   opc = prg[ip].instr;
	OPERANDO arg = prg[ip].op;

	printf("op: %d, 	arg: %d, 	i: %d\n", prg[ip].op.val.n, arg.val.n, i);

	D(printf("%3d: %-4.4s %d\n     ", ip, CODES[opc], arg));

	switch (opc) {
	  OPERANDO tmp;
	  OPERANDO op1;
	  OPERANDO op2;
	  OPERANDO res;
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

	  if (tmp.t == NUM && arg.t == NUM) {//perguntar qual o tipo de tmp nesse caso
		if (tmp.val.n != 0) {
		  ip = arg.val.n;
		  continue;
		}
	  }
	  break;
	case JIF:/*Verifica o tipo do topo da pilha e da o jump para o argumento
				caso falso*/
	  tmp = desempilha(pil);

	  if (tmp.t == NUM && arg.t == NUM) {//perguntar tambem
	    if (tmp.val.n == 0) {
		  ip = arg.val.n;
		  continue;
		}
	  }
	  break;
	case CALL:/*Empilha na pilha de excecucao o valor de rbp e de ip atuais e
				da os novos valores de rbp e de ip a serem usados na pilha de
				excecucao*/
	  if (arg.t == NUM) {
	  	op1.val.n = rbp;
	  	op1.t = NUM;
	  	op2.val.n = ip;
	  	op2.t = NUM;
	    empilha(exec, op1);
	    empilha(exec, op2);
	    rbp = exec->topo -1;
	    ip = arg.val.n;
	  }
	  continue;
	case RET:/*Verifica o tipo dos objetos no topo da pilha de excecucao e
				retorna aos ip e rbp antigos*/
	  op1 = desempilha(exec);
	  op2 = desempilha(exec);

	  if (op1.t == NUM && op2.t == NUM) {
	    ip = op1.val.n;
	    rbp = op2.val.n;
	  }
	  break;
	case STL:/*Verifica o tipo do topo da pilha e armazena na pilha de
				excecucao a variavel local.*/
	  tmp = desempilha(pil);//perguntar o tipo

	  if (tmp.t == NUM && arg.t == NUM)
	    exec->val[rbp+arg.val.n] = tmp;
	  break;
	case RCE:/*Verifica o tipo do topo na pilha de excecucao e recupera a
				variavel local da pilha de excecucao.*/
	  if (arg.t == NUM) {//perguntar o tipo
	    tmp = exec->val[rbp+arg.val.n];

	    if (tmp.t == NUM)
	      empilha(pil, tmp);
	  }
	  break;
	case EQ:/*Desempilha os 2 objetos do topo, verifica a igualdade entre eles
			e empilha um operando verdadeiro ou falso*/
	  op1 = desempilha(pil);
	  op2 = desempilha(pil);

	  if (op1.t == NUM && op2.t == NUM){
	  	if (op2.val.n == op1.val.n) {
		  	res.t = NUM;
		  	res.val.n = 1;
		  	empilha(pil, res);
		  }
		  else {
		  	res.t = NUM;
		  	res.val.n = 0;
		  	empilha(pil, res);
	  	  }
	  }
	  break;
	case GT:/*Desempilha os 2 objetos do topo, verifica se um e estritamente
				maior que o outro e empilha um operando verdadeiro ou falso*/
	  op1 = desempilha(pil);
	  op2 = desempilha(pil);

	  if (op1.t == NUM && op2.t == NUM){
	  	if (op2.val.n < op1.val.n) {
		  	res.t = NUM;
		  	res.val.n = 1;
		  	empilha(pil, res);
		  }
		  else {
		  	res.t = NUM;
		  	res.val.n = 0;
		  	empilha(pil, res);
	  	  }
	  }
	  break;
	case GE:/*Desempilha os 2 objetos do topo, verifica se um e maior ou igual
				ao outro e empilha um operando verdadeiro ou falso*/
	  op1 = desempilha(pil);
	  op2 = desempilha(pil);

	  if (op1.t == NUM && op2.t == NUM){
	  	if (op2.val.n <= op1.val.n) {
		  	res.t = NUM;
		  	res.val.n = 1;
		  	empilha(pil, res);
		  }
		  else {
		  	res.t = NUM;
		  	res.val.n = 0;
		  	empilha(pil, res);
	  	  }
	  }
	  break;
	case LT:/*Desempilha os 2 objetos do topo, verifica se um e estritamente
				menor que o outro e empilha um operando verdadeiro ou falso*/
	  op1 = desempilha(pil);
	  op2 = desempilha(pil);

	  if (op1.t == NUM && op2.t == NUM){
	  	if (op2.val.n > op1.val.n) {
		  	res.t = NUM;
		  	res.val.n = 1;
		  	empilha(pil, res);
		  }
		  else {
		  	res.t = NUM;
		  	res.val.n = 0;
		  	empilha(pil, res);
	  	  }
	  }
	  break;
	case LE:/*Desempilha os 2 objetos do topo, verifica se um e menor ou igual
				ao outro e empilha um operando verdadeiro ou falso*/
	  op1 = desempilha(pil);
	  op2 = desempilha(pil);

	  if (op1.t == NUM && op2.t == NUM){
	  	if (op2.val.n >= op1.val.n) {
		  	res.t = NUM;
		  	res.val.n = 1;
		  	empilha(pil, res);
		  }
		  else {
		  	res.t = NUM;
		  	res.val.n = 0;
		  	empilha(pil, res);
	  	  }
	  }
	case NE:/*Desempilha os 2 objetos do topo, verifica a nao igualdade entre
				eles e empilha um operando verdadeiro ou falso*/
	  op1 = desempilha(pil);
	  op2 = desempilha(pil);

	  if (op1.t == NUM && op2.t == NUM){
	  	if (op2.val.n != op1.val.n) {
		  	res.t = NUM;
		  	res.val.n = 1;
		  	empilha(pil, res);
		  }
		  else {
		  	res.t = NUM;
		  	res.val.n = 0;
		  	empilha(pil, res);
	  	  }
	  }
	  break;
	case STO:/*Armazena no vetor de memoria o objeto no topo da pilha*/
	  if (arg.t == NUM)
	    m->Mem[arg.val.n] = desempilha(pil);
	  break;
	case RCL:/*Empilha um objeto da memoria*/
	  if (arg.t == NUM)
	  	empilha(pil,m->Mem[arg.val.n]);
	  break;
	case END:/*Encerra a excecucao das instrucoes*/
	  return;
	case PRN:/*Verifica o tipo do objeto no topo da pilha e imprime o valor do
				topo da pilha e desempilha o topo, nao funciona para celulas*/
	  tmp = desempilha(pil);

	  if (tmp.t == NUM)
	    printf("%d\n", tmp.val.n);
	  else if (tmp.t == VAR)
	  	printf("%d\n", tmp.val.v);
	  break;
	case ALC:/*Funcao auxiliar aloca frames na pilha de excecucao para
				armazenar a variavel local*/
	  if (arg.t == NUM)
	    exec->topo += arg.val.n;
	  break;
	case FRE:/*Funcao auxiliar libera frames da pilha de excecucao onde estavam
				armazenadas as variaveis locais*/
	  if (arg.t == NUM)
	    exec->topo -= arg.val.n;
	  break;
	case ATR:/*Verifica o tipo do objeto do topo, o desempilha e empilha uma
				informcao da celula*/
	  tmp = desempilha(pil);

	  if (tmp.t == CELULA && arg.t == NUM){
	  	switch(arg.val.n) {
	      case 0:
	        res.val.v = tmp.val.cel.terreno;
	        res.t = VAR;
	        empilha(pil, res);
	        break;
	      case 1:
	        res.val.n = tmp.val.cel.cristais;
	        res.t = NUM;
	        empilha(pil, res);
	        break;
	      case 2:
	        res.val.n = tmp.val.cel.ocupado;
	        res.t = NUM;
	        empilha(pil, res);
	        break;
	      case 3:
	        res.val.n = tmp.val.cel.pos[1];
	        res.t = NUM;
	        empilha(pil, res);
	        break;
	      case 4:
	        res.val.n = tmp.val.cel.pos[1];
	        res.t = NUM;
	        empilha(pil, res);
	        break;
	      case 5:
	        res.val.n = tmp.val.cel.time;
	        res.t = NUM;
	        empilha(pil, res);
	        break;
	    }
	  }
	  break;
	case SISM:/*System call, verifica o tipo do argumento e tenta mover a
				maquina*/
	  if (arg.t == NUM){
	  	printf("%d\n", arg.val.n);
	    tmp = Sistema(arg, m);
	    if(tmp.val.n == 1) {
	  	  switch(arg.val.n) {
			  case 0:
			  	maqi += -2;
			  	break;
			  case 1:
				  maqi += -1;
				  maqj += 1;
				  break;
			  case 2:
				  maqi += 1;
				  maqj += 1;
				  break;
			  case 3:
				  maqi += 2;
				  break;
			  case 4:
				  maqi += 1;
				  maqj += -1;
				  break;
			  case 5:
				  maqi += -1;
				  maqj += -1;
				  break;
	  	  }
		}
	  break;
	case SISA:/*System call, verifica o tipo do argumento e tenta atacar algum
				inimigo*/
	  if (arg.t == NUM){
	    arg.val.n += 10;
	    tmp = Sistema(arg, m);
	    if(tmp.val.n == 1){
	    	m->matou += 1;
	    }
	  }
	  break;
	case SISR:/*System call, verifica o tipo do argumento e tenta recolher os
				cristais da celula*/
	  if (arg.t == NUM){
	    arg.val.n += 20;
	    tmp = Sistema(arg, m);
	    if(tmp.val.n > 0){
		    contador_cristais += tmp.val.n;
	    }
	  }
	  break;
	case SISD:/*System call, verifica o tipo do argumento e tenta depositar os
				cristais na base inimiga*/
	  if (arg.t == NUM){
	  	arg.val.n += 30;
	    tmp = Sistema(arg, m);
	    if(tmp.val.n == 1 && contador_cristais > 0) {
		    contador_cristais = 0;
	    }
	  }
	  break;
	}
	D(imprime(pil,5));
	D(imprime(exec, 10));
	D(puts("\n"));
	ip++;
  }
}}