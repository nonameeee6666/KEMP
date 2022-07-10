#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

//variáveis botões
int enter = 0;
int botcima = 1;
int botbaixo = 2;
int botesq = 4;
int botdir = 3;

//variáveis lcd
int value2 = 0; //var pra n deixar voltar pra tela inicial
int value3 = 0; //telinha da associação de resistores
int seg2 = 0; //var de segurança pra não ir pra tela do caminhar

int direct = 0;  //direct vai ser a var q vai indicar em qual pos do lcd vai ficar a letra

int value [16]; //array de armazenamento dos comandos
int fim = 0; //var para voltar pra tela inicial após caminhar, se for = 0, ele executa o caminhar, se for = 1 ele volta pra tela inicial e reseta a var
//variáveis motores
int IN1 = 6;
int IN2 = 5;
int IN3 = 8;
int IN4 = 7;

//variáveis LEDs
int ledcim = 12;
int ledesq = 11;
int leddir = 10;
int ledbai = 9;

//variáveis leitor de resistência
int aPinLeitura = A1;
int aPinLeitura2 = A0;
int R1 = 0;
float val = 0;
int Vin = 5;
float Vout = 0.0;
int RC;
int resist = 0; //var pra ler o 1° resistor
int resist2 = 0; //var pra ler a soma de 2 resistores

//Código para mostrar o sinal ômega(unidade de medida de resistência)
byte ohmChar[8] = {
  B00000,
  B00100,
  B01010,
  B10001,
  B10001,
  B01010,
  B11011,
  B00000
};
void setup() {
  // put your setup code here, to run once:
  lcd.init();
  lcd.setBacklight(HIGH);
  lcd.createChar(1, ohmChar); // Cria o símbolo ômega

  pinMode(enter, INPUT_PULLUP);
  pinMode(botcima, INPUT_PULLUP);
  pinMode(botbaixo, INPUT_PULLUP);
  pinMode(botesq, INPUT_PULLUP);
  pinMode(botdir, INPUT_PULLUP);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(ledcim, OUTPUT);
  pinMode(ledesq, OUTPUT);
  pinMode(leddir, OUTPUT);
  pinMode(ledbai, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  delay(300);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Ola, eu sou o");
  lcd.setCursor(6, 1);
  lcd.print("KEMP!");

  direct = -1;
  enter = 0;
  fim = 0;
  resist = 0;
  value2 = 0;
  value3 = 0;
  seg2 = 0;
  int a1; //vars para média de resistência
  int a2;
  int a3;
  int R2 = 580; //var do divisor de tensão

  if (digitalRead(enter) == LOW)
  {
    lcd.clear();
    lcd.setCursor(6, 0);
    lcd.print("...");
    delay(300);
    while (digitalRead(enter) == HIGH) {
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("Resistores");
      lcd.setCursor(2, 1);
      lcd.print("Caminhar");
      lcd.setCursor(0, value2);
      lcd.print(">");
      delay(200);
      if (digitalRead(botcima) == LOW) {
        value2 = 0;
        delay(100);
      }
      if (digitalRead(botbaixo) == LOW) {
        value2 = 1;
        delay(100);
      }

      if (digitalRead(enter) == LOW and value2 == 0) {
        lcd.clear();
        lcd.setCursor(6, 0);
        lcd.print("...");
        seg2 = 1;
        delay(300);
        while (digitalRead(enter) == HIGH) {
          lcd.clear();
          lcd.setCursor(2, 0);
          lcd.print("1 resistor");
          lcd.setCursor(2, 1);
          lcd.print("2 resistores");
          lcd.setCursor(0, value3);
          lcd.print(">");
          delay(200);
          if (digitalRead(botcima) == LOW) {
            value3 = 0;
            delay(100);
          }
          if (digitalRead(botbaixo) == LOW) {
            value3 = 1;
            delay(100);
          }
          if (digitalRead(enter) == LOW and value3 == 0) {
            lcd.clear();
            lcd.setCursor(6, 0);
            lcd.print("...");
            delay(300);
            while (digitalRead(enter) == HIGH) {
              val = 1.0 * analogRead (aPinLeitura); //Aquisição analógica de valores pelo pino A0
              Vout = (val * Vin) / 1024; // Fórmula para calcular o Vout
              R1 = (R2 * (Vin - Vout)) / Vout; //Fòrmula do divisor de tensão
              RC = R1 / 1000; //Fórmula para mostrar o valor com K
              a1 = R1;
              delay(250);
              a2 = R1;
              delay(250);
              a3 = R1;
              delay(250);

              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Valor: ");
              lcd.print((a1 + a2 + a3) / 3);
              lcd.write(1);

              if (R1 >= 1000) {
                lcd.setCursor(0, 1);
                lcd.print("   ou: ");
                lcd.print(RC);
                lcd.print("K");
              }
            }
          }

          if (digitalRead(enter) == LOW and value3 == 1) {
            lcd.clear();
            lcd.setCursor(6, 0);
            lcd.print("...");
            delay(300);
            while (digitalRead(enter) == HIGH) {
              if (analogRead (aPinLeitura) == 0) {
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Nao detectado");
                delay(500);
              }
              if ((analogRead (aPinLeitura) > 0) and (analogRead (aPinLeitura2) == 0)) {
                val = 1.0 * analogRead (aPinLeitura); //Aquisição analógica de valores pelo pino A0
                Vout = (val * Vin) / 1024; // Fórmula para calcular o Vout
                R1 = (R2 * (Vin - Vout)) / Vout; //Fòrmula do divisor de tensão
                RC = R1 / 1000; //Fórmula para mostrar o valor com K
                a1 = R1;
                delay(250);
                a2 = R1;
                delay(250);
                a3 = R1;
                delay(250);

                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Valor: ");
                lcd.print((a1 + a2 + a3) / 3);
                lcd.write(1);
                lcd.setCursor(0, 4);
                lcd.print("paralelo");
              }
              if ((analogRead (aPinLeitura) > 0) and (analogRead (aPinLeitura2) > 0)) {
                R2 = 380;
                val = 1.0 * analogRead (aPinLeitura2); //Aquisição analógica de valores pelo pino A0
                Vout = (val * Vin) / 1024; // Fórmula para calcular o Vout
                R1 = (R2 * (Vin - Vout)) / Vout; //Fòrmula do divisor de tensão
                RC = R1 / 1000; //Fórmula para mostrar o valor com K
                a1 = R1;
                delay(250);
                a2 = R1;
                delay(250);
                a3 = R1;
                delay(250);

                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Valor: ");
                lcd.print((a1 + a2 + a3) / 3);
                lcd.write(1);
                lcd.setCursor(0, 5);
                lcd.print("serie");
              }
            }
            /*
                          if ((analogRead(aPinLeitura) > 0) and (resist = 0)) {
                            val = 1.0 * analogRead (aPinLeitura); //Aquisição analógica de valores pelo pino A2
                            Vout = (val * Vin) / 1024; // Fórmula para calcular o Vout
                            R1 = (R2 * (Vin - Vout)) / Vout; //Fòrmula do divisor de tensão

                            resist = R1;
                            delay(200);
                          }
                          // if (analogRead(aPinLeitura) != 2% de resist)
                          //resist2 = analogRead(aPinLeitura);
                          delay(200);
                          while ((analogRead(aPinLeitura > 0)) and (resist2 > 0)) {
                            val = 1.0 * analogRead (aPinLeitura); //Aquisição analógica de valores pelo pino A2
                            Vout = (val * Vin) / 1024; // Fórmula para calcular o Vout
                            R1 = (R2 * (Vin - Vout)) / Vout; //Fòrmula do divisor de tensão
                          }*/

          }
        }
      }
    }

    if (digitalRead(enter) == LOW and seg2 == 0) {
      lcd.clear();
      lcd.setCursor(6, 0);
      lcd.print("...");
      delay(300);
      while (digitalRead(enter) == HIGH) {
        for (int seg = 0; seg <= 16;) {   //var de segurança, caso nenhum bot seja abertado, não haverá comandos
          for (int index_value = 0; index_value <= 16;) {    //value é a array, index_value é o n° dentro da array, ex: value[index_value];
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, LOW);
            digitalWrite(IN3, LOW);
            digitalWrite(IN4, LOW);
            digitalWrite(ledcim, LOW);
            digitalWrite(ledesq, LOW);
            digitalWrite(leddir, LOW);
            digitalWrite(ledbai, LOW);
            lcd.setCursor(0, 0);
            lcd.print("Para onde vou?");

            delay(100);

            if (digitalRead(botcima) == LOW) {
              seg++;
              direct++;
              index_value++;
              value[index_value] = 1;
              lcd.setCursor(direct, 1);
              lcd.print("^");
              delay(200);
            }
            if (digitalRead(botbaixo) == LOW) {
              seg++;
              direct++;
              index_value++;
              value[index_value] = 2;
              lcd.setCursor(direct, 1);
              lcd.print("v");
              delay(200);
            }
            if (digitalRead(botesq) == LOW) {
              seg++;
              direct++;
              index_value++;
              value[index_value] = 3;
              lcd.setCursor(direct, 1);
              lcd.print("<");
              delay(200);
            }
            if (digitalRead(botdir) == LOW) {
              seg++;
              direct++;
              index_value++;
              value[index_value] = 4;
              lcd.setCursor(direct, 1);
              lcd.print(">");
              delay(200);
            }
            if (index_value > 16) {
              index_value = 16;
              direct = 16;
            }
            if (digitalRead(enter) == LOW and seg > 0) {
              direct = -1;
              fim = 1;
              int index_value2 = 0;
              int index_value3 = 0;
              index_value2 = index_value;
              index_value = 0;
              lcd.clear();
              lcd.setCursor(6, 0);
              lcd.print("...");
              delay(1000);

              while (index_value3 <= index_value2) {
                delay(100);
                switch (value[index_value3]) {
                  case 0:
                    digitalWrite(IN1, LOW);
                    digitalWrite(IN2, LOW);
                    digitalWrite(IN3, LOW);
                    digitalWrite(IN4, LOW);
                    digitalWrite(ledcim, LOW);
                    digitalWrite(ledesq, LOW);
                    digitalWrite(leddir, LOW);
                    digitalWrite(ledbai, LOW);
                    delay(500);
                  case 1:
                    digitalWrite(IN1, LOW);
                    digitalWrite(IN2, LOW);
                    digitalWrite(IN3, LOW);
                    digitalWrite(IN4, LOW);
                    digitalWrite(ledcim, LOW);
                    digitalWrite(ledesq, LOW);
                    digitalWrite(leddir, LOW);
                    digitalWrite(ledbai, LOW);
                    delay(500);
                    digitalWrite(IN1, HIGH);
                    digitalWrite(IN2, LOW);
                    digitalWrite(IN3, HIGH);
                    digitalWrite(IN4, LOW);
                    digitalWrite(ledcim, HIGH);
                    value[index_value3] = 0;
                    index_value3++;
                    delay(1000);
                    break;
                  case 2:
                    digitalWrite(IN1, LOW);
                    digitalWrite(IN2, LOW);
                    digitalWrite(IN3, LOW);
                    digitalWrite(IN4, LOW);
                    digitalWrite(ledcim, LOW);
                    digitalWrite(ledesq, LOW);
                    digitalWrite(leddir, LOW);
                    digitalWrite(ledbai, LOW);
                    delay(500);
                    digitalWrite(IN1, LOW);
                    digitalWrite(IN2, HIGH);
                    digitalWrite(IN3, LOW);
                    digitalWrite(IN4, HIGH);
                    digitalWrite(ledbai, HIGH);
                    value[index_value3] = 0;
                    index_value3++;
                    delay(1000);
                    break;
                  case 3:
                    digitalWrite(IN1, LOW);
                    digitalWrite(IN2, LOW);
                    digitalWrite(IN3, LOW);
                    digitalWrite(IN4, LOW);
                    digitalWrite(ledcim, LOW);
                    digitalWrite(ledesq, LOW);
                    digitalWrite(leddir, LOW);
                    digitalWrite(ledbai, LOW);
                    delay(500);
                    digitalWrite(IN1, HIGH);
                    digitalWrite(IN2, LOW);
                    digitalWrite(IN3, LOW);
                    digitalWrite(IN4, HIGH);
                    digitalWrite(ledesq, HIGH);
                    value[index_value3] = 0;
                    index_value3++;
                    delay(1000);
                    break;
                  case 4:
                    digitalWrite(IN1, LOW);
                    digitalWrite(IN2, LOW);
                    digitalWrite(IN3, LOW);
                    digitalWrite(IN4, LOW);
                    digitalWrite(ledcim, LOW);
                    digitalWrite(ledesq, LOW);
                    digitalWrite(leddir, LOW);
                    digitalWrite(ledbai, LOW);
                    delay(500);
                    digitalWrite(IN1, LOW);
                    digitalWrite(IN2, HIGH);
                    digitalWrite(IN3, HIGH);
                    digitalWrite(IN4, LOW);
                    digitalWrite(leddir, HIGH);
                    value[index_value3] = 0;
                    index_value3++;
                    delay(1000);
                    break;
                }
              }
            }
          }
        }
      }
    }
  }
}
