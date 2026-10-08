#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/random/random.h>
#include <stdio.h>

int my_points = 0;
int pc_points = 0;

int main(void)
{
    const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(gpio0));

    gpio_pin_configure(dev, 39, GPIO_INPUT);
    gpio_pin_configure(dev, 38, GPIO_INPUT);
    gpio_pin_configure(dev, 37, GPIO_INPUT);

    printf("Game start!\n");

    while (1) {
        int my_choice = -1;

        if (gpio_pin_get(dev, 39) == 0) {
            my_choice = 0;
        } else if (gpio_pin_get(dev, 38) == 0) {
            my_choice = 1;
        } else if (gpio_pin_get(dev, 37) == 0) {
            my_choice = 2;
        }

        if (my_choice != -1) {
            int pc_choice = sys_rand32_get() % 3;

            if (my_choice == 0) {
                printf("You chose: Rock\n");
            } else if (my_choice == 1) {
                printf("You chose: Paper\n");
            } else {
                printf("You chose: Scissors\n");
            }

            if (pc_choice == 0) {
                printf("Computer chose: Rock\n");
            } else if (pc_choice == 1) {
                printf("Computer chose: Paper\n");
            } else {
                printf("Computer chose: Scissors\n");
            }

            if (my_choice == pc_choice) {
                printf("Draw!\n");
            } else if ((my_choice == 0 && pc_choice == 2) ||
                       (my_choice == 1 && pc_choice == 0) ||
                       (my_choice == 2 && pc_choice == 1)) {
                my_points++;
                printf("You won this round!\n");
            } else {
                pc_points++;
                printf("Computer won this round!\n");
            }

            printf("Points -> You: %d | PC: %d\n\n", my_points, pc_points);

            k_msleep(2000);

            while (gpio_pin_get(dev, 39) == 0 || 
                   gpio_pin_get(dev, 38) == 0 || 
                   gpio_pin_get(dev, 37) == 0) {
                k_msleep(50);
            }
        }

        k_msleep(50);
    }

    return 0;
}
