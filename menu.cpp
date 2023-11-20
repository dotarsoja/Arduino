#include "Arduino.h"
#include "menu.h"

Menu::Menu (LiquidCrystal_I2C* lcd, char* init_text, uint8_t init_str_len, uint8_t menu_max_len) {
  _lcd = lcd;
  _cursor_pos = 0;
  _menu_max_len = menu_max_len;
  memset(_lcd_text, 0, _menu_max_len);
  if (init_text != nullptr)
    memcpy(_lcd_text, init_text, init_str_len);
}
void Menu::insert_text(char* str, uint8_t len, uint8_t start_pos) {
  if (len > _menu_max_len || start_pos > _menu_max_len || (start_pos + len) > _menu_max_len)
    return;
  uint8_t tmp_text_menu[_menu_max_len];
  memset(tmp_text_menu, 0, _menu_max_len);
  memcpy(tmp_text_menu, _lcd_text, _menu_max_len);
  memcpy(tmp_text_menu + start_pos, str, len);
  if (memcmp(tmp_text_menu, _lcd_text, _menu_max_len) == 0)
    return;
  memcpy(_lcd_text, tmp_text_menu, _menu_max_len);
  _cursor_pos = start_pos + len;
  _lcd->setCursor(start_pos % 16, start_pos / 16);
  if (_cursor_pos < 16)
    memset(_lcd_text + _cursor_pos, ' ', 16 - _cursor_pos);
  _lcd->print((char *)_lcd_text + start_pos);
  if (_cursor_pos > 15) {
    memset(_lcd_text + _cursor_pos, ' ', 32 - _cursor_pos);
    _lcd->setCursor(0, 1);
    _lcd->print((char *)_lcd_text + 16);
  }
}
void Menu::del_text() {
  memset(_lcd_text, 0, _menu_max_len);
  _cursor_pos = 0;
  _lcd->setCursor(0, 0);
  _lcd->clear();
}
void Menu::redarw_text() {
  _lcd->clear();
  _lcd->setCursor(0, 0);
  _lcd->print((char *)this->_lcd_text);
  _lcd->setCursor(0, 1);
  _lcd->print((char *)this->_lcd_text + 16);
}

void null_func() {
  return;
}
