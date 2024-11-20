<!--
attribute: 
version:  0.0.1
language: en
narrator: UK English Female
title: Module Macros for WebSerial
comment:  This is placeholder module to save macros used in other modules.

@version_history 

@end

script: https://unpkg.com/simple-web-serial@latest/dist/simple-web-serial.min.js

@WebSerial.connectButton
<script id="connect" input="submit" default="Press to connect"></script>
@end

@WebSerial.status
<script>
    send.output("Initialise");

    if( window.connection )
    {
        send.output("Reuse existing connection");
        /* may need to revisit this section */
        //window.connection.removeListeners();
    }
    else
    {
        send.output("Creating connection");
        window.connection = SimpleWebSerial.setupSerialConnection({
            //warnAboutUnregisteredEvents: false,
        });
    }
    
    function status_update()
    {
        if( window.connection.ready() == null )
            send.output("Disconnected");
        else
            send.output("Connected");
    }

    document.getElementById("connect").addEventListener("click", function() {
        send.output("Connecting");
        window.connection.startConnection();
        status_update();
    });

    setInterval(status_update, 1000);

    "LIA: wait"
</script>
@end

@WebSerial.__c
    @WebSerial.connectButton
@end

@WebSerial.__s
    @WebSerial.status
@end

@WebSerial.defaultManager
<!--
style="max-width: 400px;" -->
```` ascii
 .--------------------------------------------.
 | Connection manager                         |
 +--------------------------------------------+          
 | "@WebSerial.__c ""@WebSerial.__s         " | 
 |                                            |
 '--------------------------------------------'
````
@end

@WebSerial.logger
<script>
    console.log( "$(@0)" )
</script>
@end

-->

# Module Macros

## Arduino

Rather than attempting to roll our own communications protocol.

For example:

```c
// Include the library
#include <SimpleWebSerial.h>

// Create an instance of the library
SimpleWebSerial WebSerial;

void setup() 
{
  // Initialize serial communication
  Serial.begin(57600);
  
  // Define events to listen to and their callback
  WebSerial.on("event-to-arduino", eventCallback); 
}

void eventCallback(JSONVar data) 
{
    // Do something, even sending events right back!
    WebSerial.send("event-from-arduino", data);
};

unsigned long timer = 0;

void loop() 
{
  const unsigned long now = millis();
  if( now - timer > 3000 )
  {
    WebSerial.send("arduino-alive", JSONVar(now) );
    timer = now;
  }
  
  // Check for new serial data every loop
  WebSerial.check();
  delay(5);
}
```
