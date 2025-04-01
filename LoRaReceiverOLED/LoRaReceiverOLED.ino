// Vizualizacio jelerossegre
//   
//  0dB   |   
//        |
//        |
//        |
//        |     |
// -50dB  |     | 
//        |     |                      
//        |     |           
//        |     |
//        |     |
// -104dB |     |
//        | ____|____
#include "umbrella.hpp"

char txpacket[BUFFER_SIZE];
char rxpacket[BUFFER_SIZE];

static RadioEvents_t RadioEvents;
static SSD1306Wire  display(0x3c, 500000, SDA_OLED, SCL_OLED, GEOMETRY_128_64, RST_OLED); // addr , freq , i2c group , resolution , rst

int16_t txNumber;

int16_t rssi,rxSize;

bool lora_idle = true;

void VextON(void)
{
  pinMode(Vext,OUTPUT);
  digitalWrite(Vext, LOW);
}

void setup() {
    Serial.begin(115200);
    Mcu.begin(HELTEC_BOARD,SLOW_CLK_TPYE);
    VextON();
    display.init();
    display.setFont(ArialMT_Plain_10);
    txNumber=0;
    rssi=0;
  
    RadioEvents.RxDone = OnRxDone;
    Radio.Init( &RadioEvents );
    Radio.SetChannel( RF_FREQUENCY );
    Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                               LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                               LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                               0, true, 0, 0, LORA_IQ_INVERSION_ON, true );
    pinMode(VIRTUAL_NOTIFY_LINE, OUTPUT);
    pinMode(VIRTUAL_DATA_LINE, OUTPUT);
    pinMode(VIRTUAL_CLOCK_LINE, OUTPUT);
    pinMode(VIRTUAL_ENABLE_LINE, INPUT);
}

void loop()
{
  if(lora_idle)
  {
    lora_idle = false;
    Radio.Rx(0); 
  }
  Radio.IrqProcess( );
}

void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
  display.clear();
    rssi=rssi;
    rxSize=size;
    memcpy(rxpacket, payload, size );
    rxpacket[size]='\0';
    Radio.Sleep( );

    unsigned long currentMillis = millis();
    int hours = (currentMillis / 3600000) % 24;
    int minutes = (currentMillis / 60000) % 60;
    int seconds = (currentMillis / 1000) % 60;
    
    char timestamp[20];
    sprintf(timestamp, "%02d:%02d:%02d", hours, minutes, seconds);
    std::string line = std::format("[{}] packet: \"{}\", rssi: {}, length: {}", timestamp, rxpacket, rssi, rxSize);

    sendVUARTString(line);
    display.drawString(0, 0, String(timestamp));
    display.drawString(0,16, rxpacket);
    display.drawString(0,32, String("RSSI: ") + String(rssi));

    lora_idle = true;
    display.display();
}