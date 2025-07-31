const int stepPin    = 2;
const int dirPin     = 3;
const int switchPin  = 4;   // LED 开关
const int sensorPin  = 5;   // 光电传感器
const int enaPin     = 6;   // ENA 控制

bool currentDir = HIGH;
bool lastSensorState = LOW;

void setup() {
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(switchPin, INPUT_PULLUP);
  pinMode(sensorPin, INPUT_PULLUP);
  pinMode(enaPin, OUTPUT);

  digitalWrite(dirPin, currentDir);   // 初始方向
  digitalWrite(enaPin, HIGH);         // 默认禁止（防止上电立即通电）
}

void loop() {
  bool switchState = digitalRead(switchPin);
  bool sensorState = digitalRead(sensorPin);

  if (switchState == HIGH) {
    // 开关按下：启动电机
    digitalWrite(enaPin, LOW);  // 启用驱动器

    // 脉冲驱动
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);

    // 光电状态变化：LOW → HIGH（被遮挡）
    if (sensorState == HIGH && lastSensorState == LOW) {
      currentDir = !currentDir;               // 换方向
      digitalWrite(dirPin, currentDir);
      delay(200);                             // 防抖
    }

    lastSensorState = sensorState;  // 更新状态
  } else {
    // 开关未按下：停止脉冲 + 禁用驱动器
    digitalWrite(enaPin, HIGH);  // 关闭驱动器输出
  }
}
