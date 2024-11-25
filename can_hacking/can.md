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

# CAN hacking activity

This activity works best in groups of 3, ideally every member will have their own computer.

In the event that there are insuffient participants or computers then groups of 2 or 4 will also work, individuals on their own will struggle.

<!--
style="background-color: firebrick; color: white"
-->
>⚠️**Warning**
>
> For the practical part of the activity you will need to be using a reasonably up to date version of Chrome, Edge or Opera.
>
> - Smartphone and tablet browsers generally will not work.


## Format

### DBC format

Various ways to represent CAN frame structure, we are going to use DBC.

For example: the accelerator pedal position information for a 2010 Toyota Prius could be represented as shown below.

```ascii
Frame ID     Frame Name
     \         /       .------ Frame Length (in bytes)
      \       /       /              Max value
       v      V       V                  |
   BO_ 81 GAS_PEDAL: 8 XXX               V
    SG_ GAS_PEDAL : 23|8@0+ (0.005,0) [0|1] '' XXX
           ^        ^ ^  ^ ^    ^   ^   ^ 
          /        / /   |  \    \   \   \
Signal name       / /    |   \    \   \   Min value
      Starting bit /     |    \    \  Offset
        Length (in bits) |     \  Scaling factor
                         |  Signed/Unsigned
                Motorola/Intel Format 
```                        

What this specifies is that accelerator pedal position will be transmitted as a value between 0 and 200, stored in bits 23 to 16 and that pedal position can be sent in 0.5% increments.

<details>
<summary>**Umm, actually...**</summary>

> In reality the frame ID is 581 but for the sake of simplicity for this task we are using classic CAN and so have to keep our frame IDs <256
</details>

### Encoding information

An accelerator pedal pressed three quarters down would have a value of 0.75.

Below is our 8 byte CAN frame, for the pedal position we set bits 23-16.

<!--
style="
  max-width: 600px;" -->
```ascii
         +----+----+----+----+----+----+----+----+
       0 |  7 |  6 |  5 |  4 |  3 |  2 |  1 |  0 |
         +----+----+----+----+----+----+----+----+
       1 | 15 | 14 | 13 | 12 | 11 | 10 |  9 |  8 |
         ╔════╦════╦════╦════╦════╦════╦════╦════╗
       2 ║ 23 ║ 22 ║ 21 ║ 20 ║ 19 ║ 18 ║ 17 ║ 16 ║
         ╚════╩════╩════╩════╩════╩════╩════╩════╝
Bytes  3 | 31 | 30 | 29 | 28 | 27 | 26 | 25 | 24 |
         +----+----+----+----+----+----+----+----+
       4 | 39 | 38 | 37 | 36 | 35 | 34 | 33 | 32 |
         +----+----+----+----+----+----+----+----+
       5 | 47 | 46 | 45 | 44 | 43 | 42 | 41 | 40 |
         +----+----+----+----+----+----+----+----+
       6 | 55 | 54 | 53 | 52 | 51 | 50 | 49 | 48 |
         +----+----+----+----+----+----+----+----+
       7 | 63 | 62 | 61 | 60 | 59 | 58 | 57 | 56 |
         +----+----+----+----+----+----+----+----+
```

Apply the scaling factor 0.75 / 0.005 = 150, that's 0x96 in hexadecimal or 10010110 in binary.

Which appears as shown below:

<!--
style="
  max-width: 600px;" -->
```ascii
         +----+----+----+----+----+----+----+----+
       0 |    |    |    |    |    |    |    |    |
         +----+----+----+----+----+----+----+----+
       1 |    |    |    |    |    |    |    |    |
         ╔════╦════╦════╦════╦════╦════╦════╦════╗
       2 ║  1 ║  0 ║  0 ║  1 ║  0 ║  1 ║  1 ║  0 ║
         ╚════╩════╩════╩════╩════╩════╩════╩════╝
Bytes  3 |    |    |    |    |    |    |    |    |
         +----+----+----+----+----+----+----+----+
       4 |    |    |    |    |    |    |    |    |
         +----+----+----+----+----+----+----+----+
       5 |    |    |    |    |    |    |    |    |
         +----+----+----+----+----+----+----+----+
       6 |    |    |    |    |    |    |    |    |
         +----+----+----+----+----+----+----+----+
       7 |    |    |    |    |    |    |    |    |
         +----+----+----+----+----+----+----+----+
```

Assuming that there was no other information being sent in this frame, the complete message could look something like:

0000000000000000100101100000000000000000000000000000000000000000

But as that's quite unwheldly it's not common to represent the information in hexademical (base 16) format. 
In which case it appears as 0x0000960000000000


# Activity 

These instructions are written on the assumption that you are working as part of a group of 3 as there are 3 roles that need to be filled.

These roles will be referred to as Alice, Bob and Charlie.

- Alice 👩 will be controlling the vehicle by sending CAN frames.
- Bob 👨 will be monitoring the vehicle and seeing that the CAN frames are recieved.
- Charlie 😈 will be attempting to manipulate the vehicle.

```ascii

💻👩   💻😈   💻👨
 |      |      | 
 *------*------*
```

#### Hardware setup

This is the setup we are trying to achieve. 

- Each group member connected via USB to one of the Arduino circuit boards.
- The Arduino board connected via CAN into a simple CAN bus.
- The CAN bus terminated by appropriate resistors at each end.

```ascii
             .-------------------. .-------------------.
+--------+   |      +--------+   | |      +--------+   |
|        |   +-.    |        |   | |      |        |   +-.  
|   CANL o <-+ |    |   CANL o <-.-.      |   CANL o <-+ |
|        |     #    |        |            |        |     #
|        |     #    |        |            |        |     # Resistor
|        |     #    |        |            |        |     #
|   CANH * <-+ |    |   CANH * <-.-.      |   CANH * <-+ |
|USB     |   +-.    |USB     |   | |      |USB     |   +-.
+-#------+   |      +-#------+   | |      +-#------+   |
  |          |        |          | |        |          |
 💻👩        |       💻😈        | |       💻👨        |
             .-------------------. .-------------------.
```           



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
                        "| Timestamp | CAN Frame ID | Data |\n" +
                        "|-|-|------|\n"

        for (let i = 0; i < buffer.length; ++i) {  
            let hex = buffer[i][2].map(byte => byte.toString(16).padStart(2, '0').toUpperCase()).join('');
            liatable += `| ${buffer[i][0]} | ${buffer[i][1]} | 0x${hex} |\n`;
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

