#ifndef WEATHER_H
#define WEATHER_H

#pragma pack(1)

// WEATHER STRUCTURE DEFINITIONS //
// This structure defines the current weather settings for an area.

enum ENUM_CITY_WEATHER {
    SPRING = 0,
    SUMMER,
    AUTUMN,
    WINTER
};

enum ENUM_WEATHER_SPECIAL {
    SPECIAL_HAIL = 1,
    SPECIAL_SLEET,
    SPECIAL_SNOW
};

typedef struct
{
    char wind;
    char precipitation;
    char temperature;
    char special;
} WEATHER_CONDITION;

// Wind defintion structure, this structure should give the precentage scale
// upon which each of the various wind type will happen within a season.
// Eg. calm - 3 // 0 - 3 % means calm weather occurs
// breezy - 20
// gusty - 75
// strong - 90
// gale - 100
typedef struct
{
    char calm;
    char breezy;
    char gusty;
    char strong;
    char gale;
} WIND;

// Precipitation weather structure, this structure gives the percentage chance
// of each of the various types of precipitations happening.
typedef struct
{
    char dry;
    char wet;
    char drizzle;
    char shower;
    char down_pour;
} PRECIPITATION;

// Temperature weather structure, this structure gives the percentage chance
// of each of the various types of precipitations happening.
typedef struct
{
    char freezing;
    char cold;
    char mild;
    char fine;
    char warm;
    char hot;
    char very_hot;
} TEMPERATURE;

// This structure defines a seasons weather conditions within CM3.
typedef struct
{
    short WeatherSeasonStartDay;
    WIND WeatherSeasonWind;
    PRECIPITATION WeatherSeasonPrecipitation;
    TEMPERATURE WeatherSeasonTemperature;
} WEATHER_SEASON;

#endif // WEATHER_H
