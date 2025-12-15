#include <xc.h>
#include <sys/attribs.h>

// config.h
#pragma config JTAGEN = OFF
#pragma config ICESEL = ICS_PGx3
#pragma config WINDIS = OFF         // Watchdog Timer Window Enable (Watchdog Timer is in Non-Window Mode)
#pragma config FWDTEN = OFF         // Watchdog Timer Enable (WDT Enabled)
#pragma config FSOSCEN = OFF        // Secondary Oscillator Enable (Enabled)
#pragma config PMDL1WAY = OFF       // Peripheral Module Disable Configuration (Allow only one reconfiguration)
#pragma config IOL1WAY = OFF        // Peripheral Pin Select Configuration (Allow only one reconfiguration)
#pragma config FNOSC = FRCDIV       // Oscillator Selection Bits (Fast RC Oscw/Div-by-N (FRCDIV))
#pragma config FPBDIV = DIV_8       // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/8)

// Program goes here
int count = 0;
    
void __ISR(3) InterFunc(void){ // interrupt from lab 5
    int A = PORTBbits.RB7;
    int B = PORTBbits.RB9;
    if (A == B) count ++; // clockwise
    else count --;        // counter clockwise
    INTCONbits.INT0EP ^= 1; // flip edge polarity
    IFS0bits.INT0IF = 0; // set/clear flag in IFSX register
}

void __ISR(7) InterFunc2(void){ // new interrupt 
    int A = PORTBbits.RB7;
    int B = PORTBbits.RB9;
    if (A != B) count ++; // clockwise
    else count --;        // counter clockwise
    INTCONbits.INT1EP ^= 1; // flip edge polarity
    IFS0bits.INT1IF = 0; // set/clear flag in IFSX register
}

void main(void){
    INTCONbits.MVEC = 1;
    __builtin_enable_interrupts();
    CFGCONbits.JTAGEN = 0;
    
    ANSELA = 0; // Set all A pins to output
    TRISA = 0;

	TRISBbits.TRISB7 = 1;  // B7 input
	TRISBbits.TRISB9 = 1;  // B9 input

    // PPS mapping method 2
    INT1R = 0b0100;//B9>>INT1

// INTERRUPT 1 INT0   
    // configure input/output
    // configure interrupt registers
    INTCONbits.INT0EP = 1; // rising edge trigger 
    IEC0bits.INT0IE = 1;
    IFS0bits.INT0IF = 0; 
    IPC0bits.INT0IP = 1; // interrupt priority
    
// INTERRUPT 2 INT1   
    // configure input/output
    // configure interrupt registers
    INTCONbits.INT1EP = 1; // rising edge trigger 
    IEC0bits.INT1IE = 1;
    IFS0bits.INT1IF = 0; 
    IPC1bits.INT1IP = 1; // interrupt priority
    
    // lab 1 counting code.  while loop identical to lab 1
    int count = 15;
    TRISB = 0;          // Set all B pins to output
    TRISBbits.TRISB7 = 1; // B7 input
    ANSELB = 0;
    
    while(1){
        LATB = count;	// Write count to output pins 
        count--;
        if(count < 1)
            count = 15;
    }
  }
