/*
    Remote Arduino Automation Protocol (RAAP)
    Author : Raja Jamwal <linux1@zoho.com>    
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// TODO: DUE board specific
// analogReadResolution ()
// analogWriteResolution ()

// TODO: functions manipulating int (16) should be converted
// to 8 bit value, the loss of precision should be kept low
// Currently there is no internal conversion of data,
// all functions manipulating, data size greater than 8 bit
// should be used carefully, so as not to send a value
// greater than 8 bit in size

// Mega 1280 & 2560
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#define ATmega_1280_2560 // Mega series
#elif defined(__AVR_ATmega644__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega1284P__)
#define ATmega_644_644P_1284P // Sanguino
#elif defined(__AVR_ATmega168__) ||defined(__AVR_ATmega168P__) ||defined(__AVR_ATmega328P__)
#define ATmega_168_168P_328P // 168 and 328 Arduinos
#elif defined(__AVR_ATmega32U4__)
#define ATmega_32U4 
#endif

struct FUNCTION_TABLE
{
  int command;
  int (*func) (char *, unsigned int);
};

// TODO: implement PULSE_IN
// supported funcs.
enum { PIN_MODE, DIGITAL_WRITE, DIGITAL_READ, ANALOG_REFERENCE, ANALOG_READ, ANALOG_WRITE,
       TONE, TONE_DURATION, NO_TONE, SHIFT_OUT, SHIFT_IN, PULSE_IN }; // operation
enum { PORT_OUTPUT, PORT_INPUT }; // port
enum { PORT_HIGH, PORT_LOW }; // digital
enum { OK, FAIL }; // status
enum { AREF_DEFAULT, AREF_INTERNAL, AREF_INTERNAL1V1, AREF_INTERNAL2V56, AREF_EXTERNAL }; // analog

FUNCTION_TABLE ft[] = {
  {PIN_MODE, ft_pinMode},
  {DIGITAL_WRITE, ft_digitalWrite},
  {DIGITAL_READ, ft_digitalRead},
  {ANALOG_REFERENCE, ft_analogReference},
  {ANALOG_READ, ft_analogRead},
  {ANALOG_WRITE, ft_analogWrite},
  {TONE, ft_tone}, 
  {TONE_DURATION, ft_toneDuration},
  {NO_TONE, ft_noTone},
  {SHIFT_OUT, ft_shiftOut},
  {SHIFT_IN, ft_shiftIn},
  {PULSE_IN, ft_pulseIn}
};

const uint8_t error[]         =  { FAIL, 0};
const uint8_t ok[]            =  { OK, 0};
const uint8_t start[]          = { OK, 1, 1}; // ok length version

// msg format
//<operation><size><optional_payload>

int ft_pinMode (char *cmdline, unsigned int len)
{
  if (len!=2)
  {  
    Serial.write (error, error[1]+2);
    return 1;
  }   
  pinMode (cmdline[0], ((cmdline[1]==PORT_OUTPUT)?OUTPUT:INPUT) );
  Serial.write (ok, ok[1]+2);
  Serial.flush ();
  return 0;
}

int ft_digitalWrite (char *cmdline, unsigned int len)
{
  if (len!=2)
  {  
    Serial.write (error, error[1]+2);
    return 1;
  }  
  digitalWrite (cmdline[0], ((cmdline[1]==PORT_HIGH)?HIGH:LOW) );
  Serial.write (ok, ok[1]+2);
  Serial.flush (); 
  return 0;
}

int ft_digitalRead (char *cmdline, unsigned int len)
{
  if (len!=1)
  {
    Serial.write (error, error[1]+2);
    return 1;    
  } 
  Serial.write (ok, ok[1]+2);
  uint8_t msg[] = {DIGITAL_READ, 2, cmdline[0], digitalRead(cmdline[0])==PORT_HIGH?HIGH:LOW};
  Serial.write (msg, 4);
  Serial.flush ();
  return 0;
}

int ft_analogReference (char *cmdline, unsigned int len)
{
  if (len!=1)
  {
    Serial.write (error, error[1]+2);
    return 1;    
  }
  Serial.write (ok, ok[1]+2);
  int mode = DEFAULT;
  switch (cmdline[0])
  {
    case AREF_DEFAULT:
      mode = DEFAULT;
    break;
    case AREF_INTERNAL:
      mode = INTERNAL;
    break;
    
    #ifdef ATmega_1280_2560
    case AREF_INTERNAL1V1:
      mode = INTERNAL1V1;
    break;
    case AREF_INTERNAL2V56:
      mode = INTERNAL2V56;
    break;
    #endif
    
    case AREF_EXTERNAL:
      mode = EXTERNAL;
  }
  
  analogReference (mode);
  return 0;
}

int ft_analogRead (char *cmdline, unsigned int len)
{
  if (len!=1)
  {
    Serial.write (error, error[1]+2);
    return 1;    
  }
  Serial.write (ok, ok[1]+2);
  uint8_t msg[] = {ANALOG_READ, 2, cmdline[0], analogRead(cmdline[0])};
  Serial.write (msg, 4);
  Serial.flush ();
  return 0;
}

int ft_analogWrite (char *cmdline, unsigned int len)
{
  if (len!=2)
  {
    Serial.write (error, error[1]+2);
    return 1;    
  }
  analogWrite (cmdline[0], cmdline[1]);  
  Serial.write (ok, ok[1]+2);
  Serial.flush ();
  return 0;
}

int ft_tone (char *cmdline, unsigned int len)
{
  if (len!=2)
  {
    Serial.write (error, error[1]+2);
    return 1;    
  }
  tone (cmdline[0], cmdline[1]);  
  Serial.write (ok, ok[1]+2);
  Serial.flush ();
  return 0;
}

int ft_toneDuration (char *cmdline, unsigned int len)
{
  if (len!=3)
  {
    Serial.write (error, error[1]+2);
    return 1;    
  }
  tone (cmdline[0], cmdline[1], cmdline[2]);  
  Serial.write (ok, ok[1]+2);
  Serial.flush ();
  return 0;
}

int ft_noTone (char *cmdline, unsigned int len)
{
  if (len!=1)
  {
    Serial.write (error, error[1]+2);
    return 1;    
  }
  noTone (cmdline[0]);  
  Serial.write (ok, ok[1]+2);
  Serial.flush ();  
  return 0;
}

// TODO: define and implement MSBFIRST & LSBFIRST
int ft_shiftOut (char *cmdline, unsigned int len)
{
  if (len!=4)
  {
    Serial.write (error, error[1]+2);
    return 1;    
  }
  shiftOut (cmdline[0], cmdline[1], cmdline[2], cmdline[3]);  
  Serial.write (ok, ok[1]+2);
  Serial.flush ();
  return 0;
}

int ft_shiftIn (char *cmdline, unsigned int len)
{
  if (len!=3)
  {
    Serial.write (error, error[1]+2);
    return 1;    
  }

  uint8_t val = shiftIn(cmdline[0], cmdline[1], cmdline[2]);  
  Serial.write (ok, ok[1]+2);
  uint8_t msg[] = {SHIFT_IN, 1, val};
  Serial.write (msg, 3);
  Serial.flush ();
  return 0;
}

int ft_pulseIn (char *cmdline, unsigned int len)
{
  return 0;
}

void setup()
{
 Serial.begin (9600);
 Serial.write ((const uint8_t*)start, start[1]+2);
 Serial.flush ();
 // DEBUG
 //pinMode (13, OUTPUT);
 //digitalWrite (13, HIGH);
}

void loop()
{
  if (Serial.available())
  {
      char operation = Serial.read(); // operation
      int len = Serial.read(); // length of payload
  
      char *cmdline = (char*) malloc(len);
      
      for (int i=0; i<len; i++)
      {
        cmdline[i] = Serial.read();
      }
      
      for (int i=0; i<(sizeof(ft)/sizeof(ft[0]));i++)
      {
        if (ft[i].command == operation)
        {
          ft[i].func (cmdline, len);
        }
      }   
      
      free (cmdline);
  }  
  delay (20);
}

