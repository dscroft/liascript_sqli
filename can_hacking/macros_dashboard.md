<!--
attribute: 
version:  0.0.1
language: en
narrator: UK English Female
title: Module Macros for Car dashboard
comment:  This is placeholder module to save macros used in other modules.

@version_history 

@end

script: http://localhost:8000/_can_hacking/html5-canvas-speedometer/js/fraction.min.js
script: http://localhost:8000/_can_hacking/html5-canvas-speedometer/js/speedometer2.js

@onload
window.turnSignalsStates = {
        'left':  true,
        'right': true
    }
    
window.iconsStates = {
        // main circle
        'dippedBeam': 1,
        'brake':      1,
        'drift':      1,
        'highBeam':   1,
        'lock':       1,
        'seatBelt':   1,
        'engineTemp': 2,
        'stab':       1,
        'abs':        1,
        // right circle
        'gas':        2,
        'trunk':      1,
        'bonnet':     1,
        'doors':      1,
        // left circle
        'battery':    2,
        'oil':        2,
        'engineFail': 2
    }

window.speed = 0.0;
window.gas = 0.5;
window.mileage = 12345;
window.tacho = 0.5;

window.update_dash = function()
{
    window.Dashboard.draw( document.getElementById("canvas"), window.speed, window.tacho, window.gas, window.mileage, window.turnSignalsStates, window.iconsStates );
}
@end
-->

# Module Macros

## Controller

<input type="checkbox" id="left">
<input type="checkbox" id="right" value="1">

<script>
let sendSignalMsg = function()
{
    console.log("Sending signal message");

    let data = ( document.getElementById('left').checked ? (1 << 6) : 0 ) +
               ( document.getElementById('right').checked ? (1 << 5) : 0 );

    window.connection.send( "can-send", [660, data] );
}

document.getElementById('left').addEventListener('click', sendSignalMsg);
document.getElementById('right').addEventListener('click', sendSignalMsg);
</script>

## Dashboard

<div id="speedometer">
<div style="display: none;"><img id="sprite" src="http://localhost:8000/_can_hacking/html5-canvas-speedometer/assets/icons.svg"></div>
<canvas id="canvas" width="425" height="210"></canvas>
</div>

<script>
    setTimeout(function()
    {
        if( window.Dashboard === undefined )
            send.lia( "Waiting for Dashboard module to load" );
        else
        {
            window.update_dash();
            "LIA: clear"
        }
    }, 1000/16);

    window.connection.on('can-recv', function(data) 
    {
        if( data[0] == 660 ) // ENGINE_DATA
        {
            window.turnSignalsStates.left = data[1] & (1 << 6);
            window.turnSignalsStates.right = data[1] & (1 << 5);
        }
        
    });

    "LIA: wait"
</script>
