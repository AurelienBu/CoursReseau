#include <SPI.h>
#include <WiFi.h>

// Remove this line once you've entered WiFi SSID and password below.
//#error "WiFi SSID and password required!"

// Your network SSID (network name).
// TODO: Enter the name of your wifi network here.
char ssid[] = "arduino";

// Your network password.
// TODO: Enter the password of your wifi network here.
char pass[] = "arduino1";

// Your network key Index number (needed only for WEP).
int keyIndex = 0;

// Server status flag.
int status = WL_IDLE_STATUS;

// Create WiFi server listening on the given port.
WiFiServer server(65000);

void setup()
{
  // Start serial communication with the given baud rate.
  // NOTE: Remember to set the baud rate in the Serial
  // monitor to the same value.
  Serial.begin(115200);

  // Check for the presence of the WiFi shield.
  if (WiFi.status() == WL_NO_SHIELD)
  {
    // If no shield, print message and exit setup.
    Serial.println("WiFi shield not present");
    status = WL_NO_SHIELD;
    return;
  }

  String version = WiFi.firmwareVersion();
  if (version != "1.1.0")
  {
    Serial.println("Please upgrade the firmware");
  }

  // Connect to Wifi network.
  while (status != WL_CONNECTED)
  {
    Serial.print("Connecting to Network named: ");
    Serial.println(ssid);

    // Connect to WPA/WPA2 network. Update this line if
    // using open or WEP network.
    status = WiFi.begin(ssid, pass);

    // Wait for connection.
    delay(1000);
  }

  // Start the server.
  server.begin();

  // Print WiFi status.
  printWifiStatus();
}

void loop()
{
  // Check that we are connected.
  if (status != WL_CONNECTED)
  {
    return;
  }

  // Listen for incoming client requests.
  WiFiClient client = server.available();
  if (!client)
  {
    return;
  }

  Serial.println("Client connected");

  String request = readRequest(&client);
  sendResponse(&client, "ACK");
  //executeRequest(&client, &request);

  // Close the connection.
  //client.stop();

  Serial.println("Client disonnected");
}

// Read the request line. The string from the JavaScript client ends with a newline.
String readRequest(WiFiClient* client)
{
  String request = "";

  // Loop while the client is connected.
  while (client->connected())
  {
    // Read available bytes.
    while (client->available())
    {
      // Read a byte.
      char c = client->read();

      // Print the value (for debugging).
      Serial.write(c);

      // Exit loop if end of line.
      if ('\n' == c)
      {
        return request;
      }

      // Add byte to request line.
      request += c;
    }
  }
  return request;
}


// Read the parameter from the request string.
int readParam(String* request)
{
  // This handles a hex digit 0 to F (0 to 15).
  char buffer[2];
  buffer[0] = request->charAt(1);
  buffer[1] = 0;
  return (int) strtol(buffer, NULL, 16);
}

void sendResponse(WiFiClient* client, String response)
{
  // Send response to client.
  client->println(response);

  // Debug print.
  Serial.println("sendResponse:");
  Serial.println(response);
}

void printWifiStatus()
{
  Serial.println("WiFi status");

  // Print network name.
  Serial.print("  SSID: ");
  Serial.println(WiFi.SSID());

  // Print WiFi shield IP address.
  IPAddress ip = WiFi.localIP();
  Serial.print("  IP Address: ");
  Serial.println(ip);

  // Print the signal strength.
  long rssi = WiFi.RSSI();
  Serial.print("  Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
