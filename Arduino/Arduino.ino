#include <Wire.h>

int Pin1 = 10;
int Pin2 = 11;
int Pin3 = 12;
int Pin4 = 13;

int Step = 9;
int Rotation = 0;
int FullRotation = 4096;  // Ilość kroków wymagana do wykonania pełnego obrotu
int CurrentRotation = 0;
int RememberRotation = 0;
boolean Direction = false; // false - zgodnie z ruchem wskazówek zegara, true = przeciwnie

void setup()
{
  Wire.begin(1);                // Nadanie adresu I2C
  Wire.onReceive(receiveEvent); // Zainicjowanie funkcji, która wykona się gdy Arduino otrzyma dane
  Serial.begin(9600);           // Szybkość transmisji danych

  pinMode(Pin1, OUTPUT);        
  pinMode(Pin2, OUTPUT);
  pinMode(Pin3, OUTPUT);
  pinMode(Pin4, OUTPUT);
}

void loop()
{
  switch (Step) // Pierwsze 8 case'ów wymagane są do wykonania kroku
  {             // Ostatni case to zatrzymanie silnika
    case 0:
      digitalWrite(Pin1, LOW);
      digitalWrite(Pin2, LOW);
      digitalWrite(Pin3, LOW);
      digitalWrite(Pin4, HIGH);
      break;

    case 1:
      digitalWrite(Pin1, LOW);
      digitalWrite(Pin2, LOW);
      digitalWrite(Pin3, HIGH);
      digitalWrite(Pin4, HIGH);
      break;

    case 2:
      digitalWrite(Pin1, LOW);
      digitalWrite(Pin2, LOW);
      digitalWrite(Pin3, HIGH);
      digitalWrite(Pin4, LOW);
      break;

    case 3:
      digitalWrite(Pin1, LOW);
      digitalWrite(Pin2, HIGH);
      digitalWrite(Pin3, HIGH);
      digitalWrite(Pin4, LOW);
      break;

    case 4:
      digitalWrite(Pin1, LOW);
      digitalWrite(Pin2, HIGH);
      digitalWrite(Pin3, LOW);
      digitalWrite(Pin4, LOW);
      break;

    case 5:
      digitalWrite(Pin1, HIGH);
      digitalWrite(Pin2, HIGH);
      digitalWrite(Pin3, LOW);
      digitalWrite(Pin4, LOW);
      break;

    case 6:
      digitalWrite(Pin1, HIGH);
      digitalWrite(Pin2, LOW);
      digitalWrite(Pin3, LOW);
      digitalWrite(Pin4, LOW);
      break;

    case 7:
      digitalWrite(Pin1, HIGH);
      digitalWrite(Pin2, LOW);
      digitalWrite(Pin3, LOW);
      digitalWrite(Pin4, HIGH);
      break;

    case 9:
      digitalWrite(Pin1, LOW);
      digitalWrite(Pin2, LOW);
      digitalWrite(Pin3, LOW);
      digitalWrite(Pin4, LOW);
      break;
  }
  if (Step != 9)
  {
    if (Direction)
      Step++;
    else
      Step--;

    if (Step == 8)
      Step = 0;

    if (Step < 0)
      Step = 7;
  }

  CurrentRotation--;
  if (CurrentRotation <= 0)
    Step = 9;
  delay(1);
}

void receiveEvent(int howMany)
{
  int x = Wire.read();    // Odczytanie otrzymanej wartości
  if (x == 255)           // Jeśli wartość ta = 255 następuje zmiana kierunku
    Direction = !Direction;
  if (x > 0 && x < 255)   // Jeśli wartość ta zawiera się w podanym przedziale, silnik wykona tyle obrotów
  {
    Rotation = x;
    CurrentRotation = FullRotation * Rotation;
    if (Step == 9)
      Step = 0;
  }
  if (x == 0)             // Jeśli wartość = 0, silnik ruszy lub zatrzyma się
  { 
    if (Step == 9)
    {
      CurrentRotation = FullRotation * Rotation;      
      Step = 0;
    }
    else
      Step = 9;
  }

  Serial.println(x);        
}
