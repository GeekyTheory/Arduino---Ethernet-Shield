/*
     Arduino Ethernet Shield
	
Mario Pérez Esteso
http://www.geekytheory.com
https://www.youtube.com/user/telecoreference

https://twitter.com/geekytheory

Facebook page! Join us with a LIKE!
https://www.facebook.com/geekytheory
*/

#include <SPI.h>
#include <Ethernet.h>

//Declaración de la direcciones MAC e IP. También del puerto 80
byte mac[]={0xDE,0xAD,0xBE,0xEF,0xFE,0xED}; //MAC
IPAddress ip(192,168,0,100); //IP
EthernetServer servidor(80);

int PIN_LED=8;
String readString=String(30);
String state=String(3);

void setup()
{
  Ethernet.begin(mac, ip); //Inicializamos con las direcciones asignadas
  servidor.begin();
  pinMode(PIN_LED,OUTPUT);
  digitalWrite(PIN_LED,HIGH);
  state="OFF";
}

void loop()
{
  EthernetClient cliente= servidor.available();
  
  if(cliente)
  {
    boolean lineaenblanco=true;
    while(cliente.connected())//Cliente conectado
    {
      if(cliente.available())
      {
        char c=cliente.read();
        if(readString.length()<30)//Leemos petición HTTP caracter a caracter
        {
          readString.concat(c); //Almacenar los caracteres en la variable readString
        }
        if(c=='\n' && lineaenblanco)//Si la petición HTTP ha finalizado
        {
          int LED = readString.indexOf("LED=");
          if(readString.substring(LED,LED+5)=="LED=T")
          {
            digitalWrite(PIN_LED,LOW);
            state="ON";
          } else if (readString.substring(LED,LED+5)=="LED=F")
          {
            digitalWrite(PIN_LED,HIGH);
            state="OFF";
          }
          
          
          //Cabecera HTTP estándar
          cliente.println("HTTP/1.1 200 OK");
          cliente.println("Content-Type: text/html");
          cliente.println();        
          //Página Web en HTML
          cliente.println("<html>");
          cliente.println("<head>");
          cliente.println("<title>LAMPARA ON/OFF</title>");
          cliente.println("</head>");
          cliente.println("<body width=100% height=100%>");
          cliente.println("<center>");
          cliente.println("<h1>LAMPARA ON/OFF</h1>");
          cliente.print("<br><br>");
          cliente.print("Estado de la lampara: ");
          cliente.print(state);
          cliente.print("<br><br><br><br>");
          cliente.println("<input type=submit value=ON style=width:200px;height:75px onClick=location.href='./?LED=T\'>");
          cliente.println("<input type=submit value=OFF style=width:200px;height:75px onClick=location.href='./?LED=F\'>");
          cliente.println("</center>");
          cliente.println("</body>");
          cliente.println("</html>");
          cliente.stop();//Cierro conexión con el cliente
          readString="";
        }
      }
    }
  }
}
