#include <linux/init.h>
#include <linux/module.h>
#include <asm/io.h>
#include <linux/timer.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/mm.h>

//Tested with kernel 4.9.79

MODULE_LICENSE("GPL");

struct GpioRegisters
{
    uint32_t GPFSEL[6];
    uint32_t Reserved1;
    uint32_t GPSET[2];
    uint32_t Reserved2;
    uint32_t GPCLR[2];
};

struct GpioRegisters *s_pGpioRegisters;

static void SetGPIOFunction(int GPIO, int functionCode)
{
    int registerIndex = GPIO / 10;
    int bit = (GPIO % 10) * 3;

    unsigned oldValue = s_pGpioRegisters->GPFSEL[registerIndex];
    unsigned mask = 0b111 << bit;
    printk("Changing function of GPIO%d from %x to %x\n", GPIO, (oldValue >> bit) & 0b111, functionCode);
    s_pGpioRegisters->GPFSEL[registerIndex] = (oldValue & ~mask) | ((functionCode << bit) & mask);
}

static void SetGPIOOutputValue(int GPIO, bool outputValue)
{
    if (outputValue)
        s_pGpioRegisters->GPSET[GPIO / 32] = (1 << (GPIO % 32));
    else
        s_pGpioRegisters->GPCLR[GPIO / 32] = (1 << (GPIO % 32));
}

static struct timer_list s_BlinkTimer;
static int s_BlinkPeriod = 1000;
static const int LedGpioPin = 18;

static void BlinkTimerHandler(struct timer_list *unused)
{
    static bool on = false;
    on = !on;
    SetGPIOOutputValue(LedGpioPin, on);
    mod_timer(&s_BlinkTimer, jiffies + msecs_to_jiffies(s_BlinkPeriod));
}

static ssize_t set_period_callback(struct device* dev,
    struct device_attribute* attr,
    const char* buf,
    size_t count)
{
    long period_value = 0;
    if (kstrtol(buf, 10, &period_value) < 0)
        return -EINVAL;
    if (period_value < 10)	//Safety check
    	return - EINVAL;

    s_BlinkPeriod = period_value;
    return count;
}

static DEVICE_ATTR(period, S_IRWXU | S_IRWXG, NULL, set_period_callback);

static struct class *s_pDeviceClass;
static struct device *s_pDeviceObject;

#define PERIPH_BASE 0x3f000000
#define GPIO_BASE (PERIPH_BASE + 0x200000)

static int __init LedBlinkModule_init(void)
{
    int result;
    s_pGpioRegisters = (struct GpioRegisters *)ioremap(GPIO_BASE, sizeof(struct GpioRegisters));

    SetGPIOFunction(LedGpioPin, 0b001);    	//Configure the pin as output
	
    timer_setup(&s_BlinkTimer, BlinkTimerHandler, 0);
    result = mod_timer(&s_BlinkTimer, jiffies + msecs_to_jiffies(s_BlinkPeriod));
    BUG_ON(result < 0);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 4, 0)
    s_pDeviceClass = class_create("LedBlink");
#else
    s_pDeviceClass = class_create(THIS_MODULE, "LedBlink");
#endif

    BUG_ON(IS_ERR(s_pDeviceClass));

    s_pDeviceObject = device_create(s_pDeviceClass, NULL, 0, NULL, "LedBlink");
    BUG_ON(IS_ERR(s_pDeviceObject));

    result = device_create_file(s_pDeviceObject, &dev_attr_period);
    BUG_ON(result < 0);

    return 0;
}

static void __exit LedBlinkModule_exit(void)
{
    SetGPIOFunction(LedGpioPin, 0);     	//Configure the pin as input
    iounmap(s_pGpioRegisters);
    device_remove_file(s_pDeviceObject, &dev_attr_period);
    device_destroy(s_pDeviceClass, 0);
    class_destroy(s_pDeviceClass);

    del_timer(&s_BlinkTimer);
}

module_init(LedBlinkModule_init);
module_exit(LedBlinkModule_exit);