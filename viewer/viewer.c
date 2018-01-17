#include <stdint.h>
#include <ncurses.h>
#include "viewer.h"

int8_t GetRegOffset(uint8_t RegAddr)
{
    for(uint8_t i = 0; i < sizeof(Regs); i++) {
        if(Regs[i] == RegAddr) {
            return i;
        } else {
            return -1;
        }
    }
}

void PrintReg(uint8_t RegOffset)
{
    attron(A_BOLD);
    printw("%20s", RegNames[RegOffset]);
    attroff(A_BOLD);
    /* Reg bits */
    uint8_t bit;
    for(uint8_t i = 16; i > 0; i--) {
        if(ExampleData[0] & (1 << (i-1))) {
            bit = 1;
        } else {
            bit = 0;
        }
        printw("%4i", bit);
    }
    /* Reg Value*/
    printw("  0x%04X", ExampleData[RegOffset]);
}


void Redraw()
{
    clear();
    move(0,0);
    addstr("BQ24780s Register Monitor\n");
    /* Reg header */
    attron(A_BOLD);
    printw("%20s", "REGS/BITS");
    for(uint8_t i = 16; i > 0; i--) {
        printw("%4i", i-1);
    }
    printw("%5s", "HEX");
    /* Regs */
    for(uint8_t i = 0; i < sizeof(Regs); i++) {
        move(i+2, 0);
        PrintReg(i);
    }
    refresh();
}


int main()
{
    uint8_t ch;
    initscr(); 
    Redraw();
    while(1) {
        ch = getch();
        if (ch == 'q') {
            break;
        } else if (ch == 'r') {
            Redraw();
        } else {
            //do nothing
        }
    }
    endwin();
    return 0;
}
