int ledPin=13;

#define LEN_ROBO 21
#define LEN_PC 19

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
  double *vx;          //4
  double *vy;          //4
  double *checkSum;   //4
  char *frameEnd;     //1
};
PcFrame pcF;
char pcStr[LEN_PC];

void recvFromPC() {
  for (int i = 0; i < LEN_PC; i++) {
    pcStr[i] = Serial.read();
  }
  char *p = pcStr;
  pcF.start = p;
  pcF.start1 = (char*)(p=p+sizeof(char));
  pcF.len = (int*)(p=p+sizeof(char));
  pcF.id = (int*)(p=p+sizeof(int));
  pcF.vx = (double*)(p=p+sizeof(int));
  pcF.vy = (double*)(p=p+sizeof(double));
  pcF.checkSum = (double*)(p=p+sizeof(double));
  pcF.frameEnd = (char*)(p=p+sizeof(double));
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
  if (*pcF.start == '~' && *pcF.start1 == '~' && *pcF.frameEnd == '!' && (*pcF.checkSum == (*pcF.vx + *pcF.vy))) {
    for (int i = 0; i < LEN_PC; i++) {
      Serial1.print(pcStr[i]);
    }
    digitalWrite(ledPin, HIGH);    
  }
  Serial.flush();
  Serial1.flush();
  delay(50);
}
