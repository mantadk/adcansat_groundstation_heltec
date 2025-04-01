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

#include "LoRaWan_APP.h"
#include "Arduino.h"
#include <Wire.h>  
#include "HT_SSD1306Wire.h"


#define RF_FREQUENCY                                868000000 // Hz

#define TX_OUTPUT_POWER                             10        // dBm

#define LORA_BANDWIDTH                              0         // [0: 125 kHz,
                                                              //  1: 250 kHz,
                                                              //  2: 500 kHz,
                                                              //  3: Reserved]
#define LORA_SPREADING_FACTOR                       7         // [SF7..SF12]
#define LORA_CODINGRATE                             1         // [1: 4/5,
                                                              //  2: 4/6,
                                                              //  3: 4/7,
                                                              //  4: 4/8]
#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT                         0         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON                  false
#define LORA_IQ_INVERSION_ON                        false


#define RX_TIMEOUT_VALUE                            1000
#define BUFFER_SIZE                                 1024 // Define the payload size here

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



Serial.printf("\r\n[%s] packet: \"%s\", rssi: %d, length: %d\r\n", 
                   timestamp, rxpacket, rssi, rxSize);
    Serial.flush();
    display.drawString(0, 0, String(timestamp));
    display.drawString(0,16, rxpacket);
    display.drawString(0,32, String("RSSI: ") + String(rssi));

    lora_idle = true;
    display.display();
}