/* MIT License - Copyright (c) 2019-2022 Francis Van Roie
   For full license information read the LICENSE file in the project folder */

#include "m5stackcore2.h"

#if defined(M5STACK)

#include "AXP.h" // Power Mgmt
#include "dev/esp32/esp32.h"

AXP Axp;

namespace dev {

void M5StackCore2::init(void)
{
    Wire.begin(TOUCH_SDA, TOUCH_SCL, (uint32_t)I2C_TOUCH_FREQUENCY);
    Axp.begin();

    //Determine Core2 version
    uint8_t val = Read8bit(0x03);
    if (val == 0x03) {                  //AXP192 == Core2 V1
        _core2_version = core2_v1;
    } else if (val == 0x4A) {           //AXP2101 == Core2 V1.1
        _core2_version = core2_v1_1;
    } else {                            //Unknown
        _core2_version = core2_unknown;
    }

    Axp.SetCHGCurrent(AXP192::kCHG_100mA);
    Axp.SetLcdVoltage(2800);
    _backlight_power = true;

    Axp.SetBusPowerMode(0);
    Axp.SetCHGCurrent(AXP192::kCHG_190mA);

    Axp.SetVibration(true);
    delay(150);
    Axp.SetVibration(false);

    Axp.SetLed(1);
    delay(100);
    Axp.SetLed(0);

    Axp.SetLDOVoltage(3, 3300);
    Axp.SetLed(1);
}

void M5StackCore2::set_backlight_level(uint8_t level)
{
    _backlight_level = level;
    update_backlight();
}

uint8_t M5StackCore2::get_backlight_level()
{
    return _backlight_level;
}

void M5StackCore2::set_backlight_power(bool power)
{
    _backlight_power = power;
    update_backlight();
}

bool M5StackCore2::get_backlight_power()
{
    return _backlight_power;
}

void M5StackCore2::update_backlight()
{
    if(_backlight_power) {
        uint16_t voltage = map(_backlight_level, 0, 255, 2500, 3300);
        Axp.SetLcdVoltage(voltage);
    } else {
        if(_core2_version == core2_v1) {
            Axp.SetDCVoltage(2, 2200);
        } else if (_core2_version == core2_v1_1) {
            Axp.SetLcdVoltage(0);
        }
    }
}

void M5StackCore2::get_sensors(JsonDocument& doc)
{
    Esp32Device::get_sensors(doc);

    JsonObject sensor        = doc.createNestedObject(F("AXP"));
    sensor[F("BattVoltage")] = Axp.GetBatVoltage();
    sensor[F("BattPower")]   = Axp.GetBatPower();
    sensor[F("Batt%")]             = Axp.GetBatteryLevel();
    sensor[F("BattChargeCurrent")] = Axp.GetBatChargeCurrent();
    sensor[F("Temperature")]       = Axp.GetTempInAXP192();
    sensor[F("Charging")]          = Axp.isCharging();
}

} // namespace dev

dev::M5StackCore2 haspDevice;

#endif
