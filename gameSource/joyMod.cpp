#include "minorGems/graphics/openGL/JoyHandlerGL.h"

// @TODO: can't pick up small curved shaft.
// @TODO: can't pick up froe.
// @TODO: can't pick up laso.
// @TODO: can't pick up "bow without arrow"

// @TODO: make these static members in the .h file.
static int joyAxisDownFrames = 0;
static int joyAxisDownDir = 0;
static bool alternateClick = false;
static bool alternateOtherClick = false;
static int thisJoyButtonIsDown = 99; // 99 is no button down.
static bool ignoreNextButtonUp = false;
static bool ignoreNextDPadUp = false;

//@TODO: bugs with movement/button pressing when babys are around.

//@TODO: twitching.

void LivingLifePage::joyDPadDown(int dir) {

    LiveObject *ourLiveObject = getOurLiveObject();
    int x = (int) (ourLiveObject->currentPos.x * CELL_D); //currentPos is the world grid location of player.
    int y = (int) (ourLiveObject->currentPos.y * CELL_D);
    //fprintf(stderr, "dpad down - player pos: %d, %d\n", x, y);

    if (thisJoyButtonIsDown != 99) {
        fprintf(stderr, "next square click with button: %d\n", thisJoyButtonIsDown);
        ignoreNextButtonUp = true;
        ignoreNextDPadUp = true;

        bool savedRight = isLastMouseButtonRight(); // remember last mouse button clicked.

        if (dir == JOY_N) { //up
            // clicking object north of player won't work because you will be clicking on players head instead.
            return; //y += CELL_D;
        } else if (dir == JOY_W) { //left
            x -= CELL_D;
        } else if (dir == JOY_S) { //down
            y -= CELL_D;
        } else if (dir == JOY_E) { //right
            x += CELL_D;
        } else {
            return;
        }

        if (thisJoyButtonIsDown == JOY_A) { //pickup
            setLastMouseButtonRight(false);
            if (alternateClick) {
                pointerDown(x, y - 31); // required for: rope
                pointerUp(x, y - 31);
            } else if (alternateOtherClick) {
                pointerDown(x, y + 10);
                pointerUp(x, y + 10);
            } else if (alternateClick && alternateOtherClick) {
                pointerDown(x + 25, y);
                pointerUp(x + 25, y);
            } else {
                pointerDown(x, y - 35); // tiny berry seed and thread.
                pointerUp(x, y - 35);
            }

        } else if (thisJoyButtonIsDown == JOY_Y) { //click on self.
            setLastMouseButtonRight(false);
            pointerDown(x, y + 90);
            pointerUp(x, y + 90);

        } else if (thisJoyButtonIsDown == JOY_B) { //drop
            setLastMouseButtonRight(true);
            if (alternateClick) {
                pointerDown(x + 32, y - 35);
                pointerUp(x + 32, y - 35);
            } else {
                pointerDown(x, y - 35);
                pointerUp(x, y - 35);
            }

        } else if (thisJoyButtonIsDown == JOY_X) { //backpack
            setLastMouseButtonRight(true);
            pointerDown(x - 35, y + 90);
            pointerUp(x - 35, y + 90);        
        }

        setLastMouseButtonRight(savedRight); // restore last mouse button clicked.
        return;
    }


    joyAxisDownDir = dir;
    joyAxisDownFrames = 19; //roughly the amount of frames it takes to walk one grid square.
}

void LivingLifePage::joyDPadUp() {

    if (ignoreNextDPadUp) {
        ignoreNextDPadUp = false;
        return;
    }

    // CELL_D is 128 for me.

    joyAxisDownDir = 0;  // always set to 0 so movement stops.
    joyAxisDownFrames = 0;

    //@TODO: implement input buffering.

    // retrieve character position:
    LiveObject *ourLiveObject = getOurLiveObject();
    // @TODO: make x = x * CELL_D as that is how i always use it.
    //int x = (int) ourLiveObject->currentPos.x; //currentPos is the world grid location.
    //int y = (int) ourLiveObject->currentPos.y;
    //fprintf(stderr, "dpad up - player pos: %d, %d\n", x * CELL_D, y * CELL_D);

    // button commands should only execute when character has stopped moving. (so fabricated click locations are accurate)
    if (playerActionPending || ourLiveObject->inMotion)
        return;


    bool savedRight = isLastMouseButtonRight(); // remember last mouse button clicked.

    // if (joyAxisDownFrames > 0) {
    //     joyAxisDownFrames = 0;
    //     return;
    // }

    // // when walking on road
    // mForceGroundClick = true;
    // pointerDown( nextStep.x * CELL_D, 
    //              nextStep.y * CELL_D );
    
    // pointerUp( nextStep.x * CELL_D, 
    //            nextStep.y * CELL_D );
    
    // mForceGroundClick = false;

    setLastMouseButtonRight(savedRight); // restore last mouse button clicked.
}

void LivingLifePage::joyButtonDown(int button) {
    // holding down multiple buttons simultaneously is not supported.
    thisJoyButtonIsDown = button;
}

void LivingLifePage::joyButtonUp(int button) {

    thisJoyButtonIsDown = 99;
    if (ignoreNextButtonUp) {
        ignoreNextButtonUp = false;
        return;
    }

    //@TODO: implement input buffering.

    // retrieve character position:
    LiveObject *ourLiveObject = getOurLiveObject();

    // store x,y pixel position of player:
    int x = (int) (ourLiveObject->currentPos.x * CELL_D); //currentPos is the world grid location of player.
    int y = (int) (ourLiveObject->currentPos.y * CELL_D);
    //fprintf(stderr, "button up - player pos: %d, %d (%d)\n", x, y, button);

    // button commands should only execute when character has stopped moving. (so fabricated click locations are accurate)
    if (playerActionPending || ourLiveObject->inMotion)
        return;

    bool savedRight = isLastMouseButtonRight(); // remember last mouse button clicked.

    // @TODO: these constants work for 720p but should be percentages of CELL_D instead to work for all reslutions.

    if (button == JOY_A) { //pickup
        setLastMouseButtonRight(false);
        if (alternateClick) {
            pointerDown(x, y - 31); // required for: rope.
            pointerUp(x, y - 31);
        } else if (alternateOtherClick) {
            pointerDown(x, y + 10);
            pointerUp(x, y + 10);
        } else {
            pointerDown(x, y - 35); // tiny berry seed and thread.
            pointerUp(x, y - 35);
        }

    } else if (button == JOY_Y) { // click on self.
        setLastMouseButtonRight(false);
        pointerDown(x, y + 90);
        pointerUp(x, y + 90);

    } else if (button == JOY_B) { // drop.
        setLastMouseButtonRight(true);
        if (alternateClick) {
            pointerDown(x + 32, y - 35);
            pointerUp(x + 32, y - 35);
        } else {
            pointerDown(x, y - 35);
            pointerUp(x, y - 35);
        }

    } else if (button == JOY_X) { // backpack.
        setLastMouseButtonRight(true);
        pointerDown(x - 35, y + 90);
        pointerUp(x - 35, y + 90);        
    
    }

    setLastMouseButtonRight(savedRight); // restore last mouse button clicked.
}


void LivingLifePage::joyRudder(int rudder, short pressure) {
    if (rudder == JOY_R_RUDDER)
        alternateClick = pressure > 0;
    if (rudder == JOY_L_RUDDER)
        alternateOtherClick = pressure > 0;
}

void LivingLifePage::joyThumbstick(int stick, short x, short y) {
    //fprintf(stderr, "stick #%d -> %d,%d\n", stick, x, y);
}





void LivingLifePage::joyStep(void) {

    
    if (joyAxisDownDir) {

        LiveObject *ourLiveObject = getOurLiveObject();
        if (ourLiveObject) {
        
            // @TODO: make x = x * CELL_D as that is how they are always used.
            int x = (int) ourLiveObject->currentPos.x; //currentPos is the world grid location.
            int y = (int) ourLiveObject->currentPos.y;
            int xx = (int) ourLiveObject->currentPos.x * CELL_D; //currentPos is the world grid location.
            int yy = (int) ourLiveObject->currentPos.y * CELL_D;
            
            joyAxisDownFrames++;

            if (playerActionPending || ourLiveObject->inMotion) {
                //fprintf(stderr, "[%d]", joyAxisDownFrames);
            }

            if (playerActionPending || ourLiveObject->inMotion || joyAxisDownFrames < 19) {
            } else {
                joyAxisDownFrames = 0;

                bool savedRight = isLastMouseButtonRight(); // remember last mouse button clicked.

                if (joyAxisDownDir == JOY_N) { //up
                    setLastMouseButtonRight(false);
                    pointerDown(x * CELL_D, y * CELL_D + CELL_D + 52);
                    pointerUp(x * CELL_D, y * CELL_D + CELL_D + 52);

                } else if (joyAxisDownDir == JOY_W) { //left
                    setLastMouseButtonRight(false);
                    if (alternateClick) {
                        pointerDown(x * CELL_D - CELL_D + 63, y * CELL_D + 35); // 63 is one less than half of grid size.
                        pointerUp(x * CELL_D - CELL_D + 63, y * CELL_D + 35); 
                    } else {
                        pointerDown(x * CELL_D - CELL_D, y * CELL_D + 52);
                        pointerUp(x * CELL_D - CELL_D, y * CELL_D + 52); 
                    }

                } else if (joyAxisDownDir == JOY_S) { //down
                    setLastMouseButtonRight(false);
                    pointerDown(x * CELL_D, y * CELL_D - CELL_D + 52); 
                    pointerUp(x * CELL_D, y * CELL_D - CELL_D + 52);  // has to be +52 so can walk over small objects.

                } else if (joyAxisDownDir == JOY_E) { //right
                    setLastMouseButtonRight(false);
                    if (alternateClick) {
                        pointerDown(x * CELL_D + CELL_D + 63, y * CELL_D + 35); // 63 is one less than half of grid size.
                        pointerUp(x * CELL_D + CELL_D + 63, y * CELL_D + 35);
                    } else {
                        pointerDown(x * CELL_D + CELL_D, y * CELL_D + 52);
                        pointerUp(x * CELL_D + CELL_D, y * CELL_D + 52);
                    }
                
                } else if (joyAxisDownDir == JOY_NE) {
                    setLastMouseButtonRight(false);
                    pointerDown(xx + CELL_D, yy + CELL_D + 52); 
                    pointerUp(xx + CELL_D, yy + CELL_D + 52);

                } else if (joyAxisDownDir == JOY_SE) {
                    setLastMouseButtonRight(false);
                    pointerDown(xx + CELL_D, yy - CELL_D + 52); 
                    pointerUp(xx + CELL_D, yy - CELL_D + 52);

                } else if (joyAxisDownDir == JOY_SW) {
                    setLastMouseButtonRight(false);
                    pointerDown(xx - CELL_D, yy - CELL_D + 52); 
                    pointerUp(xx - CELL_D, yy - CELL_D + 52);

                } else if (joyAxisDownDir == JOY_NW) {
                    setLastMouseButtonRight(false);
                    pointerDown(xx - CELL_D, yy + CELL_D + 52); 
                    pointerUp(xx - CELL_D, yy + CELL_D + 52);
                }

                setLastMouseButtonRight(savedRight); // restore last mouse button clicked.
            }
        }

    }
}
