/* ESP32 AWS IoT
 *  
 * Simplest possible example (that I could come up with) of using an ESP32 with AWS IoT.
 *  
 * Author: Anthony Elder 
 * License: Apache License v2
 */
#include <WiFiClientSecure.h>
#include <PubSubClient.h> // install with Library Manager, I used v2.6.0
#include <ArduinoJson.h> // Librería para obtener información en formato Json

const char* ssid = "INFINITUMA603_2.4";
const char* password = "4s53ZQ9HkU";

const char* awsEndpoint = "a7fdednisxkv8-ats.iot.us-east-1.amazonaws.com";

// Update the two certificate strings below. Paste in the text of your AWS 
// device certificate and private key. Add a quote character at the start
// of each line and a backslash, n, quote, space, backslash at the end 
// of each line:

// xxxxxxxxxx-certificate.pem.crt
const char* certificate_pem_crt = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWTCCAkGgAwIBAgIUW3WHArb8wxXOt3ZPhd/ocCJueHMwDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTIxMDMxNzEzMzQx
MloXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBANT1qMoNsqAW0GaBNRdg
B9s6CMvna11cPyf5URt+BkkAUPW3tKOGZ2cv5fOsKJX+eTyQszZIo1bVIEp9dRAz
oDKebMPLxqN4T2TX9X87V8NW+Lu2blby2S8aFlKDaT/dMAqWkrPL/PYFTl0o4nFF
nX9vxtWaE8zzUhOwHoc7a/FUlTkdC1AQRQ3oRn3IkGq/7zeQ82HWDWGkcT9zOWrQ
W9As17ydIP6iBqkodlHMyare2SweMXWrQZKGrn9ibSxiUAjdXZXaG2sU7njS6o+d
ks6bHHAN0sPw55EHxFZONz/FaGzXZx+jYUs7Op12m0HTye8ALCPPzpOcxglYc7ZP
RM8CAwEAAaNgMF4wHwYDVR0jBBgwFoAUwsq4yB/EnHQeDVk0jCr6aGT+icwwHQYD
VR0OBBYEFP+i4/qyO1yJWP3Dr1cuCpJmr1r1MAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQBC4+8k/qdaRhpQkt41djYq77hC
rGWNffgXf4Ov43zlUxi30q+Ck0lV8PPj7NHGzIIwY9cMlTI35c0I3h3K+uxHoCNI
kkoEy8wQOeQtD86i5dvjbqbqGoXgNUbA6iEEMaQ3z3PEifdLVQGDaOa5GyT1J3lC
R49K3pmUskREprSp7Qm/0GAgI+LIeIT/DRQ1w2/ebwUbQ+yYmxZJAVVH8X5doCka
uy4BYfm3JFyFv5KzzgcCjBUX+pXmJmvC7IUrvZPZLPBC7oM2CbtnW1eWMcPuZyl2
JWH51aAjCJPfykEQIxspxlTmF+TVUVHAGT7XlN4apzAO0Nvu4QtlaW8hCZA+
-----END CERTIFICATE-----
)KEY";

// xxxxxxxxxx-private.pem.key
const char* private_pem_key = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEogIBAAKCAQEA1PWoyg2yoBbQZoE1F2AH2zoIy+drXVw/J/lRG34GSQBQ9be0
o4ZnZy/l86wolf55PJCzNkijVtUgSn11EDOgMp5sw8vGo3hPZNf1fztXw1b4u7Zu
VvLZLxoWUoNpP90wCpaSs8v89gVOXSjicUWdf2/G1ZoTzPNSE7Aehztr8VSVOR0L
UBBFDehGfciQar/vN5DzYdYNYaRxP3M5atBb0CzXvJ0g/qIGqSh2UczJqt7ZLB4x
datBkoauf2JtLGJQCN1dldobaxTueNLqj52SzpsccA3Sw/DnkQfEVk43P8VobNdn
H6NhSzs6nXabQdPJ7wAsI8/Ok5zGCVhztk9EzwIDAQABAoIBAQCOZc19ffdJELg9
Mnx7qtzV/FJRdebWwI4CfrV7OqOdQLbh9vq0tXSDDQYoD5/72/LZSrBqxdtifpRe
+tDCCqM99a0opyOlvxbniXhAoT+oAInFitDxFuWel0UB25kO+Sncip6B1w7IuveR
V3tC3pfX1LgRsDs5OiJRw9xF9LfS39EXMjQjcqgTFuRuL+g+jJMFucUaGA5i5dAH
6obCAcTbgQDaE+XscXFCT6Q41ViBY/Px1qVUVQO7yYQNOUlM50k3I+30yqKS9hZf
RkZN8KM5w1uR9fEJOw7hf/RSE19MOAmtps9Mq3BrSR0TeAK3yD5KzB0pgsWHjKk7
3f4OB/RhAoGBAPkZDwzqaAAkeRCDzeQK24JeGp03PdgtNNibzU2/y+j9S7ukUPH5
MpGWrXETnVk07ezabGmp5WSu3HJMtGlzkaXPlSvDY1E42JzMc9/pRWuy5ltXlhbl
b6+3zAHaH8D70tulPopAjulHHKlfU0LPXfwkCVUEVIFmVt25biXXDhJHAoGBANrc
Qjkz7Vh4w3bcbegNSMRGvA9FWuCuDLZXMuooYTApRNbQ/O8BL62ZWeHxov6Hgrcf
8S5BdchqN9F8sA5dVy2ImHFscGoN01h0HF2XMmVqKIIFB2zMl9xB4Pqc8bX0jKVa
J1nLukDOhIQ/Q9knud9M7jfRok6Zsi/QGdJsN7U5An9L5VawmLsXUAWTApW45ZX8
Iq/5NtuhAiUX2mCzT7zRQFvS0D3ZfBPsR/yQd2l5xKa5g9OEXj+GbcC9EqHlf1Dh
NF4+90lLdgaJv8GOd4SvS9LkAcu5Li7ZWybNaahDJHcadT6Y4RKqAQC6mlDECmqY
aGJvhOI6ivPsmpL09JFVAoGBALzs16nDfoYZsTmgrKtdFEyCdwKDryA2frjEayog
80x8w3TgcQbsMjkcIK/fRAGN/rOPukVgsojv0AFhLiUoRAIFYPrATgXuq1s21+me
hB1xV0htiOxNbGXw5IHSCwEz+lkeisuEBuK8oz350trY5SWypgy/ROmu2VdWv4Oa
cThZAoGAHbbcnuU1X9YQkTexlHRzcHs+/4RzfrZxWv/F9DysoVhU5vQSurGhedf3
ITpvThawTN0g9UvktAXFSEHmboNqGqMCjZvjDUM/7FR2Ub5+xmZjPZynUb4Ad1zL
VaY43u05WKHmiV1odXAZ9G2ztlfLSj/A8EZv15KcupjqMOUoD30=
-----END RSA PRIVATE KEY-----
)KEY";

/* root CA can be downloaded in:
  https://www.symantec.com/content/en/us/enterprise/verisign/roots/VeriSign-Class%203-Public-Primary-Certification-Authority-G5.pem
*/
const char* rootCA = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";

WiFiClientSecure wiFiClient;
void msgReceived(char* topic, byte* payload, unsigned int len);
PubSubClient pubSubClient(awsEndpoint, 8883, msgReceived, wiFiClient); 

StaticJsonDocument<256> doc; // Se declara el documento Json

void setup() {
  Serial.begin(115200); delay(50); Serial.println();
  Serial.println("ESP32 AWS IoT Example");
  Serial.printf("SDK version: %s\n", ESP.getSdkVersion());

  Serial.print("Connecting to "); Serial.print(ssid);
  WiFi.begin(ssid, password);
  WiFi.waitForConnectResult();
  Serial.print(", WiFi connected, IP address: "); Serial.println(WiFi.localIP());

  wiFiClient.setCACert(rootCA);
  wiFiClient.setCertificate(certificate_pem_crt);
  wiFiClient.setPrivateKey(private_pem_key);
}

unsigned long lastPublish;
int msgCount;

void loop() {

  pubSubCheckConnect();

  if (millis() - lastPublish > 10000) {
    //String msg = String("Hello from ESP8266: ") + ++msgCount;
    int bateria = 90;
    int nPiezas = 20;

    char Data[128];

    snprintf(Data, sizeof(Data), "{\"bateria\":%d, \"piezas\":%d}", bateria, nPiezas);
    
    boolean rc = pubSubClient.publish("outTopic", Data);
    Serial.print("Published, rc="); Serial.print( (rc ? "OK: " : "FAILED: ") );
    // Serial.println(msg);
    lastPublish = millis();
  }
}

void msgReceived(char* topic, byte* payload, unsigned int length) {
  char inData[80];
  Serial.print("Message received on "); Serial.print(topic); Serial.print(": ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    inData[(i)] = (char)payload[i];
  }
  Serial.println();

  deserializeJson(doc, inData); // Pasar a Json la información recibida

  // Se obtienen los valores como JSON
  String val = doc["llave"];
  Serial.print(val);
}

void pubSubCheckConnect() {
  if ( ! pubSubClient.connected()) {
    Serial.print("PubSubClient connecting to: "); Serial.print(awsEndpoint);
    while ( ! pubSubClient.connected()) {
      Serial.print(".");
      pubSubClient.connect("ESPthingXXXX");
      delay(1000);
    }
    Serial.println(" connected");
    pubSubClient.subscribe("inTopic");
  }
  pubSubClient.loop();
}
