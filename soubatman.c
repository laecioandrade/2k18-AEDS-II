#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 4
#define MIN 2

FILE *fileEntrada, *fileSaida;
int contP=0;
int qtdAcessos=0;

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


void insere_f(Fila* f,int v){
	Lista* n=(Lista*)malloc(sizeof(Lista));
	n->n=v;
	n->prox=NULL;
    n->cont=contP;
	if(f->fim!=NULL){
		f->fim->prox=n;
	}else{
		f->ini=n;
	}
	f->fim=n;
}


void retira(Fila* f){
	Lista* t;
	int v;
	if(f->ini==NULL){
		printf("Lista vazia!");
		exit(1);
	}
    t=f->ini;
    while(t->cont == 1){
        v=t->n;
        f->ini=t->prox;
        if(f->ini==NULL){
            f->fim==NULL;
        }
        t=f->ini;
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

void reorganiza(Fila* f){
	Lista* q;
	for(q=f->ini;q!=NULL;q=q->prox){
		q->cont=1;
	}
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

//arvore
typedef struct NoArv Arv;
struct NoArv {
    int val[MAX + 1], count;
    Arv *link[MAX + 1];
};
Arv *raiz;
 
/* criando novo nó */
Arv * criarNo(int val, Arv *no) {
    Arv * novo = malloc(sizeof(Arv));
    novo->val[1] = val;
    novo->count = 1;
    novo->link[0] = raiz;
    novo->link[1] = no;
    return novo;
}
 
/* Coloca o valor na posição apropriada */
void adcValorNo(int val, int pos, Arv *no, Arv *novo) {
    int j = no->count;
    while (j > pos) {
        no->val[j + 1] = no->val[j];
        no->link[j + 1] = no->link[j];
        j--;
    }
    no->val[j + 1] = val;
    no->link[j + 1] = novo;
    no->count++;
    
}
 
/* dividir o nó */
void dividirNo(int val, int *pval, int pos, Arv *no,Arv *novo, Arv **novoNo) {
    int median, j;
 
    if (pos > MIN)
        median = MIN + 1;
    else
        median = MIN;
 
    *novoNo = malloc(sizeof(Arv));
    j = median + 1;
    while (j <= MAX) {
        (*novoNo)->val[j - median] = no->val[j];
        (*novoNo)->link[j - median] = no->link[j];
        j++;
    }
    no->count = median;
    (*novoNo)->count = MAX - median;
 
    if (pos <= MIN) {
        adcValorNo(val, pos, no, novo);
    }
    else {
        adcValorNo(val, pos - median, *novoNo, novo);
    }
    *pval = no->val[no->count];
    (*novoNo)->link[0] = no->link[no->count];
    no->count--;
}
 
/* define o valor chave no nó */
int defineChaveNo(int val, int *pval,Arv *node, Arv **child) {
 
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
    if (defineChaveNo(val, pval, node->link[pos], child)) {
        if (node->count < MAX) {
            adcValorNo(*pval, pos, node, *child);
        }
        else {
            dividirNo(*pval, pval, pos, node, *child, child);
            return 1;
        }
    }
    return 0;
}
 
/* inserir chave em B-Tree */
void inserir(int val) {
    int flag, i;
    Arv *child;
 
    flag = defineChaveNo(val, &i, raiz, &child);
    if (flag)
        raiz = criarNo(i, child);
}

 
/* buscar chave na B-Tree */
int buscarChave(Fila* f, int val, int *pos, Arv *myNode) {
    if (myNode == NULL) {
        return 0;
    }
    
    qtdAcessos=qtdAcessos+1;
    if(contP == 2){
        //int i;
        retira(f);
        reorganiza(f); 
        contP--;
    }
    contP++;
    for(*pos = 1;*pos <= myNode->count;(*pos)++){
    	fprintf(fileSaida,"%d ",myNode->val[*pos]);
        insere_f(f,myNode->val[*pos]);
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
    buscarChave(f, val, pos, myNode->link[*pos]);
    return 0;
}
//fimarvore
 
int main(int argc, char *argv[]) {
    int val, opt=0; 

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
	    	inserir(chavesInseridas[i]);
		}
		for(i=0;i<qtdInseridas;i++){
			if(1==buscar_f(f,chavesBusca[i])){
				fprintf(fileSaida, "hit");
			}else{
				buscarChave(f,chavesBusca[i], &opt,raiz);	
			}
			fprintf(fileSaida, "\n");
		}
        fprintf(fileSaida, "%d", qtdAcessos);
		printf("\n");
 	}
    system("pause");
}
