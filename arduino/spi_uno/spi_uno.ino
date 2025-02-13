#include <Keypad.h>

// Tuş takımı için satır ve sütun sayısı
const byte ROWS = 4;
const byte COLS = 4;

// Tuş takımındaki tuş dizilimi
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// Tuş takımının satır ve sütun pinleri
byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8, 9};

// Keypad nesnesi oluşturuluyor
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// SPI pin tanımları
#define MOSI 11  // Master Out Slave In
#define MISO 12  // Master In Slave Out
#define SCK 13   // Serial Clock
#define SS 10    // Slave Select

// Tuş takımından girilen sayı burada birikir
String inputNumber = "";

void setup() {
  // SPI pinlerinin giriş/çıkış olarak ayarlanması
  pinMode(MOSI, OUTPUT);
  pinMode(MISO, INPUT);
  pinMode(SCK, OUTPUT);
  pinMode(SS, OUTPUT);

  // Slave Select ve LED pinlerinin başlangıç durumları
  digitalWrite(SS, HIGH);

  // Seri haberleşme başlatılıyor
  Serial.begin(9600);
}

// SPI üzerinden veri göndermek için fonksiyon
uint8_t sendSPI(uint8_t data) {
  uint8_t received; // Karşı cihazdan alınan veri
  
  // Saat sinyali ve veri gönderimi
  digitalWrite(SCK, LOW);
  digitalWrite(MOSI, LOW);
  delayMicroseconds(200);
  
  // 8 bitlik veriyi gönder
  for (int i = 7; i >= 0; i--) {
    digitalWrite(MOSI, (data >> i) & 0x01); // Veriyi bit bit gönder
    delayMicroseconds(200);
    
    digitalWrite(SCK, HIGH); // Saat sinyalini HIGH yap
    delayMicroseconds(200);
    
    digitalWrite(SCK, LOW);  // Saat sinyalini LOW yap
    delayMicroseconds(200);
  }
  
  delayMicroseconds(500); // Karşı cihazdan yanıt bekleme
  
  // Gelen veriyi oku
  digitalWrite(SCK, HIGH);
  delayMicroseconds(200);
  received = digitalRead(MISO);
  digitalWrite(SCK, LOW);
  
  return received; // Gelen yanıtı döndür
}

void loop() {
  char key = keypad.getKey(); // Tuş takımından tuş okuma
  
  if (key) { // Eğer bir tuşa basılmışsa
    if (key == '#') { // Eğer '#' tuşuna basıldıysa
      if (inputNumber.length() > 0) { // Eğer girilen sayı boş değilse
        int number = inputNumber.toInt(); // Girilen sayıyı tam sayıya çevir
        uint8_t numberToSend = number % 256;  // Sayıyı 0-255 aralığına indir
        
        // Seri monitöre girilen ve gönderilen sayıyı yazdır
        Serial.print("Girilen sayı: ");
        Serial.println(number);
        Serial.print("Gönderilen değer: ");
        Serial.println(numberToSend);
        
        digitalWrite(SS, LOW); // Slave seçimini etkinleştir
        delayMicroseconds(500);
        
        // SPI ile sayıyı gönder ve yanıtı al
        uint8_t stm32_response = sendSPI(numberToSend);
        
        delayMicroseconds(500);
        digitalWrite(SS, HIGH); // Slave seçimini devre dışı bırak
        
        // Gelen yanıtı seri monitöre yazdır
        Serial.print("STM32'den gelen yanıt: ");
        Serial.println(stm32_response);
        Serial.println("------------------------");

        inputNumber = ""; // Girilen sayıyı sıfırla
      }
    } else if (key >= '0' && key <= '9') { // Eğer basılan tuş bir rakamsa
      inputNumber += key; // Rakamı mevcut girdiye ekle
      Serial.print("Tuşlanan sayı: ");
      Serial.println(inputNumber);
    }
  }
}
