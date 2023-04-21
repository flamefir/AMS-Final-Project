#include "ssd1963_displayDriver.hpp"

static inline void nop(unsigned n) //1 NOP = 4.166667 NS
{
    while(n--){
        _NOP();
    }
}

template <typename T>
static void swap(T &a, T &b)
{
    const auto temp = a;
    a = b;
    b = temp;
}

void ssd1963::ssdCmdSlow(uint8_t value)
{
    gpioControl_.CS_setLevel(0);
    nop(30);
    gpioControl_.RS_setLevel(0);
    nop(30);
    gpioControl_.WR_setLevel(1);
    nop(30);
    gpioControl_.sendData(value);
    nop(30);
    gpioControl_.WR_pulse();
    nop(30);
    gpioControl_.CS_setLevel(1);
    nop(30);
}

void ssd1963::ssdDataSlow(uint8_t value)
{
    gpioControl_.CS_setLevel(0);
    gpioControl_.RS_setLevel(1);
    gpioControl_.WR_setLevel(0);
    gpioControl_.sendData(value);
    gpioControl_.RS_setLevel(0);
    gpioControl_.WR_pulse();
    gpioControl_.CS_setLevel(1);
}

ssd1963::ssd1963(gpio_num_t controlPins[], gpio_num_t dataPins[])
{
    gpioControl_.initGpioPins(controlPins, dataPins);
}

ssd1963::~ssd1963()
{
}

void ssd1963::DisplayInit()
{
    DisplayReset();
    /* Se side 18
    Before the PLL output is configured as the system clock by the bit 1 of “set_pll” command 0xE0, the system
    will be clocked by the reference clock. This enables the user to send the “set_pll_mn” command 0xE2 to the
    PLL for frequency configuration.
    */
    // VCO = 10(M+1)
    // PLL = VCO / N + 1
    ssdCmdSlow(0xE2);    // PLL multiplier, set PLL clock to 100M se side 74 
    ssdDataSlow(0x1D);   // M
    ssdDataSlow(0x02);   // N
    ssdDataSlow(0x04);   // True / false : ignore multiplier and divider 
    /* When the PLL frequency is configured and the PLL was enabled with the
    bit 0 of “set_pll” command 0xE0, the user should still wait for 100us for the PLL to lock.*/
    ssdCmdSlow(0xE0);    // setPLL 
    ssdDataSlow(0x01);   // 0x01 -> enable
    delayMicroseconds(120);
    /*Then the PLL is ready and can be configured as system clock with the bit 1 of “set_pll” command 0xE0.*/
    ssdCmdSlow(0xE0);    // setPLL
    ssdDataSlow(0x03);   // 0x03 use PLL output as systemclock
    delay(10);        
    ssdCmdSlow(0x01);    // software reset
    delay(10);

    /* Se side 76 under serial LCD interface
    To obtain PCLK = 5.3MHz with PLL Frequency = 100MHz,
    5.3MHz = 100MHz * ( ( LCDC_FPR+ 1) / 2^20 )*4 
    LCDC_FPR = 13892
    */
    ssdCmdSlow(0xE6);    // PLL setting for PCLK, depends on resolution
    ssdDataSlow(0x00);   // [19:16] highest 4 bits for LCDC_FPR
    ssdDataSlow(0x36);   // [15:8] higher byte for LCDC_FPR
    ssdDataSlow(0x44);   // [7:0] lower byte for LCDC_FPR

    /* Se side 44 - 87 - 88
    Følger vi -> Figure 13-7: Serial RGB Interface Timing (with dummy mode) med data[7:0]
        LSHIFT Rising Edge
        LLINE Active High
        LFRAME Active High
    */
    ssdCmdSlow(0xB0);    // LCD SPECIFICATION
    ssdDataSlow(0x07);   // | LSHIFT Rising Edge(1) | LLINE Active High(1) | LFRAME Active High(1) |
    ssdDataSlow(0x03);   // Serial RGB+dummy mode 11
    ssdDataSlow(0x03);   // [10:8] higher bits Set HDP	799
    ssdDataSlow(0x1F);   // [7:0]  lower byte Set HDP	799
    ssdDataSlow(0x01);   // [10:8] higher bits Set VDP	479
    ssdDataSlow(0xDF);   // [7:0]  lower byte Set VDP	479
    ssdDataSlow(0x00);   // Even line RGB(0) | Odd line RGB(0)

    /* Se side 47 - 88 (set_hori_period)
    
    */
    ssdCmdSlow(0xB4);    // HSYNC period
    ssdDataSlow(0x03);   // [10:8] higher bits set HT	928
    ssdDataSlow(0xA0);   // [7:0] lower byte set HT	    928
    ssdDataSlow(0x00);   // [10:8] higher bits set HPS	46
    ssdDataSlow(0x2E);   // [7:0] lower bits set HPS	46
    ssdDataSlow(0x30);   // [6:0] lower bits set HPW	48
    ssdDataSlow(0x00);   // [10:8] higher bits set LPS	15
    ssdDataSlow(0x0F);   // [7:0] lower bits set LPS	15
    ssdDataSlow(0x00);   // [1:0] Set the horizontal sync pulse subpixel start position

    /* Se side 49 - 88 (set_vert_period)
    
    */
    ssdCmdSlow(0xB6);    // VSYNC period
    ssdDataSlow(0x02);   // [10:8] Set VT	            525
    ssdDataSlow(0x0D);   // [7:0] Set VT	            525
    ssdDataSlow(0x00);   // [10:8] Set VPS	        16
    ssdDataSlow(0x10);   // [7:0] Set VPS	        16
    ssdDataSlow(0x10);   // [7:0] lower bits set VPW	16
    ssdDataSlow(0x00);   // [10:8] higher bits set FPS	8
    ssdDataSlow(0x08);   // [7:0] lower bits set FPS	8

    /* Se side 52 - 88 (set_gpio_value)
    
    */
    ssdCmdSlow(0xBA);
    ssdDataSlow(0x05);   // GPIO[3:0] out 1

    /* Se side 50 - 88 (set_gpio_conf)
    
    */
    ssdCmdSlow(0xB8);
    ssdDataSlow(0x07);   // GPIO3=input, GPIO[2:0]=output
    ssdDataSlow(0x01);   // GPIO0 normal
    
    /* Se side 36-39  (set_address_mode)
    Host processor to frame buffer A[7:5] 
    RGB(0) - BGR(1) A[3]
    top line to bottom line(0) A[4]
    Frame buffer to display panel A[2:0] 
    */
    ssdCmdSlow(0x36);    // Address - 
    ssdDataSlow(0x00);   // Normal setup
     
    /* Se side 78 (set_pixel_data_interface)
    This command is used for display data only, the command format is always 8 bit. 
    */
    ssdCmdSlow(0x3A);
    ssdDataSlow(0x50);
    ssdCmdSlow(0xF0);    // pixel data interface
    ssdDataSlow(0x00);   // 0x00 -> 8bit

    delay(10);

    ssdCmdSlow(0x29);    // display on

    /* Se side 78 (set_pixel_data_interface)
    This command is used for display data only, the command format is always 8 bit. 
    */
    ssdCmdSlow(0xBE);    // set PWM for B/L
    ssdDataSlow(0x06);   // PWMF[7:0] PWM frequency
    ssdDataSlow(0x80);   // PWM[7:0]  PWM duty cycle Initial brightness to 100%
    ssdDataSlow(0x01);   // C[3] : PWM configuration C[0] : PWM enable
    ssdDataSlow(0xF0);   // D[7:0] : DBC manual brightness
    ssdDataSlow(0xF0);   // E[7:0] : DBC minimum brightness
    ssdDataSlow(0x00);   // F[3:0] : Brightness prescaler
    
    /* Se side 70 (set_pixel_data_interface)
    
    */
    ssdCmdSlow(0xD0);
    ssdDataSlow(0x0D);

    ssdCmdSlow(0x2C);    // write_memory_start
    
    fill(0, 0, 800, 480, 0xff, 0xff, 0xff);
}

void ssd1963::DisplayReset()
{
    gpioControl_.RST_setLevel(0);
    delay(100);

    gpioControl_.CS_setLevel(1);
    gpioControl_.RST_setLevel(1);
    gpioControl_.WR_setLevel(1);

    gpioControl_.sendData(0xff);
    delay(100);
}

void ssd1963::setColRowStartStop(uint16_t x_ColStart, uint16_t y_RowStart, uint16_t x_ColEnd, uint16_t y_RowEnd)
{
    //Change to portrait? then swap
    if (0)
    {
        ::swap(x_ColStart, y_RowStart);
        ::swap(x_ColEnd, y_RowEnd);
    }
    x_ColEnd = x_ColEnd - 1 + x_ColStart; //(end adress - start address)
    y_RowEnd = y_RowEnd - 1 + y_RowStart; //(end adress - start address)
    SetColumnAddress(x_ColStart, x_ColEnd); 
    SetPageAddress(y_RowStart, y_RowEnd);
    ssdCmdSlow(0x2c);
}

void ssd1963::fill(uint16_t x_ColStart, uint16_t y_RowStart, uint16_t x_ColEnd, uint16_t y_RowEnd, unsigned char Red, unsigned char Blue, unsigned char Green)
{
    setColRowStartStop(x_ColStart, y_RowStart, x_ColEnd, y_RowEnd);

    uint32_t pixelEnd = x_ColEnd * y_RowEnd;
    gpioControl_.CS_setLevel(0);
    gpioControl_.RS_setLevel(1);
    gpioControl_.WR_setLevel(1);
    //3x 8 bit
    for (uint32_t i = 0; i < pixelEnd; i++)
    {
        WritePixel(Red, Blue, Green);
    }
    gpioControl_.CS_setLevel(1);
}

void ssd1963::DisplayOff()
{
    ssdCmdSlow(0x28);
}

void ssd1963::DisplayOn()
{
    ssdCmdSlow(0x29);
}
void ssd1963::EnterSleepMode()
{
    ssdCmdSlow(0x10);
    delayMicroseconds(20); // wait atleast 10ms 
}

void ssd1963::ExitSleepMode()
{
    ssdCmdSlow(0x11);
    delayMicroseconds(10); // wait atleast 5ms 
}

void ssd1963::MemoryAccessControl(unsigned char parameter)
{
    
}

void ssd1963::InterfacePixelFormat(unsigned char parameter)
{
    
}

void ssd1963::WritePixel(unsigned char Red, unsigned char Blue, unsigned char Green)
{
    gpioControl_.sendData(Red);
    gpioControl_.WR_pulse();
    gpioControl_.sendData(Blue);
    gpioControl_.WR_pulse();
    gpioControl_.sendData(Green);
    gpioControl_.WR_pulse();
}

void ssd1963::SetColumnAddress(uint16_t Start, uint16_t End){
    ssdCmdSlow(0x2a);
    ssdDataSlow(Start >> 8); //start col pixel
    ssdDataSlow(Start);
    ssdDataSlow(End >> 8); //end col pixel (width)
    ssdDataSlow(End);
}
void ssd1963::SetPageAddress(uint16_t Start, uint16_t End){
    ssdCmdSlow(0x2b);
    ssdDataSlow(Start >> 8); //start page pixel (rows)
    ssdDataSlow(Start);
    ssdDataSlow(End >> 8);
    ssdDataSlow(End);
}

void ssd1963::MemoryWrite(){
    
}


