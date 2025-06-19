#ifndef ULTRASONICMODULE_H
#define ULTRASONICMODULE_H

extern int SensorTank; // centimetros detectados del sensor de tanque
extern int SensorBowl; // centimetros detectados del sensor de bowl

void ultrasonicModuleInit(int pinTrigger, int pinEcho, int pintrigger2, int pinecho2);
void ultrasonicModuleLoop();

#endif // ULTRASONICMODULE_H