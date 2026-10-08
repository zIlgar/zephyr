#include <zephyr/kernel.h>

int main(void)
{
	int a, b, result;

	printk("\n=== Quick Math Quiz ===\n\n");

	while (1) {
		a = (k_uptime_get_32() % 9) + 1;
		k_msleep(50);
		b = (k_uptime_get_32() % 9) + 1;
		result = a * b;

		printk("Question: %d x %d = ?\n", a, b);
		k_msleep(2000);
		printk("Answer: %d\n\n", result);

		k_msleep(2000);
	}

	return 0;
}