#ifndef _KB_SATURN_H_
#define _KB_SATURN_H_

#include <genesis.h>

// ============================================================
// KB_Saturn - Driver clavier Saturn (DB9) pour SGDK
// Protocole Saturn 12-nibbles (TH/TR/TL), portage JOY2RP2040
// ============================================================

// Selection du port physique (comme PORT_1/PORT_2 de joy.h)
#define KB_SATURN_PORT_1    0
#define KB_SATURN_PORT_2    1

// Etat Make/Break renvoye par le callback et par isKeyPressed
#define KB_KEY_UP           0
#define KB_KEY_DOWN         1

// Statut detaille d'un poll
#define KB_POLL_COMM_ERROR   0   // timeout / pas de reponse du clavier
#define KB_POLL_NO_EVENT     1   // clavier a repondu, mais rien de nouveau
#define KB_POLL_KEY_EVENT    2   // clavier a repondu avec un evenement touche

// Renvoie le statut du dernier poll (voir KB_POLL_*)
u8 KB_Saturn_getLastPollStatus(void);

// Callback appele a chaque evenement clavier (appui ou relachement)
// scancode : code Saturn natif (voir KB_Saturn_scancodeToName)
// state    : KB_KEY_DOWN ou KB_KEY_UP
typedef void KBSaturnEventCallback(u8 scancode, u8 state);


// ---- Cycle de vie ----

// Initialise le port et detecte la presence d'un clavier Saturn.
// Renvoie TRUE si un clavier a ete detecte, FALSE sinon.
bool KB_Saturn_Init(u16 port);

// Phase ID Saturn/MD generique (TH Control Method).
// Utile en diagnostic pour confirmer qu'un peripherique 0x05 (clavier)
// est bien detecte avant d'appeler KB_Saturn_Init.
u8 KB_Saturn_getDeviceID(u16 port);

// Renvoie TRUE si un clavier a ete detecte lors du dernier Init.
bool KB_Saturn_isKeyboardPresent(void);

// A appeler une fois par frame (avant ou apres SYS_doVBlankProcess, au choix).
// Effectue un poll protocole et met a jour l'etat interne + declenche le callback.
void KB_Saturn_Update(void);


// ---- Lecture d'etat ----

// Renvoie TRUE si la touche (scancode Saturn) est actuellement maintenue.
bool KB_Saturn_isKeyPressed(u8 scancode);

// Raccourcis pratiques
bool KB_Saturn_isShiftHeld(void);
bool KB_Saturn_isCtrlHeld(void);
bool KB_Saturn_isAltHeld(void);


// ---- Callback ----

void KB_Saturn_setEventHandler(KBSaturnEventCallback *CB);


// ---- Utilitaires ----

// Nom lisible de la touche (debug/UI), ex: "A", "F1", "SPACE"
const char* KB_Saturn_scancodeToName(u8 scancode);

// Caractere ASCII imprimable correspondant (0 si non imprimable)
// shift : TRUE pour la version majuscule/shiftee
char KB_Saturn_scancodeToAscii(u8 scancode, bool shift);


// ---- Debug / diagnostic ----
// Toujours disponibles (cout memoire negligeable), utiles pour
// diagnostiquer un souci de protocole sans avoir a modifier la lib.

// Copie les 12 nibbles bruts du dernier poll (reussi ou non) dans out12.
void KB_Saturn_getLastRawNibbles(u8 *out12);

// Renvoie TRUE si le dernier poll a produit un paquet valide.
bool KB_Saturn_getLastPollOk(void);

#endif // _KB_SATURN_H_