#ifndef UTILITIES_H
#define UTILITIES_H

extern int LED_RED;
extern int LED_GREEN;
extern int LED_BLUE;

extern unsigned int beginingTime;
extern unsigned int currentTime;
extern short int light;
extern int conditionRunway;

void assignColor(unsigned char red, unsigned char green, unsigned char blue);

#endif // UTILITIES_H