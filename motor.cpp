#include "motor.h"
#include "rotator_variables.h"
Motor::Motor(uint8_t pinA, uint8_t pinB) {
  _pinA = pinA;
  _pinB = pinB;
  pinMode(pinA, OUTPUT);
  pinMode(pinB, OUTPUT);
  direction = RIGHT;
  _status = STOP;
  _turns_cnt = 0;
}
void Motor::run() {
  if(direction == RIGHT) {
    digitalWrite(_pinA, HIGH);
    digitalWrite(_pinB, LOW);
  }
  else {
     digitalWrite(_pinA, LOW);
    digitalWrite(_pinB, HIGH);   
  }
  _status = RUN;
}

void Motor::stop() {
  digitalWrite(_pinA, LOW);
  digitalWrite(_pinB, LOW);
  _status = STOP;
}
Motor::Status Motor::get_motor_status() {
  return _status;
}
bool Motor::check_target_destination() {
  if (this->get_current_angle() == *_target_angle_p) {
    return true;
  }
  return false;
}
void Motor::set_turns(int16_t turns){
  _turns_cnt = turns;
}
int16_t Motor::get_turns(){
  return _turns_cnt;
}
uint16_t Motor::get_current_angle(){
  int16_t angle  = (int)fabs(_turns_cnt * KOEF) % 360;
  if(angle < 0)
    angle = 359 - angle;
  return angle;
}
void Motor::set_parking_angle(uint16_t * parking_angle_p){
  _parking_angle_p = parking_angle_p;
}
void Motor::set_target_angle(uint16_t * target_angle_p){
  _target_angle_p = target_angle_p;
}
void Motor::update_current_position() {
  if(direction == RIGHT) {
    _turns_cnt++;
  }
  else {
    _turns_cnt--;
  }
}