int ledPin=13;

double X,Y;
int id, Angle;

union SeFrame 
{
  double Double;
  byte Byte[4];
  };
SeFrame SeframX,SeframY;

union SeFrame2 
{
  int Int;
  byte Byte[2];
  };
SeFrame2 SeframA,SeframId;

struct Frame {
  char *start;        //1
  int *len;           //2
  int *id;            //2
  double *x;          //4
  double *y;          //4
  int *angle;         //2
  double *checkSum;   //4
  char *frameEnd;     //1
};

Frame frame;
char str[20];

void recv() {
  for (int i = 0; i < 20; i++) {
    str[i] = Serial1.read();
  }
  char *p = str;
  frame.start = p;
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

void Rev_double() { 
  char sym = Serial1.read();
  if(sym=='i') {
    SeframId.Byte[0] = Serial1.read( );
    SeframId.Byte[1] = Serial1.read( ); 
    id = SeframId.Int;
  }
    
  if(sym=='x') {
    SeframX.Byte[0] = Serial1.read( );
    SeframX.Byte[1] = Serial1.read( );
    SeframX.Byte[2] = Serial1.read( );
    SeframX.Byte[3] = Serial1.read( ); 
    X = SeframX.Double;
  }
    
  if(sym == 'y') {
    SeframY.Byte[0] = Serial1.read( );
    SeframY.Byte[1] = Serial1.read( );
    SeframY.Byte[2] = Serial1.read( );
    SeframY.Byte[3] = Serial1.read( ); 
    Y = SeframY.Double;
  }

  if(sym == 'a') {
    SeframA.Byte[0] = Serial1.read( );
    SeframA.Byte[1] = Serial1.read( );
    Angle = SeframA.Int;
  }
} 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(ledPin,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
//    Rev_double();
  recv();
  if (*frame.start == '~' && *frame.frameEnd == '!' && (*frame.checkSum == (*frame.x + *frame.y))) {
    for (int i = 0; i < 20; i++) {
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
  delay(100);
}
