#include <stdint.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include "viewer.h"
#include "serial.h"


uint8_t SerialPort[]="/dev/ttyUSB0";
uint8_t CmdDump[]="\r";
#define LINE_BUFFER_SIZE 16
uint8_t LineBuffer[LINE_BUFFER_SIZE];

int8_t GetRegOffset(uint8_t RegAddr)
{
    for(uint8_t i = 0; i <= NUM_OF_REGS; i++)
    {
        //printf("=%d; Regs[i]=%d == RegAddr=%d ?\r\n", (int) i, (int) Regs[i], (int) RegAddr);
        if(Regs[i] == RegAddr) {
            return i;
        }
    }
    return -1;
}

void PrintReg(uint8_t RegOffset)
{
    attron(A_BOLD);
    printw("%16s", RegNames[RegOffset]);
    attroff(A_BOLD);
    /* Reg bits */
    uint8_t bit;
    for(int8_t i = 15; i >= 0; i--) {
        if(CurrData[0] & (1 << i)) {
            bit = 1;
        } else {
            bit = 0;
        }
        printw("%4i", bit);
    }
    /* Reg Value*/
    printw("  0x%04X", CurrData[RegOffset]);
}


void Redraw()
{
    clear();
    move(0,32);
    attrset(COLOR_PAIR(1));
    addstr("BQ24780s Register Monitor\n");
    /* Reg header */
    attron(A_BOLD);
    attron(COLOR_PAIR(3));
    printw("%16s", "REGS/BITS");
    for(int8_t i = 15; i >= 0; i--) {
        printw("%4i", i);
    }
    printw("%8s", "HEX");
    /* Regs */
    for(uint8_t i = 0; i < sizeof(Regs); i++) {
        move(i+2, 0);
        if(i%2) {
            attrset(COLOR_PAIR(1));
        } else {
            attrset(COLOR_PAIR(2));
        }
        PrintReg(i);
    }
    refresh();
}

void ReadBQ()
{
    if (open_port(SerialPort) == 0)
    {
        write_port(CmdDump);
        while(read_port(LineBuffer, LINE_BUFFER_SIZE) > 0)
        {
            //printf("> %s", LineBuffer);
            if(LineBuffer[0] == '0')
            {    
                //Line format: 0xAA=0xBBBB
                //             0123456789ab
                //terminate str parts
                LineBuffer[4]  = '\0';
                LineBuffer[11] = '\0';
                uint8_t RegNumber = (uint8_t) strtol(&LineBuffer[0], NULL, 0);
                uint8_t RegOffset = GetRegOffset(RegNumber);
                uint16_t RegValue = (uint16_t) strtol(&LineBuffer[5], NULL, 0);
                CurrData[RegOffset] = RegValue;
                //printf("CurrData[%d/%d] = %d\r\n", (int) RegNumber, (int) RegOffset, (int) RegValue);
            }
        }
        close_port();
    }
}


int main()
{
    uint8_t ch;
    initscr();
    start_color();
    //curs_set(0);          //don't need it
    //use_default_colors(); //its even worse! 

    init_pair(1, COLOR_WHITE, COLOR_BLACK);     //1 - odd lines
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);    //2 - even lines
    init_pair(3, COLOR_WHITE, COLOR_RED);       //3 - header

    //ReadBQ(); //temporarily zero it to see how read goes
    memset(CurrData, 0, NUM_OF_REGS * sizeof(uint16_t));
    
    Redraw();
    while(1) {
        ch = getch();
        //TODO: switch-case
        if (ch == 'q') {
            break;
        } else if (ch == 'r') {
            ReadBQ();
            Redraw();
        } else if (ch == 'w'){
            //TODO: write to port
        } else {
            //do nothing
        }
    }
    endwin();
    return 0;
}
