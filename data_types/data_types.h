#ifndef DATABASE_TYPES
#define DATABASE_TYPES

// Staff classification
enum ENUM_CLASSIFICATION {
    INVALID_CLASSIFICATION,
    NON_PLAYER,
    PLAYER,
    NON_PLAYER_AND_PLAYER,
    YOUTH_PLAYER,
    NEW_HUMAN_MANAGER,
    SPARE_NON_PLAYER
};

// Selected leagues defs
enum ENUM_CLUB_COMP_SELECTED_STATUS {
    NOT_SELECTED = 0x0000,
    SELECTED_BACKGROUND = 0x0001,
    SELECTED_FOREGROUND = 0x0002,
    ALL_DIVISIONS_ACTIVE = 0x0004
};

// Club sizes
enum ENUM_CLUB_SIZES {
    DIRECTOR_SIZE = 3,
    SQUAD_SIZE = 50,
    SCOUT_SIZE = 7,
    COACH_SIZE = 5,
    PHYSIO_SIZE = 3
};

// Database days of the week
enum ENUM_DAYS {
    DB_SUN,
    DB_MON,
    DB_TUE,
    DB_WED,
    DB_THUR,
    DB_FRI,
    DB_SAT,
};

// Maximum sizes in CM 01/02
enum ENUM_MAX_SIZES {
    MAX_FRIENDLY_COMPETITIONS = 127,    // Max number of friendly competitions
    MAX_HUMAN_PLAYERS = 16,             // Max number of human players
    MAX_TACTIC_TRAINING = 4,            // Maximum number of tactics in training at once
    TEAM_SZ = 20                        // Maximum size of a squad for a match
};

// Staff playing squad
enum ENUM_PLAYING_SQUAD {
    INVALID_SQUAD = 0,
    CLUB_SENIOR_SQUAD = 0x01,
    CLUB_RESERVE_SQUAD = 0x02,
    NATION_MAIN_SQUAD = 0x04,
    NATION_B_SQUAD = 0x08
};

// Pointers
enum ENUM_POINTER {
    NO_MATCH = - 127,   // 127 is the lowest value a char can be so this value is used in case it will be stored as a char
    BLANK,
    PROTECTED,
    NONE = -1,
    FREE_AGENT = NONE,
    NO_NATION = NONE,
    NO_STAFF = NONE,
    VALID = 0
};

// Club professional status
enum ENUM_PROFESSIONAL_STATUS {
    PROFESSIONAL = 1,
    SEMI_PRO,
    AMATEUR
};

// Struct lengths
enum ENUM_TABLE_SIZES {
    DB_CITY_SIZE = 56,
    DB_CLUB_SIZE = 581,
    DB_CLUB_COMP_SIZE = 107,
    DB_CLUB_COMP_HISTORY_SIZE = 26,
    DB_COLOUR_SIZE = 58,
    DB_CONTINENT_SIZE = 198,
    DB_INDEX_SIZE = 67,
    DB_NAME_SIZE = 60,
    DB_NATION_SIZE = 290,
    DB_NON_PLAYER_SIZE = 68,
    DB_OFFICIAL_SIZE = 43,
    DB_PLAYER_SIZE = 70,
    DB_STADIUM_SIZE = 78,
    DB_STAFF_SIZE = 110,
    DB_STAFF_COMP_SIZE = 101,
    DB_STAFF_COMP_HISTORY_SIZE = 58,
    DB_STAFF_HISTORY_SIZE = 17,
    DB_STAFF_PREFERENCES_SIZE = 52,
};

// Text lengths
enum ENUM_TEXT_LENGTHS {
    MAX_INDEX_NAME = 50,
    SHORT_TEXT_LENGTH = 26,
    STANDARD_TEXT_LENGTH = 51,
    LONG_TEXT_LENGTH = 101
};

#endif // DATABASE_TYPES

