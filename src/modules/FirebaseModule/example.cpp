/**
 * The example to get data from database.
 *
 * This example uses the UserAuth class for authentication.
 * See examples/App/AppInitialization for more authentication examples.
 *
 * The complete usage guidelines, please read README.md or visit https://github.com/mobizt/FirebaseClient
 */

// Increase WiFi task stack size to prevent stack overflow (ESP32 only)

#define ENABLE_USER_AUTH
#define ENABLE_DATABASE

#include <FirebaseClient.h>
#include "ExampleFunctions.h"
#include "example.h" // Provides the functions used in the examples.

#define API_KEY "AIzaSyCYPQlWNadRzdk2A9VKXj-nwAMGgjGePyo"
#define USER_EMAIL "catwater@cat.com"
#define USER_PASSWORD "cat123"
#define DATABASE_URL "https://cat-water-home-xhy80b-default-rtdb.firebaseio.com/"

SSL_CLIENT ssl_client;

using AsyncClient = AsyncClientClass;
AsyncClient aClient(ssl_client);

UserAuth user_auth(API_KEY, USER_EMAIL, USER_PASSWORD, 3000 /* expire period in seconds (<3600) */);
FirebaseApp app;
RealtimeDatabase Database;
AsyncResult databaseResult;

bool taskComplete = false;

void firebaseSetup(const char *SSID, const char *Password)
{
  WiFi.begin(SSID, Password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.printf("Firebase Client v%s\n", FIREBASE_CLIENT_VERSION);

  set_ssl_client_insecure_and_buffer(ssl_client);

  Serial.println("Initializing app...");
  // initializeApp(aClient, app, getAuth(user_auth), auth_debug_print, "🔐 authTask");

  // Or intialize the app and wait.
  initializeApp(aClient, app, getAuth(user_auth), 120 * 1000, auth_debug_print);

  app.getApp<RealtimeDatabase>(Database);

  Database.url(DATABASE_URL);
}

void firebaseLoop()
{
  // To maintain the authentication and async tasks
  app.loop();

  if (app.ready() && !taskComplete)
  {
    taskComplete = true;
    create_dummy_data();
  }

  // For async call with AsyncResult.
  processData(databaseResult);
}

void processData(AsyncResult &aResult)
{
  // Exits when no result available when calling from the loop.
  if (!aResult.isResult())
    return;

  if (aResult.isEvent())
  {
    Firebase.printf("Event task: %s, msg: %s, code: %d\n", aResult.uid().c_str(), aResult.eventLog().message().c_str(), aResult.eventLog().code());
  }

  if (aResult.isDebug())
  {
    Firebase.printf("Debug task: %s, msg: %s\n", aResult.uid().c_str(), aResult.debug().c_str());
  }

  if (aResult.isError())
  {
    Firebase.printf("Error task: %s, msg: %s, code: %d\n", aResult.uid().c_str(), aResult.error().message().c_str(), aResult.error().code());
  }

  if (aResult.available())
  {
    Firebase.printf("task: %s, payload: %s\n", aResult.uid().c_str(), aResult.c_str());
  }
}

template <typename T>
void check_and_print_value(T value)
{
  // To make sure that we actually get the result or error.
  if (aClient.lastError().code() == 0)
  {
    Serial.print("Success, Value: ");
    Serial.println(value);
  }
  else
  {
    Firebase.printf("Error, msg: %s, code: %d\n", aClient.lastError().message().c_str(), aClient.lastError().code());
  }
}

template <typename T>
T databaseGet(String path)
{
  Serial.printf("Getting value from path: %s\n", path.c_str());
  T value1 = Database.get<T>(aClient, path);
  check_and_print_value(value1);
  return value1;
}

template <typename T>
void databaseSet(String path, T value)
{
  Serial.printf("Setting value at path: %s\n", path.c_str());
  Database.set<T>(aClient, path, value);
  if (aClient.lastError().code() == 0)
  {
    Serial.println("Set successful");
  }
  else
  {
    Firebase.printf("Error, msg: %s, code: %d\n", aClient.lastError().message().c_str(), aClient.lastError().code());
  }
}

void databasePushJson(object_t &obj, String path)
{
  Serial.printf("Pushing JSON object to %s\n", path.c_str());
  Database.push(aClient, path, obj);
  if (aClient.lastError().code() == 0)
  {
    Serial.println("Push successful");
  }
  else
  {
    Firebase.printf("Error, msg: %s, code: %d\n", aClient.lastError().message().c_str(), aClient.lastError().code());
  }
  Serial.println();
}

void create_dummy_data()
{
  Database.set<number_t>(aClient, "/examples/Get/Async/data1", number_t(123.456789, 6));
  Database.set<number_t>(aClient, "/examples/Get/Async/data2", number_t(987.654321, 6));
  Database.set<number_t>(aClient, "/examples/Get/Async/data3", number_t(321.234567, 6));
  Database.set<number_t>(aClient, "/examples/Get/Async/data4", number_t(456.789012, 6));
  Database.push<number_t>(aClient, "/examples/Get/Await/messages", number_t(63.475869, 6));
}