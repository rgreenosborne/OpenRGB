#include "RGBController_Faustus.h"

#include <dirent.h>
#include <string.h>

RGBController_Faustus::RGBController_Faustus(const std::string& dev_path)
{
    name = "Faustus";
    description = "ASUS TUF Keyboard Backlight";
    type = DEVICE_TYPE_KEYBOARD;

    modes.resize(4);
    modes[0].name   = "Static";
    modes[0].value  = FAUSTUS_MODE_STATIC;
    modes[0].flags  = MODE_FLAG_HAS_COLOR;
    modes[0].random = false;

    modes[1].name      = "Breathing";
    modes[1].value     = FAUSTUS_MODE_BREATHING;
    modes[1].flags     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_COLOR;
    modes[1].speed_min = FAUSTUS_SPEED_SLOWEST;
    modes[1].speed_max = FAUSTUS_SPEED_FASTEST;
    modes[1].random    = false;
    modes[1].speed     = FAUSTUS_SPEED_NORMAL;

    modes[2].name      = "Color Cycle";
    modes[2].value     = FAUSTUS_MODE_COLOR_CYCLE;
    modes[2].flags     = MODE_FLAG_HAS_SPEED;
    modes[2].speed_min = FAUSTUS_SPEED_SLOWEST;
    modes[2].speed_max = FAUSTUS_SPEED_FASTEST;
    modes[2].random    = false;
    modes[2].speed     = FAUSTUS_SPEED_NORMAL;

    modes[3].name      = "Strobe";
    modes[3].value     = FAUSTUS_MODE_STROBE;
    modes[3].flags     = MODE_FLAG_HAS_SPEED;
    modes[3].speed_min = FAUSTUS_SPEED_SLOWEST;
    modes[3].speed_max = FAUSTUS_SPEED_FASTEST;
    modes[3].random    = false;
    modes[3].speed     = FAUSTUS_SPEED_NORMAL;

    colors.resize(1);
    leds.resize(1);
    leds[0].name = "Keyboard backlight LED";
    zones.resize(1);
    zones[0].type = ZONE_TYPE_SINGLE;
    zones[0].name = "Keyboard backlight zone";
    zones[0].map.push_back(std::vector<int>(1, 0));

    // Prepare file streams
    r_path = dev_path;
    g_path = dev_path;
    b_path = dev_path;
    mode_path = dev_path;
    flags_path = dev_path;
    set_path = dev_path;

    r_path.append("/kbbl_red");
    g_path.append("/kbbl_green");
    b_path.append("/kbbl_blue");
    mode_path.append("/kbbl_mode");
    flags_path.append("/kbbl_flags");
    set_path.append("/kbbl_set");
}

void RGBController_Faustus::UpdateLEDs()
{
    int rv = uint8_t(RGBGetRValue(colors[0]));
    int gv = uint8_t(RGBGetGValue(colors[0]));
    int bv = uint8_t(RGBGetBValue(colors[0]));

    std::ofstream str_r;
    std::ofstream str_g;
    std::ofstream str_b;
    std::ofstream str_mode;
    std::ofstream str_flags;
    std::ofstream str_set;

    str_r.open(r_path, std::ios::out | std::ios::trunc);
    str_g.open(g_path, std::ios::out | std::ios::trunc);
    str_b.open(b_path, std::ios::out | std::ios::trunc);
    str_mode.open(mode_path, std::ios::out | std::ios::trunc);
    str_flags.open(flags_path, std::ios::out | std::ios::trunc);
    str_set.open(set_path, std::ios::out | std::ios::trunc);

    str_r << std::hex;
    str_g << std::hex;
    str_b << std::hex;
    str_mode << std::hex;
    str_flags << std::hex;
    str_set << std::hex;

    str_r << rv;
    str_g << gv;
    str_b << bv;
    str_mode << active_mode;
    str_flags << 0x2a; // All of em
    str_set << 2;

    // Flush everything
    str_r.close();
    str_g.close();
    str_b.close();
    str_mode.close();
    str_flags.close();
    str_set.close();
}

void RGBController_Faustus::UpdateZoneLEDs(int zone)
{
    UpdateLEDs();
}

void RGBController_Faustus::UpdateSingleLED(int led)
{
    UpdateLEDs();
}

void RGBController_Faustus::SetCustomMode()
{
    SetMode(0);
}

void RGBController_Faustus::UpdateMode()
{
    UpdateLEDs();
}

void DetectFaustusControllers(std::vector<RGBController*> &rgb_controllers)
{
    const char* base_path = "/sys/devices/platform/faustus/kbbl";
    DIR* dir = opendir(base_path);
    if(!dir) return;
    // Directory is present - we pretty much have a driver confirmation already, but double check for all files required just in case
    struct dirent* dent = readdir(dir);
    if(!dent) return;
    int found = 0;
    while(dent)
    {
        const char* fname = dent->d_name;
        if(!strcmp(fname, "kbbl_red") || !strcmp(fname, "kbbl_green") || !strcmp(fname, "kbbl_blue") || !strcmp(fname, "kbbl_mode") || !strcmp(fname, "kbbl_flags") || !strcmp(fname, "kbbl_set"))
        {
            ++found;
        }
        dent = readdir(dir);
    }
    closedir(dir);
    if(found != 6) return;
    rgb_controllers.push_back(new RGBController_Faustus(base_path));
}