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
#define ROW_MIN 2
#define ROW_MAX 14
#define COL_MIN 19
#define COL_MAX 79
#define V_STEP 1
#define H_STEP 4
uint8_t crow = ROW_MIN;
uint8_t ccol = COL_MIN;
uint8_t ireg, ibit;

#define DBG_ROW 16
#define DBG_COL 70

void DbgPrn(uint8_t * DbgStr)
{
    uint8_t prow, pcol;
    getyx(stdscr, prow, pcol);
    move(DBG_ROW, DBG_COL);
    addstr(DbgStr);
    move(prow, pcol);
}

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
        if(CurrData[RegOffset] & (1 << i)) {
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
    addstr("BQ24780s Register Monitor");
    move(1,0);
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
    move(16,0);
    addstr("arrows - move, space - invert bit, r - read, w - write, q - quit");
    move(crow, ccol);
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

void FlipBit()
{
    uint16_t cbit;
    ireg = crow - ROW_MIN;
    ibit = 15 - (ccol - COL_MIN) / 4;
    cbit = CurrData[ireg] & (1 << ibit);

    if(cbit == 0)
    {
        CurrData[ireg] |= (1 << ibit);
    } else {
        CurrData[ireg] &= ~(1 << ibit);
    }
    Redraw();
}


int main()
{
    uint16_t ch;
    initscr();
    start_color();
    //curs_set(0);          //don't need it
    //use_default_colors(); //its even worse!

    init_pair(1, COLOR_WHITE, COLOR_BLACK);     //1 - odd lines
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);    //2 - even lines
    init_pair(3, COLOR_WHITE, COLOR_RED);       //3 - header

    ReadBQ();
    Redraw();
    keypad(stdscr,TRUE);
    noecho();
    do {
        ch = getch();
        switch(ch)
        {
            case 'r':
                ReadBQ();
                Redraw();
                break;
            case 'w':
                printf("Write!\r\n");
                break;
            case ' ':
                FlipBit();
                break;
            case KEY_LEFT:
                if(ccol > COL_MIN) ccol -= H_STEP;
                break;
            case KEY_RIGHT:
                if(ccol < COL_MAX) ccol += H_STEP;
                break;
            case KEY_UP:
                if(crow > ROW_MIN) crow -= V_STEP;
                break;
            case KEY_DOWN:
                if(crow < ROW_MAX) crow += V_STEP;
                break;
        }
        move(crow, ccol);
    } while (ch != 'q');
    endwin();
    return 0;
}
