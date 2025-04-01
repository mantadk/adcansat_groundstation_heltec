
#include "LoRaWan_APP.h"
#include "Arduino.h"
#include <Wire.h>  
#include "HT_SSD1306Wire.h"
#include <format>
#include <string>

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


#define VIRTUAL_DATA_LINE 37
#define VIRTUAL_CLOCK_LINE 46
#define VIRTUAL_ENABLE_LINE 45
#define VIRTUAL_NOTIFY_LINE 42

void sendVUARTString(std::string data);