#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>

#define NUM_CARACTERES 92
#define CARACTERES " .,!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
#define MAX_USUARIOS 5

typedef struct {
    char login[50];
    char senha[50];
} Usuario;

int indiceCaractere(char c) {
    for (int i = 0; i < NUM_CARACTERES; i++) {
        if (CARACTERES[i] == c) {
            return i;
        }
    }
    return -1;
}

char caracterePorIndice(int indice) {
    return CARACTERES[indice % NUM_CARACTERES];
}

void cifraVigenere(char *texto, char *chave) {
    int n = strlen(texto);
    int m = strlen(chave);
    
    for (int i = 0, j = 0; i < n; i++) {
        int indiceTexto = indiceCaractere(texto[i]);
        if (indiceTexto != -1) {
            int indiceChave = indiceCaractere(chave[j % m]);
            if (indiceChave != -1) {
                int deslocamento = indiceChave;
                texto[i] = caracterePorIndice(indiceTexto + deslocamento);
                j++;
            }
        }
    }
}

void decifraVigenere(char *texto, char *chave) {
    int n = strlen(texto);
    int m = strlen(chave);
    
    for (int i = 0, j = 0; i < n; i++) {
        int indiceTexto = indiceCaractere(texto[i]);
        if (indiceTexto != -1) {
            int indiceChave = indiceCaractere(chave[j % m]);
            if (indiceChave != -1) {
                int deslocamento = indiceChave;
                texto[i] = caracterePorIndice(indiceTexto - deslocamento + NUM_CARACTERES);
                j++;
            }
        }
    }
}

int incluirUsuario(Usuario usuarios[], int *numUsuarios, char *login, char *senha, char *chave) {
    if (*numUsuarios >= MAX_USUARIOS) {
        printf("Limite de usuários atingido.\n");
        return 0;
    }
    
    for (int i = 0; i < *numUsuarios; i++) {
        if (strcmp(usuarios[i].login, login) == 0) {
            printf("Erro: login já existe.\n");
            return 0;
        }
    }

    strcpy(usuarios[*numUsuarios].login, login);
    strcpy(usuarios[*numUsuarios].senha, senha);
    
    cifraVigenere(usuarios[*numUsuarios].senha, chave);
    (*numUsuarios)++;
    printf("Usuário cadastrado com sucesso!\n");
    return 1;
}

int alterarUsuario(Usuario usuarios[], int numUsuarios, char *login, char *novoLogin, char *novaSenha, char *chave) {
    for (int i = 0; i < numUsuarios; i++) {
        if (strcmp(usuarios[i].login, login) == 0) {
            if (novoLogin != NULL) {
                strcpy(usuarios[i].login, novoLogin);
            }

            if (novaSenha != NULL) {
                strcpy(usuarios[i].senha, novaSenha);
                cifraVigenere(usuarios[i].senha, chave);
            }

            printf("Usuário alterado com sucesso!\n");
            return 1;
        }
    }
    printf("Usuário não encontrado.\n");
    return 0;
}

int excluirUsuario(Usuario usuarios[], int *numUsuarios, char *login) {
    for (int i = 0; i < *numUsuarios; i++) {
        if (strcmp(usuarios[i].login, login) == 0) {
            for (int j = i; j < *numUsuarios - 1; j++) {
                usuarios[j] = usuarios[j + 1];
            }
            (*numUsuarios)--;
            printf("Usuário excluído com sucesso!\n");
            return 1;
        }
    }
    printf("Usuário não encontrado.\n");
    return 0;
}

void listarUsuarios(Usuario usuarios[], int numUsuarios) {
    FILE *lista;
    lista = fopen("LISTA_USUARIOS_CIFRADO.txt", "w");
    if (numUsuarios == 0) {
        printf("Não há usuários cadastrados.\n");
        return;
    }
    
    printf("\nUsuários cadastrados (senhas criptografadas):\n");
    for (int i = 0; i < numUsuarios; i++) {
        fprintf(lista, "Login: %s\nSenha Criptografada: %s\n", usuarios[i].login, usuarios[i].senha);
        printf("Login: %s\nSenha Criptografada: %s\n", usuarios[i].login, usuarios[i].senha);
    }
    fclose(lista);
}

void listarUsuariosSenha(Usuario usuarios[], int numUsuarios, char *chave) {
    FILE *listaE;
    listaE = fopen("LISTA_USUARIOS_DECIFRADO.txt", "w");

    if (numUsuarios == 0) {
        printf("Não há usuários cadastrados.\n");
        return;
    }
    
    printf("\nUsuários cadastrados (senhas decifradas):\n");
    for (int i = 0; i < numUsuarios; i++) {
        char senhaDecifrada[50];
        strcpy(senhaDecifrada, usuarios[i].senha);
        
        decifraVigenere(senhaDecifrada, chave);
        
        fprintf(listaE, "Login: %s\nSenha Decifrada: %s\n", usuarios[i].login, senhaDecifrada);
        printf("Login: %s\nSenha Decifrada: %s\n", usuarios[i].login, senhaDecifrada);
    }
    fclose(listaE);
}

int main() {
    setlocale(LC_ALL,"portuguese_brazil");
    system("cls");
    Usuario usuarios[MAX_USUARIOS] = {
        {"felipinho", "!@#ohnipilef"},
        {"gordox", "coxinha@123"},
        {"paoassado", "senha123"},
        {"maria.souza", "M@x1975"},
        {"lindomar", "n0k14@560"}
    };
    
    char chave[] = "UNICESUMAR";  
    int numUsuarios = 5;

    for (int i = 0; i < numUsuarios; i++) {
        cifraVigenere(usuarios[i].senha, chave);
    }

    int opcao;
    char login[50], senha[50], novoLogin[50], novaSenha[50];

    do {
        printf("\n1. Listar Usuários (senhas criptografadas)\n2. Listar Usuários (senhas descriptografadas)\n3. Incluir Usuário\n4. Alterar Usuário\n5. Excluir Usuário\n6. Sair\nEscolha uma opção: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                listarUsuarios(usuarios, numUsuarios);
                break;

            case 2:
                listarUsuariosSenha(usuarios, numUsuarios, chave);
                break;
            case 3:
                printf("Digite o login do novo usuário: ");
                fgets(login, sizeof(login), stdin);
                login[strcspn(login, "\n")] = 0;

                printf("Digite a senha do novo usuário: ");
                fgets(senha, sizeof(senha), stdin);
                senha[strcspn(senha, "\n")] = 0;

                incluirUsuario(usuarios, &numUsuarios, login, senha, chave);
                break;

            case 4:
                printf("Digite o login do usuário a ser alterado: ");
                fgets(login, sizeof(login), stdin);
                login[strcspn(login, "\n")] = 0;

                printf("Digite o novo login (deixe vazio para não alterar): ");
                fgets(novoLogin, sizeof(novoLogin), stdin);
                novoLogin[strcspn(novoLogin, "\n")] = 0;

                printf("Digite a nova senha (deixe vazio para não alterar): ");
                fgets(novaSenha, sizeof(novaSenha), stdin);
                novaSenha[strcspn(novaSenha, "\n")] = 0;

                alterarUsuario(usuarios, numUsuarios, login, 
                               (strlen(novoLogin) > 0 ? novoLogin : NULL),
                               (strlen(novaSenha) > 0 ? novaSenha : NULL),
                               chave);
                break;

            case 5:
                printf("Digite o login do usuário a ser excluído: ");
                fgets(login, sizeof(login), stdin);
                login[strcspn(login, "\n")] = 0;
                excluirUsuario(usuarios, &numUsuarios, login);
                break;

            case 6:
                printf("Saindo...\n");
                break;

            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    } while (opcao != 6);

    return 0;
}
