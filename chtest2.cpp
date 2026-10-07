#include <Servo.h>

#define PIN_SERVO 10
#define PIN_TRIG  12 
#define PIN_ECHO  11  

#define SND_VEL 346.0 
#define INTERVAL 100 
#define PULSE_DURATION 10
#define _DIST_MIN 100.0  
#define _DIST_MAX 300.0   

#define TIMEOUT ((INTERVAL / 2) * 1000.0) 
#define SCALE (0.001 * 0.5 * SND_VEL)

#define ANGLE_HOLD 180   // 문 닫힘
#define ANGLE_UP   90    // 문 열림
#define MOVING_TIME 3000
#define SIGMOID_K 2.0    

Servo myServo;

unsigned long last_sampling_time;
unsigned long moveStartTime;
int startAngle = ANGLE_HOLD;
int targetAngle = ANGLE_HOLD;
int currentAngle = ANGLE_HOLD;

// 시그모이드: 1 / (1 + e^(-x))
float sigmoid(float x) {
  return 1.0 / (1.0 + exp(-x));
}

void setup() {
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  digitalWrite(PIN_TRIG, LOW);

  myServo.attach(PIN_SERVO);
  myServo.write(ANGLE_HOLD);

  Serial.begin(57600);
}

void loop() {
  float distance;

  if (millis() >= (last_sampling_time + INTERVAL)) {
    distance = USS_measure(PIN_TRIG, PIN_ECHO);

    int nextTarget = ((distance > 0.0) && (distance <= _DIST_MAX)) ? ANGLE_UP : ANGLE_HOLD;
    if (nextTarget != targetAngle) {
      startAngle = currentAngle;
      targetAngle = nextTarget;
      moveStartTime = millis();
    }

    // 시리얼 플로터: Min, distance, Max
    float plot_distance = distance;
    if ((distance == 0.0) || (distance > _DIST_MAX)) {
      plot_distance = _DIST_MAX + 10.0;
    } else if (distance < _DIST_MIN) {
      plot_distance = _DIST_MIN - 10.0;
    }
    Serial.print("Min:");       Serial.print(_DIST_MIN);
    Serial.print(",distance:"); Serial.print(plot_distance);
    Serial.print(",Max:");      Serial.print(_DIST_MAX);
    Serial.println("");

    last_sampling_time += INTERVAL;
  }

  unsigned long progress = millis() - moveStartTime;
  if (startAngle == targetAngle || progress >= MOVING_TIME) {
    currentAngle = targetAngle;
  } else {
    float t = (float)progress / (float)MOVING_TIME;
    float s  = sigmoid((t - 0.5) * (2.0 * SIGMOID_K));
    float s0 = sigmoid(-SIGMOID_K);
    float s1 = sigmoid(SIGMOID_K);
    float sn = (s - s0) / (s1 - s0);
    float delta = (targetAngle - startAngle) * sn;
    currentAngle = startAngle + (int)(delta >= 0 ? delta + 0.5 : delta - 0.5);
  }
  myServo.write(currentAngle);
}

float USS_measure(int TRIG, int ECHO)
{
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);

  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE; // unit: mm
}
