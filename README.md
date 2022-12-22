# robotracer pio

![image](https://user-images.githubusercontent.com/36806204/209051063-f902cf9e-0245-4504-a4e8-e4bf75fd7aad.png)


## About
This is the further development of the main branch, including an webserver, hotspot and some i2c functions.

- Requires PlatformIO, see https://platformio.org/, download, install and start
- Import project and build using PlatformIO Home
- In order to upload, change Port in platformio.ini and use the upload button in the bottom bar (arrow left)

![image](https://user-images.githubusercontent.com/36806204/209051141-cc96c933-031f-4eb2-bed2-ea62b9de8d6d.png)


## How to use

This basic example will create an Hotspot and Webserver able to control two motors.

``` c
#include <Arduino.h>
#include "Motor.h"
#include "Data.h"
#include "Wifi.h"
#include "Web.h"

Motor left; # Object for left motor
Motor right; # Object for right motor

# This function is used to handle the commands received via websocket
# I used rf for right forward, lf for left forward, rb for right backward, lb for left backward
# up and down are sent, when releasing and pressing the buttons on the webpage
void receiver(WebsocketsClient &who, String where, String what) 
{
  if (where == "rf")
  {
    if (what == "down") # Somebody is pressing right forward down
    {
      right.move_forwards(10); # Thefore, moving forward is the action
    }
    else
    {
      right.stop(); # If its not down (== up), the motor should stop
    }
  }
  if (where == "rb")
  {
    if (what == "down")
    {
      right.move_backwards(10);
    }
    else
    {
      right.stop();
    }
  }

  if (where == "lf")
  {
    if (what == "down")
    {
      left.move_forwards(10);
    }
    else
    {
      left.stop();
    }
  }
  if (where == "lb")
  {
    if (what == "down")
    {
      left.move_backwards(10);
    }
    else
    {
      left.stop();
    }
  }

}

void setup() # The main setup, same as Arduino IDE
{
  Serial.begin(9600);

  Web::receive(&receiver); # This will register the function above to receive the messages via websocket

  Wifi::development(); # This activates the development mode, see the development section in the README.md

  Web::startHTTP(); # Start the HTTP server, serving the webpage
  Web::startWS(); # Start the websocket server

  digitalWrite(D0, LOW);

  left.init(1, D2, D1, false);
  right.init(2, D3, D4, true);

  Serial.println("Setup finished");

}

void loop()
{
  Web::tick(); # Update the servers
  left.tick(); # Update the left motor
  right.tick(); # Update the right motor
  delayMicroseconds(100); # Delay, may go down to 10 if needed
}
```

## Development mode
As I'm a lazy guy, I've also added an development mode, able to let the ESP directly connect to an existing Wifi-AP
This is useful during development, as there is no auth, it is NOT secure at all for a permanent solution in your home network.

How does this work?
 - On start, ESP tries to connect to Wifi-Network
 - If no network available or configured before, start AP with config interface
 - If AP spotted, connect and open 192.168.4.1 and configure wifi connection there 
 - If connected to your Wifi, use your router, check ip and connect - If you are not able to access your router, connect ESP via usb and have a look at the console output (plug symbole in PlatformIO)


## Adding buttons to the interface:

### Build
There are two options:
#### Automatically
- node and npm
- html-minifier, use npm for that (see https://www.npmjs.com/package/html-minifier)
- groovy, see https://groovy-lang.org/
- bash, use a unix-like os or sth. like MinGW or GitBash

The Data.h will be updated automatically!

#### Manually

![image](https://user-images.githubusercontent.com/36806204/209051204-e5df04d7-9c64-435d-8561-c5f5a946c3f9.png)


 - Use sth. like https://www.willpeavy.com/tools/minifier/ and minify the content of the index.html
 - Excape all doublequotes and split the text in quoted lines of 100 chars max.
 - Update the Data::index variable.

### Make

There is an index.html file under web/
Ignore the CSS part, as it is not important and go to the 
``` html
<body class="vbox viewport">
```
line.
Every line you can see in the interface, is created by an section element,
adding 
``` html
<section class="main hbox space-between">
...
</section>
```
will add a new horizontal line.
Use 
``` html
 <article id="lf" class="flex">Left forward</article>
 ```
 to add a new button. Give the button an unique id!
 
 Jump to the
 ``` html
 <script>
 ``` 
 section, add this line
 ``` javascript
 register("lf");
 ```
 using the id, that you used before.
 
 After you made your changes, call the build-additional.sh
