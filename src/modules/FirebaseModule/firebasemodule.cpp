/**
 * FirebaseDBManager.cpp
 * Módulo completo de gestión de Realtime Database con FirebaseClient para Arduino/ESP.
 *
 * NOTA:
 * - Requiere tener conexión WiFi antes de llamar a setupFirebase().
 * - Incluye operaciones: push, set, update, get, delete.
 * - Usar loopFirebase() en el loop principal.
 * - Ajusta las credenciales y URL de tu base de datos.
 */

#define ENABLE_USER_AUTH
#define ENABLE_DATABASE
#include "firebasemodule.h"
#include <FirebaseClient.h>
#include <ESP_SSLClient.h>
#include <ArduinoJson.h>

#define API_KEY "AIzaSyCYPQlWNadRzdk2A9VKXj-nwAMGgjGePyo"
#define USER_EMAIL "catwater@cat.com"
#define USER_PASSWORD "cat123"
#define DATABASE_URL "https://cat-water-home-xhy80b-default-rtdb.firebaseio.com/"

ESP_SSLClient ssl_client;
using AsyncClient = AsyncClientClass;
AsyncClient aClient(ssl_client);

UserAuth user_auth(API_KEY, USER_EMAIL, USER_PASSWORD, 3000);
FirebaseApp app;
RealtimeDatabase Database;
AsyncResult databaseResult;

// Prototipos
void setupFirebase();
void loopFirebase();
void processData(AsyncResult &aResult);

// Operaciones
String pushValue(const String &path, const object_t &value);
bool setValue(const String &path, const object_t &value);
bool updateValue(const String &path, const object_t &jsonPatch);
String getValue(const String &path, String &outPayload);
String deleteValue(const String &path);

// Ejemplo de uso de las operaciones
void ejemploOperacionesDB();

// ===================== IMPLEMENTACIÓN =======================

void setupFirebase()
{
  Serial.println("Inicializando Firebase...");
  initializeApp(aClient, app, getAuth(user_auth));
  app.getApp<RealtimeDatabase>(Database);
  Database.url(DATABASE_URL);
  Serial.println("Firebase inicializado.");
}

void loopFirebase()
{
  app.loop();
  // Procesar resultados asíncronos (si usas las funciones async)
  processData(databaseResult);
}

// // Procesa los resultados de operaciones asíncronas (si se usan)
void processData(AsyncResult &aResult)
{
//   if (!aResult.isResult())
//     return;

//   if (aResult.isEvent())
//   {
//     Firebase.printf("Event task: %s, msg: %s, code: %d\n",
//                     aResult.uid().c_str(),
//                     aResult.eventLog().message().c_str(),
//                     aResult.eventLog().code());
//   }
//   if (aResult.isDebug())
//   {
//     Firebase.printf("Debug task: %s, msg: %s\n",
//                     aResult.uid().c_str(),
//                     aResult.debug().c_str());
//   }
//   if (aResult.isError())
//   {
//     Firebase.printf("Error task: %s, msg: %s, code: %d\n",
//                     aResult.uid().c_str(),
//                     aResult.error().message().c_str(),
//                     aResult.error().code());
//   }
//   if (aResult.available())
//   {
//     Firebase.printf("task: %s, payload: %s\n", aResult.uid().c_str(), aResult.c_str());
//   }
}

// PUSH: Añade un valor nuevo (genera clave única)
template <typename T>
String pushValue(const String &path, const T &value)
{
  String name = Database.push<T>(aClient, path, value);
  if (aClient.lastError().code() == 0)
  {
    Serial.printf("Push OK: %s\n", name.c_str());
  }
  else
  {
    Serial.printf("Push ERROR: %s (code %d)\n",
                  aClient.lastError().message().c_str(),
                  aClient.lastError().code());
  }
  return name;
}

// // SET: Sobrescribe el valor del nodo (destruye subnodos)
// bool setValue(const String &path, const object_t &value)
// {
//   bool name = Database.set(aClient, path, value);
//   if (aClient.lastError().code() == 0)
//   {
//     Serial.printf("Set OK\n");
//   }
//   else
//   {
//     Serial.printf("Set ERROR: %s (code %d)\n",
//                   aClient.lastError().message().c_str(),
//                   aClient.lastError().code());
//   }
//   return name;
// }

// // UPDATE: Actualiza campos sin borrar otros (solo para objetos)
// bool updateValue(const String &path, const object_t &jsonPatch)
// {
//   bool name = Database.update<object_t>(aClient, path, jsonPatch);
//   if (aClient.lastError().code() == 0)
//   {
//     Serial.printf("Update OK");
//   }
//   else
//   {
//     Serial.printf("Update ERROR: %s (code %d)\n",
//                   aClient.lastError().message().c_str(),
//                   aClient.lastError().code());
//   }
//   return name;
// }

// // GET: Obtiene el valor en la ruta dada
// String getValue(const String &path, String &outPayload)
// {
//   String name = Database.get(aClient, path, outPayload);
//   if (aClient.lastError().code() == 0)
//   {
//     Serial.printf("Get OK: %s\nPayload: %s\n", name.c_str(), outPayload.c_str());
//   }
//   else
//   {
//     Serial.printf("Get ERROR: %s (code %d)\n",
//                   aClient.lastError().message().c_str(),
//                   aClient.lastError().code());
//   }
//   return name;
// }

// // DELETE: Elimina el nodo en la ruta dada
// String deleteValue(const String &path)
// {
//   String name = Database.remove(aClient, path);
//   if (aClient.lastError().code() == 0)
//   {
//     Serial.printf("Delete OK: %s\n", name.c_str());
//   }
//   else
//   {
//     Serial.printf("Delete ERROR: %s (code %d)\n",
//                   aClient.lastError().message().c_str(),
//                   aClient.lastError().code());
//   }
//   return name;
// }

// Ejemplo de uso de todas las operaciones (llamar desde setupFirebase o loop)
void ejemploOperacionesDB()
{
  // Usa StaticJsonDocument para evitar problemas de heap
  JsonDocument jsonDoc;
  jsonDoc["demo"] = "Hola mundo";


  // Push
  pushValue("/ejemplo/push", jsonDoc);

  // // Set
  // setValue("/ejemplo/set", json);

//   // Update
//   object_t patch;
//   writer.create(patch, "demo", string_t("Nuevo valor actualizado"));
//   updateValue("/ejemplo/set", patch);

//   // Get
//   String payload;
//   getValue("/ejemplo/set", payload);

//   // Delete
//   deleteValue("/ejemplo/set");
}

// // ========== Ejemplo de sketch de Arduino ==========

// void setup()
// {
//   Serial.begin(115200);
//   // Conectar WiFi aquí antes de llamar a setupFirebase()
//   setupFirebase();
//   ejemploOperacionesDB(); // Puedes comentar si no quieres pruebas automáticas
// }

// void loop()
// {
//   loopFirebase();
//   // Aquí puedes llamar operaciones de base de datos según tu lógica
// }
