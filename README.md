# O3UV-safeguard
Sistem de monitorizare și control al procesului de dezinfectare al unei încăperi

Sistemul monitorizează procesul de dezinfectare a unei încăperi detectând prezenta lumini ultraviolete, măsurând concentrația ozonului din aer și se ocupă de aerisirea încăperii odată dezinfectată. În același timp, O3-UV Safeguard înregistrează toate intrările în cameră alături de concentrația de ozon din aer la momentul dat.

# Componente

  Ansamblul este organizat în module (Terminal, Ventilator, Senzor O3 și interfață bluetooth pentru transmiterea datelor și controlul lămpii cu UV).
 ### Terminalul
  Terminalul are la bază placa de dezvoltare Arduino Uno, ce dispune de un modul RTC cu logger pe card SD, un ecran 128x64, un senzor de tip reed pentru detectarea deschiderii ușii. Acesta interceptează măsurătorile trimise de senzor, unde urmează să fie stocate și trimise mai departe la interfața bluetooth. 
 ### Senzorul
  Pentru măsurarea concentrației de ozon se folosește senzorul Winsen MQ-131, iar pentru detectarea prezenței ultravioletelor, se folosește un divizor de tensiune cu un fotorezistor care are o lentilă specială montată în fața acestiua. Măsurătorile senzorului sunt preluate de o placă arduino și trimise la terminal.
 ### Interfața bluetooth
  Având la bază modulul bluetooth HC-05, acesta permite transmiterea datelor legate de concentrația de ozon din încăpera în care este amplasată senzorul, pe telefonul mobil. Totodată, acesta dispune dintr-un mecanism de DVD-ROM refolosit și un servomotor pentru acționarea telecomenzii care controlează lampa UV, permițând utilizatorului să controleze tot procesul din telefon.
### Ventilator
  După finalizarea procesului de dezinfectare, ozonul este evacuat de către acesta în mediul exterior, scâpând de necesitatea deschideri unei ferestre de către un individ, scâzând riscul de intoxicare cu ozon.
   

# Schema electrică
### Terminal
![image_2023-07-17_144517200](https://github.com/IordachePavel/O3UV-safeguard/assets/138784190/bf1bfa06-d77d-4cbb-aa20-1ec74090d1ce)
### Senzor
![image_2023-07-17_144614067](https://github.com/IordachePavel/O3UV-safeguard/assets/138784190/3716f78b-f16d-4e41-897b-99932d49fd41)
### Interfața Bluetooth 
![image_2023-07-17_144651736](https://github.com/IordachePavel/O3UV-safeguard/assets/138784190/1cbef4ce-24ff-458c-a6aa-55af5e8850bc)


# Cerințe de sistem:
Arduino IDE
Librăriile pentru program:
- Servo.h
- SoftwareSerial.h
- MQ131.h
- U8glib.h
- Wire.h
- TimeLib.h
- DS1307RTC.h
- SPI.h
- SD.h
