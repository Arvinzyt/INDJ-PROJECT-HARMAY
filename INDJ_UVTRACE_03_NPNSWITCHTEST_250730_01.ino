const int stepPin = 2;
const int dirPin  = 3;
const int switchPin = 4;    // LED 开关
const int sensorPin = 5;    // 光电传感器 NPN 输出

bool currentDir = HIGH;     // 当前方向
bool lastSensorState = LOW; // 上一轮光电状态

void setup() {
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(switchPin, INPUT_PULLUP);
  pinMode(sensorPin, INPUT_PULLUP);  // NPN 输出默认拉高

  digitalWrite(dirPin, currentDir);  // 设置初始方向
}

void loop() {
  bool switchState = digitalRead(switchPin);  // 控制启停
  bool sensorState = digitalRead(sensorPin);  // 光电状态

  // 只有开关打开时才运行电机逻辑
  if (switchState == HIGH) {

    // 产生一个脉冲
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);

    // 检测光电是否从未触发 → 触发（即 LOW → HIGH 的变化）
    if (sensorState == HIGH && lastSensorState == LOW) {
      // 换方向
      currentDir = !currentDir;
      digitalWrite(dirPin, currentDir);
      delay(200);  // 防抖（避免被多次触发）
    }

    // 更新上一轮状态
    lastSensorState = sensorState;
  }
}
