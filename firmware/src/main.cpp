#include <NimBLEDevice.h>
#include <FastLED.h>
#include <vector>

// ==========================================
// ハードウェア設定 / Hardware Configuration
// ==========================================
#define LED_PIN     4        // LEDテープのデータピン (GPIO4)
#define NUM_LEDS    216      // LEDの総数
#define COLOR_ORDER RGB      // 色の並び順
#define CHIPSET     WS2812B  // LEDチップセットの種類

// ==========================================
// LEDセグメント定義 / LED Segment Definitions
// ==========================================
// インフィニティミラーの各パーツのLED範囲を定義します
#define CENTRAL_RING_START 0
#define CENTRAL_RING_COUNT 38
#define OUTER_INNER_RING_START 38
#define OUTER_INNER_RING_COUNT 147
#define OUTER_OUTER_RING_START 185
#define OUTER_OUTER_RING_COUNT 31

// ==========================================
// グローバル変数 / Global Variables
// ==========================================
CRGB leds[NUM_LEDS];

// デバイスの状態を管理する構造体
struct DeviceState {
    int mode = 0;               // 現在の点灯モード
    uint8_t brightness = 60;    // 明るさ (0-255)
    int speed = 50;             // アニメーション速度 (0-100)
    CRGB customColor = CRGB::Blue; // ユーザー指定色
} state;

// ==========================================
// プロトタイプ宣言 / Function Prototypes
// ==========================================
void drawAurora();      // オーロラ
void drawCyberpunk();   // サイバーパンク
void drawMatrix();      // マトリックス
void drawFire();        // 炎
void drawMeteor();      // 流星
void drawSolidColor();  // 単色
void drawRainbow();     // レインボー（既存）
void drawSparkle();     // スパークル（既存）

// ==========================================
// セットアップ / Setup
// ==========================================
void setup() {
  Serial.begin(115200);
  Serial.println("Starting Infinity Mirror Firmware...");

  // FastLEDの初期化
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setMaxPowerInVoltsAndMilliamps(12, 3000); // 電流制限 (12V 3A)
  FastLED.setBrightness(state.brightness);

  // BLEの初期化
  NimBLEDevice::init("InfinityMirror");
  NimBLEServer *pServer = NimBLEDevice::createServer();
  NimBLEService *pService = pServer->createService("12345678-1234-1234-1234-1234567890ab");
  NimBLECharacteristic *pCharacteristic = pService->createCharacteristic(
    "abcd1234-5678-90ab-cdef-1234567890ab",
    NIMBLE_PROPERTY::WRITE
  );

  // BLEコールバッククラス
  class MyCallbacks : public NimBLECharacteristicCallbacks {
    void onWrite(NimBLECharacteristic *pCharacteristic) override {
      std::string value = pCharacteristic->getValue();
      Serial.printf("BLE Received: %s\n", value.c_str());
      
      // コマンド解析
      if (value.rfind("MODE:", 0) == 0) {
        state.mode = atoi(value.substr(5).c_str());
        Serial.printf("Mode -> %d\n", state.mode);
      } 
      else if (value.rfind("SPEED:", 0) == 0) {
        state.speed = atoi(value.substr(6).c_str());
        Serial.printf("Speed -> %d\n", state.speed);
      } 
      else if (value.rfind("BRIGHTNESS:", 0) == 0) {
        int b = atoi(value.substr(11).c_str());
        state.brightness = map(b, 0, 100, 0, 255);
        FastLED.setBrightness(state.brightness);
        Serial.printf("Brightness -> %d\n", state.brightness);
      } 
      else if (value.rfind("COLOR:", 0) == 0) {
        // "COLOR:r,g,b" 形式の解析
        std::string colorStr = value.substr(6);
        int firstComma = colorStr.find(',');
        int secondComma = colorStr.find(',', firstComma + 1);
        
        if (firstComma != std::string::npos && secondComma != std::string::npos) {
          int r = atoi(colorStr.substr(0, firstComma).c_str());
          int g = atoi(colorStr.substr(firstComma + 1, secondComma - firstComma - 1).c_str());
          int b = atoi(colorStr.substr(secondComma + 1).c_str());
          
          state.customColor = CRGB(r, g, b);
          state.mode = 99; // カスタムカラーモード
          Serial.printf("Color -> R:%d G:%d B:%d\n", r, g, b);
        }
      }
    }
  };

  pCharacteristic->setCallbacks(new MyCallbacks());
  pService->start();
  NimBLEDevice::getAdvertising()->addServiceUUID("12345678-1234-1234-1234-1234567890ab");
  NimBLEDevice::getAdvertising()->start();
  
  Serial.println("BLE Ready & Advertising");
}

// ==========================================
// メインループ / Main Loop
// ==========================================
void loop() {
  // モードに応じた描画関数の呼び出し
  switch (state.mode) {
    case 0: drawAurora(); break;    // オーロラ
    case 1: drawCyberpunk(); break; // サイバーパンク
    case 2: drawMatrix(); break;    // マトリックス
    case 3: drawFire(); break;      // 炎
    case 4: drawMeteor(); break;    // 流星
    case 5: drawRainbow(); break;   // レインボー
    case 6: drawSparkle(); break;   // スパークル
    case 99: drawSolidColor(); break; // カスタム単色
    default: drawAurora(); break;
  }
  
  FastLED.show();
  
  // 速度に応じたウェイト調整
  // Speed 0 (遅い) -> 100ms
  // Speed 100 (速い) -> 5ms
  int delayTime = map(state.speed, 0, 100, 100, 5);
  delay(delayTime);
}

// ==========================================
// パターン実装 / Pattern Implementations
// ==========================================

// 🌌 オーロラ: 滑らかな色の変化
void drawAurora() {
  static uint8_t hue = 0;
  // Perlin Noiseのような滑らかな変化を作るためにsin波を合成
  for (int i = 0; i < NUM_LEDS; i++) {
    uint8_t colorIndex = hue + (i * 2);
    leds[i] = ColorFromPalette(OceanColors_p, colorIndex, 255, LINEARBLEND);
  }
  hue++;
}

// 🌆 サイバーパンク: ネオンピンクと青のグリッチ演出
void drawCyberpunk() {
  static uint8_t baseHue = 0;
  
  // ベースは暗めの青紫
  fill_solid(leds, NUM_LEDS, CRGB(10, 0, 30));
  
  // ランダムな位置にネオンピンクとシアンのパルス
  if (random8() < 40) {
    int pos = random(NUM_LEDS);
    leds[pos] = CRGB::DeepPink;
  }
  if (random8() < 40) {
    int pos = random(NUM_LEDS);
    leds[pos] = CRGB::Cyan;
  }
  
  // 時々全体がフラッシュするグリッチ効果
  if (random8() < 5) {
    fill_solid(leds, NUM_LEDS, CRGB::White);
  }
}

// 🟩 マトリックス: 緑色の文字が落ちてくるような演出
void drawMatrix() {
  fadeToBlackBy(leds, NUM_LEDS, 20); // 残像を残す
  
  // ランダムな位置から「滴」を落とす
  if (random8() < 30) {
    int pos = random(NUM_LEDS);
    leds[pos] = CRGB::Green;
  }
  
  // 非常に明るい白のヘッドを追加してデジタル感を出す
  if (random8() < 10) {
    int pos = random(NUM_LEDS);
    leds[pos] = CRGB::White;
  }
}

// 🔥 炎: ゆらめく赤とオレンジ
void drawFire() {
  // HeatColorsパレットを使用して炎を表現
  static uint8_t heat[NUM_LEDS];

  // 冷却処理
  for( int i = 0; i < NUM_LEDS; i++) {
    heat[i] = qsub8( heat[i],  random8(0, ((55 * 10) / NUM_LEDS) + 2));
  }

  // 熱の拡散
  for( int k= NUM_LEDS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
  }

  // 新しい火花
  if( random8() < 120 ) {
    int y = random8(7);
    heat[y] = qadd8( heat[y], random8(160,255) );
  }

  // 色への変換
  for( int j = 0; j < NUM_LEDS; j++) {
    CRGB color = ColorFromPalette( HeatColors_p, heat[j]);
    leds[j] = color;
  }
}

// 🌠 流星: 高速で流れる光の筋
void drawMeteor() {
  fadeToBlackBy(leds, NUM_LEDS, 60); // 尾を引くためにフェードアウト
  
  static int pos = 0;
  
  // 流星の核
  leds[pos] = CRGB::White;
  
  // 尾っぽ（少し後ろを青白く）
  if (pos > 0) leds[pos-1] = CRGB(200, 200, 255);
  if (pos > 1) leds[pos-2] = CRGB(100, 100, 255);
  
  pos++;
  if (pos >= NUM_LEDS) {
    pos = 0;
  }
}

// 🌈 レインボー（既存改修）
void drawRainbow() {
  static uint8_t hue = 0;
  fill_rainbow(leds, NUM_LEDS, hue, 7);
  hue++;
}

// ✨ スパークル（既存改修）
void drawSparkle() {
  fadeToBlackBy(leds, NUM_LEDS, 10);
  if (random8() < 80) {
    leds[random(NUM_LEDS)] = CRGB::White;
  }
}

// 🎨 カスタム単色
void drawSolidColor() {
  fill_solid(leds, NUM_LEDS, state.customColor);
}
