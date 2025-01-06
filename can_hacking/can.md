<!--
author:   David Croft
email:    david.croft@warwick.ac.uk
version:  0.1.0
language: en
narrator: US English Male

classroom: false
icon: https://dscroft.github.io/liascript_materials/assets/logo.svg

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
            window.turnSignalsStates.left = (msg[1][0] & (1 << 6)) !== 0;     
            window.turnSignalsStates.right = (msg[1][0] & (1 << 5)) !== 0;
        }   
        else if( msg[0] == 81 ) // GAS_PEDAL
        {
            window.tacho = msg[1][2] * 0.005;
        }
        else if( msg[0] == 20 ) // SEATS_DOORS
        {
            console.log("seats_doors");
            
            // oh god this code is terrible, figure out javascript conditional logic at some point
            window.iconsStates.seatBelt = (msg[1][7] & (1 << 6)) !== 0 ? 1 : 0;
            window.iconsStates.doors = (msg[1][5] & (1 << 5)) !== 0 || 
                                       (msg[1][5] & (1 << 4)) !== 0 ||
                                       (msg[1][5] & (1 << 3)) !== 0 ||
                                       (msg[1][5] & (1 << 2)) ? 1 : 0;
        }
        else if( msg[0] == 40 ) // LIGHT_STALK
        {
            console.log("light stalk");
            window.iconsStates.dippedBeam = (msg[1][0] & (1 << 5)) !== 0 ? 1 : 0;
            window.iconsStates.highBeam = (msg[1][0] & (1 << 6)) !== 0 ? 1 : 0;
        }
    });
}

// Call this function to start the waiting process
waitForConnection();
@end

@can.intercept

<script style="display: block" modify="false">
    if( window.buffer ) {}
    else
        window.buffer = [];
    
    let bufferMaxSize = 10;

    function addToBuffer(line) 
    {
        buffer.push([Date.now(), ...line]);
        if (buffer.length > bufferMaxSize) 
            buffer.shift();
    }

    function displayBuffer()
    {
        let liatable =  "<!-- data-type='none' \n" +
                        "     data-title='Recived CAN frames' \n" + 
                        "     data-sortable='false' -"+"->\n" + // bodge for macro parser
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
@end

@can.retransmit
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

  let data = document.getElementById("can_frame_data").value.toUpperCase();
  if( data.startsWith("0X") ) data = data.slice(2);
  data = data.match(/.{2}/g).map(byte => parseInt(byte, 16));

  let duration = parseFloat(document.getElementById("can_frame_duration").value);
  let hz = parseFloat(document.getElementById("can_frame_hz").value);
  let num = duration * hz;

  send.lia("Sending "+num+" messages");

  let interval = setInterval(() => {
    window.connection.send("can-send", [id, data]);
    num -= 1;
    send.lia( "Messages remaining: "+num );

    
    if (num <= 0) {
      clearInterval(interval);
      send.lia("Done");
    }
  }, 1000 / hz);

</script>
@end

@can.alice
**Indicators**

<!-- data-type='none'
     data-sortable='false' -->
| Left | Right |
| :-: | :-: |
| <input type="checkbox" id="left"> | <input type="checkbox" id="right"> |

-------------------

**Headlights**

<!-- data-type='none'
     data-sortable='false' -->
| Off | Dipped | Full beam |
| :-: | :-: | :-: |
| <input type="radio" name="headlights" value="off"> | <input type="radio" name="headlights" value="low"> | <input type="radio" name="headlights" value="high"> |

-------------------

**Doors Open**

<!-- data-type='none'
     data-sortable='false' -->
| Front Left | Front Right | Rear Left | Rear Right |
| :-: | :-: | :-: | :-: |
| <input type="checkbox" id="fl_door"> | <input type="checkbox" id="fr_door"> | <input type="checkbox" id="rl_door"> | <input type="checkbox" id="rr_door"> |

-------------------

**Driver seatbelt sensor **

<input type="checkbox" id="seatbelt">

-------------------

**Accelerator**

<div class="slidecontainer">
    <input type="range" min="0" max="200" value="0" id="accelerator"><span id="accelerator_level"></span>
</div>

-------------------

<script> <!-- accelerator -->
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
        console.log( bytes );
        window.connection.send( "can-send", [81, bytes] );
    } catch (error) {
        //console.error("An error occurred:", error);
    }
}

document.getElementById("accelerator").addEventListener("input", update_accel_status );
update_accel_status();
</script>

<script> <!-- doors -->
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

<script> <!-- headlights -->
let sendLightStatus = function()
{
  /*BO_ 1570 LIGHT_STALK: 8 SCM
      SG_ AUTO_HIGH_BEAM : 37|1@0+ (1,0) [0|1] "" XXX
      SG_ FRONT_FOG : 27|1@0+ (1,0) [0|1] "" XXX
      SG_ PARKING_LIGHT : 28|1@0+ (1,0) [0|1] "" XXX
      SG_ LOW_BEAM : 29|1@0+ (1,0) [0|1] "" XXX
      SG_ HIGH_BEAM : 30|1@0+ (1,0) [0|1] "" XXX
      SG_ DAYTIME_RUNNING_LIGHT : 31|1@0+ (1,0) [0|1] "" XXX*/

    console.log("Sending light message");

    let selectedHeadlight = document.querySelector('input[name="headlights"]:checked').value;

    console.log( selectedHeadlight );

    let byte0 = ( selectedHeadlight == 'auto' ? (1 << 1) : 0 ) +
                ( selectedHeadlight == 'low' ? (1 << 5) : 0 ) +
                ( selectedHeadlight == 'high' ? (1 << 6) : 0 );
    let bytes = [ byte0, 0, 0, 0, 0, 0, 0, 0 ];

    try
    {
        console.log( bytes );
        window.connection.send( "can-send", [40, bytes] );
    }
    catch (error)
    {
        //console.error("An error occurred:", error);
    }
}

document.querySelectorAll('input[name="headlights"]').forEach(element => {
  element.addEventListener('change', sendLightStatus);
});
</script>

<script> <!-- indicators -->
let sendSignalMsg = function()
{
    console.log("Sending signal message");

    let byte0 = ( document.getElementById('left').checked ? (1 << 6) : 0 ) +
                ( document.getElementById('right').checked ? (1 << 5) : 0 );
    let bytes = [ byte0, 0, 0, 0, 0, 0, 0, 0 ];

    try {
      console.log( bytes );
      window.connection.send( "can-send", [203, bytes] );
    }
    catch (error) {
        //console.error("An error occurred:", error);
    }
}

document.getElementById('left').addEventListener('click', sendSignalMsg);
document.getElementById('right').addEventListener('click', sendSignalMsg);
</script>
@end

-->

# Introduction

This activity is designed to demonstrate how a malicious attacker can intercept and retransmit CAN frames on a CAN bus.

This sort of attack should not work in a modern vehicle but certainly was possible in older vehicles and is still possible in other systems that use CAN.

------------------------------

This activity works best in groups of 3, ideally every member will have their own computer.

In the event that there are insufficient participants or computers then groups of 2 or 4 will also work, individuals on their own will struggle.

------------------------------

You can navigate through the activity using:

- The arrow buttons at the bottom of the page.
- The arrow keys on your keyboard.
- The navigation bar on the left.

-----------------------------

<!--
style="background-color: firebrick; color: white"
-->
>⚠️**Warning**
>
> For the practical part of the activity you will need to be using a reasonably up to date version of Chrome, Edge or Opera.
>
> - Smartphone and tablet browsers generally will not work.


# CAN data format

Over the next couple of pages we will be looking at the format of CAN data and how it can be represented in a more human readable format.

## DBC Format

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

## Encoding information

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

These instructions are written on the assumption that you are working as part of a group with 3 computers, each computer fills a specific role.

These roles will be referred to as Alice, Bob and Charlie.

- Alice 👩 will be playing the part of the driver.

  - In this scenario it is a drive by wire vehicle, so the accelerator pedal is not mechanically linked to anything.
  - Pressing the pedal causes a signal to be sent to the engine control unit (ECU) to perform the appropriate action. 
- Bob 👨 will be playing the part of the vehicle.

  - We are just going to simulate the Instrument Cluster (IC) for this exercise.
    
    - In a traditional vehicle this may be handled by a dedicated microcontroller.
    - In more modern vehicle or in the future this may be handled by a container process running on a more general purpose HPC (High Performance Computer).

  - But the same principles apply to the engine control unit (ECU) and other vehicle systems.
- Charlie 😈 will be playing a malicious attacker.

  - They have gained access to the CAN bus and can send and receive messages.


{{1}}
> **Decide in your groups who will be Alice, Bob and Charlie.**


<!--
style="background-color: firebrick; color: white"
-->
>⚠️**Warning**
>
> Make sure to pay attention to the task numbers as some tasks will require you to you to do things at the same time as other group members.

```ascii

💻👩   💻😈   💻👨
 |      |      | 
 *------*------*
```

## Hardware setup

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




{{2}}
> **Click on the connect button below, or on the other pages to connect to the CAN bus.**
>
> - When pressed should get a pop-up list that allows you to select the appropriate device.
>
>   - Look for an option similar to "USB Serial Device (COMx)".
> - Make sure that you are **Connected**<!-- style="color: green;" --> to the bus.

@WebSerial.defaultManager

{{3}}
> **Go to the page that corresponds to your role.**
>
> - Alice 👩, Bob 👨 or Charlie 😈.
> 
>   - You can use the navigation bar on the left or the arrow buttons below.






## 👩 Alice 

@WebSerial.defaultManager

{{4}}
> **Use the controls below to simulate driving the vehicle.**
>
> - When you interact with the controls, the corresponding CAN frames will be sent to the CAN bus.
>
>   - These frames will be picked up by the other devices on the bus e.g. Bob.
>   - These CAN frames are based on *real* frames. E.g. the accelerator pedal frame was taken from a 2010 Toyota Prius.
>
> <script input="submit" default="Press for hint">
"Try using the indicators."
</script>

{{5}}
> **Confirm with Bob 👨 that the instrument cluster is responding to your controls.**

{{6}}
> **Work with Charlie 😈 to identify the CAN frames that are being sent.**

{{7}}
> **Wait while Charlie 😈 sends some CAN frames to the bus.**

{{8}}
> **Check to see if you still have control of the vehicle.**
>
> <script input="submit" default="Press for hint">
"Try using the accelerator."
</script>

{{9}}
> **While Charlie 😈 sends some CAN frames to the bus.**
>
> - Try and control the vehicle.

{{10}}
> **Discuss with Bob 👨 and Charlie 😈 what is happening.**
>
> - What is the effect of the frames that Charlie 😈 is sending?

@can.alice



## 👨 Bob 

@WebSerial.defaultManager

{{5}}
> **Check that the IC is responding correctly to the CAN frames that Alice is sending.** 
>
> <script input="submit" default="Press for hint">
"Check the indicators."
</script>

{{6}}
> **Work with Charlie 😈 to identify the CAN frames that are being sent.**

{{7}}
> **Wait while Charlie 😈 sends some CAN frames to the bus.**

{{8}}
> **Check that the IC is responding to the frames that Charlie 😈 is sending.**
>
> <script input="submit" default="Press for hint">
"Are both indicators on?"
</script>

{{9}}
> **While Charlie 😈 sends some CAN frames to the bus.**
>
> - Check to see if Alice 👩 still has control of the vehicle.

{{10}}
> **Discuss with Alice 👩 and Charlie 😈 what is happening.**
>
> - What is the effect of the frames that Charlie 😈 is sending?

@Dashboard.display


## 😈 Charlie 

Charlie's role has two parts:

1. Interception.
2. Retransmission.

### Interception

@WebSerial.defaultManager

----------------------

The Intercept table will show the CAN frames that are being sent and received on the CAN bus.

In this case the information that is being sent between Alice and Bob.
This is the first step in a man-in-the-middle attack.

For a CAN bus this is passive action and does not need to be literally in the middle.
As long as Charlie is on the same CAN bus, they can see all the messages being sent.

{{5}}
> **Try and identify the CAN frames that correspond to the actions that Alice 👩 is taking.**
>
> - Importantly, we don't need to decode or understand the CAN data, just identify which frames correspond to which actions.
>
>   - You may want to make notes.
>
> <script input="submit" default="Press for hint">
"E.g. Every time the indicators are used, a frame with ID 203 is intercepted."
</script>

@can.intercept



### Retransmission

@WebSerial.defaultManager

Using the form below, Charlie can transmit arbitrary CAN frames to the CAN bus.

{{7}}
> **Send a CAN frame**
>
> - Use the information from one of the frames that you intercepted previously.
>
> <script input="submit" default="Press for hint">
document.getElementById("can_frame_id").value = 203;
document.getElementById("can_frame_data").value = "0x2000000000000000";
document.getElementById("can_frame_duration").value = 1;
document.getElementById("can_frame_hz").value = 1;

"Send a frame to turn both indicators on."
</script>

{{8}}
> **Check with Bob 👨 that the IC is responding to your retransmitted CAN frames.**
>
> - Sending a single frame could have a noticeable effect on the vehicle.
>
>   - E.g. unlocking doors, turning on lights, etc.
> - More subtle attacks could be to send a frame that causes the dashboard to display incorrect information.
> <script input="submit" default="Press for hint">
"Both indicators should have turned on."
</script>

{{9}}
> **Send multiple frames**
>
> - Use the form below to send multiple frames.
> 
>   - Adjust the Duration and Rate values to control how many frames are sent and how quickly.
> - By "flooding" the CAN bus with frames we can overwhelm or drown out legitimate frames.
> 
> <script input="submit" default="Press for hint">
document.getElementById("can_frame_id").value = 81;
document.getElementById("can_frame_data").value = "0x0000C80000000000";
document.getElementById("can_frame_duration").value = 30;
document.getElementById("can_frame_hz").value = 100;

"Flooding the bus with frames saying the accelerator is fully pressed."
</script>

{{10}}
> **Discuss with Alice 👩 and Bob 👨 what is happening.**
>
> - What is the effect of the frames that you are sending?

@can.retransmit



# Summary

In this activity you have seen how a malicious attacker can intercept and retransmit CAN frames.

- This is an example of a replay attack where the attacker was able to capture real data and then replay it to the vehicle.
  - Another example of a replay attack would be to capture the signal from the key fob and then replay it to unlock the vehicle.

CAN is extremely vulnerable to these types of attacks as it was designed to be simple and robust, not secure.

- It pre-dates the kind of connectivity present in modern vehicles.


## Mitigation

There are a number of ways to mitigate or limit the efficacy of these types of attacks on CAN networks.

Other network technologies such as Ethernet have improved security due to a combination of designed in security features and inherent structure.

- But come at both a complexity and financial cost.

--------------------------

1. **Message Authentication Codes (MACs)**

    - Use cryptographic techniques to ensure the authenticity of messages.
    - Helps in verifying that the message has not been altered.

2. **Encryption**

    - Encrypt CAN messages to prevent unauthorized access and tampering.
    - Ensure that only authorized nodes can decrypt and understand the messages.

3. **Rolling Codes**

    - Use rolling codes for critical commands to prevent replay attacks.
    - Each command is valid only once, making it difficult for attackers to reuse captured messages.

4. **Intrusion Detection Systems (IDS)**

    - Implement IDS to monitor CAN traffic for anomalies.
    - Detect and alert on suspicious activities or patterns that indicate an attack.

5. **Segmentation**

    - Segment the CAN network into smaller, isolated sections.
    - Limit the impact of a compromised node by containing the attack within a segment.

6. **Rate Limiting**

    - Implement rate limiting to control the frequency of messages sent on the CAN bus.
    - Prevent attackers from flooding the network with malicious messages.

7. **Message Filtering**

    - Use message filters to allow only legitimate messages to be processed.
    - Discard any messages that do not match predefined criteria.

-----------------------

By implementing these methods, the security of CAN networks can be significantly enhanced, reducing the risk of attacks and ensuring the integrity and reliability of the communication system.
