// Proportional grip force control from a MyoWare 2.0 ENV output.
// Instead of a single on/off threshold, muscle activation is scaled 0..1
// between the calibrated rest level and max contraction, and that value
// sets the closing PWM. Once the fingers stall on an object, DC motor
// torque is roughly proportional to PWM, so squeezing harder = gripping harder.
// Relaxing opens the hand for OPEN_TIME_MS, then the motor is released.
//
// On power-up: relax for 3 s, then squeeze as hard as you can for 3 s.
// Open Serial Plotter at 115200 to see env / activation / pwm.

// ---- Pins (same TB6612 wiring as button_actuated) ----
const int AIN1 = 13;
const int AIN2 = 4;
const int PWMA = 11;
const int MYO_PIN = A0;   // MyoWare 2.0 ENV pin

// ---- Tuning ----
const int SAMPLE_MS = 5;              // 200 Hz control loop
const float SMOOTHING = 0.1;          // EMA factor (0..1), lower = smoother but laggier
const float REST_STD_MULT = 3.0;      // deadband = rest mean + this many std devs
const float MVC_FRACTION = 0.7;       // activation hits 1.0 at 70% of max squeeze
const int MIN_PWM = 80;               // PWM below which the motor stalls
const int MAX_PWM = 255;
const int OPEN_PWM = 150;             // speed used to open when relaxed
const unsigned long OPEN_TIME_MS = 1000;  // how long to drive open after relaxing

// ---- Calibration results ----
float restMean = 0;
float restStd = 0;
float mvcLevel = 0;

float envSmoothed = 0;
unsigned long lastSample = 0;
unsigned long lastActive = 0;
bool openedSinceGrip = true;


void setup() {
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  stopMotor();

  Serial.begin(115200);

  calibrate();
  envSmoothed = restMean;
  lastSample = millis();
}

void loop() {
  unsigned long now = millis();
  if (now - lastSample < SAMPLE_MS) return;
  lastSample = now;

  // Smooth the envelope
  envSmoothed += SMOOTHING * (analogRead(MYO_PIN) - envSmoothed);
  float activation = computeActivation(envSmoothed);

  int pwm = 0;
  if (activation > 0) {
    // Close with force proportional to activation
    pwm = MIN_PWM + activation * (MAX_PWM - MIN_PWM);
    closeMotor(pwm);
    lastActive = now;
    openedSinceGrip = false;
  } else if (!openedSinceGrip && now - lastActive < OPEN_TIME_MS) {
    // Relaxed after a grip: open for a fixed time
    pwm = -OPEN_PWM;
    openMotor(OPEN_PWM);
  } else {
    openedSinceGrip = true;
    stopMotor();
  }

  Serial.print(envSmoothed);
  Serial.print(",");
  Serial.print(activation * 100);
  Serial.print(",");
  Serial.println(pwm);
}


/********************************************************************************/
// Map smoothed envelope to 0..1 between the rest deadband and max contraction
float computeActivation(float env) {
  float low = restMean + REST_STD_MULT * restStd;
  float high = restMean + MVC_FRACTION * (mvcLevel - restMean);
  if (high <= low) high = low + 1;
  return constrain((env - low) / (high - low), 0.0, 1.0);
}


/********************************************************************************/
// Record rest statistics, then peak contraction
void calibrate() {
  Serial.println("RELAX for 3 seconds...");
  delay(1000);
  float sum = 0, sumSq = 0;
  int n = 0;
  unsigned long start = millis();
  while (millis() - start < 3000) {
    float v = analogRead(MYO_PIN);
    sum += v;
    sumSq += v * v;
    n++;
    delay(SAMPLE_MS);
  }
  restMean = sum / n;
  restStd = sqrt(max(sumSq / n - restMean * restMean, 0.0));

  Serial.println("SQUEEZE HARD for 3 seconds...");
  delay(1000);
  float smoothed = restMean;
  mvcLevel = restMean;
  start = millis();
  while (millis() - start < 3000) {
    smoothed += SMOOTHING * (analogRead(MYO_PIN) - smoothed);
    if (smoothed > mvcLevel) mvcLevel = smoothed;
    delay(SAMPLE_MS);
  }

  Serial.print("rest mean: ");
  Serial.print(restMean);
  Serial.print("  rest std: ");
  Serial.print(restStd);
  Serial.print("  max: ");
  Serial.println(mvcLevel);
  delay(1000);
}


/********************************************************************************/
void closeMotor(int pwm) {
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, pwm);
}

void openMotor(int pwm) {
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  analogWrite(PWMA, pwm);
}

void stopMotor() {
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, 0);
}
