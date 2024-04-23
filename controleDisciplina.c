#include <stdio.h>
#include <string.h>

/* Esse código recebe as notas dos alunos e verifica sua situação (Aprovado ou Reprovado)*/

//Estrutura que armazena os dados do discente
typedef struct {
    long int matricula;
    char nome[40];
    float nota1, nota2, nota3;
    float recuperacao;
    float media;
    char situacao[20];
    
} tDiscente;

//Função que imprime os dados do discente
void ImprimeDiscente(tDiscente aluno) {
    if (aluno.recuperacao < 0.001) {
        printf("\t%-12ld %-20s %-6.1f %-6.1f %-6.1f %-11s %-6.1f %-10s\n",
               aluno.matricula, aluno.nome, aluno.nota1, aluno.nota2, aluno.nota3, "-", aluno.media ,aluno.situacao);
    } else {
        printf("\t%-12ld %-20s %-6.1f %-6.1f %-6.1f %-11.1f %-6.1f %-10s\n",
               aluno.matricula, aluno.nome, aluno.nota1, aluno.nota2, aluno.nota3, aluno.recuperacao, aluno.media ,aluno.situacao);
    }
}

//Função que le os dados do discente
tDiscente * LeDiscente(tDiscente * retorno){
    printf("Matricula: ");
    scanf("%ld", &retorno->matricula);
    getchar();
    printf("Nome: ");
    fgets(retorno->nome, sizeof(retorno->nome), stdin);
    
    if (retorno->nome[strlen(retorno->nome) - 1] == '\n') {
        retorno->nome[strlen(retorno->nome) - 1] = '\0';
    }
    return retorno;
}

//Função que le as notas do discente
tDiscente * LeNotas(tDiscente * retorno){
    printf("\nCadastro das notas - %s\n", retorno->nome);
    printf("Nota1: ");
    scanf("%f", &retorno->nota1);
    printf("Nota2: ");
    scanf("%f", &retorno->nota2);
    printf("Nota3: ");
    scanf("%f", &retorno->nota3);
    
    return retorno;
}

//Função que le a nota da recuperação do discente
tDiscente * LeRecuperacao(tDiscente * retorno){
    printf("Recuperacao: ");
    scanf("%f", &retorno->recuperacao);
    
    return retorno;
}

//Função que calcula a media do discente
float Media(tDiscente aluno) {
    return (aluno.nota1 + aluno.nota2 + aluno.nota3) / 3;
}

//Função que calcula a media final do discente
float MediaFinal(tDiscente aluno) {
    return (aluno.media*6 + aluno.recuperacao*4) / 10;
}

//Função para ler o arquivo

void LeArquivo(char *nomeArquivo) {
    FILE *fp;
    char linha[100];
    
    fp = fopen(nomeArquivo, "r");
    
    if (!fp) {
        printf("Erro ao abrir o arquivo\n");
    }
    
    while(fgets(linha, sizeof(linha), fp) != NULL) {
        printf("%s", linha);
    }
    
    fclose(fp);
}


int main(){

    char disciplina[50];
    int qntAlunos, opc;
    tDiscente alunos[30]; 
    

    puts("----------------------------------------- ");
    printf("Bem-vindo(a) ao Controle de Disciplina\n");
    puts("----------------------------------------- ");
    
    printf("\tO que deseja fazer?\n");
    printf("\t[1] - Visualizar Notas\n");
    printf("\t[2] - Cadastrar Nova Nota\n");

    scanf("%d", &opc);

    if (opc == 1){
        LeArquivo("./relatorioDeControle.txt");
        return 0;
    }else{
        //Leitura do nome da disciplina
        printf("\nDigite nome da disciplina: ");
        fgets(disciplina, sizeof(disciplina), stdin);

        // Limpa o buffer de entrada
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        //Remove o \n do final da string
        if (disciplina[strlen(disciplina) - 1] == '\n') {
                disciplina[strlen(disciplina) - 1] = '\0';
        }
        
        //Leitura da quantidade de alunos
        printf("Digite a quantidade de alunos: ");
        scanf("%d", &qntAlunos );
        
        //Leitura dos dados dos alunos
        for (int i = 0; i < qntAlunos; i++){
            tDiscente alunoTemp; // Declaração de uma variável temporária para armazenar os dados do aluno
        
            puts("\n");
        
            // Leitura dos dados do aluno
            LeDiscente(&alunoTemp);
            
            // Copia os dados lidos para o array de alunos
            alunos[i] = alunoTemp;
        }

        //Leitura das notas dos alunos e calculo da media
        for (int i = 0; i < qntAlunos; i++){
            LeNotas(&alunos[i]);
            
            //Calcula a media do aluno
            alunos[i].media = Media(alunos[i]);
            
            //Verifica se o aluno foi aprovado, reprovado ou ficou em recuperacao
            if (alunos[i].media >= 4.0 && alunos[i].media < 7.0){
                printf("O aluno %s teve media %.2lf e ficou em recuperacao! Digite a nota da recupercao dele: \n", alunos[i].nome, alunos[i].media);
                LeRecuperacao(&alunos[i]);

                //Calcula a media final do aluno
                alunos[i].media = MediaFinal(alunos[i]);
                
                //Verifica se o aluno foi aprovado ou reprovado na recuperação
                if (alunos[i].media >= 5.0){
                    strcpy(alunos[i].situacao, "Aprovado");

                } else {
                    printf("O aluno %s teve media %.2lf e foi reprovado! \n", alunos[i].nome, alunos[i].media);
                    strcpy(alunos[i].situacao, "Reprovado");
                }

                

            //Verifica se o aluno foi reprovado sem poder fazer final
            }else if(alunos[i].media < 4.0){
                printf("O aluno %s teve media %.2lf e foi reprovado! \n", alunos[i].nome, alunos[i].media);
                strcpy(alunos[i].situacao, "Reprovado");
            

            //Verifica se o aluno foi aprovado direto
            }else{
                strcpy(alunos[i].situacao, "Aprovado");
                
            }
        }
        
        //Criando relatorioDeControle.txt de saída

        FILE *fp;
        fp = fopen("./relatorioDeControle.txt", "a");
        if (!fp) {
            printf("Erro na abertura do arquivo.\n");
            return 1;
        } else {
            // Se o arquivo estiver vazio, escreve o cabeçalho
            fseek(fp, 0, SEEK_END);
            if (ftell(fp) == 0) {
                fprintf(fp, "%-12s %-20s %-6s %-6s %-6s %-11s %-6s %-10s\n", "Matricula", "Nome", "Nota1", "Nota2", "Nota3", "Recuperacao", "Media", "Situacao");
            }
        }
        // Loop de iteração dos alunos
        for (int i = 0; i < qntAlunos; i++) {
            // Escreve os dados dos alunos no arquivo
            if (alunos[i].recuperacao < 0.001) {
                fprintf(fp, "%-12ld %-20s %-6.1f %-6.1f %-6.1f %-10s %-10.1f %-6s\n", alunos[i].matricula, alunos[i].nome, alunos[i].nota1, alunos[i].nota2, alunos[i].nota3, "-", alunos[i].media, alunos[i].situacao);
            } else{
                fprintf(fp, "%-12ld %-20s %-6.1f %-6.1f %-6.1f %-10.1f %-10.1f %-6s\n", alunos[i].matricula, alunos[i].nome, alunos[i].nota1, alunos[i].nota2, alunos[i].nota3, alunos[i].recuperacao, alunos[i].media, alunos[i].situacao);
            }
        }

        // Fecha o arquivo somente após todas as operações de escrita serem concluídas
        fclose(fp);
    }



    

    /*
    //Imprime os dados dos alunos

    puts("\n");
    printf("\t%-12s %-20s %-6s %-6s %-6s %-11s %-6s %-10s\n", "Matricula", "Nome", "Nota1", "Nota2", "Nota3", "Recuperacao", "Media", "Situacao");

    printf("\t" "--------------------------------" "-------------------" "------" "------" "------" "-----------" "------" "---------\n");
    
    
    for (int i = 0; i < qntAlunos; i++){
        ImprimeDiscente(alunos[i]);
    }*/

    puts("\n\n\n");
    puts("\t-----------------------------------------------------");
    printf("\tRelatorio de Controle de Disciplina\n");
    puts("\t-----------------------------------------------------");
     puts("\n");

    //Leitura do arquivo criado
    LeArquivo("./relatorioDeControle.txt");
    return 0;
}