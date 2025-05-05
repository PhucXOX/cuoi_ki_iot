#include <Servo.h>
#include <DHT.h>

#define DHTPIN 0         // D3 - GPIO0
#define DHTTYPE DHT11
#define TRIG_PIN 5       // D1 - GPIO5
#define ECHO_PIN 4       // D2 - GPIO4
#define BUZZER_PIN 14    // D5 - GPIO14
#define SERVO_PIN 2      // D4 - GPIO2

DHT dht(DHTPIN, DHTTYPE);
Servo myServo;

long duration;
float distance;
bool lidOpened = false;

void setup() {
  Serial.begin(9600);
  
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  dht.begin();
  myServo.attach(SERVO_PIN);
  myServo.write(0); // Đóng nắp ban đầu
}

void loop() {
  // Đo khoảng cách bằng cảm biến siêu âm
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;

  // Đọc độ ẩm
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(); // Optional

  // In thông tin ra Serial
  Serial.print("Khoang cach: ");
  Serial.print(distance);
  Serial.print(" cm\tDo am: ");
  Serial.print(humidity);
  Serial.print(" %\tNhiet do: ");
  Serial.print(temperature);
  Serial.println(" *C");

  // Nếu có vật đến gần (VD < 20cm) → mở nắp
  if (distance < 20) {
    if (!lidOpened) {
      myServo.write(180); // Mở nắp
      digitalWrite(BUZZER_PIN, HIGH); // Buzzer kêu
      delay(500);
      digitalWrite(BUZZER_PIN, LOW);
      lidOpened = true;
    }
  } else {
    if (lidOpened) {
      myServo.write(0); // Đóng nắp
      lidOpened = false;
    }
  }

  // Nếu độ ẩm > 83% → buzzer kêu cảnh báo
  if (humidity > 83) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(300);
    digitalWrite(BUZZER_PIN, LOW);
    delay(300);
  }

  delay(1000); // Lặp lại sau mỗi giây
}