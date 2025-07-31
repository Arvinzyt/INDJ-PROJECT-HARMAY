const int stepPin     = 2;
const int dirPin      = 3;
const int switchPin   = 4;   // LED 开关（常开）
const int sensorPin   = 5;   // 光电开关（常闭）
const int relayPin    = 7;   // 控制继电器（LED）
const int enaPin      = 6;   // 控制 DM556 ENA- 引脚

bool direction = true;
bool lastSensorState = HIGH;
unsigned long lastSpeedChangeTime = 0;
int stepDelay = 1000;  // 初始速度，越小越快

void setup() {
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(switchPin, INPUT_PULLUP);   // 常开开关
  pinMode(sensorPin, INPUT_PULLUP);   // 常闭光电
  pinMode(relayPin, OUTPUT);
  pinMode(enaPin, OUTPUT);

  digitalWrite(dirPin, direction ? HIGH : LOW);
  digitalWrite(relayPin, LOW);  // 默认关闭 LED
  digitalWrite(enaPin, HIGH);   // 默认电机禁用（ENA- 高电平）

  randomSeed(analogRead(0));
}

void loop() {
  bool switchState = digitalRead(switchPin);   // HIGH = 开关按下
  bool sensorState = digitalRead(sensorPin);   // LOW = 光电被触发

  if (switchState == HIGH) {
    digitalWrite(relayPin, HIGH);   // 打开 LED（继电器吸合）
    digitalWrite(enaPin, LOW);      // 使能电机驱动（ENA- 接低）

    // 检测光电从未触发 → 触发（下降沿）
    if (lastSensorState == HIGH && sensorState == LOW) {
      direction = !direction;
      digitalWrite(dirPin, direction ? HIGH : LOW);
    }
    lastSensorState = sensorState;

    // 步进一个脉冲
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(stepDelay);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(stepDelay);

    // 每秒随机一次速度
    if (millis() - lastSpeedChangeTime > 3000) {
      lastSpeedChangeTime = millis();
      stepDelay = random(300, 1200);  // 可调速度范围
    }

  } else {
    digitalWrite(relayPin, LOW);     // 关闭继电器（LED）
    digitalWrite(enaPin, HIGH);      // 禁用电机驱动（ENA- 高）
    lastSensorState = HIGH;          // 避免误触发
  }
}
