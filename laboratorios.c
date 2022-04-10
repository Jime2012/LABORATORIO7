/* 
 * File:   ejercicios.c
 * Author: Jimena de la Rosa
 *
 * Created on 3 de abril de 2022, 1:41
 */

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdint.h>
//#include <stdlib.h>
#define BOTON0 PORTBbits.RB0
#define BOTON1 PORTBbits.RB1
uint8_t cont;
uint8_t b = 0;
uint8_t u = 0;
uint8_t d = 0;
uint8_t c = 0;
uint8_t display1 = 0;
uint8_t display2 = 0;
uint8_t display3 = 0;
int display[10] = {0b10111111, 0b10000110, 0b11011011, 0b11001111, 0b11100110, 0b11101101, 0b11111101, 0b10000111, 0b11111111, 0b11101111};

/*
 * 
 */
void __interrupt() isr(void){

   if (INTCONbits.T0IF){
            TMR0 = 252;
            INTCONbits.T0IF = 0;
            PORTDbits.RD0 = 0;
            PORTDbits.RD1 = 0;
            PORTDbits.RD2 = 0;
    
            if (b == 0){
                PORTDbits.RD0 = 1;
                PORTC = display1;
                b = 1;}
            else if (b == 1){
                PORTDbits.RD1 = 1;
                PORTC = display2;
                b = 2;}
            else if (b == 2){
                PORTDbits.RD2 = 1;
                PORTC = display3;
                b = 0;}
    }
   
    if (INTCONbits.RBIF){
        PORTB = PORTB;
        INTCONbits.RBIF = 0;
        if (!BOTON1)
            {PORTA--;  
            }
        else if (!BOTON0)
            {PORTA++;
            }
        for (cont = 0; cont < 3; cont++){
             if (cont == 0) 
                u = convertir(PORTA, cont);
             else if (cont == 1) 
                d = convertir(PORTA, cont);
             else
                c = convertir(PORTA, cont);
             
            } 
        display1 = display[c];
        display2 = display[d];
        display3 = display[u];
    }
        
    
}
void setup(void){
    ANSEL = 0;
    ANSELH = 0;               // I/O digitales
    
    OSCCONbits.IRCF = 0b0100; // 4MHz
    OSCCONbits.SCS = 1;       // Oscilador interno
    
    
    TRISA = 0;     // RA como salida
    TRISC = 0;     // RC como salida
    TRISDbits.TRISD0 = 0;
    TRISDbits.TRISD1 = 0;
    TRISDbits.TRISD2 = 0;
    PORTC = 0b10111111;
    
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB1 = 1;
    OPTION_REGbits.nRBPU = 0;
    WPUBbits.WPUB0 = 1;
    WPUBbits.WPUB1 = 1;
    
    IOCBbits.IOCB0 = 1;
    IOCBbits.IOCB1 = 1;
    PORTB = 0;
    INTCONbits.RBIF = 0;
    INTCONbits.RBIE = 1;
    INTCONbits.GIE = 1;
    
    
    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS2 = 1;
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS0 = 0;
    
    TMR0 = 252;
    INTCONbits.T0IF = 0;
    INTCONbits.T0IE = 1;
    
    

    return;
}
#define _XTAL_FREQ 1000000
void main(void) {
    PORTA = 0;
    PORTB = 0;
    PORTC = 0b10111111;
    PORTD = 0;
    setup();
    while(1)
    {
    }
   return;
}

int convertir(int a, int i){ //prototipo de función
   uint8_t centenas;
   uint8_t decenas;
   uint8_t unidades;
   centenas = a/100;
   decenas = (a -(centenas*100))/10;
   unidades = ((a -(centenas*100)) -(decenas*10));
   int valores[3] = {unidades, decenas, centenas};
   int j =+ valores[i];
   return j;
}



