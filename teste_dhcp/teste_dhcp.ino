#include "EtherCard.h"
#include <IPAddress.h>
//#include <Ethernet.h>
#define STATIC 1  // set to 1 to disable DHCP (adjust myip/gwip values below)

#if STATIC
// ethernet interface ip address
static byte myip[] = { 192,168,1,6 };
// gateway ip address
static byte gwip[] = { 192,168,1,1 };
// DNS ip address
static byte dnsip[] = { 194,239,134,83 };
#endif

// ethernet mac address - must be unique on your network
static byte mymac[] = { 0x70,0x69,0x69,0x2D,0x30,0x31 };

byte Ethernet::buffer[500]; // tcp/ip send and receive buffer
int led = 5; 
char data;

String sDat;
byte ipDest[] = {192, 168, 1, 30};
unsigned int portDest = 9000;
unsigned int portMy = 1337;

// ***********************************************
void setup()
{
  Serial.begin(9600);
  Serial.println("\n[I'm listen..]");
  pinMode(led, OUTPUT);
  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0)
    Serial.println( "Failed to access Ethernet controller");
#if STATIC
  ether.staticSetup(myip, gwip, dnsip);
#else
  if (!ether.dhcpSetup())
    Serial.println("DHCP failed");
#endif

  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);
  ether.printIp("DNS: ", ether.dnsip);

  //register udpSerialPrint() to port 1337
  ether.udpServerListenOnPort(&udpSerialPrint, 1337);

  //register udpSerialPrint() to port 42.
// ether.udpServerListenOnPort(&udpSerialPrint, 42);
}

// **********************************************
void loop()
{
  //this must be called for ethercard functions to work.
  ether.packetLoop(ether.packetReceive());
}

// ***********************************************
//callback that prints received packets to the serial port

void udpSerialPrint(word port, byte ip[4], word test, const char *data, word len)
{
IPAddress src(ip[0], ip[1], ip[2], ip[3]);
Serial.println(src);   // Contain Destination IP
Serial.println(port);
Serial.println (data);
char msg[] = {"TEMP = 21.4"};
ether.sendUdp(msg, sizeof msg, portMy, ipDest, portDest);
  }
// ***********************************************
