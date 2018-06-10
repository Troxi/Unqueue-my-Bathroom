#include <Wire.h> // importerer biblioteket og funksjoner for å kommunisere mellom arduinoer med wire

//definerer portene til hver enkelt pin, nummerert etter hvilken knapp de hører til
const int redLEDPin1=13;
const int blueLEDPin1=12;
const int greenLEDPin1=11;

const int redLEDPin4=10;
const int blueLEDPin4=9;
const int greenLEDPin4=8;

const int redLEDPin3=7;
const int blueLEDPin3=6;
const int greenLEDPin3=2;

const int redLEDPin2=5;
const int blueLEDPin2=4;
const int greenLEDPin2=3;

int x = 0;

//definerer LEDene sine porter i et int array for å kunne iterere gjennom portene
int led1[]= {13, 12, 11};
int led4[]= {10, 9, 8};
int led3[]= {7, 6, 2};
int led2[]= {5, 4, 3};

//definerer int array for å itererere gjennom verdiene ved LED pæren når vi itererer gjennom
int OFF[]= {0, 0, 0};
int GREEN[] = {0, 0, 220};
int YELLOW[] = {220, 0, 220};
int RED[] = {220, 0, 0};


void setup()
{
  pinMode(redLEDPin1, OUTPUT);
  pinMode(blueLEDPin1, OUTPUT);
  pinMode(greenLEDPin1, OUTPUT);
  
  pinMode(redLEDPin2, OUTPUT);
  pinMode(blueLEDPin2, OUTPUT);
  pinMode(greenLEDPin2, OUTPUT);
  
  pinMode(redLEDPin3, OUTPUT);
  pinMode(blueLEDPin3, OUTPUT);
  pinMode(greenLEDPin3, OUTPUT);
  
  pinMode(redLEDPin4, OUTPUT);
  pinMode(blueLEDPin4, OUTPUT);
  pinMode(greenLEDPin4, OUTPUT);
  
  Serial.begin(9600);
  Wire.begin(8); //begynner koblingen mellom arduinoene
  Wire.onReceive(receiveEvent);// kaller på evenhandler som kjører hver gang arduinoen mottar data
}

void loop()
{
 
}

void receiveEvent(int howMany){
  String s;
  while (Wire.available()) { //looper gjennom alle bokstavene som kommer inn og lagrer de til en string
    char c = Wire.read();
    s = s + c;
  }
  Serial.println(s);  
  int *lednummer; //referanser til led listene og farge listene
  int *color;
  switch(s.charAt(0)){ //sjekker hvilken int som står først og setter det til tilhørende lednr
    case '1':
      lednummer = led1; //setter den loka referansen til å referere til led
      break;
    case '2':
      lednummer = led2;
      break;
    case '3':
      lednummer = led3;
      break;
    case '4':
      lednummer = led4;
      break;
    }
    //sjekker hva som står etter inten
    String colorString = s.substring(1, s.length());
    Serial.println(" COlorSTRINGEN: " + colorString);
    if(colorString.equals("off"))
      color = OFF;
    else if(colorString.equals("green"))
      color = GREEN;
    else if(colorString.equals("yellow"))
      color = YELLOW;
    else if(colorString.equals("red"))
      color = RED;

    setColor(lednummer, color);
}

//itererer seg gjennom parameterene med int array og skriver ut opplysningene til led pærene
void setColor(const int *led, const int *colorType){
  for(int i= 0; i < 3; i++){
    Serial.println(led[i]);
    Serial.println(colorType[i]);
    analogWrite(led[i], colorType[i]);
  }
}
