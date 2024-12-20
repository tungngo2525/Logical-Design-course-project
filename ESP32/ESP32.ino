#define BLYNK_TEMPLATE_ID "TMPL6I6z1LfuM"
#define BLYNK_TEMPLATE_NAME "Iot Weather"
#define BLYNK_AUTH_TOKEN "iLQrWJ1kziaggMQVP7QpIaJAjIarLUsu"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "tungngo";
char pass[] = "gacon29T"; 

float temp = 0.0;
float humidity = 0.0;
float rain = 0.0;


enum SystemState { MEASURING, OBSERVING };
SystemState currentState = MEASURING;


unsigned long lastMeasureTime = 0;
const unsigned long MEASURE_INTERVAL = 3000;
unsigned long observeStartTime = 0;
const unsigned long OBSERVE_DURATION = 10000; 

void setup() {
  
    Serial.begin(9600);

    Serial2.begin(9600, SERIAL_8N1, 16, 17); // RX ở GPIO16, TX ở GPIO17

    // Kết nối Wi-Fi
    WiFi.begin(ssid, pass);
    Serial.print("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("WiFi connected");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // Kết nối với Blynk
    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

// Xử lý dữ liệu từ Terminal (V7)
BLYNK_WRITE(V7) {
    String command = param.asString(); 
    command.trim(); 

    Serial.print("Command from Terminal: ");
    Serial.println(command);

    if (command == "!C#") {
        // Gửi lệnh Capture tới STM32
        Serial2.print(command);
        Serial.println("Command sent to STM32: Capture (!C#)");

   
        currentState = OBSERVING;
        observeStartTime = millis(); // Lưu thời gian bắt đầu quan sát
    } else if (command == "!R#") {
   
        Serial2.print(command);
        Serial.println("Command sent to STM32: Reset (!R#)");

  
        currentState = MEASURING;
        observeStartTime = 0; 
    } else {
        Serial.println("Invalid command! Use !C# for Capture or !R# for Reset.");
    }
}

void loop() {
    // Nhận dữ liệu từ STM32
    if (Serial2.available() >= 12) {
        uint8_t temp_bytes[4];
        uint8_t humidity_bytes[4];
        uint8_t rain_bytes[4];

        for (int i = 0; i < 4; i++) temp_bytes[i] = Serial2.read();

        for (int i = 0; i < 4; i++) humidity_bytes[i] = Serial2.read();

        for (int i = 0; i < 4; i++) rain_bytes[i] = Serial2.read();

        memcpy(&temp, temp_bytes, sizeof(temp));
        memcpy(&humidity, humidity_bytes, sizeof(humidity));
        memcpy(&rain, rain_bytes, sizeof(rain));

        
        Blynk.virtualWrite(V0, temp);
        Blynk.virtualWrite(V1, humidity);
        Blynk.virtualWrite(V5, rain);

       
        Serial.print("Temperature: ");
        Serial.print(temp);
        Serial.print(" C, Humidity: ");
        Serial.print(humidity);
        Serial.print(" %, Rain: ");
        Serial.print(rain);
        Serial.println(" mm");
    }

        Blynk.virtualWrite(V0, temp);      
        Blynk.virtualWrite(V1, humidity);  
        Blynk.virtualWrite(V5, rain);     

       
        if (temp >= 30.0) {
            Blynk.virtualWrite(V2, 1); 
            Blynk.virtualWrite(V3, 0);  
            Blynk.virtualWrite(V4, 0);   
        } else if (temp >= 20.0 && temp < 30.0) {
            Blynk.virtualWrite(V2, 0);    
            Blynk.virtualWrite(V3, 0);    
            Blynk.virtualWrite(V4, 1);  
        } else {
            Blynk.virtualWrite(V2, 0);    
            Blynk.virtualWrite(V4, 0);   
            Blynk.virtualWrite(V3, 1);  
        }

    if (currentState == MEASURING) {
    
        if (millis() - lastMeasureTime >= MEASURE_INTERVAL) {
            lastMeasureTime = millis();
            Serial.println("Measuring...");
        }
    } else if (currentState == OBSERVING) {
       
        if (millis() - observeStartTime >= OBSERVE_DURATION) {
            currentState = MEASURING;
            Serial.println("System returned to MEASURING state automatically.");
        }
    }


    Blynk.run();
    delay(300);
}
