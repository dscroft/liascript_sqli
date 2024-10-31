<!--
author:   André Dietrich
email:    LiaScript@web.de
version:  0.3.2
language: en
narrator: US English Male

logo:     logo.jpg

comment:  Use the real Python in your LiaScript courses, by loading this
          template. For more information and to see, which Python-modules are
          accessible visit the [pyodide-website](https://alpha.iodide.io).

import: https://raw.githubusercontent.com/LiaTemplates/Pyodide/master/README.md

script: webserial.js
script: script.js
-->


# Connecting to serial device

``

## serial port test

### dev reqs

As per https://www.reddit.com/r/webdev/comments/v2w9fb/develop_locally_on_https/?rdt=51016

sudo apt install libnss3-tools

curl -JLO "https://dl.filippo.io/mkcert/latest?for=linux/amd64"

chmod +x mkcert-v*-linux-amd64

sudo cp mkcert-v*-linux-amd64 /usr/local/bin/mkcert

npm install -g local-ssl-proxy

local-ssl-proxy --source 8010 --target 8000 --cert localhost.pem --key localhost-key.pem


### alt demo

<section id="controls">
    <button id="portButton">open port</button><br>
    Latest input from the serial port:
    <span id="readings">no input yet </span><br>
    Seconds passed:<span id="seconds">no seconds yet</span><br>
    Slide the slider:
    <input type="range" id="dim" min="0" max="255" value="10" /><br />
    Type a string, then hit enter:
    <input type="text" id="txt" />
</section>

<script>setup();</script>

### demo


<button id="listPortsButton">Click me</button>
<button id="sendButton">Send</button>

<div>Status: <p id="status">empty</p></div>
<div>Port: <span id="port"></span></div>

<script>

document.getElementById('listPortsButton').addEventListener('click', async () => 
{
    try
    {
        document.getElementById('status').innerHTML = 'Listing ports';

        let port = await navigator.serial.requestPort();
        
        document.getElementById('status').innerHTML = 'Port selected';

        await port.open({ baudRate: 9600 });
        
        document.getElementById('status').innerHTML = 'Port opened';
    }
    catch (error)
    {
        document.getElementById('status').innerHTML = error;
    }
    
});

document.getElementById('sendButton').addEventListener('click', async () => 
{
    // name of port
    if (port) {
        document.getElementById('port').innerHTML = port.getInfo();
    } else {
        document.getElementById('status').innerHTML = 'No port selected';
    }
});

</script>



