#include <stdlib.h>
#include <string.h>
#include <Arduino.h>
#include "lcd_wrapper.h"

#define LED_1RED_PIN 2
#define LED_1BLUE_PIN 3
#define LED_2RED_PIN 4
#define LED_2BLUE_PIN 5
#define LED_3RED_PIN 6
#define LED_3BLUE_PIN 7
#define LED_4RED_PIN 8
#define LED_4BLUE_PIN 9
#define BTN_1_PIN 10
#define BTN_2_PIN 11
#define BTN_3_PIN 12
#define BTN_4_PIN 13
#define BTN_ENTER_PIN A0
#define SEED_PIN A1

#define TRIES 10
extern LiquidCrystal_I2C lcd;

char *generate_code(bool repeat, int length);
void get_score(char *secret, char *guess, int *peg_a, int *peg_b);
void turn_off_leds();
void render_leds(const int peg_a, const int peg_b);
void render_history(char *secret, char history[][5], const int entry);
void play_game(char *secret);
