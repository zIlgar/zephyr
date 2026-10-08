#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/led_strip.h>
#include <zephyr/random/random.h>
#include <stdio.h>

#define NUM_LEDS 10

static const struct gpio_dt_spec btn_rock = GPIO_DT_SPEC_GET(DT_ALIAS(sw0), gpios);
static const struct gpio_dt_spec btn_paper = GPIO_DT_SPEC_GET(DT_ALIAS(sw1), gpios);
static const struct gpio_dt_spec btn_scissors = GPIO_DT_SPEC_GET(DT_ALIAS(sw2), gpios);

static const struct device *strip = DEVICE_DT_GET(DT_ALIAS(led_strip));
static struct led_rgb pixels[NUM_LEDS];

static int user_score = 0;
static int comp_score = 0;

static const char *names[] = {"Rock", "Paper", "Scissors"};

void set_color(uint8_t r, uint8_t g, uint8_t b)
{
    for (int i = 0; i < NUM_LEDS; i++) {
        pixels[i].r = r;
        pixels[i].g = g;
        pixels[i].b = b;
    }
    led_strip_update_rgb(strip, pixels, NUM_LEDS);
}

void play_round(int user_choice)
{
    int comp_choice = sys_rand32_get() % 3;

    printf("\nUser chose: %s\n", names[user_choice]);
    printf("Computer chose: %s\n", names[comp_choice]);

    if (user_choice == comp_choice) {
        printf("Result: Draw!\n");
        set_color(25, 25, 25);
    } else if ((user_choice == 0 && comp_choice == 2) ||
               (user_choice == 1 && comp_choice == 0) ||
               (user_choice == 2 && comp_choice == 1)) {
        user_score++;
        printf("Result: You Win!\n");
        set_color(0, 50, 0);
    } else {
        comp_score++;
        printf("Result: Computer Wins!\n");
        set_color(50, 0, 0);
    }

    printf("Score -> User: %d | Computer: %d\n", user_score, comp_score);

    k_msleep(2000);
    set_color(25, 25, 25);
}

int main(void)
{
    gpio_pin_configure_dt(&btn_rock, GPIO_INPUT);
    gpio_pin_configure_dt(&btn_paper, GPIO_INPUT);
    gpio_pin_configure_dt(&btn_scissors, GPIO_INPUT);

    set_color(25, 25, 25);

    printf("=== Rock Paper Scissors Started ===\n");
    printf("Press Button 1 (Rock), Button 2 (Paper), or Button 3 (Scissors)\n");

    while (1) {
        if (gpio_pin_get_dt(&btn_rock) > 0) {
            play_round(0);
            while (gpio_pin_get_dt(&btn_rock) > 0) {
                k_msleep(50);
            }
        } else if (gpio_pin_get_dt(&btn_paper) > 0) {
            play_round(1);
            while (gpio_pin_get_dt(&btn_paper) > 0) {
                k_msleep(50);
            }
        } else if (gpio_pin_get_dt(&btn_scissors) > 0) {
            play_round(2);
            while (gpio_pin_get_dt(&btn_scissors) > 0) {
                k_msleep(50);
            }
        }

        k_msleep(50);
    }

    return 0;
}