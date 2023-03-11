# WiFi-ClientBasic
 
Use the following URL to test: 
http://scu.ismy.school/index.html 
### TIPS 
1) Use this code as a starting point (it does not work) 
http://www.esp8266.com/viewtopic.php?f=6&t=4169  
2) Use code from Arduino development software (1.6.5), GoTo  
i. file->examples->ESP8266Wifi->WiFiAccessPoint 
ii. file->examples->ESP8266Wifi->WiFiClient 
3) In the sketch window, open tools->SerialMonitor (CTRL-Shift-M) to monitor and 
test  your code at work on ESP32 
4) Lookup String class operations function in the 8266 arduino development kit 
documentation. You will need this split parameters into host & URL 
5) As you will see in the example, you will need “\r\n” for every HTTP header 
line. And will need “\r\n\r\n” at end of request (remember http syntax we 
learned in class) 
Note 
Your assignment must work for ANY http:// URL sent to the proxy.  You are only required 
to support http text. No images required, https support is optional  
 
### Part 2( optional) 
1) You should be able now to easily program your ESP32 as a real proxy by 
programming your web browser’s connection to use proxy server 192.168.4.1 
