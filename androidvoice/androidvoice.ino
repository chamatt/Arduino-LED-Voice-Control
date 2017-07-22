
#include <SoftwareSerial.h>
//SoftwareSerial BTserial(2, 3); // RX | TX
// Connect the HC-06 TX to the Arduino RX on pin 2. 
// Connect the HC-06 RX to the Arduino TX on pin 3 through a voltage divider.
typedef struct{
  int sala[3] = {/*pinos R G B aqui*/};
  int quarto[3] = {/*pinos R G B aqui*/};
  int corredor1[3] = {/*pinos R G B aqui*/};
  int corredor2 [3] = {/*pinos R G B aqui*/};
  int suite1[3] = {/*pinos R G B aqui*/};
  int banheiro1[3] = {/*pinos R G B aqui*/};
  int suite2[3] = {/*pinos R G B aqui*/};
  int banheiro2[3] = {/*pinos R G B aqui*/};
  int lavabo[3] = {/*pinos R G B aqui*/};
}tComodos;

tComodos comodos;
String voice[5];
void Ligar(String* voice, int count);
void Desligar(String* voice, int count);
boolean ComodoTemDuasPalavras(String* voice, int count);
char ObterCor(String* voice, boolean check);
int ObterPinos(int* RGB, String lugar);
void LigarCor(int* RGB, int cor);

void setup() 
{
    Serial.begin(9600);
    Serial.println("Enter AT commands:");
    for(int i = 1; i < 13; i++)
    {
        pinMode(i, OUTPUT);
    }
    
}
 
void loop() 
{
  int count = 0;
  /*
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
  }  */
  //testing
    voice[1]="ligar";            //Reseta Variavel
    voice[2]="sala"; 
    voice[3]="azul"; 
    voice[4]=""; 
    voice[5]="";
    count = 3;
  //testing


  
  if (voice[1].length() > 0) 
  {
    Serial.println(voice[1]);
    if(voice[1] == "*ligar" && count>=3) 
    {
      Ligar(voice, count);
    }
    if(voice[1] == "*desligar" && count==2)
    {
      Desligar(voice, count);
    }
    
    voice[1]="";            //Reseta Variavel
    voice[2]=""; 
    voice[3]=""; 
    voice[4]=""; 
    voice[5]="";
  }
}
void Ligar(String* voice, int count)
{
    boolean check = ComodoTemDuasPalavras(voice, count);
    String lugar;
    short int RGB[42];
    char cor;
    if(check == true)
        lugar = voice[2] + " " + voice[3]; 
    else
        lugar = voice[4];
    cor = ObterCor(voice, check);
    if(cor == 'Z')
        return;
    ObterPinos(RGB, lugar);
    LigarCor(RGB, cor);
}

void Desligar(String* voice, int count)
{
    boolean check = ComodoTemDuasPalavras(voice, count);
    String lugar;
    short int RGB[42];
    if(check == true)
        lugar = voice[2] + " " + voice[3]; 
    else
        lugar = voice[4];
    ObterPinos(RGB, lugar);
    short int tam = sizeof(RGB)/sizeof(short int);
    short int i;
    for(i = 1; i <= tam; i+=3 ){
      digitalWrite(RGB[i], LOW);
      digitalWrite(RGB[i+1], LOW);
      digitalWrite(RGB[i+2], LOW);
   }

}

boolean ComodoTemDuasPalavras(String* voice, int count)
{
   if(count < 4)
      return false; 
   else if( count == 4 && (voice[4] == "forte" || voice[4] == "fraco") )
      return false;
   else
      return true;
}
char ObterCor(String* voice, boolean check)
{
    int pos;
    if( check )
        pos = 4;
    else if( !check )
        pos = 3;
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


int ObterPinos(short int* RGB, String lugar)
{
  if(lugar == "sala"){
    RGB[1] = comodos.sala[1]; 
    RGB[2] = comodos.sala[2];
    RGB[3] = comodos.sala[3];
  }
  else if(lugar == "corredor"){
    RGB[1] = comodos.corredor1[1]; 
    RGB[2] = comodos.corredor1[2];
    RGB[3] = comodos.corredor1[3];
    RGB[4] = comodos.corredor2[1]; 
    RGB[5] = comodos.corredor2[2];
    RGB[6] = comodos.corredor2[3];
  }
  else if(lugar == "lavabo"){
    RGB[1] = comodos.lavabo[1]; 
    RGB[2] = comodos.lavabo[2];
    RGB[3] = comodos.lavabo[3];
  }
  else if(lugar == "banheiro 1"){
    RGB[1] = comodos.banheiro1[1]; 
    RGB[2] = comodos.banheiro1[2];
    RGB[3] = comodos.banheiro1[3];
  }
  else if(lugar == "suíte 1"){
    RGB[1] = comodos.suite1[1]; 
    RGB[2] = comodos.suite1[2];
    RGB[3] = comodos.suite1[3];
  }
  else if(lugar == "banheiro 2"){
    RGB[1] = comodos.banheiro2[1]; 
    RGB[2] = comodos.banheiro2[2];
    RGB[3] = comodos.banheiro2[3];
  }
  else if(lugar == "suíte 2"){
    RGB[1] = comodos.suite2[1]; 
    RGB[2] = comodos.suite2[2];
    RGB[3] = comodos.suite2[3];
  }
  else if(lugar == "quarto"){
    RGB[1] = comodos.quarto[1]; 
    RGB[2] = comodos.quarto[2];
    RGB[3] = comodos.quarto[3];
  }
   else if(lugar == "todos"){
    RGB[1] = comodos.sala[1]; 
    RGB[2] = comodos.sala[2];
    RGB[3] = comodos.sala[3];
    RGB[4] = comodos.corredor1[1]; 
    RGB[5] = comodos.corredor1[2];
    RGB[6] = comodos.corredor1[3];
    RGB[7] = comodos.corredor2[1]; 
    RGB[8] = comodos.corredor2[2];
    RGB[9] = comodos.corredor2[3];
    RGB[10] = comodos.lavabo[1]; 
    RGB[11] = comodos.lavabo[2];
    RGB[12] = comodos.lavabo[3];
    RGB[13] = comodos.banheiro1[1]; 
    RGB[14] = comodos.banheiro1[2];
    RGB[15] = comodos.banheiro1[3];
    RGB[16] = comodos.suite1[1]; 
    RGB[17] = comodos.suite1[2];
    RGB[18] = comodos.suite1[3];
    RGB[19] = comodos.banheiro2[1]; 
    RGB[20] = comodos.banheiro2[2];
    RGB[21] = comodos.banheiro2[3];
    RGB[22] = comodos.suite2[1]; 
    RGB[23] = comodos.suite2[2];
    RGB[24] = comodos.suite2[3];
    RGB[25] = comodos.quarto[1]; 
    RGB[26] = comodos.quarto[2];
    RGB[27] = comodos.quarto[3];
  }
}

void LigarCor(short int* RGB, int cor)
{
  short int tam = sizeof(RGB)/sizeof(short int);
  short int i;
  if(cor == 'R'){
    for(i = 1; i <= tam; i+=3 ){
      digitalWrite(RGB[i], HIGH);
      digitalWrite(RGB[i+1], LOW);
      digitalWrite(RGB[i+2], LOW);
    }
    
  }
  else if(cor == 'G'){
    for(i = 1; i <= tam; i+=3 ){
      digitalWrite(RGB[i], LOW);
      digitalWrite(RGB[i+1], HIGH);
      digitalWrite(RGB[i+2], LOW);
    }
  }
  else if(cor == 'B'){
    for(i = 1; i <= tam; i+=3 ){
      digitalWrite(RGB[i], LOW);
      digitalWrite(RGB[i+1], LOW);
      digitalWrite(RGB[i+2], HIGH);
    }
    
  }
  else if(cor == 'C'){
    for(i = 1; i <= tam; i+=3 ){
      digitalWrite(RGB[i], LOW);
      digitalWrite(RGB[i+1], HIGH);
      digitalWrite(RGB[i+2], HIGH);
    }
  }
  else if(cor == 'M'){
    for(i = 1; i <= tam; i+=3 ){
      digitalWrite(RGB[i], HIGH);
      digitalWrite(RGB[i+1], LOW);
      digitalWrite(RGB[i+2], HIGH);
      }
  }
  else if(cor == 'Y'){
    for(i = 1; i <= tam; i+=3 ){
      digitalWrite(RGB[i], HIGH);
      digitalWrite(RGB[i+1], HIGH);
      digitalWrite(RGB[i+2], LOW);
    }
  }
  else if(cor == 'Y'){
    for(i = 1; i <= tam; i+=3 ){
      digitalWrite(RGB[i], HIGH);
      digitalWrite(RGB[i+1], HIGH);
      digitalWrite(RGB[i+2], LOW);
    }
  }
}




