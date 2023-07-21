//Aluna: Anna Paula Meneghelli de Oliveira. Disciplina: Linguagem de Programação
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

 typedef struct 
  { int hora;
    int minuto;
  } Hora;

 typedef struct
  { int dia;
    int mes;
    int ano;
  } Data;

typedef struct 
{
  Data data;
  Hora Horario_inicio;
  Hora Horario_fim;
  char local[30];
  char descricao[100];
} Evento;

void Ordena_Eventos (Evento*, int);
int Cadastrar (Evento*, int, struct tm*);
void Mostrar_eventos (Evento*, int);
void Mostrar_5_eventos (Evento*, int, struct tm*);
void Mostrar_eventos_data (Evento*, int);
void Remover (Evento*, int*);

int main(void) {
  Evento *Cadastros, teste;
  Data d;
  Hora h;
  int i, o=1, ne=0, opcao, retorno;
  struct tm *data_hora_atual;
  time_t segundos;
  FILE *arq;
  
  time(&segundos); 
  data_hora_atual = localtime(&segundos); 

  arq = fopen("arq.txt", "rb"); //Abrir o arquivo
	if(arq == NULL) 
		printf("Erro na abertura do arquivo.\n");

  fseek(arq, 0, SEEK_SET);
	while (!feof(arq)) {
    if (ne == 0) {
      ne++;
      Cadastros = malloc(sizeof(Evento)); } //Alocando memória
    else if (ne > 0) {
      ne++;
      Cadastros = realloc(Cadastros, ne*sizeof(Evento)); } //Alocando memória
    
		retorno = fread(&Cadastros[ne - 1], sizeof(Evento), 1, arq); //Lê um struct evento e salva no ponteiro Cadastros
    if(retorno != 1) {
      {ne--;
       Cadastros = realloc(Cadastros, ne* sizeof(Evento));} //Se a leitura não deu certo, realoca a memória previamente alocada
    }
	} 
  Ordena_Eventos(Cadastros, ne);
	fclose(arq); 

  printf("Opções:\n1:Cadastrar novos eventos\n2:Mostrar todos os eventos da agenda\n3:Mostrar os eventos de uma data\n4:Mostrar os próximos 5 eventos\n5:Remover evento\nPara sair: digite 0\n"); //mensagem para o usuário

  while (o == 1) { //loop para as opções do usuário
    printf("\nSelecione uma opção:\n");
    scanf("%d", &opcao);
    switch(opcao) {
    
      case 1: //Cadastrar evento
      ne+=1;
      if (ne == 1) 
      Cadastros = malloc (sizeof(Evento));
      if (ne > 1) 
      Cadastros = realloc(Cadastros, (ne) * sizeof(Evento));
      
      retorno = Cadastrar(Cadastros, ne, data_hora_atual); //Chama a função Cadastrar  
      if (retorno == 0) {
        printf("Cadastro realizado com sucesso.\n");
        Ordena_Eventos(Cadastros, ne);
      }
        
      if (retorno == 1 || retorno == 2 || retorno == 3) { //Verifica se o retorno da função indica um cadastro válido ou inválido
        ne -= 1;
        if (ne == 0)
          free(Cadastros);
        else if (ne > 0)
          Cadastros = realloc(Cadastros, (ne) * sizeof(Evento));
        
        if(retorno == 1)
          printf("Data inválida.\n");
        if (retorno == 2)
          printf("Este dia já passou.\n");
        if (retorno == 3)
          printf("Horário inválido.\n");
      }
      break;
  
    
      case 2: //Mostrar os eventos;
      if (ne == 0)
        printf("Não há eventos cadastrados em sua agenda.\n");
      else if (ne > 0) 
        Mostrar_eventos(Cadastros, ne);
      else if (ne < 0)
        printf("Temos um problema!\n");
      break;
  
    
      case 3: //Mostrar os eventos de uma data;
      if (ne == 0)
        printf("Não há eventos cadastrados em sua agenda.\n");
      else if (ne > 0) 
        Mostrar_eventos_data(Cadastros, ne);
      else if (ne < 0)
        printf("Temos um problema!\n");
      break;
  
    
      case 4: //Mostrar os 5 próximos eventos
      if (ne == 0)
        printf("Não há eventos cadastrados em sua agenda.\n");
      else if (ne > 0) 
        Mostrar_5_eventos(Cadastros, ne, data_hora_atual);
      else if (ne < 0)
        printf("Temos um problema!\n");
      break;
  
    
      case 5: //Remover evento
        if (ne == 0) 
          printf("Não há eventos cadastrados em sua agenda.\n");
        else if (ne > 0) 
          Remover(Cadastros, &ne);
        else if (ne < 0) 
          printf("Temos um problema!\n");
      break;
  
    
      case 0: //Sai do programa e salva o conteúdo apontado por Cadastros num arquivo
      arq = fopen("arq.txt", "wb");
      if(arq == NULL) {
        printf("Erro na abertura do arquivo.\n"); }

      fseek(arq, 0, SEEK_SET);
      retorno = fwrite(Cadastros, sizeof(Evento), ne, arq);  
      if (retorno != ne)
        printf("Erro na gravação do arquivo.\n");
      fclose(arq);

      o = 0;
      break;
      
    
      default: //Opções inválidas
      printf("Opção inválida\n");
      }
  }
  printf("Você fechou sua agenda!\n");
  if (ne > 0) 
    free(Cadastros); //Caso ne=0 a memória já estará liberada
  
  return 0; 
}

/*********************************************************************/
/*********************************************************************/
void Ordena_Eventos (Evento *c, int n) {
  int i, j;
  Evento aux;
  
  for(i = 0; i < n; i++) {
    for(j = 0; j < n-1; j++) {
      if(c[j].data.ano > c[j+1].data.ano) {
          aux = c[j];
          c[j] = c[j+1];
          c[j+1] = aux; 
        }
      else if(c[j].data.ano == c[j+1].data.ano && c[j].data.mes > c[j+1].data.mes) {
          aux = c[j];
          c[j] = c[j+1];
          c[j+1] = aux; 
        }
      else if (c[j].data.ano == c[j+1].data.ano && c[j].data.mes == c[j+1].data.mes && c[j].data.dia > c[j+1].data.dia) {
          aux = c[j];
          c[j] = c[j+1];
          c[j+1] = aux;         
        }
      else if (c[j].data.ano == c[j+1].data.ano && c[j].data.mes == c[j+1].data.mes && c[j].data.dia == c[j+1].data.dia && c[j].Horario_inicio.hora > c[j+1].Horario_inicio.hora) {
          aux = c[j];
          c[j] = c[j+1];
          c[j+1] = aux;         
        }
      }
    }
}

/*********************************************************************/
int Cadastrar (Evento *c, int n, struct tm *dt) {
  int i, j, dia, mes, ano, h_i, m_i, h_f, m_f;
  Evento aux;

    printf("Digite a data do evento no formato dd/mm/aaaa\n");
    scanf("%d/%d/%d", &dia, &mes, &ano);

    if (mes < 0 || mes > 12 || dia < 0) return 1; //Verific os meses válidos
    if (mes == 1 && dia > 31) return 1;
    if (mes == 2 && dia > 28) return 1;
    if (mes == 3 && dia > 31) return 1;
    if (mes == 4 && dia > 30) return 1;
    if (mes == 5 && dia > 31) return 1;
    if (mes == 6 && dia > 30) return 1;
    if (mes == 7 && dia > 31) return 1;
    if (mes == 8 && dia > 31) return 1;
    if (mes == 9 && dia > 30) return 1;
    if (mes == 10 && dia > 31) return 1;
    if (mes == 11 && dia > 30) return 1;
    if (mes == 12 && dia > 31) return 1; //Verifica os dias válidos
  
    if (ano < dt->tm_year+1900) return 2;
  else if (ano == dt->tm_year+1900 && mes < dt->tm_mon+1) return 2;
  else if (ano == dt->tm_year+1900 && mes == dt->tm_mon+1 && dia < dt->tm_mday) return 2; //Verifica se o dia é no futuro

    printf("Digite o horário de início do evento no formato hh:mm\n");
    scanf("%d:%d", &h_i, &m_i);
    if (h_i < 0 || h_i > 23 || m_i < 0 || m_i > 59) return 3; //Verifica os horários válidos
      
    printf("Digite o horário de término do evento no formato hh:mm\n");
    scanf("%d:%d", &h_f, &m_f);
    if (h_f < 0 || h_f > 23 || m_f < 0 || m_f > 59) return 3; //Verifica os horários válidos  
    getchar();

    for(i = 0; i < n; i++) {
      if(c[i].data.ano == ano && c[i].data.mes == mes && c[i].data.dia == dia) {
        if ( ( h_i > c[i].Horario_inicio.hora && h_i < c[i].Horario_fim.hora) || (h_f > c[i].Horario_inicio.hora && h_f < c[i].Horario_fim.hora)) { //Hora maior ou menor
          printf("Já há eventos cadastrados nesta data e horário.\n");
          return 1; }
        else if ( (h_i == c[i].Horario_inicio.hora && (m_i > c[i].Horario_inicio.minuto && m_i <= 59)) || (h_f == c[i].Horario_fim.hora && (m_f >= 0 && m_f < c[i].Horario_fim.minuto))) { //Hora igual
            printf("Já há eventos cadastrados nesta data e horário.\n");
            return 1;        
        }
      }
    } //Verifica se já há eventos cadastrados

    c[n-1].data.dia = dia;
    c[n-1].data.mes = mes;
    c[n-1].data.ano = ano;
    c[n-1].Horario_inicio.hora = h_i;
    c[n-1].Horario_inicio.minuto = m_i;
    c[n-1].Horario_fim.hora = h_f;
    c[n-1].Horario_fim.minuto = m_f; 

    printf("Digite o local do evento\n");
    fgets(c[n-1].local, 30, stdin);
      c[n-1].local[strlen(c[n-1].local) - 1] = '\0';
    printf("Digite a descrição do evento\n");
    fgets(c[n-1].descricao, 100, stdin);
      c[n-1].descricao[strlen(c[n-1].descricao) - 1] = '\0';

    return 0; //Se chegou aqui é porque deu tudo certo
}

/*********************************************************************/
void Mostrar_eventos (Evento *c, int n) {
  int i;
  printf("Sua agenda é:\n");
  for (i=0; i<n; i++) {
    printf("Data: %d/%d/%d\nHorário de início: %d:%d\nHorário de fim: %d:%d\nLocal:%s\nDescrição: %s\n\n", c[i].data.dia, c[i].data.mes, c[i].data.ano, c[i].Horario_inicio.hora, c[i].Horario_inicio.minuto, c[i].Horario_fim.hora, c[i].Horario_fim.minuto, c[i].local, c[i].descricao);
  }
printf("Estes são todos os eventos cadastrados em sua agenda atualemnte.\n");
}

/*********************************************************************/
void Mostrar_eventos_data (Evento *c, int n) {
  int i, a=0;
  Data d;

  printf("Digite a data que deseja visualizar no formato dd/mm/aaaa\n");
  scanf("%d/%d/%d", &d.dia, &d.mes, &d.ano);

  for(i = 0; i < n; i++) {
    
    if(c[i].data.ano == d.ano && c[i].data.mes == d.mes && c[i].data.dia == d.dia ) {
    printf("Data: %d/%d/%d\nHorário de início: %d:%d\nHorário de fim: %d:%d\nLocal: %s\nDescrição: %s\n\n", c[i].data.dia, c[i].data.mes, c[i].data.ano, c[i].Horario_inicio.hora, c[i].Horario_inicio.minuto, c[i].Horario_fim.hora, c[i].Horario_fim.minuto, c[i].local, c[i].descricao);
    a++;
    }
  }
  if(a == 0)
    printf("Não há nenhum evento nesta data.\n");
  else if (a > 0)
  printf("Estes são todos os eventos desta data.\n");
}

/*********************************************************************/
void Mostrar_5_eventos (Evento *c, int n, struct tm *dt) {
  int i, a, b=0;

  for (i = 0; i < n; i++) {
    if ((c[i].data.ano < dt->tm_year+1900) || (c[i].data.ano == dt->tm_year+1900 && c[i].data.mes < dt->tm_mon+1) || (c[i].data.ano == dt->tm_year+1900 && c[i].data.mes == dt->tm_mon+1 && c[i].data.dia < dt->tm_mday)) 
    b++; //Verifica a quantidade de eventos da agenda que são anteriores ao dia atual
  }
  if (n - b > 5) //Verifica se há mais de 5 eventos na agenda
    a = 5 + b;
  else if (n - b <= 5) //Verifica se há menos de 5 eventos na agenda
    a = n;
  
  for (i = b; i < a; i++) {
    printf("Data: %d/%d/%d\nHorário de início: %d:%d\nHorário de fim: %d:%d\nLocal: %s\nDescrição: %s\n\n", c[i].data.dia, c[i].data.mes, c[i].data.ano, c[i].Horario_inicio.hora, c[i].Horario_inicio.minuto, c[i].Horario_fim.hora, c[i].Horario_fim.minuto, c[i].local, c[i].descricao);
    }
  printf("Estes são os próximos %d eventos a partir de %d/%d/%d.\n", a - b, dt->tm_mday, dt->tm_mon+1, dt->tm_year+1900);
}

/*********************************************************************/
void Remover (Evento *c, int *n) {
  int i, j, a = 0;
  Data d;
  Hora h;

  printf("Digite a data do evento que deseja exlcuir no formato dd/mm/aaaa\n");
  scanf("%d/%d/%d", &d.dia, &d.mes, &d.ano);
  printf("Digite o horário de início do evento que deseja exlcuir no formato hh:mm\n");
  scanf("%d:%d", &h.hora, &h.minuto);
  
  for(i = 0; i <= *n; i++) {
    
    if((c[i].data.ano == d.ano && c[i].data.mes == d.mes && c[i].data.dia == d.dia ) && (c[i].Horario_inicio.hora == h.hora && c[i].Horario_inicio.minuto == h.minuto)) { //Compara a data digitada pelo usuário com as presentes na agenda
      a++; //Indica que uma data foi encontrada
      for(j = i; j < *n - 1; j++){
         c[j] = c[j + 1]; } //Sobrepõe a data encontrada
      
      if (*n == 1) {
        *n -= 1;
        free(c);
        printf("Evento excluído com sucesso.\nA agenda está vazia.\n");
        }
      if (*n > 1) {
        *n -= 1;
        c = realloc(c, (*n) * sizeof(Evento));  
        printf("Evento excluído com sucesso.\n");
        }
    }
  }
  if(a==0) //Indica que nenhuma data foi encontrada
    printf("Evento não encontrado, cheque sua agenda novamente.\n");
}