#include "U8glib.h"
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <SPI.h>
#include <SD.h>

U8GLIB_ST7920_128X64 u8g(13, 11, 12, 5);
int logCounter=0;
int mongoose=0;
File myFile;
String stat="Ok",uv_status="Ok",senzor="Connectat",alarm="ON",vent="OFF";
float con=0.01;//concentrația de ozon ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int enablePin=10;
bool updated=false;
int logs[10000]={0},ctTimp,page=1,cursorPos=2,cursorPos2=1,pageDepth=0;
tmElements_t tm;
int Day=tm.Day, Month=tm.Month, Year=tm.Year+1970, Hour=tm.Hour, Minute=tm.Minute,contorTimeChange=1;
int Timp=tm.Minute,Tst=tm.Minute;
int kGraph[4]={1,2,3,4};
int Ora,Minut,Secunda,Luna,Zi,An;
void draw(void) {
// graphic commands to redraw the complete screen should be placed here
u8g.setFont(u8g_font_chikita);


}
void backarrowicon (void){
 u8g.drawLine(120,8,124,8);
 u8g.drawLine(124,8,124,13);
 u8g.drawLine(120,13,124,13);
 u8g.drawTriangle(123,11, 123,5, 120,8);
  }
void settingsicon (int x, int y){
  u8g.drawBox(x+7,y+3,2,2);
  u8g.drawBox(x+7,y+7,2,2);
  u8g.drawBox(x+7,y+11,2,2);
}
void settingsiconDark(int x, int y){
  u8g.setColorIndex(1);
  u8g.drawBox(x,y,16,16);
  u8g.setColorIndex(0);
  u8g.drawBox(x+7,y+3,2,2);
  u8g.drawBox(x+7,y+7,2,2);
  u8g.drawBox(x+7,y+11,2,2);
  u8g.setColorIndex(1);
}
void clockicon (int x, int y){
  u8g.drawFrame(x+4,y+4,9,9);
  u8g.drawLine(x+8,y+8,x+8,y+4);
  u8g.drawLine(x+8,y+8,x+10,y+10);
}
void clockiconDark(int x, int y){
  u8g.setColorIndex(1);
  u8g.drawBox(x,y,16,16);
  u8g.setColorIndex(0);  
  u8g.drawFrame(x+4,y+4,9,9);
  u8g.drawLine(x+8,y+8,x+8,y+4);
  u8g.drawLine(x+8,y+8,x+10,y+10);
  u8g.setColorIndex(1);

}
void inserticon (void){
  
}
void graphicon (int x, int y){
  u8g.drawLine(x+4,y+12,x+4,y+4);
  u8g.drawLine(x+4,y+12,x+12,y+12);
  u8g.drawLine(x+4,y+12,x+8,y+8);
  u8g.drawLine(x+7,y+7,x+10,y+4);
}

void graphiconDark(int x, int y){
  u8g.setColorIndex(1);
  u8g.drawBox(x,y,16,16);
  u8g.setColorIndex(0); 
  u8g.drawLine(x+4,y+12,x+4,y+4);
  u8g.drawLine(x+4,y+12,x+12,y+12);
  u8g.drawLine(x+4,y+12,x+8,y+8);
  u8g.drawLine(x+7,y+7,x+10,y+4);
  u8g.setColorIndex(1);
 }
  
void logUpdate(void){
      if(Timp!=tm.Minute)
      {
        ctTimp++;
        logs[ctTimp]=con;
        Timp=tm.Minute;
        for(int i=0;i<3;i++)
        kGraph[i]=kGraph[i+1];
        kGraph[3]++;
      }    
 }


  void clockMode(){
  
      bool parse=false;
  bool config=false;

  // get the date and time the compiler was run
  if (getDate(__DATE__) && getTime(__TIME__)) {
    parse = true;
    // and configure the RTC with this info
    if (RTC.write(tm)) {
      config = true;
    }
  }

  while (!Serial) ; // wait for Arduino Serial Monitor
  if (parse && config) {
  } else if (parse) {
    Serial.println("DS1307 Communication Error :-{");
    Serial.println("Please check your circuitry");
  } else {
    Serial.print("Could not parse info from the compiler, Time=\"");
    Serial.print(__TIME__);
    Serial.print("\", Date=\"");
    Serial.print(__DATE__);
    Serial.println("\"");
  }
  if(RTC.read(tm)){   
  u8g.setPrintPos(50,30);
  if(Ora<10)
  u8g.print('0');
  u8g.print(Ora);
  u8g.print(":");
  if(Minut<10)
  u8g.print('0');
  u8g.print(Minut);
  u8g.print(":");
  if(2*Secunda/100<10)
  u8g.print("0");
  u8g.print(2*Secunda/100);
  u8g.setPrintPos(40,45);
  if(Zi<10)
  u8g.print("0");
  u8g.print(Zi);
  u8g.print("/");
    if(Luna<10)
  u8g.print("0");
  u8g.print(Luna);
  u8g.print("/");
  u8g.print(tmYearToCalendar(An));}
    }


void updateDate(){
  u8g.setPrintPos( 3, 15);
  u8g.print("Nivel Ozon: ");
  if(con<0.01)
  u8g.print("<0.01");
  else
  u8g.print(con);
  u8g.print(" mg/m^3");
  if(con>=9.816)
  u8g.print("!");
  u8g.setPrintPos(3,25);
  u8g.print("UV: ");
  u8g.print(uv_status);
  u8g.print("           Vent: ");
  if(con>=0.07)
  u8g.print("ON");
  else
  u8g.print("OFF");
  u8g.setPrintPos(3,35);
  u8g.print("Senzor: ");
  u8g.print(senzor);
  }

void updateData(void){

  if(RTC.read(tm)&&page==1){
  u8g.setPrintPos(3,50);
  u8g.print("Data: ");
       TimeUnderTen(Zi);
  u8g.print("/");
       TimeUnderTen(Luna);
  u8g.print("/");
  u8g.print(tmYearToCalendar(An));
  u8g.setPrintPos(3,60);  
  u8g.print("Ora: ");
      TimeUnderTen(Ora);
  u8g.print(":");
       TimeUnderTen(Minut);
  }
 // delay (100);
  Secunda++;
  if(Secunda>=3000){
    Secunda=1;
    Minut++;
  }
  if(Minut>=60){
    Minut=1;
    Ora++;
    }
    if(Ora==24)
    {
      Ora=0;
      Zi++;}
 }

void frame (void){
  u8g.drawFrame (0,0,128,64);
  u8g.drawLine (112,0,112,64);
  u8g.drawLine(112,16,127,16);
  u8g.drawLine(112,32,127,32);
  u8g.drawLine(112,48,127,48);
  
}

void drawSettings(){
  u8g.drawLine(0,21,112,21);
  u8g.drawLine(0,42,112,42);
   u8g.setPrintPos(3,16);
  if(cursorPos2==1){
  u8g.drawBox(0,0,112,21);
  u8g.setColorIndex(0); 
  u8g.print("Set time");
  u8g.setColorIndex(1);
  }
  else
  u8g.print("Set time");
  u8g.setPrintPos(3,35);
  if(cursorPos2==2){
  u8g.drawBox(0,21,112,21);
  u8g.setColorIndex(0);
  u8g.print("Card Logs");
  u8g.setColorIndex(1);  
   }
  else
  u8g.print("Card Logs");
  u8g.setPrintPos(3,56);
  if(cursorPos2==3){
  u8g.drawBox(0,42,112,22);
  u8g.setColorIndex(0);
  u8g.print("Alarm: ");
  u8g.print(alarm);
  u8g.setColorIndex(1); 
    }
  else{
  u8g.print("Alarm: ");
  u8g.print(alarm);} 
}

bool getTime(const char *str)
{
  int Hour, Min, Sec;

  if (sscanf(str, "%d:%d:%d", &Hour, &Min, &Sec) != 3) return false;
  tm.Hour = Hour;
  tm.Minute = Min;
  tm.Second = Sec;
  return true;
}

const char *monthName[12] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

bool getDate(const char *str)
{
  char Month[12];
  int Day, Year;
  uint8_t monthIndex;

  if (sscanf(str, "%s %d %d", Month, &Day, &Year) != 3) return false;
  for (monthIndex = 0; monthIndex < 12; monthIndex++) {
    if (strcmp(Month, monthName[monthIndex]) == 0) break;
  }
  if (monthIndex >= 12) return false;
  tm.Day = Day;
  tm.Month = monthIndex + 1;
  tm.Year = CalendarYrToTm(Year);
  return true;
}



void page1(void){
  u8g.drawFrame(0,0,128,64);
}

void setup(void) {
  Serial.begin(9600);
  pinMode(6,INPUT_PULLUP);
  pinMode(7,INPUT_PULLUP);
  pinMode(8,INPUT_PULLUP);
  pinMode(9,INPUT_PULLUP);
  pinMode(A0,OUTPUT);
  pinMode(2,INPUT_PULLUP);//buton usa
  pinMode(3,OUTPUT);//buzzer
   pinMode(enablePin, OUTPUT);
  delay(3000);
  digitalWrite(enablePin, LOW);

    bool parse=false;
  bool config=false;

  // get the date and time the compiler was run
  if (getDate(__DATE__) && getTime(__TIME__)) {
    parse = true;
    // and configure the RTC with this info
    if (RTC.write(tm)) {
      config = true;
    }
  }

  Serial.begin(9600);
  while (!Serial) ; // wait for Arduino Serial Monitor
  delay(200);
/*  if (parse && config) {
    Serial.print("DS1307 configured Time=");
    Serial.print(__TIME__);
    Serial.print(", Date=");
    Serial.println(__DATE__);
  } else if (parse) {
    Serial.println("DS1307 Communication Error :-{");
    Serial.println("Please check your circuitry");
  } else {
    Serial.print("Could not parse info from the compiler, Time=\"");
    Serial.print(__TIME__);
    Serial.print("\", Date=\"");
    Serial.print(__DATE__);
    Serial.println("\"");
  }*/
  
// assign default color value
if ( u8g.getMode() == U8G_MODE_R3G3B2 )
u8g.setColorIndex(255); // white
else if ( u8g.getMode() == U8G_MODE_GRAY2BIT )
u8g.setColorIndex(3); // max intensity
else if ( u8g.getMode() == U8G_MODE_BW )
u8g.setColorIndex(1); // pixel on
Ora=tm.Hour;
Minut=tm.Minute;
Secunda=tm.Second/2*100;
Zi=tm.Day;
Luna=tm.Month;
An=tm.Year;

}

void TimeUnderTen(int chestie){
    if(chestie<10)
      u8g.print('0');
      u8g.print(chestie);
  }

void drawTimeSettings(){
  draw();
  frame();
  backarrowicon();
  settingsiconDark(112,16);
  u8g.drawLine(0,16,112,16);
  u8g.setPrintPos(3,12);
  u8g.print("Time settings");
  u8g.setPrintPos(25,30);
  u8g.print("DD/MM/YYYY");
  u8g.setPrintPos(25,40);
  if(contorTimeChange==1){
     u8g.drawBox(25,32,10,10);
     u8g.setColorIndex(0);
     TimeUnderTen(Zi);
     u8g.setColorIndex(1);
    }
  else{
  TimeUnderTen(Zi);}
  u8g.print('/');
    if(contorTimeChange==2){    
     u8g.drawBox(38,32,10,10);
     u8g.setColorIndex(0);
      TimeUnderTen(Luna);
     u8g.setColorIndex(1);
      
      }
  else{
TimeUnderTen(Luna);}
  u8g.print('/');
    if(contorTimeChange==3){       
     u8g.drawBox(53,32,25,10);
     u8g.setColorIndex(0);
     u8g.print(An);
     u8g.setColorIndex(1);
      }
  else{
  if(contorTimeChange<3)
  u8g.print(An);
  else
  u8g.print(An+1970);
  }
  u8g.setPrintPos(25,50);
    if(contorTimeChange==4){
     u8g.drawBox(25,42,11,10);
     u8g.setColorIndex(0);
      TimeUnderTen(Ora);
     u8g.setColorIndex(1);
      }
      
  else{
   TimeUnderTen(Ora);}
  u8g.print(":");
    if(contorTimeChange==5){
     u8g.drawBox(38,42,12,10);
     u8g.setColorIndex(0);
     TimeUnderTen(Minut);
     u8g.setColorIndex(1);
      }
  else{
     TimeUnderTen(Minut);
}
  }

void HomePage(){
draw();
frame();
backarrowicon();
if(cursorPos==2)
clockiconDark(112,16);
else
clockicon(112,16);
if(cursorPos==3)
settingsiconDark(112,32);
else
settingsicon(112,32);
u8g.drawLine(0,40,111,40);
if(cursorPos==4)
graphiconDark(112,48);
else
graphicon(112,48);
updateDate();
}

void ClockPage(){
    draw();
    frame();
    clockMode();
    clockiconDark(112,16);
    backarrowicon();
}

void SettingPage(){
  draw();
  frame();
  settingsiconDark(112,16);
  drawSettings();
  backarrowicon();  
  }

void PageTranslator(){
//  if(updated){
  if(page==1)
  HomePage();
  if(page==2)
    ClockPage();
   if(page==3)
   SettingPage();
   if(page==4)
   //GraphPage();
   if(page==5)
   drawTimeSettings();
  //}
  updated=false;
  }

/*void cursorAndPage (){
  if(page==3){
          if (digitalRead(7)==false){
      if(cursorPos2<3)
        cursorPos2++;
        delay(200);
        updated=true;
      }
          if (digitalRead(8)==false){
      if(cursorPos2>1)
        cursorPos2--;
        delay(200);
        updated=true;
      }
        if (digitalRead(6)==false){
          if(cursorPos2==1||cursorPos2==2)
          page=cursorPos2+4,pageDepth++;
          if(page==5)
          contorTimeChange=1;
          else{
            if(alarm=="ON")
              alarm="OFF";
              else
              alarm="ON";
            }
          delay(200);
          updated=true;
        }
    }
if(page==1){
if (digitalRead(6)==false){
  page=cursorPos;
  delay(200);
  pageDepth++;
  updated=true;
}
if (digitalRead(7)==false){
if(cursorPos<4)
  cursorPos++;
  delay(200);
  updated=true;
}
if (digitalRead(8)==false){
  if(cursorPos>2)
    cursorPos--;
    delay(200);
    updated=true;
}
}

 if (digitalRead(9)==false){
           if(pageDepth==1){
           page=1;
}
           else if(pageDepth==2)
           page=3;
           u8g.setPrintPos(10,10);
           if(pageDepth>1)
           pageDepth--;
           u8g.print(page);
           u8g.print(pageDepth);
           updated=true;
      }
      if(page==5){
              if(contorTimeChange==1){
            if (digitalRead(7)==false){
              Day--;
              delay(200);
              updated=true;
            }
                if (digitalRead(8)==false){
              Day++;
              delay(200);
              updated=true;
            }
              if (digitalRead(6)==false){
                tm.Day=Day;
                contorTimeChange++;
                 delay(200);
                 updated=true;
                 }
                
             if(Day>31)
             Day=1;
             if(Day<1)
             Day=31;}
                 if(contorTimeChange==2){
                     if (digitalRead(7)==false){
                  Month--;
                  delay(200);
                  updated=true;
                }
                    if (digitalRead(8)==false){
                  Month++;
                  delay(200);
                  updated=true;
                }
                  if (digitalRead(6)==false){
                    tm.Month=Month;
                    contorTimeChange++;
                     delay(200);
                     updated=true;}
                    
                 if(Month>12)
                 Month=1;
                 if(Month<1)
                 Month=12;
                 updated=true;
                     }

                  if(contorTimeChange==3){
                     if (digitalRead(7)==false){
                Year--;
                delay(200);
                updated=true;
              }
                  if (digitalRead(8)==false){
                Year++;
                delay(200);
                updated=true;
              }
                if (digitalRead(6)==false){
                 Year=Year-1970;
                  tm.Year=Year;
                  contorTimeChange++;
                  delay(200);
                  updated=true; 
                    }
                  }
                   if(contorTimeChange==4){
                         if (digitalRead(7)==false){
                        Hour--;
                        delay(200);
                        updated=true;
                      }
                          if (digitalRead(8)==false){
                        Hour++;
                        delay(200);
                        updated=true;
                      }
                        if (digitalRead(6)==false){
                          tm.Hour=Hour;
                          contorTimeChange++;
                          delay(200);
                          updated=true;}
                      if(Hour<1)
                        Hour=24;
                      if(Hour>24)
                      Hour=1;
                   }
                    if(contorTimeChange==5){
                            if (digitalRead(7)==false){
                            Minute--;
                            delay(200);
                            updated=true;
                          }
                              if (digitalRead(8)==false){
                            Minute++;
                            delay(200);
                            updated=true;
                          }
                            if (digitalRead(6)==false){
                              tm.Minute=Minute;
                              contorTimeChange++;
                              delay(200);
                              updated=true;                                
                                }
                          if(Minute<0)
                            Minute=59;
                          if(Minute>59)
                          Minute=00;
                          }
    }
}*/


void Ventilator(){
  int k;
  if(page==1&&digitalRead(9)==false)
    digitalWrite(A0,LOW);
  else if(con<0.07)
  digitalWrite(A0,HIGH);
  else
  digitalWrite(A0,LOW);
  }

void Buzzer(){
  if(digitalRead(2)!=false)
  {tone(3,1000);
   delay(2000);
   logCounter++;

   String dataString="";
   dataString += String(logCounter); + ". " + String(Zi) + "/" + String(Luna) + "/" + String(An) + ":   " + String (Ora) + ":" + String(Minut) + ":" + String(Secunda) + "   Concentratie O3: " + String(con);  

   File dataFile = SD.open("datalogO3.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
  }
  else
  noTone(3);
  }


void loop(void) {
// picture loop
 while (Serial.available())                   //While have data at Serial port this loop executes
     {                
        float pwmval = Serial.parseFloat();  //Receive INTEGER value from Master throught RS-485
        if (pwmval)
        con=pwmval;
        Serial.println(con);
        delay (200);
      }
u8g.firstPage();
do {
  Ventilator();
  //cursorAndPage();
  Buzzer();
  updateData();
  PageTranslator();
} while(u8g.nextPage());{
  }
}
