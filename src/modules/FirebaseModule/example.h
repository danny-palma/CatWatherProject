#ifndef EXAMPLE_H
#define EXAMPLE_H

#include <Arduino.h>
#include <FirebaseClient.h>

void processData(AsyncResult &aResult);

template <typename T>
T databaseGet(String path);

template <typename T>
void databaseSet(String path, T value);

void databasePushJson(object_t &obj, String path = "/data");

void create_dummy_data();

void firebaseSetup(const char *SSID, const char *Password);
void firebaseLoop();

#endif // EXAMPLE_H