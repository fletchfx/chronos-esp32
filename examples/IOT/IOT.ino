/*
   MIT License

  Copyright (c) 2023 Felix Biego

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

  This is a stripped version of Fbiegos watch software, this will go along with POCSAG software to allow a
  smart phone to send notifications to an ESP32 with BLE which will spit out POCSAG to a Motorola
  Advisor pager. 

  */

#include <ChronosESP32.h>
#include <HardwareSerial.h>

HardwareSerial mySerial(0);
ChronosESP32 watch("Advisor"); // set the bluetooth name

void connectionCallback(bool state)
{
  Serial.print("Phone: ");
  Serial.println(state ? "Connected" : "Disconnected");
}

void notificationCallback(Notification notification)
{
  mySerial.print(notification.app);
  mySerial.print(" - ");
  mySerial.println(notification.message);
}

void ringerCallback(String caller, bool state)
{
  if (state)
  {
    mySerial.print("Call-");
    mySerial.println(caller);
  }
  else
  {
    //Serial.println("Ringer dismissed");
  }
}

void configCallback(Config config, uint32_t a, uint32_t b)
{
  switch (config)
  {
  case CF_TIME:

    break;
  case CF_RTW:

    break;
  case CF_RST:

    break;
  case CF_FIND:

    break;
  case CF_FONT:

    break;
  case CF_ALARM:

    break;
  case CF_QUIET:

    break;
  case CF_SLEEP:

    break;
  case CF_SED:

    break;
  case CF_WATER:

    break;
  case CF_USER:

    break;
  case CF_HOURLY:

    break;
  case CF_HR24:

    break;
  case CF_CAMERA:

    break;
  case CF_LANG:

    break;
  case CF_PBAT:

    break;
  case CF_APP:

    break;
  case CF_QR:
    // qr links
    if (a == 0){

    }
    if (a == 1)
    {

    }
    break;
  case CF_WEATHER:

    if (a)
    {
      // if a == 1, high & low temperature values might not yet be updated
      if (a == 2)
      {
        int n = watch.getWeatherCount();
        String updateTime = watch.getWeatherTime();

        for (int i = 0; i < n; i++)
        {
          // iterate through weather forecast, index 0 is today, 1 tomorrow...etc
          Weather w = watch.getWeatherAt(i);

          if (i == 0)
          {

          }
        }
      }
    }
    if (b)
    {
      //Serial.print("City name: ");
      String city = watch.getWeatherCity(); //
      //Serial.print(city);
    }
    //Serial.println();
    break;
  case CF_CONTACT:
    if (a == 0){

    }
    if (a == 1){
      //Serial.println("Received all contacts");
      int n = uint8_t(b); // contacts size -> watch.getContactCount();
      int s = uint8_t(b >> 8); // sos contact index -> watch.getSOSContactIndex();
      for (int i = 0; i < n; i++)
      {
        Contact cn = watch.getContact(i);

      }
    }
    break;
  }
}

void dataCallback(uint8_t *data, int length)
{
  //Serial.println("Received Data");
  for (int i = 0; i < length; i++)
  {
    //Serial.printf("%02X ", data[i]);
  }
  //Serial.println();
}

void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);

  // set the callbacks before calling begin funtion
  watch.setConnectionCallback(connectionCallback);
  watch.setNotificationCallback(notificationCallback);
  watch.setRingerCallback(ringerCallback);
  watch.setConfigurationCallback(configCallback);
  watch.setDataCallback(dataCallback);

  watch.begin(); // initializes the BLE

  watch.setBattery(80); // set the battery level, will be synced to the app

  watch.set24Hour(true); // the 24 hour mode will be overwritten when the command is received from the app
  // this modifies the return of the functions below
  watch.getAmPmC(true); // 12 hour mode true->(am/pm), false->(AM/PM), if 24 hour mode returns empty string ("")
  watch.getHourC();     // (0-12), (0-23)
  watch.getHourZ();     // zero padded hour (00-12), (00-23)
  watch.is24Hour();     // resturns whether in 24 hour mode
  // watch.setNotifyBattery(false); // whether to enable or disable receiving phone battery status (enabled by default)
}

void loop()
{
  watch.loop(); // handles internal routine functions

  String time = watch.getHourC() + watch.getTime(":%M ") + watch.getAmPmC();
  //Serial.println(time);
  delay(500);

}
