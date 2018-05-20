#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 4
#define MIN 2

FILE *fileEntrada, *fileSaida;
int cont=0;
//fila
struct lista{
	int n;
	int cont;
	struct lista* prox;
};
typedef struct lista Lista;

struct fila{
	Lista *ini;
	Lista *fim;
	int numpg;
};
typedef struct fila Fila;

Fila* cria_f(){
	Fila* f=(Fila*)malloc(sizeof(Fila));
	f->ini = f->fim = NULL;
	return f;
}


void insere_f(Fila* f,int v, int cont){
	Lista* n=(Lista*)malloc(sizeof(Lista));
	n->n=v;
	n->prox=NULL;
    n->cont=cont;
	if(f->fim!=NULL){
		f->fim->prox=n;
	}else{
		f->ini=n;
	}
	f->fim=n;
}


void retira(Fila* f, int cont){
	Lista* t;
	int v;
	if(f->ini==NULL){
		printf("Lista vazia!");
		exit(1);
	}
    t=f->ini;
    while(t->cont == cont){
        v=t->n;
        f->ini=t->prox;
        if(f->ini==NULL){
            f->fim==NULL;
        }
    }
    free(t);   
	//return v;
}


int buscar_f(Fila* f, int valor){
	Lista *b;
	if(f==NULL){
		return 0;
	}
	for(b=f->ini;b!=NULL;b=b->prox){
		if(valor==b->n){
			return 1;
		}
	}
	return 0;
}


void libera(Fila* f){
	Lista* t=f->ini;
	while(t!=NULL){
		Lista* q=t->prox;
		free(t);
		t=q;
	}
	free(f);
	printf("Fila liberada!\n");
}
//fimfila
//arquivo
FILE * abreArquivoLeitura(char * arquivo) {
    FILE * ptr_arquivo = NULL;
    if ((ptr_arquivo = fopen(arquivo, "r")) == NULL){
        printf("Erro ao abrir arquivo para leitura.\n");
        exit(1);
    }
    else {
        //printf("Arquivo aberto com sucesso!\n");
        return ptr_arquivo;
    }
}


// Funções de escrita

FILE * abreArquivoEscrita(char * arquivo) {
    FILE * ptr_arquivo = NULL;
    if ((ptr_arquivo = fopen(arquivo, "w")) == NULL){
        printf("Erro ao abrir arquivo para escrita.\n");
        exit(1);
    }
    else {
        //printf("Arquivo aberto com sucesso!\n");
        return ptr_arquivo;
    }
}
//fimarquivo


typedef struct btreeNode btree;
struct btreeNode {
    int val[MAX + 1], count;
    btree *link[MAX + 1];
};
btree *root;
 
/* criando novo nó */
btree * createNode(int val, btree *child) {
    btree * newNode = malloc(sizeof(btree));
    newNode->val[1] = val;
    newNode->count = 1;
    newNode->link[0] = root;
    newNode->link[1] = child;
    return newNode;
}
 
/* Coloca o valor na posição apropriada */
void addValToNode(int val, int pos, btree *node, btree *child) {
    int j = node->count;
    while (j > pos) {
        node->val[j + 1] = node->val[j];
        node->link[j + 1] = node->link[j];
        j--;
    }
    node->val[j + 1] = val;
    node->link[j + 1] = child;
    node->count++;
    
}
 
/* dividir o nó */
void splitNode(int val, int *pval, int pos, btree *node,btree *child, btree **newNode) {
    int median, j;
 
    if (pos > MIN)
        median = MIN + 1;
    else
        median = MIN;
 
    *newNode = malloc(sizeof(btree));
    j = median + 1;
    while (j <= MAX) {
        (*newNode)->val[j - median] = node->val[j];
        (*newNode)->link[j - median] = node->link[j];
        j++;
    }
    node->count = median;
    (*newNode)->count = MAX - median;
 
    if (pos <= MIN) {
        addValToNode(val, pos, node, child);
    }
    else {
        addValToNode(val, pos - median, *newNode, child);
    }
    *pval = node->val[node->count];
    (*newNode)->link[0] = node->link[node->count];
    node->count--;
}
 
/* define o valor chave no nó */
int setValueInNode(int val, int *pval,btree *node, btree **child) {
 
    int pos;
    if (!node) {
        *pval = val;
        *child = NULL;
        return 1;
    }
 
    if (val < node->val[1]) {
        pos = 0;
    }
    else {
        for (pos = node->count;
            (val < node->val[pos] && pos > 1); pos--);
        if (val == node->val[pos]) {
            
			printf("Duplicatas nao permitidas\n");
            return 0;
        }
    }
    if (setValueInNode(val, pval, node->link[pos], child)) {
        if (node->count < MAX) {
            addValToNode(*pval, pos, node, *child);
        }
        else {
            splitNode(*pval, pval, pos, node, *child, child);
            return 1;
        }
    }
    return 0;
}
 
/* inserir chave em B-Tree */
void insertion(int val) {
    int flag, i;
    btree *child;
 
    flag = setValueInNode(val, &i, root, &child);
    if (flag)
        root = createNode(i, child);
}

 
/* buscar chave na B-Tree */
int searching(Fila* f, int val, int *pos, btree *myNode,int cont) {
    if (myNode == NULL) {
        return 0;
    }
    cont++;
    for(*pos = 1;*pos <= myNode->count;(*pos)++){
    	fprintf(fileSaida,"%d ",myNode->val[*pos]);
        if(cont == 2){
            int i;
            retira(f,cont);    
            cont=0;
        }else{
          insere_f(f,myNode->val[*pos],cont);
        }
	}
    
	if (val < myNode->val[1]) {
        *pos = 0;
    }
	else {
        for (*pos = myNode->count; (val < myNode->val[*pos] && *pos > 1); (*pos)--);

		if (val == myNode->val[*pos]) {

            return 1;
        }
    }

    searching(f, val, pos, myNode->link[*pos],cont);
    return 0;
}
 
 
int main(int argc, char *argv[]) {
    int val, opt=0, qtdAcessos=0; 

    if(argc == 3){
	    int memoria, ordem, qtdChaves, i, chave, qtdInseridas, insertChaves;
		
	    char entrada[40] = "";
        char saida[40] = "";

        Fila* f;
        f=cria_f();
        strcat(entrada,argv[1]);
        strcat(saida,argv[2]);

        fileEntrada = abreArquivoLeitura(entrada);
        fileSaida = abreArquivoEscrita(saida);

		fscanf(fileEntrada, "%d", &memoria);
	    fscanf(fileEntrada, "%d", &ordem);
	    fscanf(fileEntrada, "%d", &qtdChaves);
	    
	    int paginas = (memoria/(4*(2*ordem) + 4 * (2 * ordem + 1)));
	    f->numpg=paginas;
	    
	    int chavesInseridas[qtdChaves];
	    
	    for(i=0;i<qtdChaves;i++){
	    	fscanf(fileEntrada, "%d", &chave);
	    	chavesInseridas[i]=chave;

		}
		
		fscanf(fileEntrada, "%d", &qtdInseridas);
		int chavesBusca[qtdInseridas];
		
		for(i=0;i<qtdInseridas;i++){
			fscanf(fileEntrada, "%d", &insertChaves);
			chavesBusca[i]=insertChaves;

		}
		
		for(i=0;i<qtdChaves;i++){
	    	insertion(chavesInseridas[i]);
		}
		for(i=0;i<qtdInseridas;i++){
			if(1==buscar_f(f,chavesBusca[i])){
				fprintf(fileSaida, "hit");
			}else{
                qtdAcessos=qtdAcessos+3;
				searching(f,chavesBusca[i], &opt,root, cont);	
			}
			fprintf(fileSaida, "\n");
		}
        fprintf(fileSaida, "%d ", qtdAcessos);
		printf("\n");
	    
 	}
    system("pause");
}
