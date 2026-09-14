/*
 * MegaMouse Test
 *
 */
#include "genesis.h"
#include "sprite.h"

Sprite* sprites[1];

s16 mouseX = 16;
s16 mouseY = 16;

#define MOUSE_SPEED 2
#define NUM_COLORS  8

s16 lastMouseX = 0;
s16 lastMouseY = 0;
bool mouseDetected = FALSE;

u8 portType;
bool mouseModeSet = FALSE;

u16 lastButtons = 0;
u8 colorIndex = 0;

// Palette de test : quelques couleurs bien distinctes pour voir le changement
const u16 cursorColors[NUM_COLORS] =
{
    0x0FFF, // blanc
    0x000F, // rouge (BGR)
    0x00F0, // vert
    0x0F00, // bleu
    0x00FF, // jaune
    0x0FF0, // cyan
    0x0F0F, // magenta
    0x0888  // gris
};

void handleMouseInput(void)
{
    u16 value;

    portType = JOY_getPortType(PORT_1);

    if (portType == PORT_TYPE_MOUSE)
    {
        if (!mouseModeSet)
        {
            JOY_setSupport(PORT_1, JOY_SUPPORT_MOUSE);
            mouseModeSet = TRUE;
        }

        s16 curX = JOY_readJoypadX(JOY_1);
        s16 curY = JOY_readJoypadY(JOY_1);

        if (mouseDetected)
        {
            s16 deltaX = curX - lastMouseX;
            s16 deltaY = curY - lastMouseY;

            mouseX += deltaX;
            mouseY += deltaY;
        }

        lastMouseX = curX;
        lastMouseY = curY;
        mouseDetected = TRUE;

        value = JOY_readJoypad(JOY_1);

        // Détection de front (bouton vient d'être pressé, pas juste maintenu)
        u16 pressed = value & ~lastButtons;

        // Mapping standard SGDK souris : BUTTON_B = clic gauche, BUTTON_C = clic droit
		if (pressed & BUTTON_C)
		{
			colorIndex = (colorIndex + 1) % NUM_COLORS;
			PAL_setColor(50, cursorColors[colorIndex]); // 3e couleur de PAL3 (48 + 2)
		}
		if (pressed & BUTTON_A)
		{
			colorIndex = (colorIndex == 0) ? (NUM_COLORS - 1) : (colorIndex - 1);
			PAL_setColor(50, cursorColors[colorIndex]); // idem
		}

        lastButtons = value;
    }
    else
    {
        mouseModeSet = FALSE;
        mouseDetected = FALSE;
        lastButtons = 0;

        value = JOY_readJoypad(JOY_1);
        if (value & BUTTON_UP)    mouseY -= MOUSE_SPEED;
        if (value & BUTTON_DOWN)  mouseY += MOUSE_SPEED;
        if (value & BUTTON_LEFT)  mouseX -= MOUSE_SPEED;
        if (value & BUTTON_RIGHT) mouseX += MOUSE_SPEED;
    }

    if (mouseX < 0) mouseX = 0;
    if (mouseY < 0) mouseY = 0;
    if (mouseX > 320 - 16) mouseX = 320 - 16;
    if (mouseY > 224 - 16) mouseY = 224 - 16;

    SPR_setPosition(sprites[0], mouseX, mouseY);
}

int main()
{
    SYS_disableInts();
    VDP_setScreenWidth320();

    SPR_init();

    // Fond noir : on ne charge aucun tileset/tilemap, juste la palette du curseur
    PAL_setPalette(PAL3, mouse_sprite.palette->data, DMA);
    PAL_setColor(0, 0x0000); // fond noir (couleur 0 du BG)

    sprites[0] = SPR_addSprite(&mouse_sprite, mouseX, mouseY, TILE_ATTR(PAL3, TRUE, FALSE, FALSE));

    JOY_init();

    SYS_showFrameLoad(TRUE);
    SYS_enableInts();

    while (1)
    {
        handleMouseInput();
        SPR_update();
        SYS_doVBlankProcess();
    }
}