#include <M5Atom.h>
#include <BleKeyboard.h>
#include "common.h"

const std::string DEVICE_NAME("ATOM Lite Page Clicker");
const std::string DEVICE_MANUFACTURER("M5Stack");

const CRGB CRGB_BLE_CONNECTED(0x00, 0x00, 0xf0);
// 数値は緑だが Lite では赤く光る https://github.com/m5stack/M5Atom/issues/5
const CRGB CRGB_BLE_DISCONNECTED(0x00, 0xf0, 0x00);

const bool ENABLE_SERIAL = true;
const bool ENABLE_I2C = false;
const bool ENABLE_DISPLAY = true;

BleKeyboard bleKeyboard(DEVICE_NAME, DEVICE_MANUFACTURER);
bool isBleConnected = false;
bool isLongPressed = false;
const uint32_t LONG_PRESSED_THRETHOLD_MS = 500;

void setup()
{
    bleKeyboard.begin();
    M5.begin(ENABLE_SERIAL, ENABLE_I2C, ENABLE_DISPLAY);
    M5.dis.drawpix(0, CRGB_BLE_DISCONNECTED);
}

void loop()
{
    M5.update();
    if (bleKeyboard.isConnected())
    {
        if (!isBleConnected)
        {
            M5.dis.drawpix(0, CRGB_BLE_CONNECTED);
            isBleConnected = true;
            SERIAL_PRINTLN("Connected");
        }
        if (M5.Btn.pressedFor(LONG_PRESSED_THRETHOLD_MS)) {
            isLongPressed = true;
        }
        if (M5.Btn.wasReleased())
        {
            if (isLongPressed) {
                bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
                SERIAL_PRINTLN("Previous Page");
                isLongPressed = false;
            }
            else
            {
                bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
                SERIAL_PRINTLN("Next Page");
            }
        }
    }
    else
    {
        if (isBleConnected)
        {
            M5.dis.drawpix(0, CRGB_BLE_DISCONNECTED);
            isBleConnected = false;
            SERIAL_PRINTLN("Disconnected");
        }
    }
    delay(100);
}