#include <Servo.h> // importerer biblioteket og funksjoner for Servo
#include <Wire.h> // importerer biblioteket og funksjoner for å kommunisere mellom arduinoer med wire

Servo myServo; //deklarerer servo;
int angle; //deklarerer vinkelen til servoen

int pir=5; //deklarerer pir-sensoren med port;

unsigned long lastButtonPush=0; //vaiabel for å sjekke tiden siden sist trykket
int debounceDelay=200; // angir tid mellom knappetrykk

unsigned long lastMillis; //vaiabel for å sjekke hva tiden har vært sist
long interval = 60000; // lengden på tiden en bruker får inne på badet(her har vi brukt 60 sek for å teste men dette kan endres)

bool paaBadet; //en bool som sier om noen er på badet eller ikke
bool buttonState1; //definerer om knappen har vært trykket
bool buttonState2;
bool buttonState3;
bool buttonState4;

int koPlassButton1 = -1; //deklarerer en int for plass i køen der -1 er ikke i kø
int koPlassButton2 = -1;
int koPlassButton3 = -1;
int koPlassButton4 = -1;

int antKo = -1; // antallet personer i køen totalt

const int button1=13; //deklarerer input til knappene 
const int button2=12;
const int button3=11;
const int button4=10;

void setup()
{
  myServo.attach(4); // deklarerer servoens port
  pinMode(pir,INPUT); //pir sensoren skal ta input
  pinMode(button1, INPUT); //definerer buttons som input
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  pinMode(button4, INPUT);
  Wire.begin(); //begynner koblingen mellom de to arduinoene
  Serial.begin(9600);
}

void loop(){ 
  sjekkKnapper(); 
  koSystem();
  nesteKo();
  sjekkBadet();
  //sjekker om det er noen på badet og at kø er større eller lik 0
  if((paaBadet == true) && (antKo >= 0)){
    timer(); //timeren går
  }
}

//returner int hvilken knapp som ble trykket på
int sjekkKnapper(){
  unsigned long currentTime=millis();
  if ((currentTime-lastButtonPush)>=debounceDelay){
       if (digitalRead(button1)==HIGH){  
          lastButtonPush=millis();
          buttonState1=true;
          //oppdaterLys(1);
          return 1;
       }
       if (digitalRead(button2)==HIGH){
          lastButtonPush=millis();
         buttonState2=true;
         //oppdaterLys(2);
          return 2;
       }
       if (digitalRead(button3)==HIGH){
          lastButtonPush=millis();
         buttonState3=true;
         //oppdaterLys(3);
          return 3;
       }
       if(digitalRead(button4)==HIGH){
         lastButtonPush=millis();
         buttonState4=true;
         //oppdaterLys(4);
         return 4;
       }
  }
  return 0;
}

//sjekk for om noen er på badet de første 10 sekundene, dersom det ikke er noen og tiden har gått ut iterer kosystemet til neste
void sjekkBadet(){
  if(digitalRead(pir) == HIGH &&(millis() - lastMillis) < 10000){
    paaBadet = true;
  }
  if(digitalRead(pir) == LOW && (millis() - lastMillis) > interval){
    paaBadet = false;
    }
}
//sjekker hvilken plass hver button har i køen og endrer farge av LED etter dette
void oppdaterLys(int knappNr){
  Wire.beginTransmission(8); //begynner å sende data til den andre arduinoen.
  switch(knappNr){
    case 1:
      if(koPlassButton1 == -1){
        Wire.write("1off");
      }
      else if(koPlassButton1 == 0){
        Wire.write("1green");
      }
      else if(koPlassButton1 == 1){
        Wire.write("1yellow");
      }
      else if(koPlassButton1 >= 2){
        Wire.write("1red");
      }
      break;
    case 2:
      if(koPlassButton2 == -1){
        Wire.write("2off");
      }
      else if(koPlassButton2 == 0){
        Wire.write("2green");
      }
      else if(koPlassButton2 == 1){
        Wire.write("2yellow");
      }
      else if(koPlassButton2 >= 2){
        Wire.write("2red");
      }
      break;
    case 3:
      if(koPlassButton3 == -1){
        Wire.write("3off");
      }
      else if(koPlassButton3 == 0){
        Wire.write("3green");
      }
      else if(koPlassButton3 == 1){
        Wire.write("3yellow");
      }
      else if(koPlassButton3 >= 2){
        Wire.write("3red");
      }
      break;
    case 4:
      if(koPlassButton4 == -1){
        Wire.write("4off");
      }
      else if(koPlassButton4 == 0){
        Wire.write("4green");
      }
      else if(koPlassButton4 == 1){
        Wire.write("4yellow");
      }
      else if(koPlassButton4 >= 2){
        Wire.write("4red");
      }
      break;
  }
  Wire.endTransmission(); // avslutter sendingen av data til den andre arduinoen
}

//hvis buttonState ved trykk har forandret seg og personen ikke allerede har fått plass, oppdateres lysene
void koSystem(){
  if(buttonState1 == true && koPlassButton1 == -1){
    antKo++;
    koPlassButton1 = antKo;
    oppdaterLys(1);
  }
  if(buttonState2 == true && koPlassButton2 ==-1){
    antKo++;
    koPlassButton2 = antKo;
    oppdaterLys(2);
  }
   if(buttonState3 == true && koPlassButton3==-1){
    antKo++;
    koPlassButton3 = antKo;
    oppdaterLys(3);
  }
   if(buttonState4 == true && koPlassButton4==-1){
    antKo++;
    koPlassButton4 = antKo;
    oppdaterLys(4);
   
  }
}

//dersom tiden for personen er ute og det er ingen på badet, skal køen itere seg videre og posisjonen minkes
void nesteKo(){
  if((millis() - lastMillis) > interval && (paaBadet == false)){
     if(koPlassButton1 >= 0){
    koPlassButton1 = koPlassButton1 - 1;
    oppdaterLys(1);
    if(koPlassButton1 == -1){
    buttonState1 = false;
    Serial.println("button1 er false");
    }
  }
  if(koPlassButton2 >= 0){
    koPlassButton2 = koPlassButton2 - 1;
    oppdaterLys(2);
     if(koPlassButton2 == -1){
    buttonState2 = false;
    Serial.println("button2 er false");
    }
  }
  if(koPlassButton3 >= 0){
    koPlassButton3 = koPlassButton3 - 1;
    oppdaterLys(3);
     if(koPlassButton3 == -1){
    buttonState3 = false;
    Serial.println("button3 er false");
    }
  }
  if(koPlassButton4 >= 0){
    koPlassButton4 = koPlassButton4 - 1;
    oppdaterLys(4);
     if(koPlassButton4 == -1){
    buttonState4 = false;
    Serial.println("button4 er false");
    }
  }
  antKo--;
  lastMillis = millis();
  resetTimer();
    }
}

//lager en metode for at timeren skal telle
void timer(){ 
   angle = ((millis() - lastMillis) / 1000 * 3); // her ganger vi ut ifra hva 179/ ønsket tid er. I testtilfellet er 179 / 60 = 3.
   if(angle < 179){ 
     myServo.write(angle);
   }
}
//resette timer
void resetTimer(){
    angle = 0;
    lastMillis = millis();
  myServo.write(angle);
}
