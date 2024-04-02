#include<FastLED.h>
#include<SD.h>
#include<SPI.h>
#include<TMRpcm.h>

const int NombreLed = 108;
const int PinLed = 8;
CRGB Leds[NombreLed];

const int BoutonHaut = 4;
const int BoutonBas = 5;
bool EBHaut;
bool EBBas;

TMRpcm lecteur;
const int SDpin = 10;

byte msg;
int Groupe = 1;

int BureauAnswer;
int LumiereBouton;
bool Voix;
bool LumiereOn;
bool ChoixLum;
bool ChoixJour;

void(* reset) (void) = 0;

void GroupeRappel()
{
    msg = 0;
    if(Groupe == 1)Groupe1();
    if(Groupe == 2)Groupe2();
    if(Groupe == 3)Groupe3();
}

void beginVoix()
{
  Serial.write(0xAA);
  Serial.write(0x00);
  delay(100);
  Serial.write(0xAA);
  Serial.write(0x37);
  delay(100);
  Serial.write(0xAA);
  Serial.write(0x00);
  delay(100);
  Serial.write(0xAA);
  Serial.write(0x21);
  delay(100);

  Serial.println("Ready !");
}

void Groupe1()
{
  Serial.write(0xAA);
  Serial.write(0x00);
  delay(100);
  Serial.write(0xAA);
  Serial.write(0x21);
  delay(100);
  msg=0;
}

void Groupe2()
{
  Serial.write(0xAA);
  Serial.write(0x00);
  delay(100);
  Serial.write(0xAA);
  Serial.write(0x22);
  delay(100);
  msg=0;
}

void Groupe3()
{
  Serial.write(0xAA);
  Serial.write(0x00);
  delay(100);
  Serial.write(0xAA);
  Serial.write(0x23);
  delay(100);
  msg=0;
}

void lecteurOff()
{
  delay(2000);
  digitalWrite(9, LOW);
}

void Blanc()
{
  for(int i=0; i<=NombreLed-1; i++)
  {
    Leds[i] = CRGB(40,40,40);
    FastLED.show();
  }
}

void Bleu()
{
  for(int i=0; i<=NombreLed-1; i++)
  {
    Leds[i] = CRGB(0,0,50);
    FastLED.show();
  }
}

void Rouge()
{
  for(int i=0; i<=NombreLed-1; i++)
  {
    Leds[i] = CRGB(50,0,0);
    FastLED.show();
  }
}

void Magenta()
{
  for(int i=0; i<=NombreLed-1; i++)
  {
    Leds[i] = CRGB(33,0,25);
    FastLED.show();
  }
}

void Rien()
{
  for(int i=0; i<=NombreLed-1; i++)
  {
    Leds[i] = CRGB(0,0,0);
    FastLED.show();
  }
}

void Cyan()
{
  for(int i=0; i<=NombreLed-1; i++)
  {
    Leds[i] = CRGB(0,50,50);
    FastLED.show();
  }
}
void Rose()
{
  for(int i=0; i<=NombreLed-1; i++)
  {
    Leds[i] = CRGB(50,0,40);
    FastLED.show();
  }
}
void Jaune()
{
  for(int i=0; i<=NombreLed-1; i++)
  {
    Leds[i] = CRGB(50,50,0);
    FastLED.show();
  }
}
void Vert()
{
  for(int i=0; i<=NombreLed-1; i++)
  {
    Leds[i] = CRGB(0,50,0);
    FastLED.show();
  }
}
void Tout()
{
  Rouge();
  Rose();
  Bleu(); 
  Cyan();
  Vert();
  Jaune();
  Blanc();
  Rien();
  delay(2000);
}



void LumiereSurBouton()
{
  if(!EBHaut){
    LumiereBouton++;
    delay(100);
  }
  if(LumiereBouton == 9) LumiereBouton = 0;

  switch(LumiereBouton)
  {
    case 0: Rien(); break;
    case 1: Blanc(); break;
    case 2: Bleu(); break;
    case 3: Rouge(); break;
    case 4: Cyan(); break;
    case 5: Jaune(); break;
    case 6: Rose(); break;
    case 7: Vert(); break;
    case 8: Tout(); break;
    default: break;
  }
}

void setup() {
  FastLED.addLeds<WS2812, PinLed, GRB>(Leds, NombreLed);

  pinMode(BoutonHaut, INPUT_PULLUP);
  pinMode(BoutonBas, INPUT_PULLUP);

  Serial.begin(9600);

  lecteur.speakerPin = 9;
  if(!SD.begin(SDpin))
  {
    Serial.println("Connection Failed");
    return;
  }
  else Serial.println("SD is ready");

  lecteur.volume(2);
  lecteur.play("JeSuisLa.wav");
  delay(1000);
  if(!digitalRead(BoutonBas)) reset();
  digitalWrite(9, LOW);
  Rien();
  beginVoix();

}

void loop() 
{
  EBHaut = digitalRead(BoutonHaut);
  EBBas = digitalRead(BoutonBas);

  if(!EBBas) GroupeRappel();

  if(Serial.available()) msg = Serial.read();
  
    
     if(msg == 0x12 && !Voix)         ///////Si on tape des mains, la lumière s'allume en blanc
     {
      Serial.println("Clap");
      if(!LumiereOn){
       Blanc(); 
       LumiereOn = true;}
      else if(LumiereOn){
       Rien(); 
       LumiereOn = false;}
      
      msg = 0;
     }


     if(Serial.available()) msg = Serial.read();
     
     if(msg == 0x11 && !Voix && !LumiereOn)
     {
      BureauAnswer = random(0,3);
      Voix = true;
      if(BureauAnswer == 1)lecteur.play("OuiMr.wav");
      if(BureauAnswer == 2)lecteur.play("JeSuisLa.wav");
      lecteurOff();
      
     }
     if(Serial.available()) msg = Serial.read();
     
     if(msg == 0x13 && Voix && !ChoixLum && !ChoixJour)
     {
        lecteur.play("QuelLum.wav");
        lecteurOff();
        ChoixLum = true;
        ChoixJour = false;
        Groupe2();
        Groupe = 2;
        msg = 0;
        delay(1500);
        Serial.println("Test concluant");
     }

            if(msg == 0x11 && ChoixLum)
                   {
                    lecteur.play("MBleu.wav");
                    delay(3000);
                    digitalWrite(9, LOW);
                    Bleu();
                    msg = 0;
                    ChoixLum = false;
                    Voix = false;
                    Groupe1();
                    Groupe = 1;
                    delay(1500);
                   }

                   if(msg == 0x12 && ChoixLum)
                   {
                    lecteur.play("MBlanche.wav");
                    lecteurOff();
                    Blanc();
                    msg = 0;
                    ChoixLum = false;
                    Voix = false;
                    Groupe1();
                    Groupe = 1;
                    delay(1500);
                  }

                   if(msg == 0x13 && ChoixLum)
                   {
                    lecteur.play("MRouge.wav");
                    lecteurOff();
                    Rouge();
                    msg = 0;
                    ChoixLum = false;
                    Voix = false;
                    Groupe1();
                    Groupe = 1;
                    delay(1500);
                   }

                   if(msg == 0x14 && ChoixLum)
                   {
                    lecteur.play("MBouton.wav");
                    delay(3300);
                    digitalWrite(9, LOW);
                    while(EBBas)
                    {
                      EBBas = digitalRead(BoutonBas);
                      EBHaut = digitalRead(BoutonHaut);
                     LumiereSurBouton(); 
                    }
                    
                    msg = 0;
                    ChoixLum = false;
                    Voix = false;
                    Groupe1();
                    Groupe = 1;
                    delay(1500);
                   }

                   if(msg == 0x15 && ChoixLum)
                   {
                    lecteur.play("Mmh.wav");
                    Rose();
                    delay(28000);
                    digitalWrite(9, LOW);
                    msg = 0;
                    ChoixLum = false;
                    Voix = false;
                    Groupe1();
                    Groupe = 1;
                    delay(1500);
                   }



    if(Serial.available()) msg = Serial.read();


     if(msg == 0x15 && Voix && !ChoixJour && !ChoixLum)
     {
        lecteur.play("QuelJour.wav");
        lecteurOff();
        ChoixJour = true;
        ChoixLum = false;
        Groupe3();
        Groupe = 3;
        msg = 0;
        delay(1500);
        Serial.println("Affaire");
     }

                 if(msg == 0x11 && ChoixJour)
                 {
                  lecteur.play("VLundi.wav");
                  delay(9000);
                  digitalWrite(9, LOW);
        
                  msg = 0;
                  ChoixJour = false;
                  Voix = false;
                  Groupe1();
                  Groupe = 1;
                  delay(1300);
                 }
                 
                 if(msg == 0x12 && ChoixJour)
                 {
                  Serial.println("Mardi");
                  lecteur.play("VMardi.wav");
                  delay(25000);
                  digitalWrite(9, LOW);
                  msg = 0;
                  ChoixJour = false;
                  Voix = false;
                  Groupe1();
                  Groupe = 1;
                  delay(1500);
                 }
                 if(msg == 0x13 && ChoixJour)
                 {
                  lecteur.play("VMercre.wav");
                  delay(9300);
                  digitalWrite(9, LOW);
        
                  msg = 0;
                  ChoixJour = false;
                  Voix = false;
                  Groupe1();
                  Groupe = 1;
                  delay(1500);
                 }
                 if(msg == 0x14 && ChoixJour)
                 {
                  lecteur.play("VJeudi.wav");
                  delay(18500);
                  digitalWrite(9, LOW);
        
                  msg = 0;
                  ChoixJour = false;
                  Voix = false;
                  Groupe1();
                  Groupe = 1;
                  delay(1500);
                 }
                 if(msg == 0x15 && ChoixJour)
                 {
                  lecteur.play("VVendred.wav");
                  delay(19300);
                  digitalWrite(9, LOW);
        
                  msg = 0;
                  ChoixJour = false;
                  Voix = false;
                  Groupe1();
                  Groupe = 1;
                  delay(1500);
                 }


       

  

  
}
