#include <string.h>
#include <assert.h>

#include "system.h"
#include "uart.h"
#include "can.h"
#include "timer.h"
#include "flash_ctrl.h"
#include "i2c.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define TEST_LOW_PWR_MODE
#define RUN_TIME_THRESHOLD                      5000
#define FLASH_USER_START_ADDR   	            (FLASH_BASE_ADDR + 0x20000)

#if defined FLASH_SECTOR_SIZE
	#define FLASH_ERASE_SIZE                    FLASH_SECTOR_SIZE
#else
	#define FLASH_ERASE_SIZE                    4096
#endif

/*******************************************************************************
 * Local function prototypes
 ******************************************************************************/
static void test_flash(void);
static void test_i2c(void);
 
/*******************************************************************************
 * Functions
 ******************************************************************************/
void test(void)
{
	uint8_t buf[100];
	uint16_t size = 0;
	uint32_t id = 0;
	uint32_t run_time = 0;
	uint32_t filter_id_list[] = {0x328, 0x118, 0x3B0};
	
	sys_init();
	gpio_init();
	test_flash();
	test_i2c();
	uart_init(UART_CH0, 115200, UART_DATA_BITS_8, UART_STOP_BITS_1, UART_PARITY_MODE_NONE);
	uart_init(UART_CH1, 115200, UART_DATA_BITS_8, UART_STOP_BITS_1, UART_PARITY_MODE_NONE);
	can_init(CAN_CH0, filter_id_list, sizeof(filter_id_list) / sizeof(uint32_t));
	can_init(CAN_CH1, filter_id_list, sizeof(filter_id_list) / sizeof(uint32_t));
	timer_init(TIMER0, 500);
	timer_start(TIMER0);
	wdog_enable();
	
	while (1)
	{
		wdog_refresh();
		delay(20);
		GPIO_TOGGLE_PIN(LED0_GPIO, LED0_PIN);

#if defined TEST_LOW_PWR_MODE
		if (RUN_TIME_THRESHOLD <= clock() - run_time)
		{
			GPIO_WRITE_PIN(LED0_GPIO, LED0_PIN, LED_ON);
			GPIO_WRITE_PIN(LED1_GPIO, LED1_PIN, LED_OFF);
			timer_stop(TIMER0);
			can_pwr_mode_trans(CAN_CH0, CAN_PWR_MODE_SLEEP);
			can_pwr_mode_trans(CAN_CH1, CAN_PWR_MODE_SLEEP);
			pwr_mode_trans(PWR_MODE_DEEPSLEEP);
			can_pwr_mode_trans(CAN_CH0, CAN_PWR_MODE_RUN);
			can_pwr_mode_trans(CAN_CH1, CAN_PWR_MODE_RUN);
			run_time = clock();
			timer_start(TIMER0);
			GPIO_WRITE_PIN(LED0_GPIO, LED0_PIN, LED_OFF);
			wdog_refresh();
		}
#endif

		for (uint8_t i = UART_CH0; i <= UART_CH1; i++)
		{
			if(0 != (size = uart_receive(i, buf, sizeof(buf))))
			{
				print_buf("UART-RX", i, buf, size);
				uart_send(i, buf, size);
			}
		}

		for (uint8_t i = CAN_CH0; i <= CAN_CH1; i++)
		{
			if(0 != (size = can_receive(i, &id, buf, sizeof(buf))))
			{
				print_buf("CAN-RX", id, buf, size);
				run_time = clock();
				can_send(i, id, buf, size);
			}
		}
	}
}

/**
 * Timer IRQ callback.
 *
 * @param _num the timer number
 */
void timer_irq_callback(const uint8_t _num)
{
	(void)_num;
	GPIO_TOGGLE_PIN(LED1_GPIO, LED1_PIN);
}

/*******************************************************************************
 * Local functions
 ******************************************************************************/
/**
 * Test flash.
 */
static void test_flash(void)
{
	uint8_t buf[64];
	
	assert(0 == flash_ctrl_init());
	
	/* erase and vefiry continuous sectors */
	assert(0 == flash_ctrl_erase_sector(FLASH_USER_START_ADDR, FLASH_ERASE_SIZE));
	assert(0 == flash_ctrl_verify_sector(FLASH_USER_START_ADDR, FLASH_ERASE_SIZE));

	/* program and vefify a flash memory area */
	memset(buf, 0xAA, sizeof(buf));
	assert(0 == flash_ctrl_program(FLASH_USER_START_ADDR, sizeof(buf), buf));
	assert(0 == flash_ctrl_program_verify(FLASH_USER_START_ADDR, sizeof(buf), buf));
	
    /* write the EEPROM */
	memset(buf, 0xBB, 4);
    assert(0 == flash_ctrl_write_e2(0, 4, buf));
	assert(0 == flash_ctrl_deinit());
}

/**
 * Test I2C.
 */
static void test_i2c(void)
{
#if defined MX_TB
	uint8_t temp1 = 0xAA;
	uint8_t temp2 = 0;
	
	assert(0 == i2c_master_init(I2C_CH0, 400000, false));
	
	/* write and read the EEPROM then verify */
	assert(0 == eeprom_write(EEPROM_ADDR_RESET_TYPE, &temp1, EEPROM_SIZE_RESET_TYPE));
	delay(10);
	assert(0 == eeprom_read(EEPROM_ADDR_RESET_TYPE, &temp2, EEPROM_SIZE_RESET_TYPE));
	assert(temp1 == temp2);
	assert(0 == i2c_master_deinit(I2C_CH0));
#endif
}
