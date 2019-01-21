#ifndef STAFF_CLASSIFICATION_H
#define STAFF_CLASSIFICATION_H

#include "cm_char.h"

// --- Staff Classification --- //
class StaffClassification : public CM_Char
{
public:
    // Constructor
    StaffClassification();

    // Get data
    bool isNonPlayer();
    bool isPlayer();

    // Set data
    void set(int i);

    // Values
    enum ENUM_VALUES {
        INVALID_CLASSIFICATION = 0,
        NON_PLAYER,
        PLAYER,
        NON_PLAYER_AND_PLAYER,
        YOUTH_PLAYER,
        NEW_HUMAN_MANAGER,
        SPARE_NON_PLAYER,
    };
};

#endif // STAFF_CLASSIFICATION_H
