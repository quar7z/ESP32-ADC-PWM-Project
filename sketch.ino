//==== Group 9 ====//

// Members:
// Albalate, Kyla M.
// Arbas, Marc Ken E.
// Batiquin, Edmar S.
// Vera Cruz, John Acel H.
// Vera Cruz, John Arcel H.

// Pin Definitions
#define NTC_PIN 19 // Input
#define LDR_PIN 18 // Input
#define POT_PIN 8 // Input
#define LED_PIN 20 // Output

#define PWM_Channel 0 // This defines what PWM channel the ESP32 should use.
#define PWM_Resolution 8 // The resolution of the PWM or the steps for each cycle of the ESP32 (4-bit = 15 steps, 8-bit = 255 steps).
#define PWM_Frequency 1000 // The frequency of the PWM, how frequent should the PWM go high.

float ntcBetaCoefficient = 3950; // Beta coefficient for NTC Thermistor.
float brightness_percentage = 0;
int brightness = 0;

// LDR value converting into Lux.
const float GAMMA = 0.7;
const float RL10 = 50;

void setup() 
  {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT); // Sets the LED_PIN (18) to output
    ledcAttachChannel(LED_PIN, PWM_Frequency, PWM_Resolution, PWM_Channel); // This configures PWM output of the ESP32 using the LEDC hardware.
  }

void loop()
  {
    // Reads the value of POT_PIN (8) and stores it in variable "potValue"
    int potValue = analogRead(POT_PIN);
    // Reads the value of LDR_PIN (18) and stores it in variable "ldrValue"
    int ldrValue = analogRead(LDR_PIN);
    // Reads the value of LDR_PIN (19) and stores it in variable "ntcValue"
    int ntcValue = analogRead(NTC_PIN);

    // LDR Analog Value converting into Illumination (Lux) logic code
    float voltage = ldrValue / 8191. * 5;
    float resistance = 2000 * voltage / (1 - voltage / 5);
    float lux = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, (1 / GAMMA));

    // Equation for converting the analog reading from pin 19 (NTC) to readable temperature sensor data.
    float celsius = 1 / (log(1 / (8191. / ntcValue - 1)) / ntcBetaCoefficient + 1.0 / 298.15) - 273.15;

    if(lux < 500)
      {
        // Creates a voltage value map for potentiometer value
        brightness = map(potValue, 0, 8191, 0, 255); 
        // Converts the variable brightness into percentage
        //potPercentage = (potValue / 8191.0) * 100.0; 
        // This code increases the duty cycle of pin 20 (LED_PIN) according to the value of variable "brightness".
        ledcWrite(LED_PIN, brightness); 
      }
    else
      {
        // Sets the duty cycle to 0% and this turns off the LED.
        ledcWrite(LED_PIN, 0); 
      }

    // Serial Monitor Output Code.
    Serial.print("Potentiometer: ");
    Serial.print(potValue);

    Serial.print(" | LDR: ");
    Serial.print(ldrValue);

    Serial.print(" | LDR Lux: ");
    Serial.print(lux);
    Serial.print(lux < 500 ? " Dark" : " Bright");

    Serial.print(" | NTC Temp: ");
    Serial.print(celsius);
    Serial.println(" °C");

    delay(100);
  }
