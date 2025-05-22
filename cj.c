// #include <stdlib.h>
// #include <stdio.h>
// #include <string.h>
 
// #define MAX 1000000 // Tamanho máximo para vetores RGB
 
// int calcular_padding(int largura) {
//     int linhas_bytes = largura * 3;
//     return (4 - (linhas_bytes % 4)) % 4;
// }
// //Calculei o padding necessário para que cada linha da imagem BMP tenha um tamanho múltiplo de 4 bytes (requisito do formato BMP)
 
// int ler_cabecalho(FILE *bmp, unsigned char *cabecalho) {
//     if (fread(cabecalho, sizeof(unsigned char), 54, bmp) != 54) {
//         perror("Erro ao ler cabeçalho BMP");
//         return -1; // Indica erro
//     }
//     int offset = cabecalho[10] + (cabecalho[11] << 8) + (cabecalho[12] << 16) + (cabecalho[13] << 24);
//     return offset;
// }
 
 
// // usamos o fread para ler dados de um arquivo.
// // usamos o offset para ver a posição (em bytes) onde começam efetivamente os dados dos pixels da imagem, após o cabeçalho.
// // unsigned char  para armazenar valores de 0  a 255 bits
 
// void obter_dimensoes(unsigned char *cabecalho, int *largura, int *altura) {
//     *largura = cabecalho[18] + (cabecalho[19] << 8) + (cabecalho[20] << 16) + (cabecalho[21] << 24);
//     *altura  = cabecalho[22] + (cabecalho[23] << 8) + (cabecalho[24] << 16) + (cabecalho[25] << 24);
// }
 
// //Extraimos  a larura e altura da imagem do cabeçalho BMP (campos nos bytes 18-21 e 22-25).
 
// int ler_matriz(FILE *bmp, unsigned char **matriz, int altura, int largura, int offset) {
//     int padding = calcular_padding(largura);
//     int larguraBytes = largura * 3 + padding;
//     if (fseek(bmp, offset, SEEK_SET) != 0) {
//         perror("Erro ao posicionar no início dos dados da imagem BMP");
//         return 1; // Indica erro
//     }
 
//     for (int i = altura - 1; i >= 0; i--) {
//         if (fread(matriz[i], sizeof(unsigned char), larguraBytes, bmp) != larguraBytes) {
//             perror("Erro ao ler dados da linha da imagem BMP");
//             return 1; // Indica erro
//         }
//     }
//     return 0; // Sucesso
// }
// //Lemos os dados da imagem BMP para uma matriz, considerando que as linhas são armazenadas de baixo para cima no formato BMP.
 
 
// void compacta_quadrante(unsigned char **matriz, int li, int ci, int lf, int cf, int largura, int *k,
//                         unsigned char *vetR, unsigned char *vetG, unsigned char *vetB) {
//     if (*k >= MAX) { // Adicionar verificação para evitar estouro de buffer
//         fprintf(stderr, "Aviso: Capacidade máxima de amostras (%d) atingida na compactação.\n", MAX);
//         return;
//     }

//     if ((lf - li + 1) <= 3 || (cf - ci + 1) <= 3) {
//         int centro_linha = (li + lf) / 2;
//         int centro_coluna = (ci + cf) / 2;
//         int idx = centro_coluna * 3;
 
//         vetR[*k] = matriz[centro_linha][idx + 2];
//         vetG[*k] = matriz[centro_linha][idx + 1];
//         vetB[*k] = matriz[centro_linha][idx];
//         (*k)++;
//         return;
//     }
 
//     int meio_linha = (li + lf) / 2;
//     int meio_coluna = (ci + cf) / 2;
 
//     compacta_quadrante(matriz, li, ci, meio_linha, meio_coluna, largura, k, vetR, vetG, vetB);
//     compacta_quadrante(matriz, li, meio_coluna + 1, meio_linha, cf, largura, k, vetR, vetG, vetB);
//     compacta_quadrante(matriz, meio_linha + 1, ci, lf, meio_coluna, largura, k, vetR, vetG, vetB);
//     compacta_quadrante(matriz, meio_linha + 1, meio_coluna + 1, lf, cf, largura, k, vetR, vetG, vetB);
// }
 
// //fizemos a recursiva que divide a imagem em varios  quadrantes e armazena o pixel central de cad região pequena (<=3 pixels) nos vetores R, G e B.
 
// void salvar_arquivo_compactado(const char *nome, unsigned char *cabecalho,
//                                 unsigned char *vetR, unsigned char *vetG, unsigned char *vetB, int tamanho) {
//     FILE *out = fopen(nome, "wb");
//     if (out == NULL) {
//         perror("Erro ao abrir arquivo para salvar compactado");
//         return;
//     }
//     if (fwrite(cabecalho, sizeof(unsigned char), 54, out) != 54) {
//         perror("Erro ao escrever cabeçalho no arquivo compactado");
//         fclose(out);
//         return;
//     }
//     for (int i = 0; i < tamanho; i++) {
//         if (fwrite(&vetR[i], 1, 1, out) != 1 ||
//             fwrite(&vetG[i], 1, 1, out) != 1 ||
//             fwrite(&vetB[i], 1, 1, out) != 1) {
//             perror("Erro ao escrever dados RGB no arquivo compactado");
//             fclose(out);
//             return;
//         }
//     }
//     fclose(out);
// }
// //fwrite escrever dados binarios brutos de um arquivo 
 
// void descompacta_quadrante(unsigned char **matriz, int li, int ci, int lf, int cf, int largura, int *k,
//                            unsigned char *vetR, unsigned char *vetG, unsigned char *vetB, int total_amostras) {
//     if (*k >= total_amostras) {
//         // Evita ler além das amostras disponíveis. Pode ser um aviso se a estrutura da imagem
//         // esperar mais dados do que o arquivo compactado forneceu.
//         return;
//     }

//     if ((lf - li + 1) <= 3 || (cf - ci + 1) <= 3) {
//         for (int i = li; i <= lf; i++) {
//             for (int j = ci; j <= cf; j++) {
//                 int idx = j * 3;
//                 matriz[i][idx + 2] = vetR[*k];
//                 matriz[i][idx + 1] = vetG[*k];
//                 matriz[i][idx]     = vetB[*k];
//             }
//         }
//         (*k)++;
//         return;
//     }
 
//     int meio_linha = (li + lf) / 2;
//     int meio_coluna = (ci + cf) / 2;
 
//     descompacta_quadrante(matriz, li, ci, meio_linha, meio_coluna, largura, k, vetR, vetG, vetB, total_amostras);
//     descompacta_quadrante(matriz, li, meio_coluna + 1, meio_linha, cf, largura, k, vetR, vetG, vetB, total_amostras);
//     descompacta_quadrante(matriz, meio_linha + 1, ci, lf, meio_coluna, largura, k, vetR, vetG, vetB, total_amostras);
//     descompacta_quadrante(matriz, meio_linha + 1, meio_coluna + 1, lf, cf, largura, k, vetR, vetG, vetB, total_amostras);
// }
 
// //criamos esta função  para reconstrói recursivamente a imagem, preenchendo quadrantes pequenos com um pixel representativo salvo na compactação 
 
// void escrever_imagem_bmp(const char *nome, unsigned char *cabecalho, unsigned char **matriz, int altura, int largura) {
//     FILE *out = fopen(nome, "wb");
//     if (out == NULL) {
//         perror("Erro ao abrir arquivo para escrever imagem BMP");
//         return;
//     }
//     if (fwrite(cabecalho, sizeof(unsigned char), 54, out) != 54) {
//         perror("Erro ao escrever cabeçalho no arquivo BMP");
//         fclose(out);
//         return;
//     }
//     int padding = calcular_padding(largura);
//     int larguraBytes = largura * 3 + padding;
//     for (int i = altura - 1; i >= 0; i--) {
//         if (fwrite(matriz[i], sizeof(unsigned char), larguraBytes, out) != larguraBytes) {
//             perror("Erro ao escrever dados da imagem no arquivo BMP");
//             fclose(out);
//             return;
//         }
//     }
//     fclose(out);
// }
 
// //Preserva o formato BMP com cabeçalho intacto e  a ordem invertida das linhas  e ainda incluimos o padding que garante que qualquer programa possa abrir o .bpm reconstruido 
 
// int main(int argc, char *argv[]) {
//     if (argc != 4) {
//         printf("Uso:\n  %s compacta imagem.bmp imagemCompactada.zmp\n", argv[0]);
//         printf("      %s descompacta imagemCompactada.zmp imagemDescompactada.bmp\n", argv[0]);
//         return 1;
//     }
 
//     if (strcmp(argv[1], "compacta") == 0) {
//         FILE *bmp = fopen(argv[2], "rb");
//         if (!bmp) {
//             perror("Erro ao abrir imagem BMP");
//             return 1;
//         }
 
//         unsigned char cabecalho[54];
//         int offset = ler_cabecalho(bmp, cabecalho);
//         if (offset == -1) { // Erro na leitura do cabeçalho
//             fclose(bmp);
//             return 1;
//         }
 
//         int largura, altura;
//         obter_dimensoes(cabecalho, &largura, &altura);

//         if (largura <= 0 || altura <= 0) {
//             fprintf(stderr, "Erro: Dimensões da imagem inválidas (largura: %d, altura: %d).\n", largura, altura);
//             fclose(bmp);
//             return 1;
//         }

//         int padding = calcular_padding(largura);
//         int larguraBytes = largura * 3 + padding;
 
//         unsigned char **matriz = malloc(altura * sizeof(unsigned char *));
//         if (matriz == NULL) {
//             perror("Erro ao alocar memória para linhas da matriz");
//             fclose(bmp);
//             return 1;
//         }
//         for (int i = 0; i < altura; i++) {
//             matriz[i] = malloc(larguraBytes);
//             if (matriz[i] == NULL) {
//                 perror("Erro ao alocar memória para colunas da matriz");
//                 // Liberar memória já alocada antes de sair
//                 for (int j = 0; j < i; j++) {
//                     free(matriz[j]);
//                 }
//                 free(matriz);
//                 fclose(bmp);
//                 return 1;
//             }
//         }
 
//         if (ler_matriz(bmp, matriz, altura, largura, offset) != 0) {
//             // Erro na leitura da matriz, liberar memória e sair
//             for (int i = 0; i < altura; i++) {
//                 free(matriz[i]);
//             }
//             free(matriz);
//             fclose(bmp);
//             return 1;
//         }
//         fclose(bmp);
 
//         unsigned char vetR[MAX], vetG[MAX], vetB[MAX];
//         int k = 0;
//         compacta_quadrante(matriz, 0, 0, altura - 1, largura - 1, largura, &k, vetR, vetG, vetB);
 
//         salvar_arquivo_compactado(argv[3], cabecalho, vetR, vetG, vetB, k);
 
//         for (int i = 0; i < altura; i++)
//             free(matriz[i]);
//         free(matriz);
 
//         printf("Compactação concluída! %d amostras RGB gravadas.\n", k);
 
//     } else if (strcmp(argv[1], "descompacta") == 0) {
//         FILE *cmp = fopen(argv[2], "rb");
//         if (!cmp) {
//             perror("Erro ao abrir arquivo compactado");
//             return 1;
//         }
 
//         unsigned char cabecalho[54];
//         if (fread(cabecalho, sizeof(unsigned char), 54, cmp) != 54) {
//             fprintf(stderr, "Erro ao ler cabeçalho do arquivo compactado.\n");
//             fclose(cmp);
//             return 1;
//         }
 
//         int largura, altura;
//         obter_dimensoes(cabecalho, &largura, &altura);

//         if (largura <= 0 || altura <= 0) {
//             fprintf(stderr, "Erro: Dimensões da imagem inválidas no arquivo compactado (largura: %d, altura: %d).\n", largura, altura);
//             fclose(cmp);
//             return 1;
//         }

//         int padding = calcular_padding(largura);
//         int larguraBytes = largura * 3 + padding;
 
//         unsigned char vetR[MAX], vetG[MAX], vetB[MAX];
//         int total = 0;
//         // Loop de leitura modificado para maior segurança
//         while (total < MAX) {
//             if (fread(&vetR[total], 1, 1, cmp) != 1 ||
//                 fread(&vetG[total], 1, 1, cmp) != 1 ||
//                 fread(&vetB[total], 1, 1, cmp) != 1) {
//                 // Fim do arquivo ou erro de leitura
//                 break; 
//             }
//             total++;
//         }

//         if (total == MAX) {
//             // Verifica se o arquivo ainda contém dados, indicando que MAX é muito pequeno
//             unsigned char temp_check;
//             if (fread(&temp_check, 1, 1, cmp) == 1) {
//                 fprintf(stderr, "Erro: Número de amostras RGB no arquivo excede MAX (%d)\n", MAX);
//                 fclose(cmp);
//                 return 1; // Tratar erro adequadamente
//             }
//         }
//         fclose(cmp);
 
//         unsigned char **matriz = malloc(altura * sizeof(unsigned char *));
//         if (matriz == NULL) {
//             perror("Erro ao alocar memória para linhas da matriz (descompactação)");
//             return 1;
//         }
//         for (int i = 0; i < altura; i++) {
//             matriz[i] = malloc(larguraBytes);
//             if (matriz[i] == NULL) {
//                 perror("Erro ao alocar memória para colunas da matriz (descompactação)");
//                 // Liberar memória já alocada antes de sair
//                 for (int j = 0; j < i; j++) {
//                     free(matriz[j]);
//                 }
//                 free(matriz);
//                 return 1;
//             }
//             memset(matriz[i], 0, larguraBytes);
//         }
 
//         // Atualizar campos do cabeçalho antes de escrever o BMP descompactado
//         int biSizeImage = larguraBytes * altura;
//         int bfSize = 54 + biSizeImage;

//         cabecalho[2] = (unsigned char)(bfSize);
//         cabecalho[3] = (unsigned char)(bfSize >> 8);
//         cabecalho[4] = (unsigned char)(bfSize >> 16);
//         cabecalho[5] = (unsigned char)(bfSize >> 24);

//         cabecalho[10] = 54; // bfOffBits - offset dos dados de pixel
//         cabecalho[11] = 0;
//         cabecalho[12] = 0;
//         cabecalho[13] = 0;

//         cabecalho[34] = (unsigned char)(biSizeImage); // biSizeImage - tamanho dos dados de pixel
//         cabecalho[35] = (unsigned char)(biSizeImage >> 8);
//         cabecalho[36] = (unsigned char)(biSizeImage >> 16);
//         cabecalho[37] = (unsigned char)(biSizeImage >> 24);

//         int k = 0;
//         descompacta_quadrante(matriz, 0, 0, altura - 1, largura - 1, largura, &k, vetR, vetG, vetB, total);
 
//         escrever_imagem_bmp(argv[3], cabecalho, matriz, altura, largura);
 
//         for (int i = 0; i < altura; i++)
//             free(matriz[i]);
//         free(matriz);
 
//         printf("Descompactação concluída! %d regiões preenchidas.\n", k);
//     }
 
//     return 0;
// }


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INICIAL_CAPACIDADE 1024

int calcular_padding(int largura) {
    int linhas_bytes = largura * 3;
    return (4 - (linhas_bytes % 4)) % 4;
}

int ler_cabecalho(FILE *bmp, unsigned char *cabecalho) {
    if (fread(cabecalho, sizeof(unsigned char), 54, bmp) != 54) {
        perror("Erro ao ler cabeçalho BMP");
        return -1;
    }
    int offset = cabecalho[10] + (cabecalho[11] << 8) + (cabecalho[12] << 16) + (cabecalho[13] << 24);
    return offset;
}

void obter_dimensoes(unsigned char *cabecalho, int *largura, int *altura) {
    *largura = cabecalho[18] + (cabecalho[19] << 8) + (cabecalho[20] << 16) + (cabecalho[21] << 24);
    *altura  = cabecalho[22] + (cabecalho[23] << 8) + (cabecalho[24] << 16) + (cabecalho[25] << 24);
}

int ler_matriz(FILE *bmp, unsigned char **matriz, int altura, int largura, int offset) {
    int padding = calcular_padding(largura);
    int larguraBytes = largura * 3 + padding;
    fseek(bmp, offset, SEEK_SET);
    for (int i = altura - 1; i >= 0; i--) {
        if (fread(matriz[i], sizeof(unsigned char), larguraBytes, bmp) != larguraBytes) {
            perror("Erro ao ler linha da imagem BMP");
            return 1;
        }
    }
    return 0;
}

void compacta_quadrante(unsigned char **matriz, int li, int ci, int lf, int cf, int largura, int *k,
                        unsigned char **vetR, unsigned char **vetG, unsigned char **vetB, int *capacidade) {
    if (*k >= *capacidade) {
        *capacidade *= 2;
        *vetR = realloc(*vetR, *capacidade);
        *vetG = realloc(*vetG, *capacidade);
        *vetB = realloc(*vetB, *capacidade);
        if (!*vetR || !*vetG || !*vetB) {
            fprintf(stderr, "Erro de realloc\n");
            exit(1);
        }
    }

    if ((lf - li + 1) <= 3 || (cf - ci + 1) <= 3) {
        int centro_linha = (li + lf) / 2;
        int centro_coluna = (ci + cf) / 2;
        int idx = centro_coluna * 3;
        (*vetR)[*k] = matriz[centro_linha][idx + 2];
        (*vetG)[*k] = matriz[centro_linha][idx + 1];
        (*vetB)[*k] = matriz[centro_linha][idx];
        (*k)++;
        return;
    }

    int meio_linha = (li + lf) / 2;
    int meio_coluna = (ci + cf) / 2;

    compacta_quadrante(matriz, li, ci, meio_linha, meio_coluna, largura, k, vetR, vetG, vetB, capacidade);
    compacta_quadrante(matriz, li, meio_coluna + 1, meio_linha, cf, largura, k, vetR, vetG, vetB, capacidade);
    compacta_quadrante(matriz, meio_linha + 1, ci, lf, meio_coluna, largura, k, vetR, vetG, vetB, capacidade);
    compacta_quadrante(matriz, meio_linha + 1, meio_coluna + 1, lf, cf, largura, k, vetR, vetG, vetB, capacidade);
}

void salvar_arquivo_compactado(const char *nome, unsigned char *cabecalho,
                                unsigned char *vetR, unsigned char *vetG, unsigned char *vetB, int tamanho) {
    FILE *out = fopen(nome, "wb");
    if (!out) { perror("Erro ao abrir arquivo"); return; }
    fwrite(cabecalho, 1, 54, out);
    for (int i = 0; i < tamanho; i++) {
        fwrite(&vetR[i], 1, 1, out);
        fwrite(&vetG[i], 1, 1, out);
        fwrite(&vetB[i], 1, 1, out);
    }
    fclose(out);
}

void descompacta_quadrante(unsigned char **matriz, int li, int ci, int lf, int cf, int largura, int *k,
                           unsigned char *vetR, unsigned char *vetG, unsigned char *vetB, int total) {
    if (*k >= total) return;

    if ((lf - li + 1) <= 3 || (cf - ci + 1) <= 3) {
        for (int i = li; i <= lf; i++) {
            for (int j = ci; j <= cf; j++) {
                int idx = j * 3;
                matriz[i][idx + 2] = vetR[*k];
                matriz[i][idx + 1] = vetG[*k];
                matriz[i][idx]     = vetB[*k];
            }
        }
        (*k)++;
        return;
    }

    int meio_linha = (li + lf) / 2;
    int meio_coluna = (ci + cf) / 2;

    descompacta_quadrante(matriz, li, ci, meio_linha, meio_coluna, largura, k, vetR, vetG, vetB, total);
    descompacta_quadrante(matriz, li, meio_coluna + 1, meio_linha, cf, largura, k, vetR, vetG, vetB, total);
    descompacta_quadrante(matriz, meio_linha + 1, ci, lf, meio_coluna, largura, k, vetR, vetG, vetB, total);
    descompacta_quadrante(matriz, meio_linha + 1, meio_coluna + 1, lf, cf, largura, k, vetR, vetG, vetB, total);
}

void escrever_imagem_bmp(const char *nome, unsigned char *cabecalho, unsigned char **matriz, int altura, int largura) {
    FILE *out = fopen(nome, "wb");
    if (!out) { perror("Erro ao salvar BMP"); return; }
    fwrite(cabecalho, 1, 54, out);
    int padding = calcular_padding(largura);
    int larguraBytes = largura * 3 + padding;
    for (int i = altura - 1; i >= 0; i--) {
        fwrite(matriz[i], 1, larguraBytes, out);
    }
    fclose(out);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Uso:\n  %s compacta imagem.bmp imagem.zmp\n  %s descompacta imagem.zmp imagem.bmp\n", argv[0], argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "compacta") == 0) {
        FILE *bmp = fopen(argv[2], "rb");
        if (!bmp) { perror("Erro ao abrir BMP"); return 1; }

        unsigned char cabecalho[54];
        int offset = ler_cabecalho(bmp, cabecalho);
        int largura, altura;
        obter_dimensoes(cabecalho, &largura, &altura);

        int padding = calcular_padding(largura);
        int larguraBytes = largura * 3 + padding;
        unsigned char **matriz = malloc(altura * sizeof(unsigned char *));
        for (int i = 0; i < altura; i++) {
            matriz[i] = malloc(larguraBytes);
        }
        ler_matriz(bmp, matriz, altura, largura, offset);
        fclose(bmp);

        int k = 0, capacidade = INICIAL_CAPACIDADE;
        unsigned char *vetR = malloc(capacidade);
        unsigned char *vetG = malloc(capacidade);
        unsigned char *vetB = malloc(capacidade);

        compacta_quadrante(matriz, 0, 0, altura - 1, largura - 1, largura, &k, &vetR, &vetG, &vetB, &capacidade);

        salvar_arquivo_compactado(argv[3], cabecalho, vetR, vetG, vetB, k);

        for (int i = 0; i < altura; i++) free(matriz[i]);
        free(matriz); free(vetR); free(vetG); free(vetB);
        printf("Compactação concluída com %d amostras.\n", k);

    } else if (strcmp(argv[1], "descompacta") == 0) {
        FILE *cmp = fopen(argv[2], "rb");
        if (!cmp) { perror("Erro ao abrir arquivo compactado"); return 1; }

        unsigned char cabecalho[54];
        fread(cabecalho, 1, 54, cmp);
        int largura, altura;
        obter_dimensoes(cabecalho, &largura, &altura);

        int capacidade = INICIAL_CAPACIDADE;
        unsigned char *vetR = malloc(capacidade), *vetG = malloc(capacidade), *vetB = malloc(capacidade);
        int total = 0;
        while (!feof(cmp)) {
            if (total >= capacidade) {
                capacidade *= 2;
                vetR = realloc(vetR, capacidade);
                vetG = realloc(vetG, capacidade);
                vetB = realloc(vetB, capacidade);
            }
            if (fread(&vetR[total], 1, 1, cmp) != 1) break;
            if (fread(&vetG[total], 1, 1, cmp) != 1) break;
            if (fread(&vetB[total], 1, 1, cmp) != 1) break;
            total++;
        }
        fclose(cmp);

        int padding = calcular_padding(largura);
        int larguraBytes = largura * 3 + padding;
        unsigned char **matriz = malloc(altura * sizeof(unsigned char *));
        for (int i = 0; i < altura; i++) {
            matriz[i] = malloc(larguraBytes);
            memset(matriz[i], 0, larguraBytes);
        }

        int biSizeImage = larguraBytes * altura;
        int bfSize = 54 + biSizeImage;
        cabecalho[2] = (unsigned char)(bfSize);
        cabecalho[3] = (unsigned char)(bfSize >> 8);
        cabecalho[4] = (unsigned char)(bfSize >> 16);
        cabecalho[5] = (unsigned char)(bfSize >> 24);
        cabecalho[34] = (unsigned char)(biSizeImage);
        cabecalho[35] = (unsigned char)(biSizeImage >> 8);
        cabecalho[36] = (unsigned char)(biSizeImage >> 16);
        cabecalho[37] = (unsigned char)(biSizeImage >> 24);

        int k = 0;
        descompacta_quadrante(matriz, 0, 0, altura - 1, largura - 1, largura, &k, vetR, vetG, vetB, total);

        escrever_imagem_bmp(argv[3], cabecalho, matriz, altura, largura);

        for (int i = 0; i < altura; i++) free(matriz[i]);
        free(matriz); free(vetR); free(vetG); free(vetB);
        printf("Descompactação concluída com %d regiões.\n", k);
    }

    return 0;
}


// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// #pragma pack(1)

// #define LARGURA_MAXIMA 1024
// #define ALTURA_MAXIMA 1024
// #define CABECALHO_TAM 54

// typedef struct {
//     unsigned char azul;
//     unsigned char verde;
//     unsigned char vermelho;
// } Pixel;

// Pixel imagem[ALTURA_MAXIMA][LARGURA_MAXIMA];
// unsigned char vetorR[LARGURA_MAXIMA * ALTURA_MAXIMA];
// unsigned char vetorG[LARGURA_MAXIMA * ALTURA_MAXIMA];
// unsigned char vetorB[LARGURA_MAXIMA * ALTURA_MAXIMA];
// unsigned char cabecalho[CABECALHO_TAM];

// int largura, altura, padding;
// int total_pixels = 0;
// int contador_descompactar = 0;

// void calcular_padding() {
//     padding = (4 - (largura * 3) % 4) % 4;
// }

// void compactar(int ini_linha, int fim_linha, int ini_coluna, int fim_coluna) {
//     int linhas = fim_linha - ini_linha + 1;
//     int colunas = fim_coluna - ini_coluna + 1;

//     if (linhas <= 3 || colunas <= 3) {
//         int meio_linha = (ini_linha + fim_linha) / 2;
//         int meio_coluna = (ini_coluna + fim_coluna) / 2;
//         Pixel p = imagem[meio_linha][meio_coluna];

//         vetorR[total_pixels] = p.vermelho;
//         vetorG[total_pixels] = p.verde;
//         vetorB[total_pixels] = p.azul;
//         total_pixels++;
//         return;
//     }

//     int meio_linha = (ini_linha + fim_linha) / 2;
//     int meio_coluna = (ini_coluna + fim_coluna) / 2;

//     compactar(ini_linha, meio_linha, ini_coluna, meio_coluna);
//     compactar(ini_linha, meio_linha, meio_coluna + 1, fim_coluna);
//     compactar(meio_linha + 1, fim_linha, ini_coluna, meio_coluna);
//     compactar(meio_linha + 1, fim_linha, meio_coluna + 1, fim_coluna);
// }

// void descompactar(int ini_linha, int fim_linha, int ini_coluna, int fim_coluna) {
//     int linhas = fim_linha - ini_linha + 1;
//     int colunas = fim_coluna - ini_coluna + 1;

//     if (linhas <= 3 || colunas <= 3) {
//         Pixel p = {
//             .vermelho = vetorR[contador_descompactar],
//             .verde = vetorG[contador_descompactar],
//             .azul = vetorB[contador_descompactar]
//         };
//         contador_descompactar++;

//         for (int i = ini_linha; i <= fim_linha; i++) {
//             for (int j = ini_coluna; j <= fim_coluna; j++) {
//                 imagem[i][j] = p;
//             }
//         }
//         return;
//     }

//     int meio_linha = (ini_linha + fim_linha) / 2;
//     int meio_coluna = (ini_coluna + fim_coluna) / 2;

//     descompactar(ini_linha, meio_linha, ini_coluna, meio_coluna);
//     descompactar(ini_linha, meio_linha, meio_coluna + 1, fim_coluna);
//     descompactar(meio_linha + 1, fim_linha, ini_coluna, meio_coluna);
//     descompactar(meio_linha + 1, fim_linha, meio_coluna + 1, fim_coluna);
// }

// int ler_imagem(const char *nome) {
//     FILE *entrada = fopen(nome, "rb");
//     if (!entrada) return 0;

//     fread(cabecalho, sizeof(unsigned char), CABECALHO_TAM, entrada);
//     largura = *(int*)&cabecalho[18];
//     altura = *(int*)&cabecalho[22];
//     calcular_padding();

//     for (int i = altura - 1; i >= 0; i--) {
//         for (int j = 0; j < largura; j++) {
//             fread(&imagem[i][j], sizeof(Pixel), 1, entrada);
//         }
//         fseek(entrada, padding, SEEK_CUR);
//     }
//     fclose(entrada);
//     return 1;
// }

// int salvar_imagem(const char *nome) {
//     FILE *saida = fopen(nome, "wb");
//     if (!saida) return 0;

//     fwrite(cabecalho, sizeof(unsigned char), CABECALHO_TAM, saida);
//     for (int i = altura - 1; i >= 0; i--) {
//         for (int j = 0; j < largura; j++) {
//             fwrite(&imagem[i][j], sizeof(Pixel), 1, saida);
//         }
//         for (int p = 0; p < padding; p++) {
//             fputc(0x00, saida);
//         }
//     }
//     fclose(saida);
//     return 1;
// }

// int salvar_compactado(const char *nome) {
//     FILE *arquivo = fopen(nome, "wb");
//     if (!arquivo) return 0;

//     fwrite(cabecalho, sizeof(unsigned char), CABECALHO_TAM, arquivo);
//     for (int i = 0; i < total_pixels; i++) {
//         fputc(vetorR[i], arquivo);
//         fputc(vetorG[i], arquivo);
//         fputc(vetorB[i], arquivo);
//     }
//     fclose(arquivo);
//     return 1;
// }

// int ler_compactado(const char *nome) {
//     FILE *arquivo = fopen(nome, "rb");
//     if (!arquivo) return 0;

//     fread(cabecalho, sizeof(unsigned char), CABECALHO_TAM, arquivo);
//     largura = *(int*)&cabecalho[18];
//     altura = *(int*)&cabecalho[22];
//     calcular_padding();

//     int i = 0;
//     while (fread(&vetorR[i], sizeof(unsigned char), 1, arquivo) == 1 &&
//            fread(&vetorG[i], sizeof(unsigned char), 1, arquivo) == 1 &&
//            fread(&vetorB[i], sizeof(unsigned char), 1, arquivo) == 1) {
//         i++;
//     }
//     fclose(arquivo);
//     return i;
// }

// int main() {
//     if (!ler_imagem("imagemOriginal.bmp")) {
//         printf("Erro ao abrir imagemOriginal.bmp\n");
//         return 1;
//     }

//     compactar(0, altura - 1, 0, largura - 1);

//     if (!salvar_compactado("imagemCompactada.zmp")) {
//         printf("Erro ao criar imagemCompactada.zmp\n");
//         return 1;
//     }

//     int total_lido = ler_compactado("imagemCompactada.zmp");
//     if (total_lido <= 0) {
//         printf("Erro ao abrir imagemCompactada.zmp\n");
//         return 1;
//     }

//     contador_descompactar = 0;
//     descompactar(0, altura - 1, 0, largura - 1);

//     if (!salvar_imagem("imagemDescompactada.bmp")) {
//         printf("Erro ao criar imagemDescompactada.bmp\n");
//         return 1;
//     }

//     printf("\nCompactação e descompactação concluídas!\n");
//     printf("Imagem original: imagemOriginal.bmp\n");
//     printf("Imagem compactada: imagemCompactada.zmp\n");
//     printf("Imagem descompactada: imagemDescompactada.bmp\n");
//     printf("Total de pixels representativos: %d\n", total_pixels);
//     return 0;
// }
