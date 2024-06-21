/* MIT License - Copyright (c) 2019-2022 Francis Van Roie
   For full license information read the LICENSE file in the project folder */

#ifndef HASP_DEVICE_M5STACKCORE2_H
#define HASP_DEVICE_M5STACKCORE2_H

#if defined(M5STACK)

#include "esp32.h"

namespace dev {

class M5StackCore2 : public Esp32Device {
  public:
    enum core2_version { core2_unknown = 0, core2_v1, core2_v1_1 };
    void init() override;
    void get_sensors(JsonDocument& doc);

    void set_backlight_level(uint8_t level);
    uint8_t get_backlight_level();
    void set_backlight_power(bool power);
    bool get_backlight_power();
    void update_backlight();

  private:
    core2_version _core2_version;
    uint8_t _backlight_level;
    uint8_t _backlight_power;
};

} // namespace dev

using dev::M5StackCore2;
extern dev::M5StackCore2 haspDevice;

#endif
#endif