#include <string.h>
#include <assert.h>

#include "board/board.h"
#include "uart/uart.h"
#include "can/can.h"
#include "timer/timer.h"
#include "flash_ctrl/flash_ctrl.h"
#include "i2c/i2c.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define TEST_LOW_PWR_MODE
#define RUN_TIME_THRESHOLD                      5000
#define FLASH_USER_START_ADDR   	            (FLASH_BASE_ADDR + 0x20000)
#if defined FLASH_SECTOR_SIZE
#define FLASH_ERASE_SIZE                        FLASH_SECTOR_SIZE
#else
#define FLASH_ERASE_SIZE                        4096
#endif

/*******************************************************************************
 * Local Function prototypes
 ******************************************************************************/
static void test_flash(void);
static void test_i2c(void);
 
/*******************************************************************************
 * Functions
 ******************************************************************************/
void prv_main(void)
{
	uint8_t  buf[100];
	uint16_t size = 0;
	uint32_t id = 0;
	uint32_t run_time = 0;
	uint32_t filter_id_list[] = {0x328, 0x118, 0x3B0};
	
	sys_init();
	gpio_init();
	test_flash();
	test_i2c();
	uart_init(UART0_INDEX, 115200, UART_DATA_BITS_8, UART_STOP_BITS_1, UART_PARITY_MODE_NONE);
	uart_init(UART1_INDEX, 115200, UART_DATA_BITS_8, UART_STOP_BITS_1, UART_PARITY_MODE_NONE);
	can_init(CAN0_INDEX, filter_id_list, sizeof(filter_id_list) / sizeof(uint32_t));
	can_init(CAN1_INDEX, filter_id_list, sizeof(filter_id_list) / sizeof(uint32_t));
	timer_init(TIMER0_INDEX, 500);
	timer_start(TIMER0_INDEX);
	wdog_enable();
	
	while(1)
	{
		wdog_refresh();
		
		delay(20);
		GPIO_TOGGLE_PIN(LED0_GPIO, LED0_PIN);
#if defined TEST_LOW_PWR_MODE
		if(RUN_TIME_THRESHOLD <= sys_time() - run_time)
		{
			GPIO_WRITE_PIN(LED0_GPIO, LED0_PIN, LED_ON);
			GPIO_WRITE_PIN(LED1_GPIO, LED1_PIN, LED_OFF);
			timer_stop(TIMER0_INDEX);
			can_pwr_mode_trans(CAN0_INDEX, CAN_PWR_MODE_SLEEP);
			can_pwr_mode_trans(CAN1_INDEX, CAN_PWR_MODE_SLEEP);
			pwr_mode_trans(PWR_MODE_DEEPSLEEP);
			can_pwr_mode_trans(CAN0_INDEX, CAN_PWR_MODE_RUN);
			can_pwr_mode_trans(CAN1_INDEX, CAN_PWR_MODE_RUN);
			run_time = sys_time();
			timer_start(TIMER0_INDEX);
			GPIO_WRITE_PIN(LED0_GPIO, LED0_PIN, LED_OFF);
			wdog_refresh();
		}
#endif
		for(uint8_t i = UART0_INDEX; i <= UART1_INDEX; i++)
		{
			if(0 != (size = uart_receive(i, buf, sizeof(buf))))
				uart_transmit(i, buf, size);
		}
		for(uint8_t i = CAN0_INDEX; i <= CAN1_INDEX; i++)
		{
			if(0 != (size = can_receive(i, &id, buf, sizeof(buf))))
			{
				run_time = sys_time();
				can_transmit(i, id, buf, size);
			}
		}
	}
}

/**
 * @brief Timer IRQ callback.
 *
 * @param _index Timer index.
 */
void timer_irq_callback(const uint8_t _index)
{
	(void)_index;
	
	GPIO_TOGGLE_PIN(LED1_GPIO, LED1_PIN);
}

/*******************************************************************************
 * Local Functions
 ******************************************************************************/
/**
 * @brief Test flash.
 */
static void test_flash(void)
{
	uint8_t buf[64];
	
	assert(0 == flash_ctrl_init());
	
	/* Erase and vefiry sectors */
	assert(0 == flash_ctrl_erase_sector(FLASH_USER_START_ADDR, FLASH_ERASE_SIZE));
	assert(0 == flash_ctrl_verify_sector(FLASH_USER_START_ADDR, FLASH_ERASE_SIZE));

	/*  Program and vefify */
	memset(buf, 0xAA, sizeof(buf));
	assert(0 == flash_ctrl_program(FLASH_USER_START_ADDR, sizeof(buf), buf));
	assert(0 == flash_ctrl_program_verify(FLASH_USER_START_ADDR, sizeof(buf), buf));
	
    /* Write EEPROM */
	memset(buf, 0xBB, 4);
    assert(0 == flash_ctrl_write_e2(0, 4, buf));
	
	assert(0 == flash_ctrl_deinit());
}

/**
 * @brief Test I2C.
 */
static void test_i2c(void)
{
#if defined MX_TB
	uint8_t temp1 = 0xAA;
	uint8_t temp2 = 0;
	
	assert(0 == i2c_master_init(I2C0_INDEX, 400000, false));
	
	/* Write then read EEPROM and verify */
	assert(0 == eeprom_write(EEPROM_ADDR_RESET_TYPE, &temp1, EEPROM_SIZE_RESET_TYPE));
	delay(10);
	assert(0 == eeprom_read(EEPROM_ADDR_RESET_TYPE, &temp2, EEPROM_SIZE_RESET_TYPE));
	assert(temp1 == temp2);
	
	assert(0 == i2c_master_deinit(I2C0_INDEX));
#endif
}
