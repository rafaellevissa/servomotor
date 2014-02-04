/* 
 * File:   servo.c
 * Author: Anderson
 *
 * Created on 30 de Janeiro de 2014, 22:42
 */

#include <stdio.h>
#include <stdlib.h>

// PIC16F628A Configuration Bit Settings

#include <xc.h>

// CONFIG
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = OFF      // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is digital input, MCLR internally tied to VDD)
#pragma config BOREN = ON       // Brown-out Detect Enable bit (BOD enabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (RB4/PGM pin has digital I/O function, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Data memory code protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

unsigned long valor;

void uart_rx (void);
void delay_us (unsigned long tempo);
void pwm (unsigned long duty);
void read_rx (void);

int main(void)
{
    valor = 1500;
    uart_rx ();
    while(1)
    {
        pwm(valor);
        read_rx();
    }
    return (EXIT_SUCCESS);
}

void delay_us (unsigned long tempo)
{   
   TMR1 = 65536 - tempo;
   PIR1bits.TMR1IF = 0;
   T1CON = 0b00000001;
   while(!PIR1bits.TMR1IF);
}

void pwm (unsigned long duty)
{
   TRISBbits.TRISB3 = 0;
    while(1)
    {
        PORTBbits.RB3 = 1;
        delay_us(duty);
        PORTBbits.RB3 = 0;
        delay_us(30000 - duty);

        if(PIR1bits.RCIF) break;
    }
}

void read_rx (void)
{
    PIR1bits.RCIF = 0;
    if(RCREG == 77)
    {
        if(valor > 500)
            valor = valor - 100;
    }
     if(RCREG == 75)
    {
        if(valor < 2400)
            valor = valor + 100;
    }
}

void uart_rx (void)
{
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 1;
    SPBRG = 25;
    TXSTAbits.TX9 = 0;
    RCSTAbits.RX9 = 0;
    RCSTAbits.SPEN = 1;
    RCSTAbits.CREN = 1;
}
