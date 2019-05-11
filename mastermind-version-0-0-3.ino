/*
  Jogo Mastermind
  O projeto consiste na implementação do jogo Mastermind, versão single e multi player
  
  created April 2019
  by Júlia Wotzasek Pereira
*/



// Bibliotecas Utilizadas
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

LiquidCrystal_I2C lcd(0x27,16,2);

// Definição das constantes referentes aos componetes do circuito
const int ledGreen1 = 22;
const int ledGreen2 = 24;
const int ledGreen3 = 26;
const int ledGreen4 = 28;

const int ledWhite1 = 36;
const int ledWhite2 = 38;
const int ledWhite3 = 40;
const int ledWhite4 = 42;

const int buzzer = 8;
const int melodyPin = 8;

// Inicializações do teclado
const byte linhas = 4; //4 linhas
const byte colunas = 4; //4 colunas

//teclado matricial
char matrizteclado[linhas][colunas] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte pinoslinhas[linhas] = {46,47,48,49}; //pinos utilizados nas linhas
byte pinoscolunas[colunas] = {50,51,52,53}; //pinos utilizados nas colunas

//inicializando o teclado
Keypad teclado = Keypad( makeKeymap(matrizteclado), pinoslinhas, pinoscolunas, linhas, colunas );

////Define os pinos que serão utilizados para ligação ao display
//LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define NOTE_G6  1568
#define NOTE_C7  2093
#define NOTE_E7  2637
#define NOTE_G7  3136


int melody[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7,
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
  NOTE_G6, 0, 0, 0,
};

int tempo[] = {
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
};

// Variáveis de controle do jogo
int senhaParam[4] = {1,3,2,1};
int senhaDigitada[4];
int contadorReset = 3;
int verdes = 0;
int brancos = 0;
int contadorJogadas = 9;
int contadorJogadasReset = 9;
int tom;
int sorteadorSenha = 1000;

void ganhaJogo(){
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("ACERTOU!!!");
  lcd.setCursor(2,1);
  lcd.print("Pontuacao:");
  lcd.print(contadorJogadas + 1);
  
  sing();

}

void sing() {
  
  int size = sizeof(melody) / sizeof(int);
  for (int thisNote = 0; thisNote < size; thisNote++) {
    int noteDuration = 1000 / tempo[thisNote];
    buzz(melodyPin, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    buzz(melodyPin, 0, noteDuration);
  }
}
 
void buzz(int targetPin, long frequency, long length) {
  long delayValue = 1000000 / frequency / 2; // calculate the delay value between transitions
  long numCycles = frequency * length / 1000; // calculate the number of cycles for proper timing
  for (long i = 0; i < numCycles; i++) { // for the calculated length of time...
    
    digitalWrite(ledGreen1,HIGH);
    digitalWrite(ledGreen2,HIGH);
    digitalWrite(ledGreen3,HIGH);
    digitalWrite(ledGreen4,HIGH);
  
    digitalWrite(ledWhite1,HIGH);
    digitalWrite(ledWhite2,HIGH);
    digitalWrite(ledWhite3,HIGH);
    digitalWrite(ledWhite4,HIGH);

    digitalWrite(targetPin,HIGH);
    delayMicroseconds(delayValue); // wait for the calculated delay value
    
    digitalWrite(ledGreen1,LOW);
    digitalWrite(ledGreen2,LOW);
    digitalWrite(ledGreen3,LOW);
    digitalWrite(ledGreen4,LOW);
    digitalWrite(targetPin,LOW);
  
    digitalWrite(ledWhite1,LOW);
    digitalWrite(ledWhite2,LOW);
    digitalWrite(ledWhite3,LOW);
    digitalWrite(ledWhite4,LOW);
  
    
    delayMicroseconds(delayValue); // wait again or the calculated delay value
  }
  digitalWrite(13, LOW);
}

boolean confereSenha(){
  int controleDigitada[4];
  int controleParametro[4];
  brancos = 0;
  verdes = 0; 
  for(int i = 0;i < 4;i++){
    controleDigitada[i] = 0;
    controleParametro[i] = 0;
  }
 // testa as posições corretas
  for(int i = 0;i < 4;i++){
    if(senhaDigitada[i] == senhaParam[i]){
      verdes++;
      controleDigitada[i] = 1;
      controleParametro[i] = 1;
    }
  }
  if(verdes == 4){
    return true;
  }else{
   for(int i = 0;i < 4;i++){
    if(controleDigitada[i] != 1){
      for(int j = 0; j < 4;j++){
        if(controleParametro[j] != 1){
          if(senhaParam[i] == senhaDigitada[j]){
            brancos++;
            controleParametro[j] = 1;
            break;
          }
        }
      }  
     }
   } 
  }
  return false;
}

int nextSenha(int senhaAtual){
  senhaAtual = 3 * senhaAtual - 1;
  while(senhaAtual > 9999){
    senhaAtual = senhaAtual / 2;
  }
  while(senhaAtual < 1000){
    senhaAtual = senhaAtual * 2 ;
  }
  return senhaAtual;
}

void sorteiaSenha(){
  int senha = 0;
  for(int i = 0;i < 4;i++){
    senha = senha * 10 + senhaParam[i];
  }
  for(int i = 0;i < sorteadorSenha;i++){
    senha = nextSenha(senha);  
  }
  senhaParam[0] = senha/1000;
  senhaParam[1] = (senha/100)%10;
  senhaParam[2] = (senha/10)%10%10;
  senhaParam[3] = senha%1000%100%10;
  for(int i = 0;i < 4;i++){
    if(senhaParam[i] > 6){
      senhaParam[i] = senhaParam[i]%7;
    } else if(senhaParam[i] == 0){
      senhaParam[i] = i;
    }
  }
  sorteadorSenha++;
}

void leSenha(){
  int contador = contadorReset;
  int digito;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Proponha a senha");
  lcd.setCursor(0,1);
  lcd.print("Digitos de 1 a 6");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Proponha a senha");
  lcd.setCursor(6,1);
  while(contador >= 0){
    digito = leDigito();
    if(aceitaDigito(digito)){
      senhaParam[contadorReset - contador] = digito;
      contador--;
      lcd.print("*");
    }
  }
  delay(2000);
}

void iniciaJogo(){
  int modo = selecionaModo();
  if(modo == 2){
    leSenha();
  } else {
    sorteiaSenha(); // sorteia a senha  
  }
  contadorJogadas = contadorJogadasReset; // inicializa o contador de jogadas no máximo
  //Limpa a tela
  lcd.clear();
  //Posiciona o cursor na coluna 3, linha 0;
  lcd.setCursor(3, 0);
  //Envia o texto entre aspas para o LCD
  lcd.print("MASTERMIND");
  lcd.setCursor(1, 1);
  lcd.print("Seja bem-vindo!");
  delay(5000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Sua missao, se");
  lcd.setCursor(0,1);
  lcd.print("aceitar, eh des-");
  delay(2500);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("aceitar, eh des-");
  lcd.setCursor(0,1);
  lcd.print("cobrir a senha");
  delay(2500);
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Voce tem 10");
  lcd.setCursor(4,1);
  lcd.print("chances");
  delay(2500);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("As senhas sao");
  lcd.setCursor(3,1);
  lcd.print(" de 4 digitos");
  delay(2500);
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Os digitos vao");
  lcd.setCursor(4,1);
  lcd.print("de 1 a 6");
  delay(2500);
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("BOA SORTE!");
  lcd.setCursor(3,1);
  lcd.print("MASTERMIND");
  delay(2500);
  
  
  
}

int leDigito(){
  char digito = teclado.getKey();
    while(!digito){
      digito = teclado.getKey();
    }
  int d = (int)digito;
  tom = 420 + 100 * (d - 48);
  digitalWrite(buzzer,HIGH);
  delayMicroseconds(tom);
  delay(100);
  digitalWrite(buzzer,LOW);
  delayMicroseconds(tom);
  delay(100);
  return d - 48;
}

boolean aceitaDigito(int digito){
  if(digito == 1 || digito == 2 || digito == 3 || digito == 4 || digito == 5 || digito == 6){
    return true;
  }
  return false;
}

void imprimeTentativa(){
  lcd.clear();
  int tentativa = contadorJogadasReset - contadorJogadas + 1;
  if(tentativa == 1){
    lcd.setCursor(3,0);
    lcd.print("Primeira");  
  }
  if(tentativa == 2){
    lcd.setCursor(4,0);
    lcd.print("Segunda");  
  }
  if(tentativa == 3){
    lcd.setCursor(3,0);
    lcd.print("Terceira");  
  }
  if(tentativa == 4){
    lcd.setCursor(4,0);
    lcd.print("Quarta");  
  }
  if(tentativa == 5){
    lcd.setCursor(4,0);
    lcd.print("Quinta");  
  }
  if(tentativa == 6){
    lcd.setCursor(5,0);
    lcd.print("Sexta");  
  }
  if(tentativa == 7){
    lcd.setCursor(4,0);
    lcd.print("Setima");  
  }
  if(tentativa == 8){
    lcd.setCursor(4,0);
    lcd.print("Oitava");  
  }
  if(tentativa == 9){
    lcd.setCursor(5,0);
    lcd.print("Nona");  
  }
  if(tentativa == 10){
    lcd.setCursor(4,0);
    lcd.print("ULTIMA");  
  }
  lcd.setCursor(3,1);
  lcd.print("Tentativa");
  //Rolagem para a esquerda
  for (int posicao = 0; posicao < 3; posicao++)
  {
    lcd.scrollDisplayLeft();
    delay(400);
  }
   
  //Rolagem para a direita
  for (int posicao = 0; posicao < 6; posicao++)
  {
    lcd.scrollDisplayRight();
    delay(400);
  }
}

void recebeChute(){
  imprimeTentativa();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Qual eh a senha?");
  int contador = contadorReset;
  int digito;
  if(contadorJogadas < 9){
    lcd.setCursor(0,1);
    for(int i = 0;i < 4;i++){
      lcd.print(senhaDigitada[i]);
    }    
  }
  lcd.setCursor(6,1);
  while(contador >= 0){
    digito = leDigito();
    if(aceitaDigito(digito)){
      senhaDigitada[contadorReset - contador] = digito;
      contador--;
      lcd.print(digito);
    }
  }
  delay(2000);
}

int selecionaModo(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SinglePlayer?(1)");
  lcd.setCursor(0,1);
  lcd.print("MultiPlayer? (2)");
  int digito = leDigito();
  while(digito != 1 && digito != 2){
    digito = leDigito();
  }
  return digito;
}

void respondeJogada(){
  if(verdes == 0){
    digitalWrite(ledGreen1,LOW);
    digitalWrite(ledGreen2,LOW);
    digitalWrite(ledGreen3,LOW);
    digitalWrite(ledGreen4,LOW);
  } else if(verdes == 1){
    digitalWrite(ledGreen1,HIGH);
    digitalWrite(ledGreen2,LOW);
    digitalWrite(ledGreen3,LOW);
    digitalWrite(ledGreen4,LOW);
  } else if(verdes == 2){
    digitalWrite(ledGreen1,HIGH);
    digitalWrite(ledGreen2,HIGH);
    digitalWrite(ledGreen3,LOW);
    digitalWrite(ledGreen4,LOW);
  } else if(verdes == 3){
    digitalWrite(ledGreen1,HIGH);
    digitalWrite(ledGreen2,HIGH);
    digitalWrite(ledGreen3,HIGH);
    digitalWrite(ledGreen4,LOW);
  }
  
  if(brancos == 0){
    digitalWrite(ledWhite1,LOW);
    digitalWrite(ledWhite2,LOW);
    digitalWrite(ledWhite3,LOW);
    digitalWrite(ledWhite4,LOW);
  } else if(brancos == 1){
    digitalWrite(ledWhite1,HIGH);
    digitalWrite(ledWhite2,LOW);
    digitalWrite(ledWhite3,LOW);
    digitalWrite(ledWhite4,LOW);
  } else if(brancos == 2){
    digitalWrite(ledWhite1,HIGH);
    digitalWrite(ledWhite2,HIGH);
    digitalWrite(ledWhite3,LOW);
    digitalWrite(ledWhite4,LOW);
  } else if(brancos == 3){
    digitalWrite(ledWhite1,HIGH);
    digitalWrite(ledWhite2,HIGH);
    digitalWrite(ledWhite3,HIGH);
    digitalWrite(ledWhite4,LOW);
  } else if(brancos == 4) {
    digitalWrite(ledWhite1,HIGH);
    digitalWrite(ledWhite2,HIGH);
    digitalWrite(ledWhite3,HIGH);
    digitalWrite(ledWhite4,HIGH);
  }
}

void perdeJogo(){
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print("PERDEU");
  lcd.setCursor(3,1);
  lcd.print("GAME OVER");
  #define tempo 10
  int frequencia = 0;
  for (frequencia = 150; frequencia < 1800; frequencia += 1){
    tone(buzzer, frequencia, tempo);
    digitalWrite(ledGreen1,HIGH);
    digitalWrite(ledGreen2,HIGH);
    digitalWrite(ledGreen3,HIGH);
    digitalWrite(ledGreen4,HIGH);
  
    digitalWrite(ledWhite1,HIGH);
    digitalWrite(ledWhite2,HIGH);
    digitalWrite(ledWhite3,HIGH);
    digitalWrite(ledWhite4,HIGH); 
    delay(1);
  }
  for (frequencia = 1800; frequencia > 150; frequencia -= 1){
    tone(buzzer, frequencia, tempo);
    digitalWrite(ledGreen1,HIGH);
    digitalWrite(ledGreen2,HIGH);
    digitalWrite(ledGreen3,HIGH);
    digitalWrite(ledGreen4,HIGH);
  
    digitalWrite(ledWhite1,HIGH);
    digitalWrite(ledWhite2,HIGH);
    digitalWrite(ledWhite3,HIGH);
    digitalWrite(ledWhite4,HIGH);  
    delay(1);
  }
  digitalWrite(ledGreen1,LOW);
  digitalWrite(ledGreen2,LOW);
  digitalWrite(ledGreen3,LOW);
  digitalWrite(ledGreen4,LOW);

  digitalWrite(ledWhite1,LOW);
  digitalWrite(ledWhite2,LOW);
  digitalWrite(ledWhite3,LOW);
  digitalWrite(ledWhite4,LOW);  
  
  delay(1000);
}

void setup() {
  //Leds de resposta
  pinMode(ledGreen1,OUTPUT);
  pinMode(ledGreen2,OUTPUT);
  pinMode(ledGreen3,OUTPUT);
  pinMode(ledGreen4,OUTPUT);

  //Leds de resposta
  pinMode(ledWhite1,OUTPUT);
  pinMode(ledWhite2,OUTPUT);
  pinMode(ledWhite3,OUTPUT);
  pinMode(ledWhite4,OUTPUT);

  // Buzzer
  pinMode(buzzer,OUTPUT);

  //Define o número de colunas e linhas do LCD
  lcd.init();
  lcd.setBacklight(HIGH);
  //lcd.begin(16, 2);
  
  iniciaJogo();
}

void loop(){
  // Jogadas ainda são aceitas
  boolean ganhou = false;
  if(contadorJogadas >= 0 || ganhou){
    recebeChute();
    ganhou = confereSenha();
    if(!ganhou){
      respondeJogada();
      contadorJogadas--;
    } else {
      ganhaJogo();
      iniciaJogo();
    }
  } else {
    perdeJogo();
    iniciaJogo();
  }
}
