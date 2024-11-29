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

            // true if bit 6 of data is set
            window.turnSignalsStates.left = (data & (1 << 6)) !== 0;     
            window.turnSignalsStates.right = (data & (1 << 5)) !== 0;
        }   
        else if( msg[0] == 81 ) // GAS_PEDAL
        {
            window.tacho = msg[1][2] * 0.005;
        }
        else if( msg[0] == 20 ) // SEATS_DOORS
        {
            window.iconsStates.seatBelt = (msg[1][7] & (1 << 6)) !== 0;
            window.iconsStates.doors = (msg[1][5] & 0x3C) !== 0;
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

- Alice 👩 will be playing the part of the driver.

  - In this scenario it is a drive by wire vehicle, so the accelerator pedal is not physically linked to anything.
  - Pressing the pedal causes a signal to be sent to the engine control unit (ECU) to perform the appropriate action. 
- Bob 👨 will be playing the part of the vehicle.

  - We are just going to simulate the dashboard for this exercise.
  - But the same principles apply to the engine control unit (ECU) and other vehicle systems.
- Charlie 😈 will be playing a malicious attacker.

  - They have gained access to the CAN bus and can send and receive messages.

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



### 👩 Alice 

@WebSerial.defaultManager

**Indicators**

<input type="checkbox" id="left">
<input type="checkbox" id="right">

-------------------

**Headlights**

-------------------

**Doors**

<input type="checkbox" id="fl_door">
<input type="checkbox" id="fr_door">
<input type="checkbox" id="rl_door">
<input type="checkbox" id="rr_door">

-------------------

**Seatbelts**

<input type="checkbox" id="seatbelt">

-------------------

**Accelerator**

<div class="slidecontainer">
    <input type="range" min="0" max="200" value="0" id="accelerator"><span id="accelerator_level"></span>
</div>

-------------------

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
    try {
        window.connection.send( "can-send", [81, bytes] );
    } catch (error) {
        //console.error("An error occurred:", error);
    }
}

document.getElementById("accelerator").addEventListener("input", update_accel_status );
update_accel_status();
</script>

<script>
function update_door_status()
{
    console.log("Sending door message");

    /*BO_ 1568 SEATS_DOORS: 8 XXX
        SG_ SEATBELT_DRIVER_UNLATCHED : 62|1@0+ (1,0) [0|1] "" XXX
        SG_ DOOR_OPEN_FL : 45|1@0+ (1,0) [0|1] "" XXX
        SG_ DOOR_OPEN_RL : 42|1@0+ (1,0) [0|1] "" XXX
        SG_ DOOR_OPEN_RR : 43|1@0+ (1,0) [0|1] "" XXX
        SG_ DOOR_OPEN_FR : 44|1@0+ (1,0) [0|1] "" XXX*/
    let byte5 = ( document.getElementById('fl_door').checked ? (1 << 5) : 0 ) +
                ( document.getElementById('fr_door').checked ? (1 << 4) : 0 ) +
                ( document.getElementById('rl_door').checked ? (1 << 3) : 0 ) +
                ( document.getElementById('rr_door').checked ? (1 << 2) : 0 );
    let byte7 = document.getElementById('seatbelt').checked ? (1 << 6) : 0;
    let bytes = [ 0, 0, 0, 0, 0, byte5, 0, byte7 ];

    console.log( bytes );
    try {
        window.connection.send( "can-send", [20, bytes] );
    } catch (error) {
        //console.error("An error occurred:", error);
    }
}

document.getElementById('seatbelt').addEventListener('click', update_door_status);
document.getElementById('fl_door').addEventListener('click', update_door_status);
document.getElementById('fr_door').addEventListener('click', update_door_status);
document.getElementById('rl_door').addEventListener('click', update_door_status);
document.getElementById('rr_door').addEventListener('click', update_door_status);
</script>

<script>
function update_light_status()
{
  /*BO_ 1570 LIGHT_STALK: 8 SCM
      SG_ AUTO_HIGH_BEAM : 37|1@0+ (1,0) [0|1] "" XXX
      SG_ FRONT_FOG : 27|1@0+ (1,0) [0|1] "" XXX
      SG_ PARKING_LIGHT : 28|1@0+ (1,0) [0|1] "" XXX
      SG_ LOW_BEAM : 29|1@0+ (1,0) [0|1] "" XXX
      SG_ HIGH_BEAM : 30|1@0+ (1,0) [0|1] "" XXX
      SG_ DAYTIME_RUNNING_LIGHT : 31|1@0+ (1,0) [0|1] "" XXX*/
}
</script>

<script>
let sendSignalMsg = function()
{
    console.log("Sending signal message");

    let byte0 = ( document.getElementById('left').checked ? (1 << 6) : 0 ) +
                ( document.getElementById('right').checked ? (1 << 5) : 0 );
    let bytes = [ byte0, 0, 0, 0, 0, 0, 0, 0 ];

    try {
      window.connection.send( "can-send", [203, bytes] );
    }
    catch (error) {
        //console.error("An error occurred:", error);
    }
}

document.getElementById('left').addEventListener('click', sendSignalMsg);
document.getElementById('right').addEventListener('click', sendSignalMsg);
</script>

### 👨 Bob 

@WebSerial.defaultManager

@Dashboard.display

### 😈 Charlie 

#### Intercept

@WebSerial.defaultManager

<div id="framesLogger"></div>

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


#### Retransmit

@WebSerial.defaultManager

<label>CAN Frame ID: </label><input class="lia-quiz__input" type="text" id="can_frame_id" placeholder="123">
<label>CAN Data: </label><input class="lia-quiz__input" type="text" id="can_frame_data" placeholder="A1B2C3D4E5F6">
<label>Duration: </label><span id="duration"></span><input type="range" min="1" max="30" value="1" id="can_frame_duration">
<label>Rate: </label><span id="hz"></span><input type="range" min="1" max="100" value="1" id="can_frame_hz">

<script>
  function update_frame_duration()
  {
    let value = document.getElementById("can_frame_duration").value;
    document.getElementById("duration").innerHTML = value + " second/s";
  }

  function update_frame_hz()
  {
    let value = document.getElementById("can_frame_hz").value;
    document.getElementById("hz").innerHTML = value + " Hz";
  }

  document.getElementById("can_frame_duration").addEventListener("input", update_frame_duration);
  update_frame_duration();

  document.getElementById("can_frame_hz").addEventListener("input", update_frame_hz);
  update_frame_hz();

</script>

<script input="submit" default="Send">
  let id = parseInt(document.getElementById("can_frame_id").value);
  let data = document.getElementById("can_frame_data").value.split(' ').map(byte => parseInt(byte, 16));

  let duration = parseFloat(document.getElementById("can_frame_duration").value);
  let hz = parseFloat(document.getElementById("can_frame_hz").value);
  let num = duration * hz;

  send.lia("Sending "+num+" messages");

  let interval = setInterval(() => {
    //window.connection.send("can-send", [id, data]);
    num -= 1;
    send.lia( "Messages remaining: "+num );

    
    if (num <= 0) {
      clearInterval(interval);
      send.lia("Done");
    }
  }, 1000 / hz);


  //console.log( "Sending CAN frame: "+id+" "+data );
</script>





