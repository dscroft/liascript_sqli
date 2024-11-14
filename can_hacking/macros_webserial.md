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
        window.connection = SimpleWebSerial.setupSerialConnection({});
    }
    
    function status_update()
    {
        if( window.connection.ready() == null )
        {
            send.output("Disconnected");
        }
        else
        {
            send.output("Connected");
        }
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

-->

# Module Macros


