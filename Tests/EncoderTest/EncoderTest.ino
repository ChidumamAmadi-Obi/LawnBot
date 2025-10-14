#include <Arduino.h>

// CONFIG 
const uint8_t ENCODER_1_A = 4; // Left Motor - Channel A
const uint8_t ENCODER_1_B = 5; // Left Motor - Channel B
const uint8_t ENCODER_2_A = 6; // Right Motor - Channel A
const uint8_t ENCODER_2_B = 7; // Right Motor - Channel B
const uint8_t ENCODER_PPR = 11; // Pulses Per Revolution 
const int RPM_INTERVAL_MS = 250; 
const float WHEEL_RADIUS = 0.05; // cm radius 
const float WHEEL_BASE = 0.20;   // cm between wheels

int32_t totalEncoderCount1 = 0; 
int32_t totalEncoderCount2 = 0;
int32_t periodEncoderCount1 = 0; // For RPM (reset every period)
int32_t periodEncoderCount2 = 0;

uint8_t prevState1 = 0;
uint8_t prevState2 = 0;

unsigned long prevRPMMillis = 0;

int32_t netCount1 = 0; 
int32_t netCount2 = 0;
volatile float distanceLeft = 0.0; //(meters)
volatile float distanceRight = 0.0; 
volatile uint16_t rpm1 = 0;
volatile uint16_t rpm2 = 0;

void isrEncoder1() {
  //creates a 2-bit number that represents the current state of both encoder channels
  //ENCODER_1_A gives us the least significant bit (bit 0)
  //digitalRead(ENCODER_1_B) << 1 shifts channel B's value to (bit 1) the most significant bit
  uint8_t state = (digitalRead(ENCODER_1_B) << 1) | digitalRead(ENCODER_1_A);

  // this gives us 4 possible encoder states:
    // 0b00 or 0 both channels low
    // 0b01 or 1 A high, B low
    // 0b10 or 2 A low, B high
    // 0b11 or 3 both channels high

  // this switch statement looks for specific 4-bit patterns that indicate valid movemwnts
  switch ((prevState1 << 2) | state) { // combines current and previous state into 4-bit number
    case 0b0001: case 0b0111: case 0b1110: case 0b1000:
      totalEncoderCount1++; periodEncoderCount1++; break; // Forward
    case 0b0010: case 0b1011: case 0b1101: case 0b0100:
      totalEncoderCount1--; periodEncoderCount1--; break; // Backward
  }
  prevState1 = state;
}
void isrEncoder2() {
  uint8_t state = (digitalRead(ENCODER_2_B) << 1) | digitalRead(ENCODER_2_A);
  switch ((prevState2 << 2) | state) {
    case 0b0001: case 0b0111: case 0b1110: case 0b1000:
      totalEncoderCount2++; periodEncoderCount2++; break; // Forward
    case 0b0010: case 0b1011: case 0b1101: case 0b0100:
      totalEncoderCount2--; periodEncoderCount2--; break; // Backward
  }
  prevState2 = state;
}

void setup() {
  Serial.begin(115200);
  Serial.println("\n=== Encoder Odometry Test ===\n");

  pinMode(ENCODER_1_A, INPUT_PULLUP);
  pinMode(ENCODER_1_B, INPUT_PULLUP);
  pinMode(ENCODER_2_A, INPUT_PULLUP);
  pinMode(ENCODER_2_B, INPUT_PULLUP);
  
  // Read initial encoder states
  prevState1 = (digitalRead(ENCODER_1_B) << 1) | digitalRead(ENCODER_1_A);
  prevState2 = (digitalRead(ENCODER_2_B) << 1) | digitalRead(ENCODER_2_A);
  
  attachInterrupt(digitalPinToInterrupt(ENCODER_1_A), isrEncoder1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_1_B), isrEncoder1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_2_A), isrEncoder2, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_2_B), isrEncoder2, CHANGE);

  Serial.println("Encoder interrupts initialized");
  Serial.println("Monitoring encoder counts...");
  Serial.println("Format: LeftTotal | LeftRPM | RightTotal | RightRPM");
  Serial.println("===================================================");
}

void loop() {
  unsigned long currentMillis = millis();
  Serial.printf("L: %+6d (%3d RPM) | R: %+6d (%3d RPM) | ", totalEncoderCount1, rpm1, totalEncoderCount2, rpm2);
  
  delay(10); // Add a small delay to prevent Serial output flood
 
}

void calculateRPM() {
  if (currentMillis - prevRPMMillis >= RPM_INTERVAL_MS) {
    noInterrupts(); // Disable interrupts briefly to safely read and reset period counts
    
    int32_t count1 = periodEncoderCount1;
    int32_t count2 = periodEncoderCount2;
    periodEncoderCount1 = 0;
    periodEncoderCount2 = 0;
    interrupts();
    
    rpm1 = abs(count1) * 60 / (ENCODER_PPR * (RPM_INTERVAL_MS / 1000.0)); // Calculate RPM (absolute value for speed display)
    rpm2 = abs(count2) * 60 / (ENCODER_PPR * (RPM_INTERVAL_MS / 1000.0));
    
    if (count1 > 0) Serial.print("L→ ");
    else if (count1 < 0) Serial.print("L← ");
    else Serial.print("L• ");
    
    if (count2 > 0) Serial.print("R→ ");
    else if (count2 < 0) Serial.print("R← ");
    else Serial.print("R• ");
    Serial.println();
    prevRPMMillis = currentMillis;
  }
}