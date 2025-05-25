#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 Controller (Keyboard/Mouse).
 */

#define KBD_IRQ 		1 	/**< @brief Keyboard IRQ line */
#define MOUSE_IRQ		12	/**< @brief Mouse IRQ line */

/* I/O port addresses */

#define KBC_OUT_BUF      0x60    /**< @brief KBC output buffer */
#define KBC_ST_REG       0x64    /**< @brief KBC status register */
#define KBC_CMD_REG      0X64    /**< @brief commands to KBC */
#define KBC_W_CMD_BYTE   0X60    /**< @brief arguments to KBC commands */

/* Timer control */

/* KBC Status Register */

#define PARITY  BIT(7)      /**< @brief Parity error - invalid data */
#define TIMEOUT BIT(6)      /**< @brief Timeout error - invalid data  */
#define AUX     BIT(5)      /**< @brief Mouse data */
#define INH     BIT(4)      /**< @brief Inhibit flag: 0 if keyboard is inhibited */
#define A2      BIT(3)      /**< @brief A2 input line */
#define SYS     BIT(2)      /**< @brief System flag */
#define IBF     BIT(1)      /**< @brief Input buffer full */
#define OBF     BIT(0)      /**< @brief Output buffer full */

/* Keyboard KBC commands */

#define RD_CMD           0x20    /**< @brief Read command byte */
#define WT_CMD           0x60    /**< @brief Write command byte */
#define CHK_KBC          0XAA    /**< @brief Check KBC */
#define CHK_KBD_INT      0xAB    /**< @brief Check Keyboard Interface */
#define DS_KBD_INT       0xAD    /**< @brief Disable Keyboard Interface  */
#define EN_KBD_INT       0xAE    /**< @brief Enable Keyboard Interface  */

/* KBC Command Byte */

#define DIS2    BIT(5)      /**< @brief Disable mouse interface */
#define DIS     BIT(4)      /**< @brief Disable keyboard interface */
#define INT2    BIT(1)      /**< @brief Enable interrupt on OBF, from mouse */
#define INT     BIT(0)      /**< @brief Enable interrupt on OBF, from keyboard */

/* KBD check Break and double-scancode */

#define CHK_BREAK   BIT(7)
#define DBL_SCCD    0xE0

/* Mouse Commands */

#define WRT_BYTE_MOUSE 		0xD4		/**< @brief Request sending the command byte to the mouse */
#define SUCCESS_CMD_MOUSE 	0xFA		/**< @brief Succesful write command */
#define FIRST_BYTE_CHECK	BIT(3) 	/**< @brief Check if this packet can be the first one */
#define RETRY			0XFE		/**< @brief Retry to send the whole command */
#define DISABLE_MOUSE		0XA7
#define ENABLE_MOUSE 		0xA8
#define CHECK_MOUSE_INT		0XA9
#define DISABLE_DATA_REP		0XF5
#define ENABLE_DATA_REP		0XF4

/**@}*/

#endif /* _LCOM_I8042_H */

