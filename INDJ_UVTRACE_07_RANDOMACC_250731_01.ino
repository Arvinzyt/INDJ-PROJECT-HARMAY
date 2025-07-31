const int stepPin     = 2;
const int dirPin      = 3;
const int switchPin   = 4;   // LED 开关（常开）
const int sensorPin   = 5;   // 光电开关（常闭）
const int relayPin    = 7;   // 控制继电器（LED）
const int enaPin      = 6;   // 控制 DM556 ENA- 引脚

bool direction = true;
bool lastSensorState = HIGH;
unsigned long lastSpeedChangeTime = 0;

int stepDelay = 1000;       // 当前速度（微秒）
int targetDelay = 1000;     // 目标速度（微秒）

void setup() {
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(switchPin, INPUT_PULLUP);   // 常开开关
  pinMode(sensorPin, INPUT_PULLUP);   // 常闭光电
  pinMode(relayPin, OUTPUT);
  pinMode(enaPin, OUTPUT);

  digitalWrite(dirPin, direction ? HIGH : LOW);
  digitalWrite(relayPin, LOW);  // 默认关闭 LED
  digitalWrite(enaPin, HIGH);   // 默认禁用电机

  randomSeed(analogRead(0));   // 初始化随机种子
}

void loop() {
  bool switchState = digitalRead(switchPin);   // HIGH = 开关按下
  bool sensorState = digitalRead(sensorPin);   // LOW = 光电被触发

  if (switchState == HIGH) {
    digitalWrite(relayPin, HIGH);   // 打开 LED
    digitalWrite(enaPin, LOW);      // 启动电机驱动

    // 光电开关触发方向反转
    if (lastSensorState == HIGH && sensorState == LOW) {
      direction = !direction;
      digitalWrite(dirPin, direction ? HIGH : LOW);
    }
    lastSensorState = sensorState;

    // 平滑逼近目标速度
    if (stepDelay != targetDelay) {
      int diff = targetDelay - stepDelay;
      stepDelay += diff / 30;  // 调整速率百分比（越小越平滑）
    }

    // 发送步进脉冲
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(stepDelay);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(stepDelay);

    // 每 1.5 秒随机更新目标速度
    if (millis() - lastSpeedChangeTime > 3000) {
      lastSpeedChangeTime = millis();
      targetDelay = random(300, 1500);  // 新的目标速度
    }

  } else {
    digitalWrite(relayPin, LOW);     // 关闭继电器
    digitalWrite(enaPin, HIGH);      // 关闭电机驱动
    lastSensorState = HIGH;          // 重置传感器状态
  }
}
