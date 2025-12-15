#include <xc.h>
#include <sys/attribs.h>

// config.h FROM LAB 5
#pragma config JTAGEN = OFF
#pragma config ICESEL = ICS_PGx3
#pragma config WINDIS = OFF         // Watchdog Timer Window Enable (Watchdog Timer is in Non-Window Mode)
#pragma config FWDTEN = OFF         // Watchdog Timer Enable (WDT Enabled)
#pragma config FSOSCEN = OFF        // Secondary Oscillator Enable (Enabled)
#pragma config PMDL1WAY = OFF       // Peripheral Module Disable Configuration (Allow only one reconfiguration)
#pragma config IOL1WAY = OFF        // Peripheral Pin Select Configuration (Allow only one reconfiguration)
#pragma config FNOSC = FRCDIV       // Oscillator Selection Bits (Fast RC Oscw/Div-by-N (FRCDIV))
#pragma config FPBDIV = DIV_8       // Peripheral Clock Divisor (Pb_Clk = Sys_Clk/8)

//LAB 7 stuff -r is a Rest. If r, do not toggle! (don't want noise to play)
// note constants
#define r 3000
#define a 4545
#define b 4050
#define C 3817
#define C_ 3610
#define D 3402
#define D_ 3216
#define E 3031
#define F 2866
#define F_ 2703
#define G 2551
#define G_ 2410
#define A 2273
#define A_ 2146
#define B 2025
#define CC 1911
#define q 400
#define qdot q * 1.5
#define e q/2
#define s e/2
#define t32 s/2
#define sdot s+t32
#define h q*2
#define hdot q+e
#define edot e+s
#define num_notes 30
#define ONE_MS_COUNTS 50 // (tested and 500ms works perfect for actual timing) for countdown timer. Specifically for FPBDIV = DIV_8

// alarm sound arrays  from lab 7
short delay[num_notes] = {t32,t32,t32,t32,t32,t32,t32,t32,t32,t32,s,sdot,t32,t32,
t32,t32,t32,t32,t32,t32,t32,t32,t32,s,sdot,t32,t32,t32,
t32,t32,t32,t32,t32,t32,t32,t32,s,sdot,t32,t32,t32,t32,
t32,t32,t32,t32,t32,t32,t32,t32,s,e};

short music_notes[num_notes] =
{b,r,B,r,F_,r,D_,r,B,F,r,D,r,C,r,CC,r,G,r,E,r,CC,G,r,
E,r,b,r,B,r,F_,r,D_,r,B,F,r,D,r,D,E,D_,r,F,F_,G,r,G,G_,A,r,B};

// initializations/variables
int i,j;
unsigned char countdown_on = 0; // countdown, 1 = 60 sec countdown active
unsigned char alarm_on = 0; // piezo playing sound, 1 = alarm beeping

// 7- segment setup
// wire RB8?RB11 to ones decoder A,B,C,D (A=LSB)
// wire R12?RB15 to tens decoder A,B,C,D
// A = Least significant bit (1), B = bit 2, C = bit 3, D = most significant bit (8)
// example: writing 0101 (5 decimal) the segment should show ?5?
// leave control pins (LE/LT/BI) in ?normal display?

# define ONE_A LATBbits.LATB8      // 1 digit -> BCD input A
# define ONE_B LATBbits.LATB9     // 1 digit -> BCD input B
# define ONE_C LATBbits.LATB10    // 1 digit -> BCD input C
# define ONE_D LATBbits.LATB11    // 1 digit -> BCD input D

# define TENS_A LATBbits.LATB12     // 10s digit -> BCD input A
# define TENS_B LATBbits.LATB13    // 10 digit -> BCD input B
# define TENS_C LATBbits.LATB14     // 10 digit -> BCD input C
# define TENS_D LATBbits.LATB15     // 10 digit -> BCD input D

int num;
// writing a number 0-9 into the ONES digit BCD
void write_ones_digit(int num){	
	if (num < 0) num = 0;  // clamp below 0
	if (num > 9) num = 9;  // clamp above 9

// writing each bit of number to the decoder
ONE_A = (num >> 0 ) & 1;  // bit0 -> A
ONE_B = (num >> 1) & 1;  // bit1 -> B
ONE_C = (num >> 2) & 1;  // bit2 -> C
ONE_D = (num >> 3) & 1;  // bit3 -> D
}

// writing a number 0-9 into the TENS digit BCD
void write_tens_digit(int num){	
	if (num < 0) num = 0;  // clamp below 0
	if (num > 9) num = 9;  // clamp above 9
// writing each bit of number to the decoder
TENS_A = (num >> 0 ) & 1;  // bit0 -> A
TENS_B = (num >> 1) & 1;  // bit1 -> B
TENS_C = (num >> 2) & 1;  // bit2 -> C
TENS_D = (num >> 3) & 1;  // bit3 -> D
}

int value;
// to display 2 digits on 2 different displays 00-99
void display_number(int value){	
	if (value < 0) value = 0;  
	if (value > 99) value = 99;  
	
	int tens = value / 10;         // left segment
	int ones = value % 10;      // right segment

write_tens_digit(tens);
write_ones_digit(ones);

}
//Interrupt Function
void __ISR(12)musicccc(void){
    if (alarm_on){
        PR2 = music_notes[i];   // period for the current note
        j++;                               // how long the note has been on for 

        if (music_notes[i] != r){
            LATBbits.LATB0 ^= 1;
        }
        display_number(0);      // during alarm should display 00
    }
    else if (countdown_on){
        PR2 = ONE_MS_COUNTS; // 1 MS ticks
        j++; // MS count

        int seconds_left = 60 - (j/1000);
        if (seconds_left < 0 ) seconds_left = 0;
       display_number(seconds_left); 

    } else {
       display_number(60);   
    }   
    IFS0bits.T3IF = 0;
}
    
void __ISR(3) ButtonISR(void){ // button interrupt 
    IFS0bits.INT0IF = 0; // set/clear flag in IFSX register   
    if (alarm_on == 1){ // stops alarm/buzzer early if still playing 
        alarm_on = 0;
        LATBbits.LATB0 = 0;
    }
    else if (countdown_on == 1){ // cancel countdown
        countdown_on = 0;
    }
    else { // start a new 60 sec timer
        countdown_on = 1;
        j = 0; 
    }
}
    
void main(){
    INTCONbits.MVEC = 1;
    __builtin_enable_interrupts();
    
    // housekeeping (Tris I/O and ANSEL)
    TRISBbits.TRISB0 = 0; // buzzer output
    LATBbits.LATB0 = 0;    // buzzer off initially
    ANSELA = 0;
    ANSELB = 0;
    TRISA = 0; 
    
    TRISBbits.TRISB7 = 1; // button on B7

 // 7 segment outputs
    TRISBbits.TRISB8 = 0; // ones A
    TRISBbits.TRISB9 = 0; // ones B
    TRISBbits.TRISB10 = 0; // ones C
    TRISBbits.TRISB11 = 0; // ones D
    TRISBbits.TRISB12 = 0; // tens A
    TRISBbits.TRISB13 = 0; // tens B
    TRISBbits.TRISB14 = 0; // tens C
    TRISBbits.TRISB15 = 0; //tens D

    display_number(60);


    INTCONbits.INT0EP = 1; // rising edge trigger 
    IEC0bits.INT0IE = 1;  // enable
    IFS0bits.INT0IF = 0;  // clear flag
    IPC0bits.INT0IP = 1; // interrupt priority
    
    // timer 2/3 set up)
    T2CONbits.ON = 0;
    T2CONbits.TCKPS = 0x000;
    T2CONbits.T32 = 1;
    T2CONbits.TCS = 0;
    
    PR2 = ONE_MS_COUNTS;     // start with 1 MS period
    TMR2 = 0;

    T2CONbits.ON = 1;     // turn on T2
    IEC0bits.T3IE = 1;
    IFS0bits.T3IF = 0;     //clear TMR2
    IPC3bits.T3IP = 1;
    

// MAIN WHILE LOOP
    while(1)               
    {
        if (countdown_on == 1){ //countdown for 60 seconds
            j = 0; 
            while (countdown_on == 1 && j < 60000){ // wait 600000 MS
        }
            if (countdown_on == 1){ //countdown ended -> start alarm
                countdown_on = 0;
                alarm_on = 1;
               // int ms = 0; // count alarm time
                for(i=0; i < num_notes && alarm_on == 1; i++){
                    j = 0;
                    while(j < delay[i] && alarm_on == 1){
                    //    ms++;
                    //    if (ms > 10000) break; // turns off after 10 seconds
                    }
                   // if (ms > 10000) break;
            }
                alarm_on = 0; // buzzer off
                LATBbits.LATB0 = 0; // buzzer off
            }
        }
    }
    
}


