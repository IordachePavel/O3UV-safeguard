#include <U8glib.h>
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <SPI.h>
#include <SD.h>

U8GLIB_ST7920_128X64 u8g(A2, A3, 12, 5);
bool vent = false;
String uv_status = "OFF", alarm = "ON";
float con = 0.01;  
File LGF;
int page = 1, cursorPos = 2, cursorPos2 = 1, pageDepth = 0;
tmElements_t tm;
int Day = tm.Day, Month = tm.Month, Year = tm.Year + 1970, Hour = tm.Hour, Minute = tm.Minute, contorTimeChange = 1;
bool door_trig=false;

int Ora, Minut, Secunda, Luna, Zi, An;
void draw(void) {
  u8g.setFont(u8g_font_chikita);
}
void backarrowicon(void) {
  u8g.drawLine(120, 8, 124, 8);
  u8g.drawLine(124, 8, 124, 13);
  u8g.drawLine(120, 13, 124, 13);
  u8g.drawTriangle(123, 11, 123, 5, 120, 8);
}

void clockicon(int x, int y) {
  u8g.drawFrame(x + 4, y + 4, 9, 9);
  u8g.drawLine(x + 8, y + 8, x + 8, y + 4);
  u8g.drawLine(x + 8, y + 8, x + 10, y + 10);
}


void drawBox2(int x, int y){
    u8g.drawFrame(x,y,16,16);
  }

void inserticon(void) {
}
void graphicon(int x, int y) {
  u8g.drawFrame(x + 4, y + 4, 7, 9);
  u8g.drawLine(x + 6, y + 6, x + 6, y + 6);
  u8g.drawLine(x + 6, y + 8, x + 6, y + 8);
  u8g.drawLine(x + 6, y + 10, x + 6, y + 10);
  u8g.drawLine(x + 8, y + 6, x + 8, y + 6);
  u8g.drawLine(x + 8, y + 8, x + 8, y + 8);
  u8g.drawLine(x + 8, y + 10, x + 8, y + 10);
}


void TimeUnderTen(int chestie) {
  if (chestie < 10)
    u8g.print('0');
  u8g.print(chestie);
}

void updateDate() {
  u8g.setPrintPos(3, 15);
  u8g.print("Nivel Ozon: ");
    u8g.print(con);
  u8g.print(" mg/m^3");
  if (con >= 0.09)
    u8g.print("!");
  u8g.setPrintPos(3, 25);
  u8g.print("UV: ");
  u8g.print(uv_status);
  u8g.print("           Vent: ");
  if (con >= 0.07 || vent)
    u8g.print("ON");
  else
    u8g.print("OFF");
}

void updateData(void) {

  if (RTC.read(tm) && page == 1) {
    u8g.setPrintPos(3, 50);
    u8g.print("Data: ");
    TimeUnderTen(Zi);
    u8g.print("/");
    TimeUnderTen(Luna);
    u8g.print("/");
    u8g.print(tmYearToCalendar(An));
    u8g.setPrintPos(3, 60);
    u8g.print("Ora: ");
    TimeUnderTen(Ora);
    u8g.print(":");
    TimeUnderTen(Minut);
  }
  Secunda++;
  if (Secunda >= 3000) {
    Secunda = 1;
    Minut++;
  }
  if (Minut >= 60) {
    Minut = 1;
    Ora++;
  }
  if (Ora == 24) {
    Ora = 0;
    Zi++;
  }
}

void frame(void) {
  u8g.drawFrame(0, 0, 128, 64);
  u8g.drawLine(112, 0, 112, 64);
}

bool getTime(const char *str) {
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

bool getDate(const char *str) {
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



void page1(void) {
  u8g.drawFrame(0, 0, 128, 64);
}

void setup(void) {
   Serial.begin(9600);
  if (!SD.begin(4)) {
    Serial.println("SD INIT FAIL");
    while (1);
  }
  Serial.println("SD INIT OK");
  
  const int enablePin = 10;
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(A0, OUTPUT);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, OUTPUT);  
  pinMode(enablePin, OUTPUT);
  delay(3000);
  digitalWrite(enablePin, LOW);

  bool parse = false;
  bool config = false;

  if (getDate(__DATE__) && getTime(__TIME__)) {
    parse = true;
    if (RTC.write(tm)) {
      config = true;
    }
  }

  Serial.begin(9600);
  while (!Serial)
    ;
  delay(200);

  if (u8g.getMode() == U8G_MODE_R3G3B2)
    u8g.setColorIndex(255);
  else if (u8g.getMode() == U8G_MODE_GRAY2BIT)
    u8g.setColorIndex(3);
  else if (u8g.getMode() == U8G_MODE_BW)
    u8g.setColorIndex(1);
  Ora = tm.Hour;
  Minut = tm.Minute;
  Secunda = tm.Second / 2 * 100;
  Zi = tm.Day;
  Luna = tm.Month;
  An = tm.Year;
}


void drawBox(int x, int y) {
  u8g.drawFrame(x - 5, y - 10, 15, 15);
}

void ControlPage() {
  int boxPos = 1;
  draw();
  frame();
  backarrowicon();
  graphicon(112, 16);
  drawBox2(112,16);
  struct posXY {
    int x, y;
  } posControl[7];
  posControl[1].x = 50, posControl[1].y = 15, posControl[2].x = 70, posControl[2].y = 15, posControl[3].x = 50, posControl[3].y = 35, posControl[4].x = 70, posControl[4].y = 35, posControl[5].x = 50, posControl[5].y = 55, posControl[6].x = 70, posControl[6].y = 55;
  drawBox(posControl[boxPos].x, posControl[boxPos].y);
  u8g.setPrintPos(50, 15);
  u8g.print("1    2");
  u8g.setPrintPos(50, 35);
  u8g.print("3    4");
  u8g.setPrintPos(50, 55);
  u8g.print("5    6");

  if (digitalRead(7) == false) {
    if (boxPos < 6)
      boxPos++;
    delay(500);
  }

  if (digitalRead(8) == false) {
    if (boxPos > 1)
      boxPos--;
    delay(500);
  }

  if (digitalRead(9) == false) {
    page == 1;
  }

  if (digitalRead(6) == false) {

    int nr = 0;

    if (boxPos == 1 || boxPos == 2)
      nr = 2;
      else if (boxPos == 3 || boxPos == 4)
        nr = 3;
    else nr = 4;
    Serial.println(nr);
    delay(250);
    if (boxPos % 2 == 0)
      Serial.println(5);
    else
      Serial.println(6);
    delay(1500);
     Serial.println(1);
     delay(250);
  }
}

void HomePage() {
  draw();
  frame();
  backarrowicon();
  if (cursorPos == 2)
    drawBox2(112, 16);
  else
    clockicon(112, 16);
  if (cursorPos == 3)
    drawBox2(112, 32);
  else
    graphicon(112, 32);
  u8g.drawLine(0, 40, 111, 40);
  updateDate();
}





void PageTranslator() {
  if (page == 1)
    HomePage();
  if (page == 2)
    ControlPage();
}

void cursorAndPage() {
  if (page == 1) {
    if (digitalRead(6) == false) {
      page = 2;
      delay(500);
      pageDepth++;
    }
  }

  if (digitalRead(9) == false) {
      page = 1;
  }
}
void Ventilator() {
  int k;
  if (con < 0.07 || vent)
    digitalWrite(A0, HIGH);
  else
    digitalWrite(A0, LOW), vent = 1;
}




void loop(void) {
  while(Serial.available()){
        float pwmval = Serial.parseFloat();
        Serial.println(pwmval);
        if(pwmval == 7){
          uv_status="NO UV";
        } else if (pwmval == 8)
          uv_status="UV PRESENT";
          else if (pwmval == 9)
            vent=false;
          else if (pwmval == 10)
            vent=true;
          else if (pwmval!=0 && pwmval<=1)
            con= pwmval;
            delay(500);
      }

      if (digitalRead(2)!=false){
    if (door_trig==false){
    door_trig=true;
    tone(3, 500);
    delay (500);
    tone(3, 400);
    delay (500);
    tone(3, 550);
    delay (750);
    noTone(3);
    String DateAndTime, DateS, YearS, MonthS, DayS, HourS, MinutesS;
    int Y,M,D,H,MIN;
    YearS=String(Y);
    MonthS=String(M);
    DayS=String(D);
    HourS=String(H);
    MinutesS=String(MIN);
    DateS=YearS+"_"+MonthS+"_"+DayS;
    DateAndTime=YearS+","+MonthS+","+DayS+" "+HourS+":"+MinutesS+" ";
    String MakeExtensionTXT;
    MakeExtensionTXT=DateS+".txt";
    Serial.println(MakeExtensionTXT);
    Serial.println(String(An));
    Serial.println(String(Luna));
    Serial.println(Zi);
    Serial.println(String(Ora));
    Serial.println(Minut);
    LGF = SD.open(DateS, FILE_WRITE);
    if (LGF) {
    LGF.println(DateAndTime+" "+con);
    LGF.close();
  } else {
    Serial.println("WRT ERR");
  }
    }
   }
   else{
    door_trig=false;
    delay(2000);
   }
  u8g.firstPage();
  do {
    Ventilator();
    cursorAndPage();
    updateData();
    PageTranslator();
  } while (u8g.nextPage());
  {
  }
}
