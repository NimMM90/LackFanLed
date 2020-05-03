#include <FastLED.h>
#include <CommonBusEncoders.h>

//FastLED
#define DATA_PIN        1
#define LED_TYPE        WS2813
#define NUM_LEDS        31
CRGB leds[NUM_LEDS];
#define COLOR_ORDER GRB

//Fan PWM
#define PWM6            OCR4D
#define PWM6_MAX        OCR4C

//Encoders
CommonBusEncoders encoders(20,19,21,2);

//Default values
int hsv[3] = {10,10,100};             //HSV array
int spd = 120;                      //fanspeed
bool LIGHT;

//Setup
void setup(){
    FastLED.clear();
    
    //encoders
    encoders.resetChronoAfter(2000);
    //Encoder 1 cycles through HSV values for FastLED
    encoders.addEncoder(1,4,18,3,100,0);
    //Encoder 2 sets the fanspeed in mode 1 and the LED brightness in mode 2
    encoders.addEncoder(2,4,15,2,200,299);

    //Fan control
    pwm6configure();
    pwmSet6();

    //LED control
    FastLED.addLeds<LED_TYPE, DATA_PIN>(leds, NUM_LEDS);
    setColorHSV();
    LIGHT = true;
}

//Main loop
void loop(){
    int code = encoders.readAll();
    // switch case for codes
    if (code > 0){
        setValues(code);
    }
    //Fan PWM
    pwmSet6();
    //LED control
    if(LIGHT){setColorHSV();}
}

/*-------------------------------- HELPERS ------------------------------*/
//Encoder handler
void setValues(int code){
    switch(code){
        case 100:{                         //Increase H
            if(hsv[0] == 255){hsv[0] = 0;}    
            else{hsv[0]+= 5;}
            break;
        }
        case 101:{                          //Decrease H
            if(hsv[0] == 0){hsv[0] = 255;}
            else{hsv[0]-= 5;}
            break;
        }
        case 102:{                          //Increase S
            if(hsv[1] == 255){hsv[1] = 0;}    
            else{hsv[1]+= 5;}
            break;
        }
        case 103:{                          //Decrease S
            if(hsv[1] == 0){hsv[1] = 255;}
            else{hsv[1]-= 5;}
            break;
        }
        case 104:{                          //Increase V
            if(hsv[2] == 255){hsv[2] = 0;}    
            else{hsv[2]+= 5;}
            break;
        }
        case 105:{                          //Decrease V
            if(hsv[2] == 0){hsv[2] = 255;}
            else{hsv[2]-= 5;}
            break;
        }
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
            LIGHT = !LIGHT;
            if(!LIGHT){FastLED.clear(true); }
            else{setColorHSV();}          
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

//set HSV
void setColorHSV() {
  // create a new HSV color
  CHSV color = CHSV(hsv[0], hsv[1], hsv[2]);

  // use FastLED to set the color of all LEDs in the strip to the same color
  fill_solid(leds, NUM_LEDS, color);
  FastLED.show();
}
