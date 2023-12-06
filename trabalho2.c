//LINK VIDEO: Algoritmo Paginas Segunda chance exemplo basico:https://www.youtube.com/watch?v=_cUQcisicgA
//-----------------------------------------------
//FAZER ABERTURA DE ARQUIVO PARA TESTAR
//ERRO NA FUNÇÃO clearReferenceBits
//POSSIVEISERROS:
//MEMORIA --> CORRIGIDO

//------------------------------------------------
#include <stdio.h>
#include <stdlib.h>

// página
typedef struct {
    int id;        // Identificador da página
    int referenced; // Bit de referência
} Page;

// memória
typedef struct {
    int size;       // Tamanho da memória
    Page *pages;    // Array de páginas na memória
} Memory;

// Função para inicializar a memória
Memory initializeMemory(int size) {
    Memory memory;
    memory.size = size;
    memory.pages = (Page *)malloc(size * sizeof(Page));

    // Inicializa as páginas
    for (int i = 0; i < size; i++) {
        memory.pages[i].id = -1;        // Identificador inválido
        memory.pages[i].referenced = 0; // Bit de referência inicializado como 0
    }

    return memory;
}

// Função para imprimir o estado atual da memória
void printMemoryStatus(Memory memory) {
    printf("Estado da Memória:\n");
    for (int i = 0; i < memory.size; i++) {
        if (memory.pages[i].id != -1) {
            printf("Página %d | Referenciada: %d\n", memory.pages[i].id, memory.pages[i].referenced);
        } else {
            printf("Slot vazio\n");
        }
    }
    printf("\n");
}

// Função para tratar uma falta de página usando o algoritmo Segunda Chance
void handlePageFault(Memory *memory, int pageId) {
    printf("Falta de página para a página %d.\n", pageId);

    // Verifica se há um slot vazio na memória
    int emptySlotIndex = -1;
    for (int i = 0; i < memory->size; i++) {
        if (memory->pages[i].id == -1) {
            emptySlotIndex = i;
            break;
        }
    }

    // Se houver um slot vazio, coloca a nova página na memória
    if (emptySlotIndex != -1) {
        memory->pages[emptySlotIndex].id = pageId;
        memory->pages[emptySlotIndex].referenced = 1;
        printf("Página %d carregada na posição %d da memória.\n", pageId, emptySlotIndex);
    } else {
        // Se não houver um slot vazio, aplica o algoritmo Segunda Chance para escolher a página a ser substituída
        int replacedPageIndex = -1;

        while (replacedPageIndex == -1) {
            for (int i = 0; i < memory->size; i++) {
                if (memory->pages[i].referenced == 0) {
                    replacedPageIndex = i;
                    break;
                } else {
                    // Atualiza o bit de referência e continua a busca
                    memory->pages[i].referenced = 0;
                }
            }
        }

        printf("Substituindo página %d na posição %d por página %d.\n", memory->pages[replacedPageIndex].id, replacedPageIndex, pageId);

        // Substitui a página escolhida
        memory->pages[replacedPageIndex].id = pageId;
        memory->pages[replacedPageIndex].referenced = 1;
    }

    // Atualiza o estado da memória
    printMemoryStatus(*memory);
}


// Função para limpar os bits de referência das páginas após X requisições
void clearReferenceBits(Memory *memory, int clearAfter, int newPageId) {
    for (int i = 0; i < memory->size; i++) {
        if (memory->pages[i].id != -1) {
            // Limpa o bit de referência após X requisições
            if (clearAfter > 0) {
                memory->pages[i].referenced = 0;
            } else {
                // Limpa o bit de referência se a página não foi referenciada recentemente
                if (memory->pages[i].referenced == 0) {
                    printf("Bit de referência da página %d limpo.\n", memory->pages[i].id);
                    
                    // Substitui a página escolhida
                    int replacedPageIndex = i;

                    // Atualiza o estado da memória após a substituição
                    printf("Substituindo página %d na posição %d por página %d.\n", memory->pages[replacedPageIndex].id, replacedPageIndex, newPageId);
                    memory->pages[replacedPageIndex].id = newPageId;
                    memory->pages[replacedPageIndex].referenced = 1;
                    printMemoryStatus(*memory);
                }
            }
        }
    }
}


int main() {
    // Tamanho da memória e número de requisições
    int memorySize = 4;
    int numRequests = 10;
    int clearAfter = 5; // Número de requisições após as quais os bits de referência serão limpos
    int executionMode = 1;
    Memory memory = initializeMemory(memorySize);   // Inicializa a memória
    //------------------------------------------------------------------
    // substituir pelo arquivo:
    int requests[] = {0, 1, 2, 3, 4, 0, 1, 5, 6, 0};

    for (int i = 0; i < numRequests; i++) {
        int pageId = requests[i];

        // Limpa os bits de referência após X requisições
        if (i > 0 && i % clearAfter == 0) {
            clearReferenceBits(&memory, clearAfter);
        }

        // Verifica se a página está na memória
        int pageInMemory = 0;
        for (int j = 0; j < memory.size; j++) {
            if (memory.pages[j].id == pageId) {
                memory.pages[j].referenced = 1; // Atualiza o bit de referência
                pageInMemory = 1;
                break;
            }
        }

        // Se a página não estiver na memória, trata a falta de página
        if (!pageInMemory) {
            handlePageFault(&memory, pageId);
        }

            // MENU INTERATIVO
    //----------------------------------------------------------------------------
    printf("\n---------------------------------------------");
    printf("\nESCOLHA O MODO DE EXECUCAO QUE DESEJA: \n");
    printf("\n digite '1' --> modo de execucao passo a passo\n");
    printf("\n digite '2' --> modo de execucao direta\n");
    scanf("%d",&executionMode);

        // Modo de execução - Passo a passo
        if (executionMode == 1) {
            // Aguarda a entrada do usuário para continuar
            printf("Pressione Enter para continuar...");
            getchar();
        }
    }

    //  Execução direta
    if (executionMode == 2) {
        // Exibe o estado final da memória
        printMemoryStatus(memory);
    }

    // Libera a memória alocada
    free(memory.pages);

    return 0;
}
