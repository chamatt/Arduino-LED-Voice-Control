//Comodos
int corredor1[4] = {30, 28, 26, 4};
int corredor2[4] = {31, 29, 27};
int quarto[4] = {36, 34, 32};
int cozinha[4] = {37, 35, 33};
int sala[4] = {38, 40, 42};
int quartofernando[4] = {44, 46, 48};
int banheirofernando[4] = {39, 41, 43};
int lavabo[4] = {45, 47, 49};

typedef struct{
  int pino;
  byte estadoatual;
  byte estadoanterior;
} tBotao;

typedef struct{
  tBotao sala;
  tBotao quarto;
  tBotao corredor;
  tBotao cozinha;
  tBotao quartofernando;
  tBotao banheirofernando;
  tBotao lavabo;
}tBotoes;
tBotoes botoes;




String voice[5];

void InicializaBotoes(tBotoes *botoes);
void Ligar(String* voice, int count);
void Desligar(String* voice, int count);
boolean ComodoTemDuasPalavras(String* voice, int count);
char ObterCor(String* voice, boolean check);
int ObterPinos(short int* RGB, short int* PWM, String lugar);
void LigarCor(short int* RGB, char cor, short int tam);
void LigaIntensidade(short int* PWM, String* voice, boolean check, int count, short int tam);
void ChecarBotoes(tBotoes *botoes);

void setup() 
{
    //Seta o serial
    Serial.begin(9600);
    
    //Seta os botoes
    InicializaBotoes(&botoes);
    pinMode(botoes.sala.pino, INPUT);
    pinMode(botoes.quarto.pino, INPUT);
    pinMode(botoes.corredor.pino, INPUT);
    pinMode(botoes.sala.pino, INPUT);
    pinMode(botoes.cozinha.pino, INPUT);
    pinMode(botoes.quartofernando.pino, INPUT);
    pinMode(botoes.banheirofernando.pino, INPUT);
    pinMode(botoes.lavabo.pino, INPUT);
    
    //Seta os RGB   
    for(int i = 26; i <= 49; i++)
    {
        pinMode(i, OUTPUT);
    }

    //Seta os PWM
    for(int i = 4; i <= 11; i++)
    {
        pinMode(i, OUTPUT);
    }    
}
 
void loop() 
{
  int count = 0;
  while ( Serial.available() )     //Checa se existe algo pra ler
  {   
      delay(10);                   //Delay pra não dar merda
      char c = Serial.read();      //Faz a leitura serial
      if (c == '#')                //Para a leitura se encontrar o char '#'
        break;                     
      if(c == ' ')
      {
          voice[count].toLowerCase();      //Transforma a palavra em minuscula para facilitar as comparaçoes posteriores;
          count++;                  //Se caractere lido for separação de palavras, aumenta o contador de palavras.
      }
      else
      {
          voice[count] += c;       // Se não for nada disso, adiciona o char lido a string da palavra.
      }
  }
  /*    //testing
    voice[0]="ligar";
    voice[1]="sala"; 
    voice[2]="azul"; 
    voice[3]=""; 
    voice[4]="";
    count = 3;
  */
  if (voice[1].length() > 0) 
  {
    Serial.println(voice[1]);
    if(voice[0] == "*ligar" && count>=3) 
    {
      Ligar(voice, count); //Liga o(s) LED de acordo com os parametros passado por voz (comodo, cor, intensidade)
    }
    if(voice[0] == "*desligar" && count==2)
    {
      Desligar(voice, count); //Liga o LED respectivo ao comodo.
    }
    
    voice[0]="";            //Reseta Variavel
    voice[1]=""; 
    voice[2]=""; 
    voice[3]=""; 
    voice[4]="";
  }
  ChecarBotoes(&botoes);
  
}
void Ligar(String* voice, int count)
{
    boolean check = ComodoTemDuasPalavras(voice, count); //Checa se o nome do comodo tem 2 palavras
    String lugar;
    short int RGB[42];
    short int PWM[14]; 
    char cor;
    if(check == true)
        lugar = voice[1] + " " + voice[2]; //Concatena as duas palavras do nome do comodo
    else
        lugar = voice[3];
    cor = ObterCor(voice, check); // Obter um char que representa a cor, podendo ser R, G,B, C, M ou Y.
    if(cor == 'Z')
        return;
    short int tam;
    tam = ObterPinos(RGB, PWM, lugar); // Obtem os pinos referentes ao comodo.
    LigarCor(RGB, cor, tam); // Funcao que liga os pinos dependendo da cor que se deseja obter do LED RGB.
    LigaIntensidade(PWM, voice, check, count, tam);
}

void Desligar(String* voice, int count)
{
    boolean check = ComodoTemDuasPalavras(voice, count);
    String lugar;
    short int RGB[42];
    short int tam;
    short int PWM[14];
    if(check == true)
        lugar = voice[1] + " " + voice[2]; 
    else
        lugar = voice[3];
    tam = ObterPinos(RGB, PWM, lugar);
    short int i;
    for(i = 0; i < tam; i+=3){ //Desliga todos os LED dos pinos recebidos.
      digitalWrite(RGB[i], HIGH);
      digitalWrite(RGB[i+1], HIGH);
      digitalWrite(RGB[i+2], HIGH);
   }
}

boolean ComodoTemDuasPalavras(String* voice, int count)
{
   if(count < 4)
      return false; 
   else if( count == 4 && (voice[3] == "forte" || voice[3] == "fraco") )
      return false;
   else
      return true;
}
char ObterCor(String* voice, boolean check)
{
    int pos;
    if( check )
        pos = 3;
    else if( !check )
        pos = 2;
    if(voice[pos] == "branco")
        return 'W';
    if(voice[pos] == "vermelho")
        return 'R';
    if(voice[pos] == "verde")
        return 'G';
    if(voice[pos] == "azul")
        return 'B';
    if(voice[pos] == "ciano")
        return 'C';
    if(voice[pos] == "roxo")
        return 'M';
    if(voice[pos] == "amarelo")
        return 'Y';
    else
        return 'Z';
}


int ObterPinos(short int* RGB, short int* PWM, String lugar)
{
  if(lugar == "corredor"){
    RGB[0] = corredor1[0]; 
    RGB[1] = corredor1[1];
    RGB[2] = corredor1[2];
    RGB[3] = corredor2[0]; 
    RGB[4] = corredor2[1];
    RGB[5] = corredor2[2];
    PWM[0] = corredor1[3];
    PWM[1] = corredor2[3];
    return 6;
  }
  else if(lugar == "lavabo"){
    RGB[0] = lavabo[0]; 
    RGB[1] = lavabo[1];
    RGB[2] = lavabo[2];
    PWM[0] = lavabo[3];
    return 3;
  }
  else if(lugar == "cozinha"){
    RGB[0] = cozinha[0]; 
    RGB[1] = cozinha[1];
    RGB[2] = cozinha[2];
    PWM[0] = cozinha[3];
    return 3;
  }
  else if(lugar == "sala"){
    RGB[0] = sala[0]; 
    RGB[1] = sala[1];
    RGB[2] = sala[2];
    PWM[0] = sala[3];
    return 3;
  }
  else if(lugar == "banheiro fernando"){
    RGB[0] = banheirofernando[0]; 
    RGB[1] = banheirofernando[1];
    RGB[2] = banheirofernando[2];
    PWM[0] = banheirofernando[3];
    return 3;
  }
  else if(lugar == "quarto fernando"){
    RGB[0] = quartofernando[0]; 
    RGB[1] = quartofernando[1];
    RGB[2] = quartofernando[2];
    PWM[0] = quartofernando[3];
    return 3;
  }
  else if(lugar == "quarto"){
    RGB[0] = quarto[0]; 
    RGB[1] = quarto[1];
    RGB[2] = quarto[2];
    PWM[0] = quarto[3];
    return 3;
  }
   else if(lugar == "todos"){
    RGB[3] = corredor1[0]; 
    RGB[4] = corredor1[1];
    RGB[5] = corredor1[2];
    RGB[6] = corredor2[0]; 
    RGB[7] = corredor2[1];
    RGB[8] = corredor2[2];
    RGB[9] = lavabo[0]; 
    RGB[10] = lavabo[1];
    RGB[11] = lavabo[2];
    RGB[12] = cozinha[0]; 
    RGB[13] = cozinha[1];
    RGB[14] = cozinha[2];
    RGB[15] = sala[0]; 
    RGB[16] = sala[1];
    RGB[17] = sala[2];
    RGB[18] = banheirofernando[0]; 
    RGB[19] = banheirofernando[1];
    RGB[20] = banheirofernando[2];
    RGB[21] = quartofernando[0]; 
    RGB[22] = quartofernando[1];
    RGB[23] = quartofernando[2];
    RGB[24] = quarto[0]; 
    RGB[25] = quarto[1];
    RGB[26] = quarto[2];

    PWM[0] = corredor1[0]; 
    PWM[1] = corredor2[0]; 
    PWM[2] = lavabo[0]; 
    PWM[3] = cozinha[0]; 
    PWM[4] = sala[0]; 
    PWM[5] = banheirofernando[0];
    PWM[6] = quartofernando[0];
    PWM[7] = quarto[0];
    return 24;
  }
}

void LigarCor(short int* RGB, char cor, short int tam)
{
  short int i;
  if(cor == 'R'){
    for(i = 0; i < tam; i+=3){
      digitalWrite(RGB[i], LOW);
      digitalWrite(RGB[i+1], HIGH);
      digitalWrite(RGB[i+2], HIGH);
    }
    
  }
  else if(cor == 'G'){
    for(i = 0; i < tam; i+=3 ){
      digitalWrite(RGB[i], HIGH);
      digitalWrite(RGB[i+1], LOW);
      digitalWrite(RGB[i+2], HIGH);
    }
  }
  else if(cor == 'B'){
    for(i = 0; i < tam; i+=3 ){
      digitalWrite(RGB[i], HIGH);
      digitalWrite(RGB[i+1], HIGH);
      digitalWrite(RGB[i+2], LOW);
    }
    
  }
  else if(cor == 'C'){
    for(i = 0; i < tam; i+=3 ){
      digitalWrite(RGB[i], HIGH);
      digitalWrite(RGB[i+1], LOW);
      digitalWrite(RGB[i+2], LOW);
    }
  }
  else if(cor == 'M'){
    for(i = 0; i < tam; i+=3 ){
      digitalWrite(RGB[i], LOW);
      digitalWrite(RGB[i+1], HIGH);
      digitalWrite(RGB[i+2], LOW);
      }
  }
  else if(cor == 'Y'){
    for(i = 0; i < tam; i+=3 ){
      digitalWrite(RGB[i], LOW);
      digitalWrite(RGB[i+1], LOW);
      digitalWrite(RGB[i+2], HIGH);
    }
  }
  else if(cor == 'W'){
    for(i = 0; i < tam; i+=3 ){
      digitalWrite(RGB[i], LOW);
      digitalWrite(RGB[i+1], LOW);
      digitalWrite(RGB[i+2], LOW);
    }
  }
}
void LigaIntensidade(short int* PWM, String* voice, boolean check, int count, short int tam)
{
    short int i;
    if( (check && count == 4) || (!check && count == 3) ) // intensidade não especificada -> forte
    { 
        for(i = 0; i < tam/3; i++){
            analogWrite(PWM[i], 255);
        }
    }
    else if(count == 4 || count == 5 )                   // intensidade especificada -> forte
    {
      if(voice[count-1] == "forte")
      {
        for(i = 0; i < tam/3; i++){
            analogWrite(PWM[i], 255);
        }
      }
      else if(voice[count-1] == "fraco")
      {
        for(i = 0; i < tam/3; i++)
        {
            analogWrite(PWM[i], 127);
        }
      }
      else if(voice[count-1] == "escuro")
      {
        for(i = 0; i < tam/3; i++)
        {
            analogWrite(PWM[i], 80);
        }
      }
    }
}

void InicializaBotoes(tBotoes *botoes)
{
    botoes->corredor.pino = 22;
    botoes->corredor.estadoanterior = 0;
    botoes->quarto.pino = 23;
    botoes->quarto.estadoanterior = 0;
    botoes->sala.pino = 24;
    botoes->sala.estadoanterior = 0;
    botoes->cozinha.pino = 25;
    botoes->cozinha.estadoanterior = 0;
    botoes->quartofernando.pino = 50;
    botoes->quartofernando.estadoanterior = 0;
    botoes->banheirofernando.pino = 51;
    botoes->banheirofernando.estadoanterior = 0;
    botoes->lavabo.pino = 52;
    botoes->lavabo.estadoanterior = 0;
}


void ChecarBotoes(tBotoes *botoes)
{
  botoes->quarto.estadoatual = digitalRead(botoes->quarto.pino);
  botoes->corredor.estadoatual = digitalRead(botoes->corredor.pino);
  botoes->sala.estadoatual = digitalRead(botoes->sala.pino);
  botoes->cozinha.estadoatual = digitalRead(botoes->cozinha.pino);
  botoes->quartofernando.estadoatual = digitalRead(botoes->quartofernando.pino);
  botoes->banheirofernando.estadoatual = digitalRead(botoes->banheirofernando.pino);
  botoes->lavabo.estadoatual = digitalRead(botoes->lavabo.pino);
  
  if(  botoes->quarto.estadoatual && ! botoes->quarto.estadoanterior )
  {
    if( digitalRead(quarto[0]) || digitalRead(quarto[1]) || digitalRead(quarto[2]) )
      {
        digitalWrite(quarto[0], LOW);
        digitalWrite(quarto[1], LOW);
        digitalWrite(quarto[2], LOW);
      }
      botoes->quarto.estadoanterior = botoes->quarto.estadoatual;
      delay(50);
  }
  else if( botoes->corredor.estadoatual && !botoes->corredor.estadoatual )
  {
      if( digitalRead(corredor1[0]) || digitalRead(corredor1[1]) || digitalRead(corredor1[2])
                || digitalRead(corredor2[0]) || digitalRead(corredor2[1]) || digitalRead(corredor2[2]) )
      {
        digitalWrite(corredor1[0], HIGH);
        digitalWrite(corredor1[1], HIGH);
        digitalWrite(corredor1[2], HIGH);
        digitalWrite(corredor2[0], HIGH);
        digitalWrite(corredor2[1], HIGH);
        digitalWrite(corredor2[2], HIGH);
      }
      else
      {
        digitalWrite(corredor1[0], LOW);
        digitalWrite(corredor1[1], LOW);
        digitalWrite(corredor1[2], LOW);
        digitalWrite(corredor2[0], LOW);
        digitalWrite(corredor2[1], LOW);
        digitalWrite(corredor2[2], LOW);
      }
      botoes->corredor.estadoanterior = botoes->corredor.estadoatual;
      delay(50);
      
  }
  else if( botoes->sala.estadoatual && !botoes->sala.estadoanterior )
  {
      if( digitalRead(sala[0]) || digitalRead(sala[1]) || digitalRead(sala[2]) )
      {
        digitalWrite(sala[0], HIGH);
        digitalWrite(sala[1], HIGH);
        digitalWrite(sala[2], HIGH);
      }
      else
      {
        digitalWrite(sala[0], LOW);
        digitalWrite(sala[1], LOW);
        digitalWrite(sala[2], LOW);
      }
      botoes->sala.estadoanterior = botoes->sala.estadoatual;
      delay(50);
  }
  else if( botoes->cozinha.estadoatual && !botoes->cozinha.estadoanterior )
  {
      if( digitalRead(cozinha[0]) || digitalRead(cozinha[1]) || digitalRead(cozinha[2]) )
      {
        digitalWrite(cozinha[0], HIGH);
        digitalWrite(cozinha[1], HIGH);
        digitalWrite(cozinha[2], HIGH);
      }
      else
      {
        digitalWrite(cozinha[0], LOW);
        digitalWrite(cozinha[1], LOW);
        digitalWrite(cozinha[2], LOW);
      }
      botoes->cozinha.estadoanterior = botoes->cozinha.estadoatual;
      delay(50);
  }
  else if( botoes->quartofernando.estadoatual && !botoes->quartofernando.estadoanterior )
  {
      if( digitalRead(quartofernando[0]) || digitalRead(quartofernando[1]) || digitalRead(quartofernando[2]) )
      {
        digitalWrite(quartofernando[0], HIGH);
        digitalWrite(quartofernando[1], HIGH);
        digitalWrite(quartofernando[2], HIGH);
      }
      else
      {
        digitalWrite(quartofernando[0], LOW);
        digitalWrite(quartofernando[1], LOW);
        digitalWrite(quartofernando[2], LOW);
      }
      botoes->quartofernando.estadoanterior = botoes->quartofernando.estadoatual;
      delay(50);
  }
  else if( botoes->banheirofernando.estadoatual && !botoes->banheirofernando.estadoanterior )
  {
      if( digitalRead(banheirofernando[0]) || digitalRead(banheirofernando[1]) || digitalRead(banheirofernando[2]) )
      {
        digitalWrite(banheirofernando[0], HIGH);
        digitalWrite(banheirofernando[1], HIGH);
        digitalWrite(banheirofernando[2], HIGH);
      }
      else
      {
        digitalWrite(banheirofernando[0], LOW);
        digitalWrite(banheirofernando[1], LOW);
        digitalWrite(banheirofernando[2], LOW);
      }
      botoes->banheirofernando.estadoanterior = botoes->banheirofernando.estadoatual;
      delay(50);
  }
  else if( botoes->lavabo.estadoatual && !botoes->lavabo.estadoanterior )
  {
      if( digitalRead(lavabo[0]) || digitalRead(lavabo[1]) || digitalRead(lavabo[2]) )
      {
        digitalWrite(lavabo[0], HIGH);
        digitalWrite(lavabo[1], HIGH);
        digitalWrite(lavabo[2], HIGH);
      }
      else
      {
        digitalWrite(lavabo[0], LOW);
        digitalWrite(lavabo[1], LOW);
        digitalWrite(lavabo[2], LOW);
      }
      botoes->lavabo.estadoanterior = botoes->lavabo.estadoatual;
      delay(50);
  }  
}
