int Count_limit = 0; // declare count limit variable to zero
int IR_Sensor_INPUT = A0; 
long int counter = 0; // Set counter variable to 0
int hitObject = false;
int Buzzer = 9; 
int Set_Reset_Button = A1; 
int UP_button = A2; 
int Down_button = A3; 
int Set_Reset_Button_State= 1;
int Up_Button_State = 1;
int Down_Button_State = 0;
int LED = 8; 

#include "LedControl.h" // include the Led display library:
LedControl lc=LedControl(11,13,10,1);// initialize the library with the numbers of the interface pins
#include <LiquidCrystal.h> 
LiquidCrystal lcd(7, 6, 5, 4, 3, 2); 
#include <EEPROM.h> // include the EEPROM library to store last count even after power loose

int addr = 0; //Set EEPROM Address to zero
void setup() 
{
pinMode(Set_Reset_Button, INPUT); 
pinMode(UP_button, INPUT); 
pinMode(Down_button, INPUT); 
pinMode(Buzzer, OUTPUT); 
pinMode(LED, OUTPUT); 

lc.shutdown(0,false);
lc.setIntensity(0,8);
lc.clearDisplay(0);
Serial.begin(9600);
pinMode(IR_Sensor_INPUT,INPUT_PULLUP); // IR sensor as INPUT(Used inbuilt pullup resister using program)

lcd.begin(16, 4); 
lcd.print("================");
lcd.setCursor(0,1);
lcd.print("    Digital");
lcd.setCursor(0,2);
lcd.print("Counter Machine");
lcd.setCursor(0,3);
lcd.print("================");

//Turn on Buzzer and off after 200ms delay//
digitalWrite(Buzzer, HIGH);
delay(200);
digitalWrite(Buzzer, LOW);
delay(1000);

lcd.clear();
lcd.setCursor(0,0);
lcd.print("Last count was:");
lcd.setCursor(0,1);
lcd.print(EEPROM.read(addr));// show the previous count
delay(2000);

set_count_limit(); //cal set_count function

lcd.clear();
lcd.setCursor(0,0);
lcd.print("Object Count:");
lcd.setCursor(0,1);
lcd.print(counter); // show counter value
led_display();
}

void loop() 
{
counter_(); // call counter
counter = 0;
led_display();
digitalWrite(Buzzer, LOW);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("last count was:");
lcd.setCursor(0,1);
lcd.print(EEPROM.read(addr));
delay(2000);

Set_Reset_Button_State = HIGH;
set_count_limit();
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Object Count:");
lcd.setCursor(0,1);
lcd.print(counter);
led_display();
}

void counter_()
{
while(1)
{
lcd.setCursor(0,3);
lcd.print("SET RST");
Set_Reset_Button_State= digitalRead(Set_Reset_Button); 
Down_Button_State = digitalRead(Down_button); 
Up_Button_State = digitalRead(UP_button); 

if(Set_Reset_Button_State== HIGH){ // This if loop will be executed if the SET button is HIGH
lcd.setCursor(0,1);
int val = digitalRead(IR_Sensor_INPUT); 

if( (val == 0) && (hitObject == false) ){ // this code will execute when object in range
counter++;
hitObject = true;
EEPROM.write(addr, counter); // store the count value in EEPROM
Serial.print("Counter = ");
lcd.print(counter);
led_display();

if(counter == Count_limit){ // this loop will be executed once the count reach the count limit
digitalWrite(Buzzer, HIGH); // buzzer ON 30 sec
lcd.setCursor(0,2);
lcd.print("Cnt Lmt Reached!");
lcd.setCursor(0,3);
lcd.print(" RST");

for (int i =0; i<150; i++){ // LED will blink for 30 sec  
Set_Reset_Button_State= digitalRead(Set_Reset_Button); // read the reset button sate(LOW or HIGH)

if (Set_Reset_Button_State== LOW){ //this loop will be executed if the SET button is LOW else stay count limit reached
Set_Reset_Button_State = HIGH;
return;
}
else
{
digitalWrite(LED,HIGH);
delay(100);
digitalWrite(LED,LOW);
delay(100);
}
}
delay(30000);

digitalWrite(Buzzer, LOW);
lcd.setCursor(0,2);
lcd.print(" ");
lcd.setCursor(0,3);
//lcd.print(" RST");
}

else{
digitalWrite(Buzzer, HIGH);
delay(20);
digitalWrite(Buzzer, LOW);
lcd.setCursor(0,2);
}
}

else if( (val ==1) && (hitObject == true) ){ // this code will execute when object is not in range
hitObject = false;
}
}

else if (Set_Reset_Button_State== LOW){ // this code will execute when reset button pressed
counter = 0;
digitalWrite(Buzzer, HIGH);
delay(200);
digitalWrite(Buzzer, LOW);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Object Count:");
lcd.setCursor(0,1);
lcd.print(counter);
led_display();
}

if(Down_Button_State == LOW){ // Count set loop will be executed once down button is pressed.
digitalWrite(Buzzer, HIGH);
delay(70);
digitalWrite(Buzzer, LOW);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("last count was:");
lcd.setCursor(0,1);
lcd.print(EEPROM.read(addr));
delay(800);

set_count_limit(); // call set count limit
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Object Count:");
lcd.setCursor(0,1);
counter = 0;
lcd.print(counter);
led_display();
}
}
}

int set_count_limit() //defined the count limit set function here.
{
counter=0;
led_display();

while (Set_Reset_Button_State == HIGH)
{
Set_Reset_Button_State= digitalRead(Set_Reset_Button);
Up_Button_State = digitalRead(UP_button);
Down_Button_State = digitalRead(Down_button);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("    SETTINGS");
lcd.setCursor(0,1);
lcd.print("Set Count Limit:");
lcd.setCursor(0,2);
lcd.print(Count_limit);
lcd.setCursor(0,3);
lcd.print(" DOWN  UP  SET");
delay(100);

if (Up_Button_State == LOW) //Increase the count Limit
{
Count_limit++;
lcd.setCursor(0,2);
lcd.print(Count_limit);
digitalWrite(Buzzer, HIGH);
delay(70);
digitalWrite(Buzzer, LOW);
}

else if(Down_Button_State == LOW) //Decrese the count Limit
{ 
if (Count_limit > 0)
{
Count_limit--;
lcd.setCursor(0,2);
lcd.print(Count_limit);
digitalWrite(Buzzer, HIGH);
delay(70);
digitalWrite(Buzzer, LOW);
}
}
}

digitalWrite(Buzzer, HIGH);
delay(70);
digitalWrite(Buzzer, LOW);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Count Limit Set:");

if(Count_limit==0){ // if the count limit is zero then the counter set infinite limit
lcd.print("Infinity");
}

else{
lcd.setCursor(0,1);
lcd.print(Count_limit);
}
delay(2000);
}

void led_display(){ // led display function
int digit[8]={0,0,0,0,0,0,0,0}; // lc.clearDisplay(0);
long int temp=counter;
Serial.println( counter);
int j=0,i=0;
for(i=0;temp>0 && i<8;i++ ,temp=temp/10)digit[i]=temp%10;
for(;i<8;i++)digit[i]=0;
delay(1);
}
