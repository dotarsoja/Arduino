#include <LiquidCrystal_I2C.h> 
#define TEXT_INTRO " Simple project   for Arduino"
#define TEXT_START " Click to enter    Setup menu"
#define TEXT_SET_PARK " Parking angle"
#define TEXT_SET_START "  Start angle"
#define TEXT_SET_STOP "  Stop angle"
#define TEXT_SET_DRIVE "<- Left: 0\1"
#define DRIVE_PATTERN "      %d\1"
const uint8_t cel[8] ={B00111,B00101,B00111,B00000,B00000,B00000,B00000};
#define INIT_TEXT(a) a, strlen(a)
void null_func();
class Menu{
  public:
    void insert_text(char* str, uint8_t len, uint8_t start_pos = 0);
    void del_text();
    void redarw_text();
    Menu(LiquidCrystal_I2C* lcd, char* init_text = nullptr, uint8_t init_str_len = 0, uint8_t menu_max_len = 32);
    void (*left_click)() = null_func;
    void (*right_click)() = null_func;
    void (*short_click)() = null_func;
    void (*long_click)() = null_func;
    bool is_edit_mode;
    int  value;

  private:
    LiquidCrystal_I2C* _lcd;
    uint8_t _cursor_pos;
    uint8_t _lcd_text[32];
    uint8_t _menu_max_len;
};
