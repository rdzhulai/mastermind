#include "mastermind.h"

void setup()
{
    // Serial.begin(9600);
    randomSeed(analogRead(SEED_PIN));
    pinMode(LED_1RED_PIN, OUTPUT);
    pinMode(LED_2RED_PIN, OUTPUT);
    pinMode(LED_3RED_PIN, OUTPUT);
    pinMode(LED_4RED_PIN, OUTPUT);
    pinMode(LED_1BLUE_PIN, OUTPUT);
    pinMode(LED_2BLUE_PIN, OUTPUT);
    pinMode(LED_3BLUE_PIN, OUTPUT);
    pinMode(LED_4BLUE_PIN, OUTPUT);
    pinMode(BTN_1_PIN, INPUT);
    pinMode(BTN_2_PIN, INPUT);
    pinMode(BTN_3_PIN, INPUT);
    pinMode(BTN_4_PIN, INPUT);
    pinMode(BTN_ENTER_PIN, INPUT);

    lcd_init();
}

void loop()
{
    char *code = generate_code(false, 4);
    // Serial.print("Secret: " + String(code) + "\n");
    play_game(code);
    free(code);
}
