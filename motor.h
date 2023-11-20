#include "Arduino.h"
#define PIN_A 10
#define PIN_B 11

class Motor {
  public:
    enum Direction {LEFT,RIGHT};
    enum Status {RUN,STOP};
    Motor(uint8_t pinA = PIN_A, uint8_t pinB = PIN_B);
    void run();
    void stop();
    bool check_target_destination();
    void Motor::set_turns(int16_t turns);
    int16_t Motor::get_turns();
    uint16_t get_current_angle();
    void set_parking_angle(uint16_t * parking_angle_p);
    void set_target_angle(uint16_t * target_angle_p);
    void update_current_position();
    Status get_motor_status();
    Direction direction;
  private:
    uint16_t _current_angle_deg;
    uint16_t *_parking_angle_p;
    uint16_t *_target_angle_p;
    uint8_t _pinA;
    uint8_t _pinB;
    int16_t _turns_cnt;
    Status _status;
};