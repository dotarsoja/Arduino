#include <EEPROM.h>
#define ADDRESS 0
typedef struct{
    uint16_t parking_val;
    uint16_t start_val;
    uint16_t stop_val;
    int16_t turns;
} setting_t;
void save_settings();
void load_settings();
void clear_settings();
setting_t settings;
