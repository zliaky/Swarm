int ledPin=13;

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
char str[21];

void recv() {
  for (int i = 0; i < 21; i++) {
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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(ledPin,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  recv();
  if (*frame.start == '~' && *frame.start1 == '~' && *frame.frameEnd == '!' && (*frame.checkSum == (*frame.x + *frame.y))) {
    for (int i = 0; i < 21; i++) {
//      Serial.print(str[i]);
    }
    Serial.print(id);
    Serial.print(',');
    Serial.print(X);
    Serial.print(',');
    Serial.print(Y);
    Serial.print(',');
    Serial.println(Angle);
  }
  delay(100);
}
