#include <EEPROM.h>

int flag = 0;
int i = 0;
int sum = 1; //int8_t
int address = 0;


uint8_t table [10] = {
  0x3F,
  0x06,
  0x5B,
  0x4F,
  0x66,
  0x6D,
  0x7D,
  0x07,
  0x7F,
  0x6F
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  DDRD = B11111100;
  DDRB = B100011;

  address = 0;
  i = EEPROM.read(address);
  if (i > 9) { i = 0; }
  address += sizeof(int); //1
  sum = EEPROM.read(address);
  if (sum == 255) { 
    sum = -1; //EEPROM no reconoce negativos 
  } else if ( sum != 1) { sum = 1; }


}

void loop () {
  pause();
  if (flag == 0) {  
    PORTD = (PORTD&=0x03) | (table[i]<<2);
    PORTB = (PORTB&=0x7C) | (table[i]>>6);
    if (sum == -1) { PINB=(1<<1); }
    PINB=(1<<5);
  }
  
  address = 0;
  EEPROM.put(address, i);

  address += sizeof(int);
  EEPROM.put(address, sum);
  
  if (i == 9) { sum = -1; }
  
  if (i == 0) { sum = 1; }

  i += sum;
  
  delay(1000);
}

int pause() {
  if(Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim(); command.toLowerCase();
    if (command == "pause on") {
      flag = 1;
    } else if (command == "pause off") {
      flag = 0;
    } else if (command == "reset") {
      i = 0;
      sum = 1;
    //Código adicional
    } else if (command.startsWith("set") && isDigit(command.charAt(4))) {
      int x = command.charAt(4) - '0';
      if (x >= 0 && x <= 9) {
        if (command.endsWith("increment")) {
          sum = 1;
        } else if (command.endsWith("decrement")) {
          sum = -1;
        } else if (!command.endsWith(String(x))) {
          bad_command(command);
          return 0;
        }
        i = x;
      } else {
        bad_command(command);
      }
    } else {
      bad_command(command);
    }
  }
}

void bad_command(String& err) {
  Serial.print("[ERROR] bad command \""); Serial.print(err); Serial.println("\"");
}


