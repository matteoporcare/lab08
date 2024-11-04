// Nome: Matteo Porcare RA: 10418276
// Nome: Lucas Monteiro Soares RA: 10417881

#include <stdio.h>
#include <stdlib.h>

#define TAMANHO_PAGINA 4096  // Definindo o tamanho de cada p�gina e frame
#define NUM_FRAMES 8         // N�mero de frames na mem�ria f�sica
#define NUM_PAGINAS 16       // N�mero de p�ginas na mem�ria virtual

// Estrutura para representar um frame na mem�ria f�sica
typedef struct {
    int numero_frame;
    int livre; // 1 indica que o frame est� livre, 0 indica que est� ocupado
} Frame;

// Estrutura para representar uma p�gina na mem�ria virtual
typedef struct {
    int numero_pagina;
    int frame_alocado; // -1 indica que a p�gina n�o est� alocada
} Pagina;

// Estrutura para gerenciar a mem�ria f�sica
typedef struct {
    Frame frames[NUM_FRAMES];
} MemoriaFisica;

// Estrutura para gerenciar a mem�ria virtual
typedef struct {
    Pagina paginas[NUM_PAGINAS];
} MemoriaVirtual;

// Estrutura para a tabela de p�ginas, mapeando p�ginas virtuais para frames f�sicos
typedef struct {
    int tabela_paginas[NUM_PAGINAS];
} TabelaPaginas;

// Estrutura para representar um processo com uma lista de endere�os virtuais que ele acessa
typedef struct {
    int id;
    int enderecos_virtuais[5];  // Lista de endere�os virtuais que o processo tentar� acessar
} Processo;

// Fun��o para inicializar a mem�ria f�sica
void inicializarMemoriaFisica(MemoriaFisica *memoria) {
    for (int i = 0; i < NUM_FRAMES; i++) {
        memoria->frames[i].numero_frame = i;
        memoria->frames[i].livre = 1; // Marca todos os frames como livres inicialmente
    }
}

// Fun��o para inicializar a mem�ria virtual
void inicializarMemoriaVirtual(MemoriaVirtual *memoria) {
    for (int i = 0; i < NUM_PAGINAS; i++) {
        memoria->paginas[i].numero_pagina = i;
        memoria->paginas[i].frame_alocado = -1; // -1 indica que n�o est� alocado
    }
}

// Fun��o para inicializar a tabela de p�ginas
void inicializarTabelaPaginas(TabelaPaginas *tabela) {
    for (int i = 0; i < NUM_PAGINAS; i++) {
        tabela->tabela_paginas[i] = -1; // -1 indica que a p�gina n�o est� mapeada para nenhum frame
    }
}

// Fun��o para inicializar o processo com endere�os virtuais
void inicializarProcesso(Processo *processo, int id, int enderecos[]) {
    processo->id = id;
    for (int i = 0; i < 5; i++) {
        processo->enderecos_virtuais[i] = enderecos[i];
    }
}

// Fun��o para traduzir endere�o virtual para f�sico
int traduzirEndereco(int endereco_virtual, TabelaPaginas *tabela) {
    int numero_pagina = endereco_virtual / TAMANHO_PAGINA;
    int offset = endereco_virtual % TAMANHO_PAGINA;

    int frame = tabela->tabela_paginas[numero_pagina];
    if (frame == -1) {
        printf("Page fault! P�gina %d n�o est� na mem�ria f�sica.\n", numero_pagina);
        return -1;
    }

    return frame * TAMANHO_PAGINA + offset;
}

// Fun��o para alocar uma p�gina a um frame livre
void alocarPagina(MemoriaFisica *memoria, TabelaPaginas *tabela, int numero_pagina) {
    for (int i = 0; i < NUM_FRAMES; i++) {
        if (memoria->frames[i].livre) {
            memoria->frames[i].livre = 0;
            tabela->tabela_paginas[numero_pagina] = memoria->frames[i].numero_frame;
            printf("P�gina %d alocada no frame %d.\n", numero_pagina, i);
            return;
        }
    }
    printf("N�o h� frames livres para alocar a p�gina %d.\n", numero_pagina);
}

// Fun��o de teste para simular o acesso do processo a endere�os virtuais
void simularAcesso(Processo *processo, TabelaPaginas *tabela, MemoriaFisica *memoria) {
    printf("\nSimula��o de acesso do processo %d\n", processo->id);
    for (int i = 0; i < 5; i++) {
        int endereco_virtual = processo->enderecos_virtuais[i];
        int endereco_fisico = traduzirEndereco(endereco_virtual, tabela);

        if (endereco_fisico == -1) {  // Page fault
            int numero_pagina = endereco_virtual / TAMANHO_PAGINA;
            alocarPagina(memoria, tabela, numero_pagina);
            endereco_fisico = traduzirEndereco(endereco_virtual, tabela);
        }

        if (endereco_fisico != -1) {
            printf("Endere�o virtual %d traduzido para endere�o f�sico %d\n", endereco_virtual, endereco_fisico);
        }
    }
}

int main() {
    MemoriaFisica memoria_fisica;
    MemoriaVirtual memoria_virtual;
    TabelaPaginas tabela_paginas;
    Processo processo;

    inicializarMemoriaFisica(&memoria_fisica);
    inicializarMemoriaVirtual(&memoria_virtual);
    inicializarTabelaPaginas(&tabela_paginas);

    int enderecos_processo[] = {8192, 4096, 12288, 16384, 20480};  // Endere�os virtuais que o processo tentar� acessar
    inicializarProcesso(&processo, 1, enderecos_processo);

    simularAcesso(&processo, &tabela_paginas, &memoria_fisica);

    return 0;
}
