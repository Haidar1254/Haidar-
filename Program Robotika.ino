// Pin pushbutton
const int pb2 = 7;  // Untuk berhenti
const int pb1 = 4;  // Untuk mulai

// Status aktif program
bool aktif = false;

// Motor kiri
const int motor1Pin1 = 6;
const int motor1Pin2 = 9;
const int pwmMotor1 = 6; // PWM kiri

// Motor kanan
const int motor2Pin1 = 3;
const int motor2Pin2 = 5;
const int pwmMotor2 = 3; // PWM kanan

// Ambang batas sensor
const int thresholdA0 = 80;
const int thresholdA7 = 950;
const int thresholdA3 = 50;
const int thresholdA6 = 220;
const int thresholdA1 = 980;

void setup() {
  pinMode(pb2, INPUT_PULLUP);
  pinMode(pb1, INPUT_PULLUP);

  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(pwmMotor1, OUTPUT);
  pinMode(pwmMotor2, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // Cek tombol untuk start
  if (digitalRead(pb1) == LOW) {
    aktif = true;
    Serial.println("Program aktif");
    delay(300);  // debouncing
  }

  // Cek tombol untuk stop
  if (digitalRead(pb2) == LOW) {
    aktif = false;
    motorStop(1);
    motorStop(2);
    Serial.println("Program berhenti");
    delay(300);  // debouncing
  }

  // Jika belum aktif, jangan lanjut
  if (!aktif) return;

  // --- SENSOR & KONTROL GARIS ---
  int sensorA0 = analogRead(A0);  // Sensor kanan
  int sensorA1 = analogRead(A1);  // Koreksi kiri (digunakan)
  int sensorA2 = analogRead(A2);  // Tampil di Serial Monitor saja
  int sensorA3 = analogRead(A3);  // Sensor tengah
  int sensorA6 = analogRead(A6);  // Koreksi ke kanan
  int sensorA7 = analogRead(A7);  // Sensor kiri

  // Tampilkan semua data sensor
  Serial.print("A0: "); Serial.print(sensorA0);
  Serial.print(" | A1: "); Serial.print(sensorA1);
  Serial.print(" | A2: "); Serial.print(sensorA2);
  Serial.print(" | A3: "); Serial.print(sensorA3);
  Serial.print(" | A6: "); Serial.print(sensorA6);
  Serial.print(" | A7: "); Serial.println(sensorA7);

  if (sensorA1 >= thresholdA1 && sensorA7 < thresholdA7) {
    // Belok kanan
    motorStop(2);
    motorMaju(1, 200);
  }
  else if (sensorA7 >= thresholdA7 && sensorA1 < thresholdA1) {
    // Belok kiri
    motorMaju(2, 200);
    motorStop(1);
  }
  else if (sensorA3 >= thresholdA3) {
    // Maju lurus
    motorMaju(1, 138);
    motorMaju(2, 128);
  }
  else if (sensorA3 <= thresholdA3 && sensorA7 < thresholdA7 && sensorA1 < thresholdA1) {
    // Maju lurus saat tidak mendeteksi garis
    motorMaju(1, 138);
    motorMaju(2, 128);
  }
  else if (sensorA1 >= thresholdA1 && sensorA7 >= thresholdA7 ) {
    // Maju lurus jika kedua sisi mendeteksi hitam
    motorMaju(1, 138);
    motorMaju(2, 128);
  }
  else if (sensorA1 >= thresholdA1 && sensorA7 >= thresholdA7 && sensorA3 >= thresholdA3 ) {
    // Maju lurus tambahan
    motorMaju(1, 138);
    motorMaju(2, 128);
  }
}

// Fungsi motor maju
void motorMaju(int motorNum, int pwmSpeed) {
  if (motorNum == 1) {
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    analogWrite(pwmMotor1, pwmSpeed);
  } else if (motorNum == 2) {
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
    analogWrite(pwmMotor2, pwmSpeed);
  }
}

// Fungsi motor stop
void motorStop(int motorNum) {
  if (motorNum == 1) {
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    analogWrite(pwmMotor1, 0);
  } else if (motorNum == 2) {
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
    analogWrite(pwmMotor2, 0);
  }
}
