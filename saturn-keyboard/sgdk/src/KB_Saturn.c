#include "KB_Saturn.h"

// ============================================================
// Bas niveau : bits du registre data port controleur MD
// ============================================================
#define TH_BIT      0x40   // pin 7
#define TR_BIT      0x20   // pin 9
#define TL_BIT      0x10   // pin 6 - bit d'ACK protocole Saturn
#define DATA_MASK   0x0F   // D0-D3

#define POLL_NB_NIBBLES 12
#define POLL_TIMEOUT    128

// ============================================================
// Bitmap 256 bits (etat maintenu par scancode 0x00-0xFF)
// ============================================================
static u8 keyState[32];

#define KB_BIT_SET(n)   (keyState[(n) >> 3] |=  (1 << ((n) & 7)))
#define KB_BIT_CLR(n)   (keyState[(n) >> 3] &= ~(1 << ((n) & 7)))
#define KB_BIT_TEST(n)  (keyState[(n) >> 3] &   (1 << ((n) & 7)))

// ============================================================
// Etat interne du driver
// ============================================================
static vu8 *portData;
static vu8 *portCtrl;
static bool keyboardPresent = FALSE;

static bool lShiftHeld = FALSE;
static bool rShiftHeld = FALSE;
static bool lCtrlHeld  = FALSE;
static bool rCtrlHeld  = FALSE;
static bool lAltHeld   = FALSE;
static bool rAltHeld   = FALSE;

static KBSaturnEventCallback *eventCB = NULL;

// Debug / diagnostic (toujours compile, cout negligeable)
static u8 lastRawNibbles[POLL_NB_NIBBLES];
static bool lastPollOk = FALSE;


// ============================================================
// Acces bas niveau port
// ============================================================
static inline void setCtrl(u8 mask) { *portCtrl = mask; }
static inline void setData(u8 val)  { *portData = val; }
static inline u8   getData(void)    { return *portData; }


// ============================================================
// Poll bas niveau (protocole 12-nibbles)
// ============================================================
static u8 lastPollStatus = KB_POLL_COMM_ERROR;

static bool KB_Saturn_poll(u8 *outMakeBreak, u8 *outScancode)
{
    u16 timeout = 0;
    u8 nibble[POLL_NB_NIBBLES] = {0};
    u8 i;

    for (i = 0; i < POLL_NB_NIBBLES; i += 2)
    {
        setData(0);
        while ((getData() & TL_BIT) != 0)
        {
            if (timeout++ >= POLL_TIMEOUT)
            {
                setData(TH_BIT | TR_BIT);
                lastPollStatus = KB_POLL_COMM_ERROR;
                return FALSE;
            }
        }
        timeout = 0;
        nibble[i] = getData() & DATA_MASK;

        setData(TR_BIT);
        while ((getData() & TL_BIT) != TL_BIT)
        {
            if (timeout++ >= POLL_TIMEOUT)
            {
                setData(TH_BIT | TR_BIT);
                lastPollStatus = KB_POLL_COMM_ERROR;
                return FALSE;
            }
        }
        timeout = 0;
        nibble[i + 1] = getData() & DATA_MASK;
    }

    setData(TH_BIT | TR_BIT);

    for (i = 0; i < POLL_NB_NIBBLES; i++) lastRawNibbles[i] = nibble[i];

    // Signature clavier invalide = vraie erreur de communication
    if ((nibble[0] != 3) || (nibble[1] != 4))
    {
        lastPollStatus = KB_POLL_COMM_ERROR;
        return FALSE;
    }

    // Signature OK mais pas de Make/Break = transaction reussie, rien a rapporter
    if ((nibble[7] & 9) == 0)
    {
        lastPollStatus = KB_POLL_NO_EVENT;
        return FALSE; // pas d'evenement, mais ce n'est plus une erreur
    }

    *outMakeBreak = nibble[7] & 1;
    *outScancode  = (nibble[8] << 4) | nibble[9];

    lastPollStatus = KB_POLL_KEY_EVENT;
    return TRUE;
}

u8 KB_Saturn_getLastPollStatus(void)
{
    return lastPollStatus;
}

u8 KB_Saturn_getDeviceID(u16 port)
{
    vu8 *pb = (vu8*)(0xA10003 + (port * 2));
    vu8 *pc = (vu8*)(0xA10009 + (port * 2));
    u8 a, id;

    *pc = TH_BIT;
    *pb = TH_BIT;
    VDP_waitVSync();
    VDP_waitVSync();

    a = *pb;
    *pb = 0x00;
    id  = ((a & 8) | (a & 4)) ? 8 : 0;
    id |= ((a & 2) | (a & 1)) ? 4 : 0;

    a = *pb;
    *pb = TH_BIT;
    id |= ((a & 8) | (a & 4)) ? 2 : 0;
    id |= ((a & 2) | (a & 1)) ? 1 : 0;

    return id;
}


// ============================================================
// Cycle de vie
// ============================================================
bool KB_Saturn_Init(u16 port)
{
    u16 i;

    portData = (vu8*)(0xA10003 + (port * 2));
    portCtrl = (vu8*)(0xA10009 + (port * 2));

    for (i = 0; i < 32; i++) keyState[i] = 0;
    lShiftHeld = rShiftHeld = FALSE;
    lCtrlHeld  = rCtrlHeld  = FALSE;
    lAltHeld   = rAltHeld   = FALSE;

    setCtrl(TH_BIT | TR_BIT);       // TH+TR sortie, TL+D0-3 entree
    setData(TH_BIT | TR_BIT);
    setData(TR_BIT);                 // query "es-tu un clavier ?"

    if ((getData() & DATA_MASK) != 1)
    {
        setData(TH_BIT | TR_BIT);
        keyboardPresent = FALSE;
        return FALSE;
    }

    setData(TH_BIT | TR_BIT);
    keyboardPresent = TRUE;
    return TRUE;
}

bool KB_Saturn_isKeyboardPresent(void)
{
    return keyboardPresent;
}

void KB_Saturn_Update(void)
{
    u8 mb, sc;

    if (!keyboardPresent) return;

    bool ok = KB_Saturn_poll(&mb, &sc);
    lastPollOk = ok;

    if (!ok) return;

    // Suivi des modifiers
    switch (sc)
    {
        case 0x12: lShiftHeld = (mb == KB_KEY_DOWN); break;
        case 0x59: rShiftHeld = (mb == KB_KEY_DOWN); break;
        case 0x14: lCtrlHeld  = (mb == KB_KEY_DOWN); break;
        case 0x18: rCtrlHeld  = (mb == KB_KEY_DOWN); break;
        case 0x11: lAltHeld   = (mb == KB_KEY_DOWN); break;
        case 0x17: rAltHeld   = (mb == KB_KEY_DOWN); break;
        default: break;
    }

    if (mb == KB_KEY_DOWN) KB_BIT_SET(sc);
    else KB_BIT_CLR(sc);

    if (eventCB) eventCB(sc, mb);
}


// ============================================================
// Lecture d'etat
// ============================================================
bool KB_Saturn_isKeyPressed(u8 scancode)
{
    return KB_BIT_TEST(scancode) != 0;
}

bool KB_Saturn_isShiftHeld(void) { return lShiftHeld || rShiftHeld; }
bool KB_Saturn_isCtrlHeld(void)  { return lCtrlHeld  || rCtrlHeld;  }
bool KB_Saturn_isAltHeld(void)   { return lAltHeld   || rAltHeld;   }


// ============================================================
// Callback
// ============================================================
void KB_Saturn_setEventHandler(KBSaturnEventCallback *CB)
{
    eventCB = CB;
}


// ============================================================
// Table scancode Saturn (AT Set 2) -> nom lisible
// ============================================================
const char* KB_Saturn_scancodeToName(u8 code)
{
    switch (code)
    {
        case 0x01: return "F9";
        case 0x03: return "F5";
        case 0x04: return "F3";
        case 0x05: return "F1";
        case 0x06: return "F2";
        case 0x07: return "F12";
        case 0x09: return "F10";
        case 0x0A: return "F8";
        case 0x0B: return "F6";
        case 0x0C: return "F4";
        case 0x0D: return "TAB";
        case 0x0E: return "` ~";

        case 0x11: return "L-ALT";
        case 0x12: return "L-SHIFT";
        case 0x14: return "L-CTRL";
        case 0x15: return "Q";
        case 0x16: return "1";
        case 0x17: return "R-ALT";
        case 0x18: return "R-CTRL";
        case 0x19: return "NUM-ENTER";
        case 0x1A: return "Z";
        case 0x1B: return "S";
        case 0x1C: return "A";
        case 0x1D: return "W";
        case 0x1E: return "2";
        case 0x1F: return "L-GUI";

        case 0x21: return "C";
        case 0x22: return "X";
        case 0x23: return "D";
        case 0x24: return "E";
        case 0x25: return "4";
        case 0x26: return "3";
        case 0x27: return "R-GUI";
        case 0x29: return "SPACE";
        case 0x2A: return "V";
        case 0x2B: return "F";
        case 0x2C: return "T";
        case 0x2D: return "R";
        case 0x2E: return "5";
        case 0x2F: return "MENU";

        case 0x31: return "N";
        case 0x32: return "B";
        case 0x33: return "H";
        case 0x34: return "G";
        case 0x35: return "Y";
        case 0x36: return "6";
        case 0x3A: return "M";
        case 0x3B: return "J";
        case 0x3C: return "U";
        case 0x3D: return "7";
        case 0x3E: return "8";

        case 0x41: return ", <";
        case 0x42: return "K";
        case 0x43: return "I";
        case 0x44: return "O";
        case 0x45: return "0";
        case 0x46: return "9";
        case 0x49: return ". >";
        case 0x4A: return "/ ?";
        case 0x4B: return "L";
        case 0x4C: return "; :";
        case 0x4D: return "P";
        case 0x4E: return "- _";

        case 0x51: return "' \"";
        case 0x54: return "[ {";
        case 0x55: return "= +";
        case 0x58: return "CAPSLOCK";
        case 0x59: return "R-SHIFT";
        case 0x5A: return "ENTER";
        case 0x5B: return "] }";
        case 0x5D: return "\\ |";

        case 0x66: return "BACKSPACE";
        case 0x69: return "NUM-1";
        case 0x6B: return "NUM-4";
        case 0x6C: return "NUM-7";

        case 0x70: return "NUM-0";
        case 0x71: return "NUM-.";
        case 0x72: return "NUM-2";
        case 0x73: return "NUM-5";
        case 0x74: return "NUM-6";
        case 0x75: return "NUM-8";
        case 0x76: return "ESC";
        case 0x77: return "NUMLOCK";
        case 0x78: return "F11";
        case 0x79: return "NUM-+";
        case 0x7A: return "NUM-3";
        case 0x7B: return "NUM--";
        case 0x7C: return "NUM-*";
        case 0x7D: return "NUM-9";
        case 0x7E: return "SCROLLLOCK";

        case 0x80: return "NUM-/";
        case 0x81: return "INSERT";
        case 0x82: return "PAUSE";
        case 0x83: return "F7";
        case 0x84: return "PRINTSCR";
        case 0x85: return "DELETE";
        case 0x86: return "LEFT";
        case 0x87: return "HOME";
        case 0x88: return "END";
        case 0x89: return "UP";
        case 0x8A: return "DOWN";
        case 0x8B: return "PAGEUP";
        case 0x8C: return "PAGEDOWN";
        case 0x8D: return "RIGHT";

        default: return "?";
    }
}

char KB_Saturn_scancodeToAscii(u8 code, bool shift)
{
    switch (code)
    {
        case 0x1C: return shift ? 'A' : 'a';
        case 0x32: return shift ? 'B' : 'b';
        case 0x21: return shift ? 'C' : 'c';
        case 0x23: return shift ? 'D' : 'd';
        case 0x24: return shift ? 'E' : 'e';
        case 0x2B: return shift ? 'F' : 'f';
        case 0x34: return shift ? 'G' : 'g';
        case 0x33: return shift ? 'H' : 'h';
        case 0x43: return shift ? 'I' : 'i';
        case 0x3B: return shift ? 'J' : 'j';
        case 0x42: return shift ? 'K' : 'k';
        case 0x4B: return shift ? 'L' : 'l';
        case 0x3A: return shift ? 'M' : 'm';
        case 0x31: return shift ? 'N' : 'n';
        case 0x44: return shift ? 'O' : 'o';
        case 0x4D: return shift ? 'P' : 'p';
        case 0x15: return shift ? 'Q' : 'q';
        case 0x2D: return shift ? 'R' : 'r';
        case 0x1B: return shift ? 'S' : 's';
        case 0x2C: return shift ? 'T' : 't';
        case 0x3C: return shift ? 'U' : 'u';
        case 0x2A: return shift ? 'V' : 'v';
        case 0x1D: return shift ? 'W' : 'w';
        case 0x22: return shift ? 'X' : 'x';
        case 0x35: return shift ? 'Y' : 'y';
        case 0x1A: return shift ? 'Z' : 'z';

        case 0x16: return '1';
        case 0x1E: return '2';
        case 0x26: return '3';
        case 0x25: return '4';
        case 0x2E: return '5';
        case 0x36: return '6';
        case 0x3D: return '7';
        case 0x3E: return '8';
        case 0x46: return '9';
        case 0x45: return '0';

        case 0x41: return shift ? '<' : ',';
        case 0x49: return shift ? '>' : '.';
        case 0x4A: return shift ? '?' : '/';
        case 0x4E: return shift ? '_' : '-';
        case 0x55: return shift ? '+' : '=';

        case 0x29: return ' ';

        default: return 0;
    }
}


// ============================================================
// Debug / diagnostic (toujours compile)
// ============================================================
void KB_Saturn_getLastRawNibbles(u8 *out12)
{
    u8 i;
    for (i = 0; i < POLL_NB_NIBBLES; i++) out12[i] = lastRawNibbles[i];
}

bool KB_Saturn_getLastPollOk(void)
{
    return lastPollOk;
}