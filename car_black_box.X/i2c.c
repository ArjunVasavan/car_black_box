#include <xc.h> 

void init_i2c() {
    TRISC3 = 1; // SCL pin -> input
    TRISC4 = 1; // SDA pin -> input
    /* NOTE: Why SCL and SDA is set as input
     * I2C is an open drain protocol
     * the pins are never driven HIGH by PIC directly. instead ↓
     * Pull-up resistors keep lines HIGH by default  
     * PIC pulls them low when needed
     * So direction must be INPUT (high impedence)
     * The I2C Hardware Module controls them internally
     */

    /* NOTE: Serial Synchronization port
     * 0x28 = 0010 1000
              ││││ ││││
              │││└─┴┴┴┴→ SSPM = 1000 → I2C Master mode
              ││└──────→ CKP  = 1    → clock idle high
              │└───────→ SSPEN= 1    → enable SSP module
              └────────→ WCOL = 0    → no write collision
     * SSPEN = 1 -> turns on the I2C Hardware
     * SSPM  = 1000 -> Sets it as I2C Master
     */

    SSPCON1 = 0x28;

    /* NOTE: Baud Rate for I2C
     * 0x31 is 49 in decimal, 
     * SSPADD = (Fosc / (4 × Baud Rate)) - 1
     * For 100kHz (standard I2C speed):
       SSPADD = (20,000,000 / (4 × 100,000)) - 1
       = (20,000,000 / 400,000) - 1
       = 50 - 1
       = 49
       = 0x31
     */
    SSPADD = 0x31;

    /* CKE is used for I2C voltage levels
     * CKE = 0 means use I2C input levels
     * CKE = 1 means use SPI input levels
     */
    CKE = 0;

    /* NOTE: Slew Rate
     * Slew rate control -> controls how fast the signal transitions from LOW to HIGH
     * SMP = 1 -> Slew rate control is DISABLED -> for 100 KHz ( standard mode )
     * SMP = 0 -> Slew rate control is ENABLED  -> for 400 KHz ( fast mode )
     * at 100 kHz we dont need Slew rate thats the reason it is DISABLED
     */
    SMP = 1;

    SSPIF = 0; // clearing SSP interrupt flag
    BCLIF = 0; // clearing bus collision flag
    /* SSPIF → set by hardware when I2C operation completes
     * BCLIF → set when two masters try to use bus at same time
     */
}

void i2c_idle() {
    /* hardware sets SSPIF = 1 when it completes the I2C operation
     * this function is mainly used for waiting for the completion of I2C operation
     */
    while (!SSPIF);
    SSPIF = 0; // clearing it manually for next operation
}

/* NOTE: i2c_start logic
 * before any communication happens master announces to slaves about start of 
 * communication. this is START condition
 * on wire it looks like: 
 * SDA: HIGH → LOW  (while SCL is HIGH)
 * SCL: HIGH
 * this specific pattern means that communication is going to start
 * SEN -> start enable 
 * SEN = 1 -> hardware generates start condition on SDA and SCL 
 *         -> hardware clears SEN back to 0 automatically when done
 *         -> hardware sets SSPIF = 1 when done
 */
void i2c_start() {
    SEN = 1;
    i2c_idle();
}

/* NOTE: i2c_stop logic
 * after communication is done master must announce all slaves that communication is done
 * this is the STOP condition
 * physically on wire : 
 * SDA: LOW → HIGH  (while SCL is HIGH)
 * SCL: HIGH
 * PEN -> Pstop ENable
 * PEN = 1 -> hardware generates stop condition on SCL and SDA
 *         -> hardware clears PEN back to 0 automatically when done
 *         -> hardware sets SSPIF = 1 automatically when done
 */
void i2c_stop() {
    PEN = 1;
    i2c_idle();
}

/* NOTE: Repeated Start
 * Like a phone call where you don't hang up between two questions
 * instead of:
 *   START → tell DS1307 which register → STOP → START → read data
 * we do:
 *   START → tell DS1307 which register → REP_START → read data → STOP
 * This keeps the bus locked to us the whole time
 * No other device can jump in between
 * RSEN = 1 → hardware generates repeated start condition
 *          → hardware clears RSEN back to 0 automatically
 *          → hardware sets SSPIF = 1 when done
 */
void i2c_rep_start(){
	RSEN = 1;
	i2c_idle();
}

/* NOTE: SSPBUF ( Synchronous Serial Port Buffer )
 * hardware sends bit by bit on SDA
 * expmple: 
            data = 0xD0 = 1101 0000
            bit7 → 1
            bit6 → 1
            bit5 → 0
            bit4 → 1
            bit3 → 0
            bit2 → 0
            bit1 → 0
            bit0 → 0
   the receiver sends ACK back 
   after receiving ACK SSPIF = 1 thus i2c_idle exits
 */
void i2c_write(unsigned char data) {
	SSPBUF = data;
	i2c_idle();
}

void i2c_rx_mode() {
    /* NOTE: RCEN ReCeive ENable
     * by default SSP module is on transmit mode
     * to Receive data we need to explicitly set receiver mode to enable
     * RCEN = 1 -> tells hardware it want to receive now
     *          -> hardware starts sampling SDA wire
     *          -> stores received bytes in SSPBUF
     *          -> sets SSPIF = 1 when done
     */
	RCEN = 1;
	i2c_idle();
}

void i2c_no_ack(){
	ACKDT = 1; // ACKDT = 1 means NACK ( stop sending ) 
	ACKEN = 1; // trigger sending ACKDT value on bus
  //  i2c_idle(); // FIXME: change here if theres an issue
}

unsigned char i2c_read(){
	i2c_rx_mode();
	i2c_no_ack();
	return SSPBUF;
}
