<!--
author:   André Dietrich
email:    LiaScript@web.de
version:  0.3.2
language: en
narrator: US English Male

logo:     logo.jpg

import: macros_webserial.md
import: macros_dashboard.md

@onload
async function waitForConnection() {
  while (!window.connection) {
    await new Promise(resolve => setTimeout(resolve, 100)); // wait 100ms
  }
  // Once window.connection is available
  connectionAvailable();
}

function connectionAvailable() {
    window.connection.on('can-recv', function(msg) 
    {
        console.log( "Can-recv: "+msg );
       
        if( msg[0] == 203 ) // SCM_FEEDBACK
        {   
            // data[1] is an array of multiple integers where each element represents the value of a byte
            // convert into a single integer where the lsb were contained in element 0
            let data = 0;
            for (let i = 0; i < msg[1].length; i++)
                data |= msg[1][i] << (8 * i);

            // true if bit 6 of data is set
            window.turnSignalsStates.left = (data & (1 << 6)) !== 0;     
            window.turnSignalsStates.right = (data & (1 << 5)) !== 0;
        }   
        else if( msg[0] == 81 ) // GAS_PEDAL
        {
            window.tacho = msg[1][2] * 0.005;
        }     
    });
}

// Call this function to start the waiting process
waitForConnection();
@end
-->

# aaa


## Sender

@WebSerial.defaultManager

Indicators

<input type="checkbox" id="left">
<input type="checkbox" id="right">

Accelerator

<div class="slidecontainer">
    <input type="range" min="0" max="200" value="0" id="accelerator"><span id="accelerator_level"></span>
</div>

<script>


function update_accel_status()
{
    console.log("Sending accelerator message");

    let value = document.getElementById("accelerator").value;
    document.getElementById("accelerator_level").innerHTML = (value * 0.005).toFixed(3);

    /*Toyota prius
      BO_ 581 GAS_PEDAL: 8 XXX
        SG_ GAS_PEDAL : 23|8@0+ (0.005,0) [0|1] "" XXX*/

    let data = Math.min(200,Math.max(0,parseInt(value)));   
    let bytes = [ 0, 0, data, 0, 0, 0, 0, 0 ];
    window.connection.send( "can-send", [81, bytes] );
}

document.getElementById("accelerator").addEventListener("input", update_accel_status );
update_accel_status();
</script>


<script>
let sendSignalMsg = function()
{
    console.log("Sending signal message");

    let data = ( document.getElementById('left').checked ? (1 << 6) : 0 ) +
               ( document.getElementById('right').checked ? (1 << 5) : 0 );

    //convert data to an 8 byte array where hte lsb of data are stored in 
    let bytes = new Array(8);
    for (let i = 0; i < bytes.length; i++) 
        bytes[i] = (data >> (i * 8)) & 0xFF;
    
    window.connection.send( "can-send", [203, bytes] );
}

document.getElementById('left').addEventListener('click', sendSignalMsg);
document.getElementById('right').addEventListener('click', sendSignalMsg);
</script>



## Hacker

@WebSerial.defaultManager

<script style="display: block">
    let buffer = [];
    let counter = 0;
    let bufferMaxSize = 10;

    function addToBuffer(line) 
    {
        buffer.push([Date.now(), ...line]);
        if (buffer.length > bufferMaxSize) 
            buffer.shift();
    }

    function displayBuffer()
    {
        let table = document.getElementById("framesLogger");

        let liatable =  "<!-- data-type='none' \n" +
                        "     data-title='Recived CAN frames' \n" + 
                        "     data-sortable='false' -->\n" +
                        "| Timetstamp | CAN Frame ID | Data |\n" +
                        "|-|-|------|\n"

        for (let i = 0; i < buffer.length; ++i) {  
            let hex = buffer[i][2].map(byte => byte.toString(16).padStart(2, '0').toUpperCase()).join('');
            liatable += `| ${buffer[i][0]} | ${buffer[i][1]} | ${hex} |\n`;
        }

        send.lia( "LIASCRIPT: "+liatable );
    }

    window.connection.on('can-recv', function(data) 
    {
        addToBuffer( data );
        displayBuffer();
    });

    displayBuffer();

    "LIA: wait"
</script>


## Recviever

@WebSerial.defaultManager

@Dashboard.display

