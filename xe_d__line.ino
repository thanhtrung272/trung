#define AIN1 4
#define BIN1 7
#define AIN2 5
#define BIN2 8
#define PWMA 3
#define PWMB 9
#define STBY 6

int position , error, previous_error ;
int a[] = {A5, A4, A3, A2, A1};
float P, I, D, PID_value;

float Kp = 1.3;
float Ki = 0.001;
float Kd = 0.6;
const uint8_t motor_speed = 150;

void setup()
{
  digitalWrite( STBY , 1);
  Serial.begin(9600);
}
void readline() {

  a[1] = !digitalRead(A5);
  a[2] = !digitalRead(A4);
  a[3] = !digitalRead(A3);
  a[4] = !digitalRead(A2);
  a[5] = !digitalRead(A1);
  position = (100 * a[1] + 200 * a[2] + 300 * a[3] + 400 * a[4] + 500 * a[5]) /
             ( a[1] + a[2] + a[3] + a[4] + a[5] );
  Serial.print("vi tri ");
  if ( position > 300 )
    Serial.print(" lech trai ");
  else Serial.print(" lech phai ");

  return position;
}

void pid()
{
  error = 300 - position ;
  P = error;
  I = I + error;
  D = error - previous_error;
  PID_value = (Kp * P) + (Ki * I) + (Kd * D);
  previous_error = error;
  Serial.print("PID  ");
  Serial.print(PID_value);


}

void motor_control()
{
  int left_motor_speed = motor_speed - PID_value;
  int right_motor_speed = motor_speed + PID_value;

  if (left_motor_speed > 255) left_motor_speed = 255;
  if (right_motor_speed > 255) right_motor_speed = 255;

  if (left_motor_speed < 0) left_motor_speed = 0;
  if (right_motor_speed < 0) right_motor_speed = 0;
  analogWrite(PWMA, left_motor_speed);
  analogWrite(PWMB, right_motor_speed);

  digitalWrite( AIN1, 1);
  digitalWrite( AIN2, 0);
  digitalWrite( BIN1, 1);
  digitalWrite( BIN2, 0);
  Serial.print("  dc trai  ");
  Serial.print(left_motor_speed);
  Serial.print("  dc phai ");
  Serial.print(right_motor_speed);
  Serial.println(' ');
}
void loop()
{
  readline();
  pid();
  motor_control();
}
