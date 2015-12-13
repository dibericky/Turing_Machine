#include <Stepper.h>
const int steps = 97;  //passo
Stepper myStepper(steps, 8, 9, 10,11);
int fotoRes = A0;  //pin analogico fotoresistenza
//s leggi scrivi s+1 MOVE

/*
-legge istruzione
-legge colore
-esegui istruzione

*/


int move = 0; //    1= dx  0 = sx   2 = fermo
boolean canOperate = false;
// numero_led = pin su arduino
int led1 = 50;
int led2 = 48;
int led3 = 46;
int led4 = 44;
int led5 = 42;
int led6 = 40;
int led7 = 38;
int led8 = 36;
int led9 = 34;
int led10 = 32;
int led11 = 30;
int led12 = 28;
int led13 = 26;
int led14 = 24;
int led15 = 22;



int led[15] = {led1,led2, led3, led4, led5, led6, led7, led8, led9, led10, led11, led12, led13, led14, led15};
//stato dei led
int statoLed[15] = {1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0};
int n_led = 15;



 
  /*
array_istruzioni: { 
  {stato_attuale, leggi, scrivi, stato_succ, move},
  {stato_attuale, leggi, scrivi, stato_succ, move},
  {stato_attuale, leggi, scrivi, stato_succ, move}
  }*/
  
  //addizione
  int istruzioni_add[6][5] = {
  {0, 1, 1, 0, 1},
  {0, 0, 1, 1, 1},
  {1, 1, 1, 1, 1},
  {1, 0, 0, 2 , 0},
  {2, 1, 0, 3, 0},
  {3, 1, 0, 4, -1}
  };
 int row_istruzioni = 6; //numero quintuple
 

int stato_corrente = 0;
int pos = 0;
int acceso = 0;
int spento = 0;
String output = "";
int nowValues = 0;


//per il setup ->  permette di identificare il primo led, da cui partirà il programma
//Il led da cui dovrà partire è identificato dal led che lampeggia, arduino si muoverà per trovarlo

int firstMisure;
int contVerifica = 0;
boolean firstFound = false;
boolean on_offLed = true;
boolean needSetup = true;



void setup() {
  
  Serial.begin(9600);
   
  Serial.print("N LED:");
  Serial.println(n_led); //stampo numero di led
  
  //imposto tutti i pin dei led come dispositivi OUTPUT
  for(int l = 0; l < n_led; l++){
  pinMode(led[l], OUTPUT);
  }
  Serial.println("GETTING ON OFF VALUES");
  //spengo tutti i led   (logica negata:  HIGH per spegnere, LOW per accendere
  
  for(int n = 0; n < n_led; n++){
      digitalWrite(led[n], HIGH);
   }
  delay(1000);
   
   //leggo il valore della fotoresistenza quando il led è spento
   spento = analogRead(fotoRes);
   Serial.println("SPENTO :"+String(spento));
   delay(1000);
   
    //accendo tutti i led
   for(int n = 0; n < n_led; n++){
      digitalWrite(led[n], LOW);
   }
  delay(1000);
  
  //leggo il valore della fotoresistenza quando i led sono accesi
  acceso = analogRead(fotoRes);
  Serial.println("ACCESO :"+String(acceso));
  
  myStepper.setSpeed(60);
  delay(1000);
   
   Serial.println("DIFF : "+String(acceso - spento));
   delay(3000);
   //richiamo il setup 
   while(needSetup == true){
      makeSetup(); 
   }
}

void loop() {
    delay(500);
     
    //imposto i led in base al loro stato (acceso o spento), definito dall'array statoLed e applico la logica negata
    for(int n = 0; n < n_led; n++){
        digitalWrite(led[n], !statoLed[n]);
    }
    
    delay(1500);
    if(pos < n_led && pos >= 0){
      Serial.print("POS: ");
      Serial.println(pos);
      Serial.print("STATO LED: ");
      Serial.println(statoLed[pos]);
      nowValues = analogRead(fotoRes);
      Serial.println("VALORE LETTO :"+String(nowValues));
      int diff = acceso - spento;
       int valToPass = 0;
       if(nowValues <= spento + 100){
           valToPass = 0;
       }else{
           valToPass = 1; 
       }
       addizione(valToPass);
       Serial.println("________________________________"+String(valToPass));
    }else{
        Serial.print("STATO ");
        Serial.println(stato_corrente);
        Serial.print("POS ");
        Serial.println(pos);
        
    }
}

void addizione(int val_letto){   // passo il valore del led (Acceso/Spento) e analizzo ogni istruzione per vedere quella da eseguire
    Serial.println("ESEGUI");
    
    boolean thereIsState = false;
    //controllo se esiste una quintupla corrispondente allo stato corrente
    for(int j = 0; j < row_istruzioni; j++){
       if(istruzioni_add[j][0] == stato_corrente){
         thereIsState = true;
         Serial.print("STATO CORRENTE ");
         Serial.println(stato_corrente);
         break;
       } 
    }
    //se esiste la quintupla corrispondente
    if(thereIsState == true){
        for(int x = 0; x < row_istruzioni; x++){
           if(istruzioni_add[x][0] == stato_corrente){   //controllo lo stato
              if(istruzioni_add[x][1] == val_letto){  //guardo se il valore letto è uguale a quello della quintupla
                  Serial.println("FIND!");
                  Serial.println(" QUINTUPLA : "+String(istruzioni_add[x][0])+" "+String(istruzioni_add[x][1])+ " "+String(istruzioni_add[x][2])+ " "+String(istruzioni_add[x][3])+ " "+String(istruzioni_add[x][4]));
                  
                  stato_corrente = istruzioni_add[x][3]; //aggiorno lo stato a quello corrispondente a "stato successivo" della quintupla
                  //aggiorno il valore del led in base alla quintupla
                  int newValLed = istruzioni_add[x][2];
                  statoLed[pos] = newValLed;
                  //muovo la testina in base al movimento della quintupla
                  move = istruzioni_add[x][4];
                  move_macchina(move);
                  break; //una volta trovata l'ìstruzione non deve continuare
              }else{
                 Serial.print("NOT FOUND ");
                 Serial.println(istruzioni_add[x][1]);
                 
                Serial.print("val_letto ");
                Serial.println(val_letto);
              }
          }else{
             Serial.print(istruzioni_add[x][0]);
             Serial.print(" DIVERSO ");
             Serial.println(stato_corrente); 
          }
        }
       
     
   }else{
     //non esiste la quintupla corrispondente
     Serial.print("NO STATO TROVATO ");
     Serial.println(stato_corrente);
      pos = 99999999; 
   }
   
}
void move_macchina(int where){ //muove la testina
    String out = "MUOVI ";
    Serial.println(out+ where);
     //myStepper.step(stepPasso);
  if(where == 0){
   //dx
    myStepper.step(-steps);
  pos = pos - 1;
    Serial.print("INDIETRO ");
    
    
  }else if( where == 1){
  //sx 
  pos = pos + 1;
   myStepper.step(steps);
  Serial.print("AVANTI ");
  }else{
    //fermo
     pos = 999999;
     Serial.print("FERMO ");
  }  
  Serial.print("POS MOVE ");
  Serial.println(pos);
   
}

void makeSetup(){
  //eseguo conto per ogni led 5 volte
    for(int x = 0; x < 5; x++){
      //accendo tutti i led
      for(int y = 1; y < (n_led-1); y++){
        digitalWrite(led[y], LOW);
      }
      
     digitalWrite(led[0], LOW);
     digitalWrite(led[n_led-1], LOW);
     //fine accensione tutti led
     delay(100);
     //led  valore da fotoresistenza
    int v = analogRead(fotoRes);
     delay(500);
      //spengo il primo led
     digitalWrite(led[0], HIGH);
     //spengo ultimo led
     digitalWrite(led[n_led-1], HIGH); 
     delay(100);
     //leggo valore
     int v2 = analogRead(fotoRes);
     //se il valore letto quando primo led era acceso è uguale o maggiore del valore letto quando ultimo led era spento
     //allora la testina è sul primo led
     if(v >= v2 +40){
        Serial.println("È il primo");
        contVerifica++;
      
     }else{
        Serial.println(String(v) + " - "+String(v2));  
     }
     delay(500); 
   }
   //se è verificato almeno 3 volte che è il primo led, allora non c'è più bisogno di fare il setup.
   if(contVerifica > 3){
      needSetup = false; 
      contVerifica = 0;
   }else{
     //altrimenti muovi la testina
      movingOnSetup(); 
      contVerifica = 0;
   }
}
//muove la testina verso sinistra
void movingOnSetup(){
  myStepper.step(-steps);
  
}
