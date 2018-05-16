#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct lista Lista;
typedef struct pilha Pilha;

Pilha* cria();
void insere(Pilha* p, int valor);
int remover(Pilha* p);
int acessa(Pilha* p);
void imprime(Pilha* p);
void libera(Pilha* p);

struct lista{
	int valor;
	struct lista* prox;
};

struct pilha{
	Lista* prim;
};

Pilha* cria(){
	Pilha* p = (Pilha*)malloc(sizeof(Pilha));
	p->prim = NULL;
	return p;
}


void insere(Pilha* p, int valor){
	Lista* l = (Lista*)malloc(sizeof(Lista));
	l->valor=valor;
	l->prox=p->prim;
	p->prim=l;	
}

int acessa(Pilha* p){
	Lista* l;
	if(p==NULL){
		printf("Lista vazia!\n");
	}
	l=p->prim;
	return l->valor;
}


int remover(Pilha* p){
	Lista* l;
	int c;
	if(p==NULL){
		printf("Lista vazia!\n");
	}
	l = p->prim;
	c=l->valor;
	p->prim = l->prox;
	free(l);
	return c;
}

void imprime(Pilha* p){
	Lista* l;
	for(l=p->prim;l!=NULL;l=l->prox){
		printf("%d\n",l->valor);
	}
}

void libera(Pilha* p){
	Lista* l = p->prim;
	while(l!=NULL){
		Lista* l2 = l->prox;
		free(l);
		l=l2;
	}
	free(p);
	printf("Lista liberada!\n");
}

int main() {
	Pilha *STE;
	int c;
	STE=cria();
	insere(STE,1);
	insere(STE,2);
	printf("oi");
	c=remover(STE);
	insere(STE,3);
	acessa(STE);
	printf("\n");
	insere(STE,4);
	acessa(STE);
	printf("\n");
	insere(STE,5);
	c=remover(STE);
	insere(STE,acessa(STE));
	insere(STE,remover(STE));
	insere(STE,6);
	c=remover(STE);
	printf("\n");
	insere(STE,acessa(STE));
	imprime(STE);
	printf("\n");
	libera(STE);
	system("pause");
	return 0;
}
