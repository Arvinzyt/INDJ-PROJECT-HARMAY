const int stepPin    = 2;
const int dirPin     = 3;
const int switchPin  = 4;   // LED 开关输入
const int sensorPin  = 5;   // 光电传感器（常闭）
const int enaPin     = 6;   // DM556 ENA 控制
const int ledRelay   = 7;   // 控制 LED 的继电器模块

bool currentDir = HIGH;
bool lastSensorState = LOW;

void setup() {
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(switchPin, INPUT_PULLUP);
  pinMode(sensorPin, INPUT_PULLUP);
  pinMode(enaPin, OUTPUT);
  pinMode(ledRelay, OUTPUT);

  digitalWrite(dirPin, currentDir);
  digitalWrite(enaPin, HIGH);     // 默认电机不输出
  digitalWrite(ledRelay, LOW);    // 默认继电器关闭（LED 关闭）
}

void loop() {
  bool switchState = digitalRead(switchPin);
  bool sensorState = digitalRead(sensorPin);

  if (switchState == HIGH) {
    // 开关按下，启动电机和LED
    digitalWrite(enaPin, LOW);      // 启用电机驱动
    digitalWrite(ledRelay, HIGH);   // 启动继电器（LED点亮）

    // 电机运行：发送脉冲
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);

    // 光电触发一次，切换方向
    if (sensorState == HIGH && lastSensorState == LOW) {
      currentDir = !currentDir;
      digitalWrite(dirPin, currentDir);
      delay(200);  // 防抖
    }

    lastSensorState = sensorState;

  } else {
    // 开关未按，关闭电机和LED
    digitalWrite(enaPin, HIGH);     
    digitalWrite(ledRelay, LOW);    
}
