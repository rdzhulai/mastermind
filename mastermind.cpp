#include "mastermind.h"

enum btn_state
{
    ON,
    OFF
};

char *generate_code(bool repeat, int length)
{
    if (length < 1 || (repeat == false && length > 9))
        return NULL;

    char *code = (char *)malloc(sizeof(char) * length + 1);
    if (code == NULL)
        return NULL;
    if (repeat)
    {
        for (int i = 0; i < length; ++i)
        {
            code[i] = random(0, 10) + '0';
        }
    }
    else
    {
        code[0] = random(0, 10) + '0';
        for (int i = 1; i < length; ++i)
        {
            code[i] = random(0, 10) + '0';
            for (int j = 0; j < i; ++j)
            {
                if (code[j] == code[i])
                {
                    --i;
                    break;
                }
            }
        }
    }
    code[length] = '\0';
    return code;
}

void get_score(char *secret, char *guess, int *peg_a, int *peg_b)
{
    if (secret == NULL || guess == NULL)
        return;
    size_t len = strlen(secret);
    if (len == 0 || len != strlen(guess))
        return;
    *peg_a = 0, *peg_b = 0;

    if (secret[0] == guess[0])
        ++*peg_a;
    else
        for (int i = 1; i < len; ++i)
        {
            if (secret[0] == guess[i])
            {
                ++*peg_b;
                break;
            }
        }

    for (int i = 1; i < len; ++i)
    {
        if (secret[i] == guess[i])
            ++*peg_a;
        else
            for (int j = 0; j < len; ++j)
            {
                if (secret[i] == guess[j])
                {
                    ++*peg_b;
                    break;
                }
            }
    }
}

void turn_off_leds()
{
    digitalWrite(LED_1RED_PIN, LOW);
    digitalWrite(LED_2RED_PIN, LOW);
    digitalWrite(LED_3RED_PIN, LOW);
    digitalWrite(LED_4RED_PIN, LOW);
    digitalWrite(LED_1BLUE_PIN, LOW);
    digitalWrite(LED_2BLUE_PIN, LOW);
    digitalWrite(LED_3BLUE_PIN, LOW);
    digitalWrite(LED_4BLUE_PIN, LOW);
}

void render_leds(const int peg_a, const int peg_b)
{
    if (peg_a > 0)
        digitalWrite(LED_1RED_PIN, HIGH);
    if (peg_a > 1)
        digitalWrite(LED_2RED_PIN, HIGH);
    if (peg_a > 2)
        digitalWrite(LED_3RED_PIN, HIGH);
    if (peg_a > 3)
        digitalWrite(LED_4RED_PIN, HIGH);
    if (peg_b > 0)
        digitalWrite(LED_1BLUE_PIN, HIGH);
    if (peg_b > 1)
        digitalWrite(LED_2BLUE_PIN, HIGH);
    if (peg_b > 2)
        digitalWrite(LED_3BLUE_PIN, HIGH);
    if (peg_b > 3)
        digitalWrite(LED_4BLUE_PIN, HIGH);
}

void input_guess(char history[][5], const int entry)
{
    btn_state btn_states[5], last_btn_states[5];
    bool scroll_history_mode = false;
    int current_pos = entry;

    btn_states[0] = digitalRead(BTN_1_PIN) == HIGH ? ON : OFF;
    btn_states[1] = digitalRead(BTN_2_PIN) == HIGH ? ON : OFF;
    btn_states[2] = digitalRead(BTN_3_PIN) == HIGH ? ON : OFF;
    btn_states[3] = digitalRead(BTN_4_PIN) == HIGH ? ON : OFF;
    btn_states[4] = digitalRead(BTN_ENTER_PIN) == HIGH ? ON : OFF;

    memcpy(history[entry], "0000", 5);
    lcd.setCursor(0, 1);
    lcd.print("GUESS: " + String(history[entry]));

    for (int i = 0;;)
    {

        last_btn_states[0] = btn_states[0];
        last_btn_states[1] = btn_states[1];
        last_btn_states[2] = btn_states[2];
        last_btn_states[3] = btn_states[3];
        last_btn_states[4] = btn_states[4];

        btn_states[0] = digitalRead(BTN_1_PIN) == HIGH ? ON : OFF;
        btn_states[1] = digitalRead(BTN_2_PIN) == HIGH ? ON : OFF;
        btn_states[2] = digitalRead(BTN_3_PIN) == HIGH ? ON : OFF;
        btn_states[3] = digitalRead(BTN_4_PIN) == HIGH ? ON : OFF;
        btn_states[4] = digitalRead(BTN_ENTER_PIN) == HIGH ? ON : OFF;

        if (btn_states[4] != last_btn_states[4] && btn_states[4] == ON)
        {
            if (++i == 4)
                break;
        }

        switch (i)
        {
        case 0:
            if (btn_states[0] != last_btn_states[0] && btn_states[0] == ON)
            {
                scroll_history_mode = true;
                current_pos = entry;
                history[entry][0] = (history[entry][0] - '0' + 1) % 10 + '0';
            }
            else if (scroll_history_mode)
            {
                if (btn_states[1] != last_btn_states[1] && btn_states[1] == ON && current_pos - 1 >= 0)
                {
                    strncpy(history[entry], history[--current_pos], 5);
                }
                else if (btn_states[2] != last_btn_states[2] && btn_states[2] == ON && current_pos + 1 < entry)
                {
                    strncpy(history[entry], history[++current_pos], 5);
                }
            }
            break;
        case 1:
            if (btn_states[1] != last_btn_states[1] && btn_states[1] == ON)
            {
                history[entry][1] = (history[entry][1] - '0' + 1) % 10 + '0';
            }
            break;
        case 2:
            if (btn_states[2] != last_btn_states[2] && btn_states[2] == ON)
            {
                history[entry][2] = (history[entry][2] - '0' + 1) % 10 + '0';
            }
            break;
        case 3:
            if (btn_states[3] != last_btn_states[3] && btn_states[3] == ON)
            {
                history[entry][3] = (history[entry][3] - '0' + 1) % 10 + '0';
            }
            break;
        }

        lcd.setCursor(7, 1);
        lcd.print(history[entry]);
    }
    lcd.clear();
}

void render_history(char *secret, char history[][5], const int entry)
{
    int peg_a, peg_b;
    get_score(secret, history[entry], &peg_a, &peg_b);
    lcd.print(String(entry + 1) + ": " + String(history[entry]) + " " +
              String(peg_a) + "A" + String(peg_b) + "B");

    turn_off_leds();
    render_leds(peg_a, peg_b);

    if (peg_a == 4)
    {
        lcd.setCursor(0, 1);
        lcd.print("You won!");
        btn_state enter_state = digitalRead(BTN_ENTER_PIN) == HIGH ? ON : OFF;
        btn_state last_enter_state;
        do
        {
            last_enter_state = enter_state;
            enter_state = digitalRead(BTN_ENTER_PIN) == HIGH ? ON : OFF;
        } while (!(enter_state != last_enter_state && enter_state == ON));
    }
    else if (entry + 1 == TRIES)
    {
        lcd.setCursor(0, 1);
        lcd.print("You lost! " + String(secret));
        btn_state enter_state = digitalRead(BTN_ENTER_PIN) == HIGH ? ON : OFF;
        btn_state last_enter_state;
        do
        {
            last_enter_state = enter_state;
            enter_state = digitalRead(BTN_ENTER_PIN) == HIGH ? ON : OFF;
        } while (!(enter_state != last_enter_state && enter_state == ON));
    }
    else
        input_guess(history, entry + 1);

    lcd.clear();
}

bool is_ended(char *secret, char history[][5], const int entry)
{
    return strcmp(secret, history[entry]) == 0 || entry + 1 >= TRIES;
}

void play_game(char *secret)
{
    char history[TRIES][5];

    turn_off_leds();
    int i = 0;
    input_guess(history, 0);
    do
    {
        render_history(secret, history, i);
    } while (!is_ended(secret, history, i++));
}
