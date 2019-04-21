

#define PWM1 5
#define PWM2 2
#define PWM3 3

#define SWITCH1_1 22
#define SWITCH1_2 23
#define SWITCH2_1 24
#define SWITCH2_2 25
#define SWITCH3_1 26
#define SWITCH3_2 27

void setup() {
  // put your setup code here, to run once:

  pinMode(SWITCH1_1, OUTPUT);
  pinMode(SWITCH1_2, OUTPUT);
  pinMode(SWITCH2_1, OUTPUT);
  pinMode(SWITCH2_2, OUTPUT);
  pinMode(SWITCH3_1, OUTPUT);
  pinMode(SWITCH3_2, OUTPUT);
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);
  pinMode(PWM3, OUTPUT);
}
#define pwm 44
void motorMove() {
  digitalWrite(SWITCH1_1, 0);
  digitalWrite(SWITCH1_2, 1);
  digitalWrite(SWITCH2_1, 1);
  digitalWrite(SWITCH2_2, 0);
  digitalWrite(SWITCH3_1, 0);
  digitalWrite(SWITCH3_2, 1);
  analogWrite(PWM1, pwm);
  analogWrite(PWM2, 0);
  analogWrite(PWM3, pwm);
}

void loop() {
  // put your main code here, to run repeatedly:
  motorMove();
}
