#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 4
#define MIN 2

FILE *fileEntrada, *fileSaida;


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
 
/* cópia sucessora para o valor a ser excluído */
void copySuccessor(btree *myNode, int pos) {
    btree *dummy;
    dummy = myNode->link[pos];
 
    for (; dummy->link[0] != NULL;)
        dummy = dummy->link[0];
    myNode->val[pos] = dummy->val[1];
 
}
 
/* remove o valor do nó fornecido e reorganiza os valores */
void removeVal(btree *myNode, int pos) {
    int i = pos + 1;
    while (i <= myNode->count) {
        myNode->val[i - 1] = myNode->val[i];
        myNode->link[i - 1] = myNode->link[i];
        i++;
    }
    myNode->count--;
}
 
/* muda o valor do pai para o filho certo */
void doRightShift(btree *myNode, int pos) {
    btree *x = myNode->link[pos];
    int j = x->count;
 
    while (j > 0) {
        x->val[j + 1] = x->val[j];
        x->link[j + 1] = x->link[j];
    }
    x->val[1] = myNode->val[pos];
    x->link[1] = x->link[0];
    x->count++;
 
    x = myNode->link[pos - 1];
    myNode->val[pos] = x->val[x->count];
    myNode->link[pos] = x->link[x->count];
    x->count--;
    return;
}
 
/* muda o valor do pai para o filho esquerdo */
void doLeftShift(btree *myNode, int pos) {
    int j = 1;
    btree *x = myNode->link[pos - 1];
 
    x->count++;
    x->val[x->count] = myNode->val[pos];
    x->link[x->count] = myNode->link[pos]->link[0];
 
    x = myNode->link[pos];
    myNode->val[pos] = x->val[1];
    x->link[0] = x->link[1];
    x->count--;
 
    while (j <= x->count) {
        x->val[j] = x->val[j + 1];
        x->link[j] = x->link[j + 1];
        j++;
    }
    return;
}
 
/* nós de mesclagem */
void mergeNodes(btree *myNode, int pos) {
    int j = 1;
    btree *x1 = myNode->link[pos], *x2 = myNode->link[pos - 1];
 
    x2->count++;
    x2->val[x2->count] = myNode->val[pos];
    x2->link[x2->count] = myNode->link[0];
 
    while (j <= x1->count) {
        x2->count++;
        x2->val[x2->count] = x1->val[j];
        x2->link[x2->count] = x1->link[j];
        j++;
    }
 
    j = pos;
    while (j < myNode->count) {
        myNode->val[j] = myNode->val[j + 1];
        myNode->link[j] = myNode->link[j + 1];
        j++;
    }
    myNode->count--;
    free(x1);
}
 
/*  ajusta o nó dado */
void adjustNode(btree *myNode, int pos) {
    if (!pos) {
        if (myNode->link[1]->count > MIN) {
            doLeftShift(myNode, 1);
        }
        else {
            mergeNodes(myNode, 1);
        }
    }
    else {
        if (myNode->count != pos) {
            if (myNode->link[pos - 1]->count > MIN) {
                doRightShift(myNode, pos);
            }
            else {
                if (myNode->link[pos + 1]->count > MIN) {
                    doLeftShift(myNode, pos + 1);
                }
                else {
                    mergeNodes(myNode, pos);
                }
            }
        }
        else {
            if (myNode->link[pos - 1]->count > MIN)
                doRightShift(myNode, pos);
            else
                mergeNodes(myNode, pos);
        }
    }
}
 
/* excluir chave do nó */
int delValFromNode(int val,btree *myNode) {
    int pos, flag = 0;
    if (myNode) {
        if (val < myNode->val[1]) {
            pos = 0;
            flag = 0;
        }
        else {
            for (pos = myNode->count;
                (val < myNode->val[pos] && pos > 1); pos--);
            if (val == myNode->val[pos]) {
                flag = 1;
            }
            else {
                flag = 0;
            }
        }
        if (flag) {
            if (myNode->link[pos - 1]) {
                copySuccessor(myNode, pos);
                flag = delValFromNode(myNode->val[pos], myNode->link[pos]);
                if (flag == 0) {
                    printf("Dado nao esta presente em B-Tree\n");
                }
            }
            else {
                removeVal(myNode, pos);
            }
        }
        else {
            flag = delValFromNode(val, myNode->link[pos]);
        }
        if (myNode->link[pos]) {
            if (myNode->link[pos]->count < MIN)
                adjustNode(myNode, pos);
        }
    }
    return flag;
}
 
/* excluir chave de B-tree */
void deletion(int val,btree *myNode) {
    btree *tmp;
    if (!delValFromNode(val, myNode)) {
        printf("Dado, valor nao esta presente em B-Tree\n");
        return;
    }
    else {
        if (myNode->count == 0) {
            tmp = myNode;
            myNode = myNode->link[0];
            free(tmp);
        }
    }
    root = myNode;
    return;
}
 
/* buscar chave na B-Tree */
int searching(int val, int *pos,btree *myNode) {
    if (myNode == NULL) {
        return 0;
    }
    
    for(*pos = 1;*pos <= myNode->count;(*pos)++){
    	fprintf(fileSaida,"%d ",myNode->val[*pos]);
	}
    
	if (val < myNode->val[1]) {
        *pos = 0;
    }
	else {
        for (*pos = myNode->count; (val < myNode->val[*pos] && *pos > 1); (*pos)--);
        //fprintf(fileSaida, "%d ",myNode->val[*pos]);
		if (val == myNode->val[*pos]) {
            //printf("\nDado encontrado\n");
            return 1;
        }
    }
    //adicionei
    //printf("%d ",myNode->val[*pos]);	
 	//fprintf(fileSaida, "%d ", myNode->val[*pos]);
    //fimadicionei
    searching(val, pos, myNode->link[*pos]);
    return 0;
}
 
/* Imprimir a B-Tree */
void traversal(btree *myNode) {
    int i;
    if (myNode) {
        for (i = 0; i < myNode->count; i++) {
            printf("%d\n",myNode->val[i + 1]);
			traversal(myNode->link[i]);
        }
        traversal(myNode->link[i]);
    }
}
 
int main(int argc, char *argv[]) {
    int val, opt=0;
    
	//adicionei
    //char *fileEntrada, *fileSaida;
    if(argc == 3){
	    int memoria, ordem, qtdChaves, i, chave, qtdInseridas, insertChaves;
		
	    char entrada[40] = "";
        char saida[40] = "";

        strcat(entrada,argv[1]);
        strcat(saida,argv[2]);

        fileEntrada = abreArquivoLeitura(entrada);
        fileSaida = abreArquivoEscrita(saida);

		fscanf(fileEntrada, "%d", &memoria);
	    fscanf(fileEntrada, "%d", &ordem);
	    fscanf(fileEntrada, "%d", &qtdChaves);
	    
	    
	    int chavesInseridas[qtdChaves];
	    
	    for(i=0;i<qtdChaves;i++){
	    	fscanf(fileEntrada, "%d", &chave);
	    	chavesInseridas[i]=chave;
	    	//printf("Chave %d ", chave);
		}
		
		fscanf(fileEntrada, "%d", &qtdInseridas);
		int chavesBusca[qtdInseridas];
		
		for(i=0;i<qtdInseridas;i++){
			fscanf(fileEntrada, "%d", &insertChaves);
			chavesBusca[i]=insertChaves;
			/*printf("Chave: %d", insertChaves);*/	
		}
		
		for(i=0;i<qtdChaves;i++){
	    	insertion(chavesInseridas[i]);
	    	//fprintf(fileSaida,"%d ",chavesInseridas[i]);
		}
		for(i=0;i<qtdInseridas;i++){
			opt=0;
			searching(chavesBusca[i], &opt, root);
			fprintf(fileSaida, "\n");
		}
		printf("\n");
		traversal(root);
		//fimadicionei
	    
	    /*while(opt!=5) {
	        printf("1. Inserir\t2. Deletar\n");
	        printf("3. Buscar\t4. Imprimir\n");
	        printf("5. Sair\nSelecione a opcao: ");
	        scanf("%d\n",&opt);
	        switch (opt) {
		        case 1:
		            printf("Informe a chave:");
		            scanf("%d",&val);
		            insertion(val);
		            break;
		        case 2:
		            printf("Informe a chave que desaja deletar:");
		            scanf("%d",&val);
		            deletion(val, root);
		            break;
		        case 3:
		            printf("Informe o elemento que deseja buscar:");
		            scanf("%d",&val);
		            int n;
					n=searching(val, &opt, root);
		            if(n==0){
		            	printf("Dado nao encontrado!\n");
					}
		            break;
		        case 4:
		            traversal(root);
		            break;
		        case 5:
		            exit(1);
		            break;
		    }
		    printf("\n");
	    }*/
		
	    
 	}
    system("pause");
}
