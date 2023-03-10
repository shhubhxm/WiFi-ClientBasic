/*
 *  This sketch sends a message to a TCP server.
 */

#include <WiFi.h>
#include <WiFiAP.h>

// credentials to connect the wifi;
const char *ssid = " DropItLikeItsHotspot";
const char *password = "IbelieveWicanFi";

// credentials to set up the access point.
const char *newssid = "shubham";
const char *newpassword = "";

WiFiServer server(80);
WiFiClient webclient;  // listen for new clients who may be attempting to connect.

// Retrieves the webpage from the internet
void callWeb2(String hostName, String urlName) {
    Serial.println("reached the Web client");

    WiFiClient client;
    const char *host = hostName.c_str();
    const char *url = urlName.c_str();

    const uint16_t port = 80;

    String readRequest = String("GET ") + urlName + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n";

    if (!client.connect(host, port)) {
      return;
    }
    Serial.println("connected");
    client.print(readRequest);
    readResponse(&client);
  
}

// This function reads data from the server and prints it to the web client (browser) of the client.
void readResponse(WiFiClient *client) {
  Serial.println("writing to Page");
  unsigned long timeout = millis();
  while (client->available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client->stop();
      return;
    }
  }

 
  while (client->available()) {
    String line = client->readStringUntil('\r\n\r\n');
    line.replace("Browser", "FIREFOX");
    line.replace("browser", "FIREFOX");
    webclient.print(line);
  }
  // Terminate the connection:
  webclient.println();

  webclient.stop();
  Serial.println("Client is Disconnected.");


  Serial.println("\nClosing the connection\n\n");
}

// loads up the WifiClient to fetch page and server using the given credentials.
void setup() {
  Serial.begin(115200);
  delay(10);

  if (!WiFi.softAP(newssid, newpassword)) {
    log_e("Soft AP creation failed.");
    while (1)
      ;
  }
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();
  Serial.println("Server started");

  // To begin, we establish a connection with a wireless network.
  WiFi.begin(ssid, password);
  Serial.println();
  Serial.println();
  Serial.print("Waiting for WiFi... ");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  delay(500);
}

// Keeps on a lookout for potential clients to connect to and accepts connection requests from those clients, which are then forwarded to subsequent processing steps.
void loop() {
  webclient = server.available();
  if (webclient) {                  // If a client is connected,
    Serial.println("New Client.");  // output a message to the serial port and create a String variable
    String currentLine = "";        // to store the data received from the client.
    String clientRequest = "";
    while (webclient.connected()) {  // While the client is connected,
      if (webclient.available()) {   // continuously check for incoming data.

        char c = webclient.read();  //  If there is any data available from the client, read a byte and display it on the serial monitor.
        // Serial.write(c);            // If the byte is a newline character and the current line is empty,
        if (c == '\n') {            // , it means that there are two consecutive newline characters,

          // indicating the end of the client's HTTP request.
          // In this case, send a response.
          if (currentLine.length() == 0) {
            // The beginning of an HTTP header is marked by a response code, such as "HTTP/1.1 200 OK", which indicates the status of the response,
            // followed by a content-type to inform the client of what to expect. Finally, a blank line is included to signify the end of the header.
            webclient.println("HTTP/1.1 200 OK");
            webclient.println("Content-type:text/html");
            webclient.println();

            Serial.println(clientRequest);
            if(clientRequest.indexOf("Referer: ")!=-1){
              int first_index = clientRequest.indexOf("Referer: ");
              first_index += 9;
              int last_index = clientRequest.indexOf("Accept-Encoding:");
              String entire_url = clientRequest.substring(first_index, last_index);
              entire_url.replace("http://", "");
              int entire_urlStartIndex = entire_url.indexOf("/");
              String ipaddress = entire_url.substring(0, entire_urlStartIndex + 1);
              entire_url.replace(ipaddress, "");
              int urlIndex = entire_url.indexOf("/");
              String urlName = entire_url.substring(urlIndex);
              String hostName = entire_url;
              hostName.replace(urlName, "");
              callWeb2(hostName, urlName);
            }else if(clientRequest.indexOf("GET")!=-1){
              clientRequest.replace("GET /", "");
              int hostEndIndex = clientRequest.indexOf("/");
              String hostName = clientRequest.substring(0,hostEndIndex);
              clientRequest.replace(hostName, "");
              int urlEndIndex = clientRequest.indexOf(" ");
              String urlName = clientRequest.substring(0,urlEndIndex);
              callWeb2(hostName, urlName);
              
            }

            // The HTTP response is terminated by a blank line.
            // The code then breaks out of the while loop:
            break;
          } else {  // If a newline character is detected, then the currentLine variable is emptied.
            currentLine = "";
          }
        } else if (c != '\r') {  // If you receive a character other than the carriage return,
          currentLine += c;      // then append it to the end of the currentLine variable.
          clientRequest += c;
        }
        // Determine whether the client's request was for "GET /H" or "GET /L":        
      }
    }
  }
}
