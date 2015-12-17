 /*  Author: Benjamin Low (benjamin.low@digimagic.com.sg)

    Description: Temporary solution for the Oculus Rift station to simulate two physical buttons with
    only one actual physical button. The proprietary Oculus Rift content does not allow software interface
    to reset it, so we use hardware method. The Arduino controls a two channel relay. Once it receives
    a button press signal from the actual physical button, it starts a TBD second timer
    while sending a momentary switch close to the keyboard encoder with the spacebar signal mapped to
    that connection. After TBD seconds, the second relay sends a momentary switch close to the second
    keyboard encoder connection with Ctrl-R mapped. Ctrl-R will reset the Oculus Rift content. This is
    a temporary hack because there is no time to do a second physical button. Once the Oculus Rift content done
    by us is ready, we will switch back to the normal Arduino LED button box.

    Last updated: 7 Dec 2015
*/

#define first_relay_pin 6 //connected to relay. Relay is active-low.
#define second_relay_pin 7
#define switch_pin 8 //connected to button switch

const long content_duration = 4000; //USER DEFINED SETTING


long start_time, elapsed_time; //when actual button is pressed
boolean is_button_locked; //for locking the button once pressed until the content finishes

void setup() {

  pinMode(switch_pin, INPUT_PULLUP); //active low switch
  pinMode(first_relay_pin, OUTPUT);
  pinMode(second_relay_pin, OUTPUT);

  Serial.begin(9600);

  digitalWrite(first_relay_pin, HIGH); //active low relay
  digitalWrite(second_relay_pin, HIGH);
}

void loop() {

   run_keyboard_simulation(); 
   
  if ( digitalRead(switch_pin) == LOW && is_button_locked == false ) {

    start_time = millis();
    is_button_locked = true;

    write_keyboard_press(first_relay_pin);
    Serial.println("SPACEBAR");
  }

  elapsed_time = millis() - start_time;
//  Serial.println(elapsed_time);

  if (elapsed_time > content_duration && is_button_locked == true ) {

    write_keyboard_press(second_relay_pin);
    is_button_locked = false;

    Serial.println("CTRL-R");
  }
}

void run_keyboard_simulation() {

  if (Serial.available()) {
    
    char my_command = Serial.read();

    if (my_command == '1') {

      start_time = millis();
      is_button_locked = true;

      write_keyboard_press(first_relay_pin);
      Serial.println("SPACEBAR");

    }

    else if (my_command == '2') {
  
      write_keyboard_press(second_relay_pin);
      Serial.println("CTRL-R");
    }

  }
}

void write_keyboard_press (int which_pin) {

  digitalWrite(which_pin, LOW);
  delay(100);
  digitalWrite(which_pin, HIGH);
  delay(100);
}

