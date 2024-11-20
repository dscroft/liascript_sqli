<!--
author:   André Dietrich
email:    LiaScript@web.de
version:  0.3.2
language: en
narrator: US English Male

logo:     logo.jpg

import: macros_webserial.md
import: macros_dashboard.md
-->

# aaa


## Send

@WebSerial.defaultManager

Indicators

<input type="checkbox" id="left">
<input type="checkbox" id="right" value="1">

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


## Recv

@WebSerial.defaultManager

<div id="speedometer" style="transform: scale(1.2); transform-origin: top left;">
<div style="display: none;"><img id="sprite" src="http://localhost:8000/_can_hacking/html5-canvas-speedometer/assets/icons.svg"></div>
<canvas id="canvas" width="425" height="210"></canvas>
</div>

<script>
    setInterval(function()
    {
        try
        {
            window.update_dash();
        }
        catch (error)
        {
            console.error("An error occurred:", error);
        }
        
    }, 1000/16);

"LIA: wait"
</script>

<script>
    window.connection.on('can-recv', function(msg) 
    {
       
        if( msg[0] == 203 ) // SCM_FEEDBACK
        {   
            console.log( "message" );
            // data[1] is an array of multiple integers where each element represents the value of a byte
            // convert into a single integer where the lsb were contained in element 0
            let data = 0;
            for (let i = 0; i < msg[1].length; i++) {
                data |= msg[1][i] << (8 * i);
            }
            console.log( data );

            // true if bit 6 of data is set
            window.turnSignalsStates.left = (data & (1 << 6)) !== 0;     
            window.turnSignalsStates.right = (data & (1 << 5)) !== 0;     

            console.log( window.turnSignalsStates.left );
        }        
    });

    //"LIA: wait"
</script>