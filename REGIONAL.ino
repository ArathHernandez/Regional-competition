#include <MotoresB.h>
#include <NewPing.h>
#include <Adafruit_VL53L0X.h>
#include <Servo.h>
#include <Wire.h>
#include <Rampa.h>
#include <Adafruit_TCS34725.h>
#include <Adafruit_MLX90614.h>

#define MAX_DISTANCE 220
#define ECHO_PIN_E 49
#define TRIGGER_PIN_E 48
#define TRIGGER_PIN_A 42
#define ECHO_PIN_A 43
#define TRIGGER_PIN_DE   46
#define ECHO_PIN_DE      47
#define TRIGGER_PIN_DA 40
#define ECHO_PIN_DA 41
#define TRIGGER_PIN_IE   44
#define ECHO_PIN_IE     45
#define TRIGGER_PIN_IA 38
#define ECHO_PIN_IA 39

MotoresB robot;
Rampa subir;
NewPing sonarE(TRIGGER_PIN_E, ECHO_PIN_E, MAX_DISTANCE); 
NewPing sonarA(TRIGGER_PIN_A, ECHO_PIN_A, MAX_DISTANCE); 
NewPing sonarDE(TRIGGER_PIN_DE, ECHO_PIN_DE, MAX_DISTANCE); 
NewPing sonarDA(TRIGGER_PIN_DA, ECHO_PIN_DA, MAX_DISTANCE); 
NewPing sonarIE(TRIGGER_PIN_IE, ECHO_PIN_IE, MAX_DISTANCE);  
NewPing sonarIA(TRIGGER_PIN_IA, ECHO_PIN_IA, MAX_DISTANCE);  
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
Servo myservo;

int pasado = false;
char orientacion = 'N';
bool visitados[18][18][2];
bool victima[18][18][2];
bool blackTile[18][18][2];
bool victimas[18][18][2];
bool negro = false;
int x = 9;
int y = 9; 
int z = 0;
int distanciaE;
int distanciaA;
int distanciaDE;
int distanciaDA;
int distanciaIE;
int distanciaIA;
int negroRmenor=100;
int negroRmayor=1800;
int negroGmenor=100;
int negroGmayor=500;
int negroBmenor=100;
int negroBmayor=500;
int contador = 0;

int ledAzul= 22;
int ledAmarillo= 23;

int distanciaEnfrente()
{
  int uSDE = sonarE.ping_median();
  int distancia;
  int turns = 0;
  distancia = uSDE / US_ROUNDTRIP_CM;
  
  if(distancia < 4 && turns < 3){
  distancia = uSDE / US_ROUNDTRIP_CM;
  turns++;}
  else if(distancia < 4)
  distancia = 2000;

  Serial.print("Distancia Enfrente= ");
  Serial.println(distancia);
  return distancia;
}

int distanciaAtras()
{
  int uSDE = sonarA.ping_median();
  int distancia;
  int turns = 0;
  distancia = uSDE / US_ROUNDTRIP_CM;
  
  if(distancia < 4 && turns < 3){
  distancia = uSDE / US_ROUNDTRIP_CM;
  turns++;}
  else if(distancia < 4)
  distancia = 2000;

  Serial.print("Distancia Atras= ");
  Serial.println(distancia);
  return distancia;
}

int distanciaDerechaEnfrente()
{
  int uSDE = sonarDE.ping_median();
  int distancia;
  int turns = 0;
  distancia = uSDE / US_ROUNDTRIP_CM;
  
  if(distancia < 4 && turns < 3){
  distancia = uSDE / US_ROUNDTRIP_CM;
  turns++;}
  else if(distancia < 4)
  distancia = 2000;

  
  Serial.print("Distancia Derecha Enfrente= ");
  Serial.println(distancia);
  return distancia;
}

int distanciaDerechaAtras()
{
  int uSDE = sonarDA.ping_median();
  int distancia;
  int turns = 0;
  distancia = uSDE / US_ROUNDTRIP_CM;
  
  if(distancia < 4 && turns < 3){
  distancia = uSDE / US_ROUNDTRIP_CM;
  turns++;}
  else if(distancia < 4)
  distancia = 2000;


  Serial.print("Distancia Derecha Atras= ");
  Serial.println(distancia);
  return distancia;
}

int distanciaIzquierdaEnfrente()
{
  int uSDE = sonarIE.ping_median();
  int distancia;
  int turns = 0;
  distancia = uSDE / US_ROUNDTRIP_CM;
  
  if(distancia < 4 && turns < 3){
  distancia = uSDE / US_ROUNDTRIP_CM;
  turns++;}
  else if(distancia < 4)
  distancia = 2000;

  Serial.print("Distancia Izquierda Enfrente= ");
  Serial.println(distancia);
  return distancia;
}

int distanciaIzquierdaAtras()
{
  int uSDE = sonarIA.ping_median();
  int distancia;
  int turns = 0;
  distancia = uSDE / US_ROUNDTRIP_CM;
  
  if(distancia < 4 && turns < 3){
  distancia = uSDE / US_ROUNDTRIP_CM;
  turns++;}
  else if(distancia < 4)
  distancia = 2000;

  Serial.print("Distancia Izquierda Atras= ");
  Serial.println(distancia);
  Serial.println("***********");
  return distancia;
}

void ignore()
{
  
}

bool isBlack()
{
  int r, g, b, c, colorTemp, lux;
  
  tcs.getRawData(&r, &g, &b, &c);
  delay(300);
  colorTemp = tcs.calculateColorTemperature(r, g, b);
  lux = tcs.calculateLux(r, g, b); 
  if((r<=negroRmayor)){
  blackTile[x][y][z] = true;
  return true;}
  else
  return false;
}


void hayVictima()
{
  int pos;
    digitalWrite(22, HIGH);
  for (pos = 117; pos <= 200; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(5);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 200; pos >= 117; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(5);                       // waits 15ms for the servo to reach the position
  }
  digitalWrite(22, LOW);
}

void setup() {
  Serial.begin(9600);
  mlx.begin();  
  myservo.write(117);
  myservo.attach(37);
  pinMode(29, INPUT_PULLUP);
  pinMode(32, INPUT_PULLUP);
  pinMode(22, OUTPUT);
  
  for(int i = 0; i < 18; i++)
    for(int j = 0; j < 18; j++)
      for(int k = 0; k < 2; k++)
        {
          visitados[i][j][k] == false;
          victima[i][j][k] == false;
        }
  robot.setup();
  subir.setup();

  if (!tcs.begin()) 
  {
    Serial.println("Error al iniciar TCS34725");
    while (1) delay(1000);
  }
  
  visitados[x][y][z] = true;
  
  delay(2000);
}

void loop() {

int pos;
    int valor = 0;

    for(int j=0; j<90; j++){
    if((digitalRead(32)==LOW)&&j<80)
    {
      robot.acomodoI();
      delay(280);
      robot.detenerse();
      j=j-15;
    }
    else if((digitalRead(29)==LOW)&&j<80){
      robot.acomodoD();
      delay(280);
      robot.detenerse();
      j=j-15;
    }
    robot.moveAdelante();
    delay(1);
     
    if(mlx.readObjectTempC() > 27 && pasado == false){
      pasado = true;
      robot.detenerse();
      victimas[x][y][z] == true;
      digitalWrite(22, HIGH);
      delay(5000);
  for (pos = 117; pos <= 200; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(5);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 200; pos >= 117; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(5);                       // waits 15ms for the servo to reach the position
    digitalWrite(22, LOW);
  }
}
    }

    pasado = false;

  valor = subir.detectaRampa();
  if(valor != 0){
  delay(250);
  if(z == 1)
  z = 0;
  else
  z = 1;}
  
  robot.detenerse();
  delay(50);
  distanciaE=distanciaEnfrente();
    distanciaA=distanciaAtras();
    distanciaDE=distanciaDerechaEnfrente();
    distanciaDA=distanciaDerechaAtras();
    distanciaIE=distanciaIzquierdaEnfrente();
    distanciaIA=distanciaIzquierdaAtras();
    if((distanciaDE!=distanciaDA||distanciaIE!=distanciaIA)&&((distanciaDE<20 && distanciaDA<20)||(distanciaIE<20 && distanciaIA<20))){
  Serial.println("Desalineado********");
  if(distanciaDE>distanciaDA && distanciaIE<distanciaIA){
    while(distanciaDE>distanciaDA && distanciaIE<distanciaIA){
       robot.moveDerAcomodo();
       digitalWrite(ledAzul, HIGH); 
       distanciaDE=distanciaDerechaEnfrente();
       distanciaDA=distanciaDerechaAtras();
       delay(50);
    }  
    robot.detenerse();
    digitalWrite(ledAzul, LOW);    
  }
   
  if(distanciaDE<distanciaDA && distanciaIE>distanciaIA){
    while(distanciaDE<distanciaDA && distanciaIE>distanciaIA){
     robot.moveIzqAcomodo();  
   digitalWrite(ledAmarillo, HIGH);
   distanciaIE=distanciaIzquierdaEnfrente();
   distanciaIA=distanciaIzquierdaAtras();
   delay(50);
    } 
   robot.detenerse();
   digitalWrite(ledAmarillo, LOW);  
}

}

  distanciaE = distanciaEnfrente();

  if(distanciaE > 6 && distanciaE < 15)
   {
    while(distanciaE > 6){
      distanciaE = distanciaEnfrente();
      robot.moveAdelanteLento();}
   }

   robot.detenerse();

  switch(orientacion)
    {
      case 'N':
        x--;
        break;
      case 'E':
        y++;
        break;
      case 'O':
        y--;
        break;
       case 'S':
       x++;
       break;  
    }

   visitados[x][y][z] = true;

   negro = isBlack();

  if(negro == false)
  {
    distanciaDE = distanciaDerechaEnfrente();
    distanciaIE = distanciaIzquierdaEnfrente();
    distanciaE = distanciaEnfrente();

    if((distanciaDE == 2000 || distanciaDE > 15) && orientacion == 'N' && visitados[x][y+1][z] == false)
      {
        robot.moveDer();
        robot.detenerse();
        delay(200);
        orientacion = 'E';

        if(contador >= 3 && distanciaIE < 15)
          {
            contador = 0;
            robot.moveAtras();
            delay(300);
            contador  = 0;
            robot.moveAdelante();
            delay(200);
            robot.detenerse();
            delay(100);
          }
      }
      else if((distanciaDE == 2000 || distanciaDE > 15) && orientacion == 'E' && visitados[x+1][y][z] == false)
      {
        robot.moveDer();
        robot.detenerse();
        delay(200);
        orientacion = 'S';

        if(contador >= 3 && distanciaIE < 15)
          {
            robot.moveAtras();
            delay(300);
            contador  = 0;
            robot.moveAdelante();
            delay(200);
            robot.detenerse();
            delay(100);
          }
      }
      else if((distanciaDE == 2000 || distanciaDE > 15) && orientacion == 'S' && visitados[x][y-1][z] == false)
      {
        robot.moveDer();
        robot.detenerse();
        delay(200);
        orientacion = 'O';

        if(contador >= 3 && distanciaIE <20)
          {
            robot.moveAtras();
            delay(300);
            contador  = 0;
            robot.moveAdelante();
            delay(200);
            robot.detenerse();
            delay(100);
          }
      }
      else if((distanciaDE == 2000 || distanciaDE > 15) == 'O' && visitados[x-1][y][z] == false)
      {
        robot.moveDer();
        robot.detenerse();
        delay(200);
        orientacion = 'N';

        if(contador >= 3 && distanciaIE <20)
          {
            robot.moveAtras();
            delay(300);
            contador  = 0;
            robot.moveAdelante();
            delay(200);
            robot.detenerse();
            delay(100);
          }
      }
      else if((distanciaE == 0 || distanciaE > 15) && orientacion == 'N' && visitados[x-1][y][z] == false)
      {
        ignore();
      }
      else if((distanciaE == 0 || distanciaE > 15) && orientacion == 'E' && visitados[x][y+1][z] == false)
      {
        ignore();
      }
      else if((distanciaE == 0 || distanciaE > 15) && orientacion == 'S' && visitados[x-1][y][z] == false)
      {
        ignore();
      }
      else if((distanciaE == 0 || distanciaE > 15) && orientacion == 'O' && visitados[x][y-1][z] == false)
      {
        ignore();
      }
      else if((distanciaIE == 2000 || distanciaIE > 15) && orientacion == 'N' && visitados[x][y-1][z] == false)
      {
        robot.moveIzq();
        robot.detenerse();
        delay(200);
        orientacion = 'O';

        if(contador >= 3 && distanciaDE <20)
          {
            robot.moveAtras();
            delay(300);
            contador  = 0;
            robot.moveAdelante();
            robot.moveAdelante();
            delay(200);
            robot.detenerse();
            delay(100);
          }
      }
      else if((distanciaIE == 2000 || distanciaIE > 15) && orientacion == 'E' && visitados[x-1][y][z] == false)
      {
        robot.moveIzq();
        robot.detenerse();
        delay(200);
        orientacion = 'N';

        if(contador >= 3 && distanciaDE <20)
          {
            robot.moveAtras();
            delay(300);
            contador  = 0;
            robot.moveAdelante();
            delay(200);
            robot.detenerse();
            delay(100);
          }
      }
      else if((distanciaIE == 2000 || distanciaIE > 15) && orientacion == 'S' && visitados[x][y+1][z] == false)
      {
        robot.moveIzq();
        robot.detenerse();
        delay(200);
        orientacion = 'E';

        if(contador >= 3 && distanciaDE <20)
          {
            robot.moveAtras();
            delay(300);
            contador  = 0;
            robot.moveAdelante();
            delay(200);
            robot.detenerse();
            delay(100);
          }
      }
      else if((distanciaIE == 2000 || distanciaIE > 15) && orientacion == 'O' && visitados[x+1][y][z] == false)
      {
        robot.moveIzq();
        robot.detenerse();
        delay(200);
        orientacion = 'S';

        if(contador >= 3 && distanciaDE <20)
          {
            robot.moveAtras();
            delay(300);
            contador  = 0;
            robot.moveAdelante();
            delay(200);
            robot.detenerse();
            delay(100);
          }
      }
      else if(distanciaIE < 15 && distanciaDE < 15 && distanciaE < 15)
      {
        robot.moveDer();
        robot.detenerse();
        delay(200);
        robot.moveDer();
        robot.detenerse();
        delay(200);
        if(orientacion == 'N')
          orientacion = 'S';
          else if(orientacion == 'E')
            orientacion = 'O';
            else if(orientacion == 'S')
              orientacion = 'N';
                else if(orientacion == 'O')
                  orientacion = 'E';
      }
      else if((distanciaDE == 2000 || distanciaDE > 15) && orientacion == 'N' && blackTile[x][y+1][z] == false)
      {
        robot.moveDer();
        robot.detenerse();
        delay(200);
        orientacion = 'E';
        contador++;
        
        if(contador >= 3 && distanciaIE <20)
          {
            robot.moveAtras();
            delay(300);
            contador  = 0;
            robot.moveAdelante();
            delay(200);
            robot.detenerse();
            delay(100);
          }
      }
      else if((distanciaDE == 2000 || distanciaDE > 15) && orientacion == 'E' && blackTile[x+1][y][z] == false)
      {
        robot.moveDer();
        robot.detenerse();
        delay(200);
        orientacion = 'S';
        contador++;

        if(contador >= 3 && distanciaIE <20)
          {
            robot.moveAtras();
            delay(300);
            contador  = 0;
           robot.moveAdelante();
            delay(200);
            robot.detenerse();
            delay(100);
          }
      }
      else if((distanciaDE == 2000 || distanciaDE > 15) && orientacion == 'S' && blackTile[x][y-1][z] == false)
      {
        robot.moveDer();
        robot.detenerse();
        delay(200);
        orientacion = 'O';
        contador++;

        if(contador >= 3 && distanciaIE <20)
          {
            robot.moveAtras();
            delay(300);
            contador  = 0;
            robot.moveAdelante();
            delay(200);
            robot.detenerse();
            delay(100);
          }
      }
      else if((distanciaDE == 2000 || distanciaDE > 15) && orientacion == 'O' && blackTile[x-1][y][z] == false)
      {
        robot.moveDer();
        robot.detenerse();
        delay(200);
        orientacion = 'N'; 
        contador++;

        if(contador >= 3 && distanciaIE <20)
          {
            robot.moveAtras();
            delay(300);
            contador  = 0;
            robot.moveAdelante();
            delay(200);
            robot.detenerse();
            delay(100);
          }
      }
      else if((distanciaE == 0 || distanciaE > 15) && orientacion == 'N' && blackTile[x-1][y][z] == false)
      {
        ignore();
      }
      else if((distanciaE == 0 || distanciaE > 15) && orientacion == 'E' && blackTile[x][y+1][z] == false)
      {
        ignore();
      }
      else if((distanciaE == 0 || distanciaE > 15) && orientacion == 'O' && blackTile[x][y-1][z] == false)
      {
        ignore();
      }
      else if((distanciaE == 0 || distanciaE > 15) && orientacion == 'S' && blackTile[x+1][y][z] == false)
      {
        ignore();
      }
      else if((distanciaIE == 2000 || distanciaIE > 15) && orientacion == 'N' && blackTile[x][y-1][z] == false)
      {
        robot.moveIzq();
        robot.detenerse();
        delay(200);
        orientacion = 'O';
        contador++;

        if(contador >= 3 && distanciaDE <20)
          {
            robot.moveAtras();
            delay(300);
            contador  = 0;
            robot.moveAdelante();
            delay(200);
            robot.detenerse();
            delay(100);
          }
      }
      else if((distanciaIE == 2000 || distanciaIE > 15) && orientacion == 'E' && blackTile[x-1][y][z] == false)
      {
        robot.moveIzq();
        robot.detenerse();
        delay(200);
        orientacion = 'N';
        contador++;

        if(contador >= 3 && distanciaDE <20)
          {
            robot.moveAtras();
            delay(300);
            contador  = 0;
            robot.moveAdelante();
            delay(200);
            robot.detenerse();
            delay(100);
          }
      }
      else if((distanciaIE == 2000 || distanciaIE > 15) && orientacion == 'O' && blackTile[x+1][y][z] == false)
      {
        robot.moveIzq();
        robot.detenerse();
        delay(200);
        orientacion = 'S';
        contador++;

        if(contador >= 3 && distanciaDE <20)
          {
            robot.moveAtras();
            delay(300);
            contador  = 0;
            robot.moveAdelante();
            delay(200);
            robot.detenerse();
            delay(100);
          }
      }
      else if((distanciaIE == 2000 || distanciaIE > 15) && orientacion == 'S' && blackTile[x][y+1][z] == false)
      {
        robot.moveIzq();
        robot.detenerse();
        delay(200);
        orientacion = 'E';
        contador++;

        if(contador >= 3 && distanciaDE <20)
          {
            robot.moveAtras();
            delay(300);
            contador  = 0;
            robot.moveAdelante();
            delay(200);
            robot.detenerse();
            delay(100);
          }
      }
      else
      {
        contador = contador +2;;
        robot.moveDer();
        robot.detenerse();
        delay(200);
        robot.moveDer();
        robot.detenerse();
        delay(200);
        if(orientacion == 'N')
          orientacion = 'S';
          else if(orientacion == 'E')
            orientacion = 'O';
            else if(orientacion == 'S')
              orientacion = 'N';
                else if(orientacion == 'O')
                  orientacion = 'E';
      }
  }
  else
    {
      robot.moveAtras();
      delay(890);
      robot.detenerse();
      delay(200);

      switch(orientacion)
       {
      case 'N':
        x++;
        break;
      case 'E':
        y--;
        break;
      case 'O':
        y++;
        break;
       case 'S':
       x--;
       break;  
      }

      distanciaDE = distanciaDerechaEnfrente();
      distanciaIE = distanciaIzquierdaEnfrente();

      if(distanciaDE > 15 && orientacion == 'N' && visitados[x][y+1][z] == false)
      {
        contador++;
        robot.moveDer();
        robot.detenerse();
        delay(200);
        orientacion = 'E';

        if(contador >= 3 && distanciaIE <20)
          {
            robot.moveAtras();
            delay(300);
            contador  = 0;
            robot.moveAdelante();
            delay(200);
            robot.detenerse();
            delay(100);
          }
      }
      else if(distanciaDE > 15 && orientacion == 'E' && visitados[x+1][y][z] == false)
      {
        contador++;
        robot.moveDer();
        robot.detenerse();
        delay(200);
        orientacion = 'S';

        if(contador >= 3 && distanciaIE <20)
          {
            robot.moveAtras();
            delay(300);
            contador  = 0;
            robot.moveAdelante();
            delay(200);
            robot.detenerse();
            delay(100);
          }
      }
      else if(distanciaDE > 15 && orientacion == 'S' && visitados[x][y-1][z] == false)
      {
        contador++;
        robot.moveDer();
        robot.detenerse();
        delay(200);
        orientacion = 'O';

        if(contador >= 3 && distanciaIE <20)
          {
            robot.moveAtras();
            delay(300);
            contador  = 0;
            robot.moveAdelante();
            delay(200);
            robot.detenerse();
            delay(100);
          }
      }
      else if(distanciaDE > 15 && orientacion == 'O' && visitados[x-1][y][z] == false)
      {
        contador++;
        robot.moveDer();
        robot.detenerse();
        delay(200);
        orientacion = 'N';

        if(contador >= 3 && distanciaIE <20)
          {
            robot.moveAtras();
            delay(300);
            contador  = 0;
            robot.moveAdelante();
            delay(200);
            robot.detenerse();
            delay(100);
          }
      }
      else if(distanciaIE > 15 && orientacion == 'N' && visitados[x][y-1][z] == false)
      {
        contador++;
        robot.moveIzq();
        robot.detenerse();
        delay(200);
        orientacion = 'O';

        if(contador >= 3 && distanciaDE <20)
          {
            robot.moveAtras();
            delay(300);
            contador  = 0;
            robot.moveAdelante();
            delay(200);
            robot.detenerse();
            delay(100);
          }
      }
      else if(distanciaIE > 15 && orientacion == 'E' && visitados[x-1][y][z] == false)
      {
        contador++;
        robot.moveIzq();
        robot.detenerse();
        delay(200);
        orientacion = 'N';

        if(contador >= 3 && distanciaDE <20)
          {
            robot.moveAtras();
            delay(300);
            contador  = 0;
            robot.moveAdelante();
            delay(200);
            robot.detenerse();
            delay(100);
          }
      }
      else if(distanciaIE > 15 && orientacion == 'S' && visitados[x][y+1][z] == false)
      {
        contador++;
        robot.moveIzq();
        robot.detenerse();
        delay(200);
        orientacion = 'E';

        if(contador >= 3 && distanciaDE <20)
          {
            robot.moveAtras();
            delay(300);
            contador  = 0;
            robot.moveAdelante();
            delay(200);
            robot.detenerse();
            delay(100);
          }
      }
      else if(distanciaIE > 15 && orientacion == 'O' && visitados[x+1][y][z] == false)
      {
        contador++;
        robot.moveIzq();
        robot.detenerse();
        delay(200);
        orientacion = 'S';

        if(contador >= 3 && distanciaDE <20)
          {
            robot.moveAtras();
            delay(300);
            contador  = 0;
            robot.moveAdelante();
            delay(200);
            robot.detenerse();
            delay(100);
          }
      }
      else{
        contador = contador +2;;
        robot.moveDer();
        robot.detenerse();
        delay(200);
        robot.moveDer();
        robot.detenerse();
        delay(200);
        if(orientacion == 'N')
          orientacion = 'S';
          else if(orientacion == 'E')
            orientacion = 'O';
            else if(orientacion == 'S')
              orientacion = 'N';
                else if(orientacion == 'O')
                  orientacion = 'E';

       distanciaE = distanciaEnfrente();

       if(distanciaE > 15)
        {
          ignore();
        }
        else
        {
          if(distanciaE > 6 && distanciaE < 15)
           {
            contador++;
            while(distanciaE > 6){
              distanciaE = distanciaEnfrente();
              robot.moveAdelanteLento();}
           }
          robot.moveDer();
          robot.detenerse();
          delay(200);
          switch(orientacion)
          {
            case 'N':
              orientacion = 'E';
              break;
             case 'E':
              orientacion = 'S';
              break;
             case 'S':
              orientacion = 'O';
              break;
             case 'O':
              orientacion = 'N';
              break;
          }
        }
      }
    }
  
}
