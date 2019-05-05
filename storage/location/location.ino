int ledPin=13;

#define LEN_ROBO 21
#define LEN_PC 21
#define LEN_DEBUG 19

double X,Y;
int id, Angle;
bool firstFrame, firstPcFrame;

int count=0;
struct Frame {
  char *start0;        //1
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
char lastStr[LEN_ROBO];

void recvFromRobot() {
  char start0 = Serial1.read();
  char start1 = Serial1.read();
  if (start0 == '~' && start1 == '~') {
    str[0] = start0;
    str[1] = start1;
    for (int i = 2; i < LEN_ROBO; i++) {
      str[i] = Serial1.read();
    }
    char *p = str;
    frame.start0 = p;
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
    if (*frame.frameEnd == '!' && (*frame.checkSum == (*frame.x + *frame.y))) {
      firstFrame = true;
      digitalWrite(ledPin, HIGH);
      for (int i = 0; i < LEN_ROBO; i++) {
        count++;
  //      Serial.print(str[i]);
        lastStr[i] = str[i];
      }
    }
  }
}

struct PcFrame {
  char *start0;        //1
  char *start1;       //1
  int *len;           //2
  int *id;            //2
  int *mode;          //2
  double *x;          //4
  double *y;          //4
  double *checkSum;   //4
  char *frameEnd;     //1
};
PcFrame pcF;
char pcStr[LEN_PC];
char lastPcStr[LEN_PC];

struct DebugFrame {
  char *start0;    //1
  char *start1;   //1
  int *id;       //2
  int *dir0;     //2
  int *dir1;     //2
  int *dir2;     //2
  int *pwm0;     //2
  int *pwm1;     //2
  int *pwm2;     //2
  int *checkSum; //2
  char *frameEnd; //1
};
DebugFrame dFrame;
char dStr[LEN_DEBUG];

void recvFromPC() {
  char start0 = Serial.read();
  char start1 = Serial.read();
  if (start0 == '~' && start1 == '~') {
    pcStr[0] = start0;
    pcStr[1] = start1;
    for (int i = 2; i < LEN_PC; i++) {
      pcStr[i] = Serial.read();
    }
    char *p = pcStr;
    pcF.start0 = p;
    pcF.start1 = (char*)(p=p+sizeof(char));
    pcF.len = (int*)(p=p+sizeof(char));
    pcF.id = (int*)(p=p+sizeof(int));
    pcF.mode = (int*)(p=p+sizeof(int));
    pcF.x = (double*)(p=p+sizeof(int));
    pcF.y = (double*)(p=p+sizeof(double));
    pcF.checkSum = (double*)(p=p+sizeof(double));
    pcF.frameEnd = (char*)(p=p+sizeof(double));
    if (*pcF.frameEnd == '!' && (*pcF.checkSum == (*pcF.x + *pcF.y))) {
      firstPcFrame = true;
      for (int i = 0; i < LEN_PC; i++) {
//        Serial1.print(pcStr[i]);
        count++;
        lastPcStr[i] = pcStr[i];
      }
    }
  } else if (start0 == 'D' && start1 == 'e') {
    dStr[0] = start0;
    dStr[1] = start1;
    for (int i = 2; i < LEN_DEBUG; i++) {
      dStr[i] = Serial.read();
    }
    char *p = dStr;
    dFrame.start0 = p;
    dFrame.start1 = (char*)(p=p+sizeof(char));
    dFrame.id = (int*)(p=p+sizeof(char));
    dFrame.dir0 = (int*)(p=p+sizeof(int));
    dFrame.dir1 = (int*)(p=p+sizeof(int));
    dFrame.dir2 = (int*)(p=p+sizeof(int));
    dFrame.pwm0 = (int*)(p=p+sizeof(int));
    dFrame.pwm1 = (int*)(p=p+sizeof(int));
    dFrame.pwm2 = (int*)(p=p+sizeof(int));
    dFrame.checkSum = (int*)(p=p+sizeof(int));
    dFrame.frameEnd = (char*)(p=p+sizeof(int));
    if (*dFrame.frameEnd == '!' && (*dFrame.checkSum == (*dFrame.dir0 + *dFrame.pwm0))) {
      for (int i = 0; i < LEN_DEBUG; i++) {
        Serial1.print(dStr[i]);
      }
    }
  }
}

void sendToRobot() {
  for (int i = 0; i < LEN_PC; i++) {
    Serial1.print(lastPcStr[i]);
  }
}

void sendToPC() {
  for (int i = 0; i < LEN_ROBO; i++) {
    Serial.print(lastStr[i]);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(ledPin,OUTPUT);
  digitalWrite(ledPin, LOW);
  firstFrame = false;
  firstPcFrame = false;
}
void loop() {
  digitalWrite(ledPin, LOW);
  recvFromRobot();
  recvFromPC();
  if (firstFrame) {
    sendToPC();
  }
  if (firstPcFrame) {
    sendToRobot();
  }
 
  if(count>3){
     Serial.flush();
     Serial1.flush();
     count=0;
 }
  delay(50);
}
