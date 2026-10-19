#include <Servo.h>

#define PIN_SERVO 10

#define _DUTY_MIN 500
#define _DUTY_NEU 1500
#define _DUTY_MAX 2500

#define _MOVE_ANGLE  90.0
#define _SERVO_SPEED 0.3

#define INTERVAL 20

#define _DUTY_PER_DEGREE ((_DUTY_MAX - _DUTY_MIN) / 180.0)

#define _POS_START (_DUTY_MIN)
#define _POS_END   (_DUTY_MIN + (_MOVE_ANGLE * _DUTY_PER_DEGREE))

unsigned long last_sampling_time;

Servo myservo;

float duty_change_per_interval;
float duty_target;
float duty_curr;

bool finished = false;

void setup() {
  myservo.attach(PIN_SERVO);

  duty_curr = _POS_START;
  duty_target = _POS_END;
  myservo.writeMicroseconds((int)(duty_curr + 0.5));

  Serial.begin(57600);

  duty_change_per_interval =
    _SERVO_SPEED * (INTERVAL / 1000.0) * _DUTY_PER_DEGREE;

  Serial.print("duty_change_per_interval:");
  Serial.println(duty_change_per_interval);

  last_sampling_time = 0;
}

void loop() {
  if (millis() < (last_sampling_time + INTERVAL))
    return;

  if (!finished) {
    if (duty_target > duty_curr) {
      duty_curr += duty_change_per_interval;
      if (duty_curr >= duty_target) {
        duty_curr = duty_target;
        finished = true;
      }
    } else {
      duty_curr -= duty_change_per_interval;
      if (duty_curr <= duty_target) {
        duty_curr = duty_target;
        finished = true;
      }
    }

    myservo.writeMicroseconds((int)(duty_curr + 0.5));
  }

  Serial.print("Min:500");
  Serial.print(",duty_target:"); Serial.print(duty_target);
  Serial.print(",duty_curr:");   Serial.print(duty_curr);
  Serial.println(",Max:2500");

  last_sampling_time += INTERVAL;
}
