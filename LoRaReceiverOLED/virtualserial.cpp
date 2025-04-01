#include "umbrella.hpp"

void notif() {
    digitalWrite(VIRTUAL_NOTIFY_LINE, 1);
}

void stopnotif() {
    digitalWrite(VIRTUAL_NOTIFY_LINE, 0);
}

void write(bool bit)
{
    digitalWrite(VIRTUAL_DATA_LINE, bit);
}

void clock(bool state)
{
    digitalWrite(VIRTUAL_CLOCK_LINE, state);
}

void waitForEnab() {
    while (digitalRead(VIRTUAL_ENABLE_LINE) == LOW) ;
}
void waitForDisab() {
    while (digitalRead(VIRTUAL_ENABLE_LINE) == HIGH) ;
}

void char_to_bool_array(char c, bool *bits)
{
    for (int i = 0; i < 8; i++)
    {
        bits[i] = (c >> i) & 1;
    }
}

void sendChar(char c)
{
    bool bits[8];
    char_to_bool_array(c, bits);
    for (int j = 0; j < 8; j++)
    {
        write(false);
        clock(false);
        waitForEnab();
        write(bits[j]);
        clock(true);
        waitForDisab();
    }
    clock(false);
}

void sendVUARTString(std::string data)
{
    data += '\n';
    notif();
    for (int i = 0; i < data.length(); i++)
    {
        sendChar(data[i]);
    }
    stopnotif();
}