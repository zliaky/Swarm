int ledPin=13;

#define LEN_ROBO 21
#define LEN_PC 33

double X,Y;
int id, Angle;

struct Frame {
  char *start;        //1
  char *start1;       //1
  int *len;           //2
  int *id;            //2
  double *x;          //4
  double *y;          //4
  int *angle;         //2
  double *checkSum;   //4
  char *frameEnd;     //1
};

Frame frame;
char str[LEN_ROBO];

void recvFromRobot() {
  for (int i = 0; i < LEN_ROBO; i++) {
    str[i] = Serial1.read();
  }
  char *p = str;
  frame.start = p;
  frame.start1 = (char*)(p=p+sizeof(char));
  frame.len = (int*)(p=p+sizeof(char));
  frame.id = (int*)(p=p+sizeof(int));
  frame.x = (double*)(p=p+sizeof(int));
  frame.y = (double*)(p=p+sizeof(double));
  frame.angle = (int*)(p=p+sizeof(double));
  frame.checkSum = (double*)(p=p+sizeof(int));
  frame.frameEnd = (char*)(p=p+sizeof(double));
  id = *frame.id;
  X = *frame.x;
  Y = *frame.y;
  Angle = *frame.angle;
}

struct PcFrame {
  char *start;        //1
  char *start1;       //1
  int *len;           //2
  int *id;            //2
  double *x;          //4
  double *y;          //4
  double *vx;         //4
  double *vy;         //4
  int *dA;            //2
  double *angV;       //4
  double *checkSum;   //4
  char *frameEnd;     //1
};
PcFrame pcF;
char pcStr[LEN_PC];

struct DebugFrame {
  char *start;
  char *start1;
  long *id;
  long *dir[3];
  long *pwm[3];
  long *checkSum;
  char *frameEnd;
};
DebugFrame dFrame;
char dStr[sizeof(DebugFrame)];

void recvFromPC() {
  char start = Serial.read();
  char start1 = Serial.read();
  if (start == '~' && start1 == '~') {
    pcStr[0] = start;
    pcStr[1] = start1;
    for (int i = 2; i < LEN_PC; i++) {
      pcStr[i] = Serial.read();
    }
    char *p = pcStr;
    pcF.start = p;
    pcF.start1 = (char*)(p=p+sizeof(char));
    pcF.len = (int*)(p=p+sizeof(char));
    pcF.id = (int*)(p=p+sizeof(int));
    pcF.x = (double*)(p=p+sizeof(int));
    pcF.y = (double*)(p=p+sizeof(double));
    pcF.vx = (double*)(p=p+sizeof(double));
    pcF.vy = (double*)(p=p+sizeof(double));
    pcF.dA = (int*)(p=p+sizeof(double));
    pcF.angV = (double*)(p=p+sizeof(int));
    pcF.checkSum = (double*)(p=p+sizeof(double));
    pcF.frameEnd = (char*)(p=p+sizeof(double));
    if (*pcF.frameEnd == '!' && (*pcF.checkSum == (*pcF.x + *pcF.y))) {
      for (int i = 0; i < LEN_PC; i++) {
        Serial1.print(pcStr[i]);
      }
      digitalWrite(ledPin, HIGH);
    }
  } else if (start == 'D' && start1 == 'e') {
    dStr[0] = start;
    dStr[1] = start1;
    for (int i = 2; i < sizeof(DebugFrame); i++) {
      dStr[i] = Serial.read();
    }
    char *p = dStr;
    dFrame.start = p;
    dFrame.start1 = (char*)(p=p+sizeof(char));
    dFrame.id = (long*)(p=p+sizeof(char));
    dFrame.dir[0] = (long*)(p=p+sizeof(long));
    dFrame.dir[1] = (long*)(p=p+sizeof(long));
    dFrame.dir[2] = (long*)(p=p+sizeof(long));
    dFrame.pwm[0] = (long*)(p=p+sizeof(long));
    dFrame.pwm[1] = (long*)(p=p+sizeof(long));
    dFrame.pwm[2] = (long*)(p=p+sizeof(long));
    dFrame.checkSum = (long*)(p=p+sizeof(long));
    dFrame.frameEnd = (char*)(p=p+sizeof(long));
    if (*dFrame.frameEnd == '!' && (*dFrame.checkSum == (*dFrame.dir[0] + *dFrame.pwm[0]))) {
      for (int i = 0; i < sizeof(DebugFrame); i++) {
        Serial1.print(dStr[i]);
      }
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(ledPin,OUTPUT);
}

void loop() {
  digitalWrite(ledPin, LOW);
  // put your main code here, to run repeatedly:
  recvFromRobot();
  if (*frame.start == '~' && *frame.start1 == '~' && *frame.frameEnd == '!' && (*frame.checkSum == (*frame.x + *frame.y))) {
    for (int i = 0; i < LEN_ROBO; i++) {
      Serial.print(str[i]);
    }
/*    Serial.print(id);
    Serial.print(',');
    Serial.print(X);
    Serial.print(',');
    Serial.print(Y);
    Serial.print(',');
    Serial.println(Angle);*/
  }
  recvFromPC();
  Serial.flush();
  Serial1.flush();
  delay(50);
}
