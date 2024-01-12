#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

struct tp_produto {
	int cod;
    int qtde_CO;
    int qtde_min;
    int vendas;
    int necessidade;
    int transfere;
	int estq_Apos_V; 
}Prod;

struct vendas{
	int cod;
    int qtde_Vendas;
    int sit;//situacao
    int canal; //canal de venda
}Vendas;


void imprime_comeco(void) {
    printf("----------------------------------------------");
    printf("\nPROJETO 3 - Linguagem C : Professor Banin\n");
    printf("\nNomes dos integrantes: ");
    printf("\nIgor");
    printf("\nJulia");
    printf("\nKissa");
    printf("\nRebeca");
    printf("\n----------------------------------------------");
    printf("\n");
    system("pause");
    system("cls");
    return;
}

int main(void){
    
	int i = 0, j = 0;
    tp_produto auxProd;
    int TamProd = 0;
    tp_produto *Prod;
    
    vendas aux;
    vendas *Vendas;
    int TamV = 0;
    Vendas = NULL;
    
    // Variáveis para os totais por canal
    int tot_Repres = 0, tot_Webs = 0, tot_App_A = 0, tot_App_I = 0;
    
    // Verifica se o código de produto existe
    int prod_enct = 0;
    
    // Inicializa o array com zeros
    int tot_Canal[5] = {0};

    //Le o arquivo VENDAS.TXT
    FILE *Arq_produtos;
    Arq_produtos = fopen("c1_produtos.txt", "r");

	Prod = NULL;

	while(fscanf(Arq_produtos, "%d;%d;%d", &auxProd.cod, &auxProd.qtde_CO, &auxProd.qtde_min) != EOF) {
		TamProd++;
	    Prod = (tp_produto *)realloc(Prod, sizeof(tp_produto) * TamProd);
	    Prod[TamProd-1].vendas = 0;
	    Prod[TamProd-1].transfere = 0;
	    Prod[TamProd-1].necessidade = 0;
	    Prod[TamProd-1] = auxProd;
	}
	
	if (Arq_produtos == NULL) {
        printf("Existe um erro ao abrir o arquivo (PRODUTOS.TXT)\n");
        return 1;
    }

    fclose(Arq_produtos);
    printf("Leitura realizada com sucesso (PRODUTOS.TXT)\n");
    
    FILE *Arq_Vendas;
    Arq_Vendas = fopen("c1_vendas.txt", "r");
    
    Vendas = NULL;
    
	//Le o arquivo VENDAS.TXT
    while(fscanf(Arq_Vendas, "%d;%d;%d;%d", &aux.cod, &aux.qtde_Vendas, &aux.sit, &aux.canal) != EOF) {
	    TamV++;
	    Vendas = (vendas *)realloc(Vendas, sizeof(vendas) * TamV);
	    Vendas[TamV-1] = aux;
	}
	if (Arq_Vendas == NULL) {
        printf("Existe um erro ao abrir o arquivo VENDAS.TXT\n");
        return 1;
    }
	
	fclose(Arq_Vendas);
	printf("Leitura realizada com sucesso (VENDAS.TXT)\n");
	system("pause");
    system("cls");
    
    FILE *Arq_Transfere = fopen("TRANSFERE.txt", "w");
    FILE *Arq_Divergencias = fopen("DIVERGENCIAS.txt", "w");
    FILE *Arq_tot_canais = fopen("TOT_CANAIS.txt", "w");
    
    fprintf(Arq_Transfere, "Produto\tQtCO\tQtMin\tQtVendas\tEstq.após\tNecess.\tTransf. de Vendas Arm p/ CO\n");
    
    
    int vendas;
    
    for(i = 0; i < TamProd; i++) {
    	vendas = 0;
    	for(j = 0; j < TamV; j++) {
    		if(Vendas[j].cod == Prod[i].cod) {
    			if(Vendas[j].sit == 100 || Vendas[j].sit == 102) {
    				vendas += Vendas[j].qtde_Vendas;
				}
			}
		}
		Prod[i].vendas = vendas;
		
		Prod[i].estq_Apos_V = Prod[i].qtde_CO - Prod[i].vendas;
		Prod[i].necessidade = (Prod[i].estq_Apos_V < Prod[i].qtde_min) ? (Prod[i].qtde_min - Prod[i].estq_Apos_V) : 0;
        Prod[i].transfere = (Prod[i].necessidade > 1 && Prod[i].necessidade < 10) ? 10 : Prod[i].necessidade;
	}
	
	for(i = 0; i < TamProd; i++) {
		// Gera a linha no arquivo TRANSFERE.TXT
		fprintf(Arq_Transfere, "%d		%d		%d		%d		%d		%d		%d\n",
				Prod[i].cod, Prod[i].qtde_CO, Prod[i].qtde_min, Prod[i].vendas, Prod[i].estq_Apos_V, Prod[i].necessidade, Prod[i].transfere);
	}
    
    for (i = 0; i < TamV; i++){
		for (j = 0; j < TamProd; j++){
            if (Vendas[i].cod == Prod[j].cod) {
            	prod_enct = 1;
				// Verifica a situação da venda
				if (Vendas[i].sit == 100 || Vendas[i].sit == 102) {
    				// Atualiza os totais por canal
					if (Vendas[i].canal == 1) {
    					tot_Repres += Vendas[i].qtde_Vendas;
					} else if (Vendas[i].canal == 2) {
    					tot_Webs += Vendas[i].qtde_Vendas;
					} else if (Vendas[i].canal == 3) {
    					tot_App_A += Vendas[i].qtde_Vendas;
					} else if (Vendas[i].canal == 4) {
    					tot_App_I += Vendas[i].qtde_Vendas;
					}
				}
			}
		}
    }
    
    for (i = 0; i < TamV; i++){
		for(j = 0; j < TamProd; j++) {
			if(Vendas[i].cod == Prod[j].cod) {
				prod_enct = 1;
			}
		}		
		// Se o código de produto não existe, gera divergência
		if(!prod_enct) {
    		fprintf(Arq_Divergencias, "Linha %d – Codigo do Produto nao encontrado %d\n", i + 1, Vendas[i].cod);
		}

		// Gera divergência se a situação da venda for cancelada
		if (Vendas[i].sit == 135) {
    		fprintf(Arq_Divergencias, "Linha %d – Venda cancelada\n", i + 1);
		}

		// Gera divergência se a situação da venda for não finalizada
		if (Vendas[i].sit == 190) {
    		fprintf(Arq_Divergencias, "Linha %d – Venda nao finalizada\n", i + 1);
		}

		// Gera divergência se a situação da venda for erro desconhecido
		if (Vendas[i].sit == 999) {
    		fprintf(Arq_Divergencias, "Linha %d – Erro desconhecido. Acionar equipe de TI.\n", i + 1);
    	}
    	prod_enct = 0;
	}
    
    // Gera o arquivo TOTCANAIS.TXT
    fprintf(Arq_tot_canais, "1: Representante Comercial %d\n", tot_Repres);
    fprintf(Arq_tot_canais, "2: Website %d\n", tot_Webs);
    fprintf(Arq_tot_canais, "3: Aplicativo movel Android %d\n", tot_App_A);
    fprintf(Arq_tot_canais, "4: Aplicativo movel IPhone %d\n", tot_App_I);
    	
	fclose(Arq_Transfere);
	printf("Arquivo gravado com sucesso (TRANSFERE.TXT)\n");
	fclose(Arq_tot_canais);
	printf("Arquivo gravado com sucesso (TOT_CANAIS.TXT)\n");
	fclose(Arq_Divergencias);
	printf("Arquivo gravado com sucesso (DIVERGENCIAS.TXT)\n");
	
	return 0;
}
