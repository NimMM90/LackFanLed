#include <CommonBusEncoders.h>

//Fan PWM
#define PWM6            OCR4D
#define PWM6_MAX        OCR4C

//Encoders
CommonBusEncoders encoders(20,19,21,1);

//Default values
int spd = 120;                      //fanspeed
bool FANS;

//Setup
void setup(){  
    //encoders
    encoders.resetChronoAfter(2000);
    //Encoder 1 on switchpin 18 sets the fanspeed and toggles the fans on/off
    encoders.addEncoder(1,4,18,1,200,299);

    //Fan control
    FANS = true;
    pwm6configure();
    pwmSet6();
}

//Main loop
void loop(){
    int code = encoders.readAll();
    // switch case for codes
    if (code > 0){
        setValues(code);
    }
    //Fan PWM
    if(FAN){pwmSet6()};
}

/*-------------------------------- HELPERS ------------------------------*/
//Encoder handler
void setValues(int code){
    switch(code){
        case 200:{                          //Increase fanspeed
            if(spd == 255){}    
            else{spd += 5;}
            break;
        }
        case 201:{                          //Decrease fanspeed
            if(spd == 0){}
            else{spd -= 5;}
            break;
        }
        case 299:{                          //Toggle the LEDs on/off                       
            FANS = !FANS;
            if(!FANS){spd = 0; }
            else{spd = 120;}
            pwmSet6();          
        }
    }
}

// Configure the PWM clock
void pwm6configure()
{
  // TCCR4B configuration
  TCCR4B = 4; /* 4 sets 23437Hz */

  // TCCR4C configuration
  TCCR4C = 0;

  // TCCR4D configuration
  TCCR4D = 0;

  // PLL Configuration
  PLLFRQ = (PLLFRQ & 0xCF) | 0x30;

  // Terminal count for Timer 4 PWM
  OCR4C = 255;
}

// Set PWM to D6 (Timer4 D)
// Argument is PWM between 0 and 255
void pwmSet6()
{
  OCR4D = spd;      // Set PWM value
  DDRD |= 1 << 7;   // Set Output Mode D7
  TCCR4C |= 0x09;   // Activate channel D
}

