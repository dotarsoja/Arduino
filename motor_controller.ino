#include <LiquidCrystal_I2C.h>
#include <EncButton.h>
#include "menu.h"
#include "setting.h"
#include "motor.h"

#define ENC_BUTTON 3
#define ENC_CLK 9
#define ENC_DT 8

#define GERKON 2
#define PARK_BUTTON 5

LiquidCrystal_I2C lcd(0x27, 16, 2);
EncButton<EB_TICK, ENC_CLK, ENC_DT, ENC_BUTTON> encoder;
EncButton<EB_TICK, PARK_BUTTON> prk_btn;
Menu* current_menu;
Menu intro_menu(&lcd, INIT_TEXT(TEXT_INTRO));
Menu setup_menu(&lcd, INIT_TEXT(TEXT_START));
Menu parking_menu(&lcd, INIT_TEXT(TEXT_SET_PARK));
Menu start_menu(&lcd, INIT_TEXT(TEXT_SET_START));
Menu stop_menu(&lcd, INIT_TEXT(TEXT_SET_STOP));
Menu drive_menu(&lcd, INIT_TEXT(TEXT_SET_DRIVE));

Motor motor;

bool is_saved_settigs = false;
volatile bool is_turn = false;

void save_settings() {
  EEPROM.put(ADDRESS, settings);
}

void load_settings() {
  EEPROM.get(ADDRESS, settings);
}

void clear_settings() {
  memset(&settings, 0xff, sizeof(settings));
  save_settings();
}

void update_value(int8_t value) {
  uint8_t position_text = 0;
  char text[32];
  current_menu->value += value;
  if (current_menu->value > 359)
    current_menu->value = 359;  //current_menu->value % 360;
  if (current_menu->value < 0)
    current_menu->value = 0;  //360 + current_menu->value;
  if (current_menu == &drive_menu) {
    position_text = 0;
    if (motor.get_motor_status() == Motor::RUN) {
      position_text = 16;
      sprintf(text, "   Azimut: %d\1", motor.get_current_angle() + 1);
    } else {
      if (current_menu->value > stop_menu.value) {
        current_menu->value = stop_menu.value;
      } else if (current_menu->value < start_menu.value) {
        current_menu->value = start_menu.value;
      }
      if (current_menu->value > motor.get_current_angle()) {
        sprintf(text, "<- Right: %d\1", (current_menu->value - motor.get_current_angle()));
      } else {
        sprintf(text, "<- Left: %d\1", (motor.get_current_angle() - current_menu->value));
      }
    }
  } else if (current_menu == &parking_menu) {
    position_text = 16;
    sprintf(text, DRIVE_PATTERN, current_menu->value + 1);
  } else if (current_menu == &start_menu) {
    position_text = 16;
    sprintf(text, DRIVE_PATTERN, current_menu->value + 1);
  } else if (current_menu == &stop_menu) {
    position_text = 16;
    sprintf(text, DRIVE_PATTERN, current_menu->value + 1);
  } else if (current_menu == &intro_menu) {
    return;
  }

  current_menu->insert_text(text, strlen(text), position_text);
}

void drive_menu_left_click_cb() {
  if (motor.get_motor_status() != Motor::RUN) {
    update_value(-1);
  }
}
void drive_menu_right_click_cb() {
  if (motor.get_motor_status() != Motor::RUN) {
    update_value(1);
  }
}
void drive_menu_short_click_cb() {
  if (motor.get_motor_status() != Motor::RUN) {

    if (motor.get_current_angle() < drive_menu.value) {
      motor.direction = Motor::RIGHT;
    } else {
      motor.direction = Motor::LEFT;
    }
    if (motor.get_current_angle() != drive_menu.value) {
      motor.run();
      update_value(0);
      save_settings();
    }

  } else {
    motor.stop();
    save_settings();
  }
}
void drive_menu_long_click_cb() {
  drive_menu_short_click_cb();
}
void start_menu_right_click_cb() {
  if (current_menu->is_edit_mode) {
    update_value(1);
  } else {
    update_menu(&stop_menu);
  }
}
void start_menu_left_click_cb() {
  if (current_menu->is_edit_mode) {
    update_value(-1);
  } else {
    update_menu(&parking_menu);
  }
}
void start_menu_long_click_cb() {
  update_menu(&drive_menu);
}
void start_menu_short_click_cb() {
  if (current_menu->is_edit_mode) {
    settings.start_val = start_menu.value;
    save_settings();
    current_menu->is_edit_mode = false;
  } else {
    current_menu->is_edit_mode = true;
  }
}
void stop_menu_right_click_cb() {
  if (current_menu->is_edit_mode) {
    update_value(1);
  } else {
    update_menu(&parking_menu);
  }
}
void stop_menu_left_click_cb() {
  if (current_menu->is_edit_mode) {
    update_value(-1);
  } else {
    update_menu(&start_menu);
  }
}
void stop_menu_long_click_cb() {
  update_menu(&drive_menu);
}
void stop_menu_short_click_cb() {
  if (current_menu->is_edit_mode) {
    settings.stop_val = stop_menu.value;
    save_settings();
    current_menu->is_edit_mode = false;
  } else {
    current_menu->is_edit_mode = true;
  }
}
void parking_menu_right_click_cb() {
  if (current_menu->is_edit_mode) {
    update_value(1);
  } else {
    update_menu(&start_menu);
  }
}
void parking_menu_lef_click_cb() {
  if (current_menu->is_edit_mode) {
    update_value(-1);
  } else {
    update_menu(&stop_menu);
  }
}
void parking_menu_long_click_cb() {
  update_menu(&drive_menu);
}
void parking_menu_short_click_cb() {
  if (current_menu->is_edit_mode) {
    settings.parking_val = parking_menu.value;
    save_settings();
    current_menu->is_edit_mode = false;
  } else {
    current_menu->is_edit_mode = true;
  }
}

void init_menu() {
  drive_menu.left_click = drive_menu_left_click_cb;
  drive_menu.right_click = drive_menu_right_click_cb;
  drive_menu.long_click = drive_menu_long_click_cb;
  drive_menu.short_click = drive_menu_short_click_cb;
  start_menu.right_click = start_menu_right_click_cb;
  start_menu.left_click = start_menu_left_click_cb;
  start_menu.long_click = start_menu_long_click_cb;
  start_menu.short_click = start_menu_short_click_cb;
  stop_menu.right_click = stop_menu_right_click_cb;
  stop_menu.left_click = stop_menu_left_click_cb;
  stop_menu.long_click = stop_menu_long_click_cb;
  stop_menu.short_click = stop_menu_short_click_cb;
  parking_menu.right_click = parking_menu_right_click_cb;
  parking_menu.left_click = parking_menu_lef_click_cb;
  parking_menu.long_click = parking_menu_long_click_cb;
  parking_menu.short_click = parking_menu_short_click_cb;

  load_settings();
  settings.parking_val = 10;
  settings.start_val = 0;
  settings.stop_val = 359;
  settings.turns = 50;
  if (settings.parking_val < 360) {
    is_saved_settigs = true;
    parking_menu.value = settings.parking_val;
    start_menu.value = settings.start_val;
    stop_menu.value = settings.stop_val;
    motor.set_turns(settings.turns);
    drive_menu.value = motor.get_current_angle();
  }
}

void add_degrees_symbol(int position) {
  lcd.setCursor(position % 16, position / 16);
  lcd.write(1);
}

void isr_enc() {
  //Serial.println("tick");
}

void isr_gerkon() {
  is_turn = true;
}

void update_menu(Menu* menu) {
  if (menu == nullptr)
    return;

  current_menu = menu;
  current_menu->redarw_text();
  update_value(0);
}

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  lcd.createChar(1, cel);
  attachInterrupt(0, isr_gerkon, FALLING);
  attachInterrupt(1, isr_enc, CHANGE);
  init_menu();
  motor.set_target_angle(&drive_menu.value);
  motor.set_parking_angle(&parking_menu.value);

  update_menu(&intro_menu);
  delay(3000);
  if (is_saved_settigs) {
    update_menu(&drive_menu);
  } else {
    update_menu(&setup_menu);
  }
}

void loop() {
  encoder.tick();
  prk_btn.tick();
  if (is_turn) {
    is_turn = false;
    motor.update_current_position();
    if (motor.check_target_destination()) {
      update_value(0);
      motor.stop();
    }
    update_value(0);
  }
  if (encoder.isRight()) {
    current_menu->right_click();
  }
  if (encoder.isLeft()) {
    current_menu->left_click();
  }
  if (encoder.isHolded()) {
    current_menu->long_click();
  } else if (encoder.isRelease()) {
    current_menu->short_click();
  }
  if (prk_btn.isHolded()) {
    if (motor.get_motor_status() != Motor::RUN) {
      update_menu(&parking_menu);
    }
  } else if (prk_btn.isRelease()) {
      drive_menu.value = parking_menu.value;
      int diff = drive_menu.value - motor.get_current_angle();
      if(abs(diff) > 180)
      {
        diff = -diff;
      }
      motor.direction = (diff) < 0 ?  Motor::LEFT : Motor::RIGHT;
      motor.run();
      update_value(0);

  }
}
