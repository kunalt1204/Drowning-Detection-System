#include <LiquidCrystal.h>
#include<SoftwareSerial.h>

//SoftwareSerial beat(13, 8);
//Definitions  

LiquidCrystal lcd(A3,A2,12,11,10,9);

const int HR_RX = 7;
byte oldSample, sample;
int count = 0;
int beats=0;
int x=30;
int y=120; 
unsigned long pulse;
unsigned int i=1;
long measurementStartTime = 0;
int e_chk = 0;
int menu = 1;
int menu_sel = 1;
int distress = 0;
int ret = 0;
long time1;
long time2;
int last_rate;
const int HELP = 4;
const int entr=0;
const int bck=1;
const int up=2;
const int dwn=3;     // the number of the pushbutton pin
const int helpled = 6;
const int mtx= A5;
const int dtx= A4;

const int normal =  5;

void setup()
{
   lcd.begin(16,2);
//  beat.begin(9600);
 // Serial.begin(9600);
  pinMode (HR_RX, INPUT);  //Signal pin to input  
  pinMode(entr,INPUT); 
  pinMode(bck,INPUT);
  pinMode (up,INPUT);
  pinMode (dwn,INPUT);
  pinMode(HELP, INPUT);
  pinMode(normal,OUTPUT);

  pinMode(helpled, OUTPUT);
  pinMode (mtx,OUTPUT);
  pinMode (dtx,OUTPUT);

  digitalWrite (normal,LOW);
  digitalWrite (helpled,HIGH);
  lcd.print(" ANTI-DROWNING ");
  lcd.setCursor(0, 1);
  lcd.print("     SYSTEM     ");
  delay(1000);
  digitalWrite(mtx,LOW);
  digitalWrite(dtx,LOW);
} 

void loop()
{
  while(menu == 1)
  {
    if(menu_sel == 1)
    {
      lcd.clear();
      lcd.print("->>1.Start");
      lcd.setCursor(0, 1);
      lcd.print("   2.Setting");
    }
    else
    {
      lcd.clear();
      lcd.print("   1.Start");
      lcd.setCursor(0, 1);
      lcd.print("->>2.Setting");
    }
    if(digitalRead(entr) == LOW)
      menu = 0;
    if(digitalRead(up) == 0)
      menu_sel = 1;
    if(digitalRead(dwn) == 0)
      menu_sel = 2;
      
     e_chk = 0;
     digitalWrite(normal,LOW);
     digitalWrite(dtx,LOW);
     digitalWrite(mtx,LOW);
     digitalWrite(helpled,HIGH);
    delay(500);
  }

  if(menu_sel == 1)
  {
    lcd.clear();
    lcd.print("Initiallizing.......");
    while(digitalRead(bck) == HIGH)
    {
      hbt();
      if(beats < x || beats > y)
      {
        e_chk++;
        if(e_chk > 3)
          {
            digitalWrite(normal,HIGH);
            digitalWrite(dtx,HIGH);
            lcd.clear();
            lcd.print("EMERGENCY");
            digitalWrite(helpled,LOW);
            digitalWrite(normal,HIGH);
            delay(200);
          }
       }
       else
       {
        e_chk = 0;
        digitalWrite(normal,LOW);
        digitalWrite(dtx,LOW);
        digitalWrite(helpled,HIGH);
       }
       if(digitalRead(HELP) == LOW)
       {
        distress = 1;
        break;
       }
     }
     menu = 1;
     menu_sel = 1;
     delay(500);
  }

  if(menu_sel == 2 && distress == 0)
  {
    lcd.clear(); 
    lcd.print("Set lower limit");
    lcd.setCursor(8,1);
    lcd.print(x);
    delay(1000);
    do
    {
      lcd.setCursor(8,1);
      lcd.print(x);
      if (digitalRead(up)==LOW)
      {
        x = x+10;
        if(x>70)
        {
          x=30;
        }
        delay(200);
      }
      else if(digitalRead(dwn) == LOW)
      {
        x = x-10;
        if(x<30)
        {
          x=70;
        }
        delay(200);
      }
    }while(digitalRead(entr == HIGH));
    delay(500);

    lcd.clear(); 
    lcd.print("Set higher limit");
    lcd.setCursor(8,1);
    lcd.print(y);
    lcd.print("  ");
    do
    {
      lcd.setCursor(8,1);
      lcd.print(y);
      lcd.print("  ");
      if (digitalRead(up)==LOW)
      {
        y = y+10;
        if(y>200)
        {
          y=70;
        }
        delay(500);
      }
      else if(digitalRead(dwn) == LOW)
      {
        y = y-10;
        if(y<70)
        {
          y=200;
        }
        delay(500);
      }
    }while(digitalRead(entr == HIGH));
    delay(500);

    menu = 1;
  }

  if(distress == 1 || digitalRead(HELP) == LOW)
  {
    lcd.clear();
    lcd.print("Distress Signal");
    lcd.setCursor(0,1);
    lcd.print("Sent!");
    digitalWrite(helpled,LOW);
    digitalWrite(mtx,HIGH);
    digitalWrite(normal,HIGH);
    while(digitalRead(bck) == HIGH);
    
    menu = 1;
    distress = 0;
    digitalWrite(helpled,HIGH);
    digitalWrite(mtx,LOW);
    digitalWrite(normal,LOW);
    while(digitalRead(bck) == HIGH);
  }

  
}


int hbt()
{
  int k=0;
//  lcd.clear();
//  lcd.print("Please wait.......");
  long t_out1 = millis();
  bool zero = false;
  while(k<10)
  {
   if(digitalRead(HR_RX))
   {
    t_out1 = millis();
    if(k==0)
    time1=millis();
    k++;
    while(digitalRead(HR_RX));
   }
    if(millis() - t_out1 > 2000)
    {
      zero = true;
      break;
    }
  }
  if(!zero)
  {
    time2=millis();
    beats=time2-time1;
    beats=beats/10;
    beats=60000/beats;
    if(beats >= 200)
    {
      beats = last_rate;
    }
//    lcd.clear();   
  }
  else
  {
    k=0;
    beats=0;
  }
  lcd.clear();
  lcd.print(F("Beats/minute:"));
  lcd.print(beats);
//  beat.println(beats);
//  lcd.write(1); 
  lcd.print("   ");
  last_rate = beats;     
  k=0;
}