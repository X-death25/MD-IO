/*
 * KB Saturn Demo - JOY2RP2040
 * Demontre l'utilisation de la lib KB_Saturn (init/update/callback)
 */

#define KB_SATURN_DEBUG   // active les getters de debug (nibbles bruts)

#include "genesis.h"
#include "KB_Saturn.h"

// globals
static u8 cursorX = 0;
static u8 cursorY = 10;
#define TEXT_AREA_Y_START 10
#define TEXT_AREA_Y_END   27
#define TEXT_AREA_WIDTH   40

static char buf_status[40];

static void keyboardEvent(u8 scancode, u8 state);
static void handleTextInput(u8 scancode, u8 state);
static void displayDebugInfo(void);
static char buf_status[40];
static char buf_nibbles[40];   

int main()
{
    SYS_disableInts();
    VDP_init();
    VDP_setTextPalette(PAL0);

    VDP_drawText("KB Saturn demo - JOY2RP2040", 2, 1);
	
	u8 devID = KB_Saturn_getDeviceID(KB_SATURN_PORT_1);
	char buf_id[40];
	sprintf(buf_id, "Device ID: 0x%02X", devID);
	VDP_drawText(buf_id, 2, 3);

	bool present = KB_Saturn_Init(KB_SATURN_PORT_1);
	VDP_drawText(present ? "Keyboard: OK        " : "Keyboard: NOT FOUND ", 2, 4);

    KB_Saturn_setEventHandler(keyboardEvent);

    VDP_drawText("--- Zone de saisie texte ci-dessous ---", 2, 9);

    SYS_enableInts();

    while (TRUE)
    {
        KB_Saturn_Update();
        displayDebugInfo();

        SYS_doVBlankProcess();
    }

    return 0;
}

static void keyboardEvent(u8 scancode, u8 state)
{
    handleTextInput(scancode, state);
}

static void handleTextInput(u8 scancode, u8 state)
{
    // On n'agit que sur les appuis (Make), pas les relachements (Break)
    if (state != KB_KEY_DOWN) return;

    bool shift = KB_Saturn_isShiftHeld();

    if (scancode == 0x5A) // Enter
    {
        cursorX = 0;
        cursorY++;
        if (cursorY > TEXT_AREA_Y_END)
        {
            VDP_clearText(0, TEXT_AREA_Y_START, TEXT_AREA_WIDTH);
            cursorY = TEXT_AREA_Y_START;
        }
        return;
    }

    if (scancode == 0x66) // Backspace
    {
        if (cursorX > 0)
        {
            cursorX--;
            VDP_drawText(" ", cursorX + 2, cursorY);
        }
        return;
    }

    char c = KB_Saturn_scancodeToAscii(scancode, shift);
    if (c != 0)
    {
        char s[2] = { c, '\0' };
        VDP_drawText(s, cursorX + 2, cursorY);
        cursorX++;

        if (cursorX >= TEXT_AREA_WIDTH)
        {
            cursorX = 0;
            cursorY++;
            if (cursorY > TEXT_AREA_Y_END)
            {
                VDP_clearText(0, TEXT_AREA_Y_START, TEXT_AREA_WIDTH);
                cursorY = TEXT_AREA_Y_START;
            }
        }
    }
}

static void displayDebugInfo(void)
{
    u8 nibbles[12];
    u8 i;
    u8 status = KB_Saturn_getLastPollStatus();

    KB_Saturn_getLastRawNibbles(nibbles);
    for (i = 0; i < 12; i++) sprintf(buf_nibbles + i*2, "%X ", nibbles[i]);
    buf_nibbles[24] = '\0';
    VDP_drawText(buf_nibbles, 2, 5);

    switch (status)
    {
        case KB_POLL_COMM_ERROR: sprintf(buf_status, "POLL: COMM ERROR   "); break;
        case KB_POLL_NO_EVENT:   sprintf(buf_status, "POLL: OK (no key)  "); break;
        case KB_POLL_KEY_EVENT:  sprintf(buf_status, "POLL: OK (key evt) "); break;
    }
    VDP_drawText(buf_status, 2, 7);
}