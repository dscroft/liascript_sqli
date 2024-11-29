<!--
attribute: 
version:  0.0.1
language: en
narrator: UK English Female
title: Module Macros for Car dashboard
comment:  This is placeholder module to save macros used in other modules.

@version_history 

@end

script: assets/html5-canvas-speedometer/js/fraction.min.js
script: assets/html5-canvas-speedometer/js/speedometer.js

@onload
console.log("Loading dashboard module");

async function waitForDashboard() {
  while (!window.Dashboard) {
    await new Promise(resolve => setTimeout(resolve, 100)); // wait 100ms
  }
  // Once window.connection is available
  dashboardAvailable();
}

function dashboardAvailable() {
    console.log("Dashboard module loaded");
}

// Call this function to start the waiting process
waitForDashboard();

window.turnSignalsStates = {
        'left':  false,
        'right': false
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
window.tacho = 0.0;

@end

@Dashboard.display
<div id="speedometer" style="transform: scale(1.0); transform-origin: top left;">
<div style="display: none;"><img id="sprite" src="assets/html5-canvas-speedometer/assets/icons.svg"></div>
<canvas id="canvas" width="560" height="280"></canvas>
</div>

<script>
    setInterval(function()
    {
        try
        {
            window.Dashboard.draw( document.getElementById("canvas"), 
                window.speed, 
                window.tacho, 
                window.gas, 
                window.mileage, 
                window.turnSignalsStates, 
                window.iconsStates );
        }
        catch (error)
        {
            console.error("An error occurred:", error);
        }
        
    }, 1000/16);

"LIA: wait"
</script>
@end
-->

# Module Macros

## Dashboard

@Dashboard.display

<script>
    setInterval(function()
    {
        console.log( "Update" );

        try
        {
            window.mileage += 1;
            window.speed = (window.speed + 0.001) % 1;
            window.tacho = (window.tacho + 0.01) % 1;
            window.gas = (window.gas + 0.01) % 1;
        }
        catch (error)
        {
            console.error("An error occurred:", error);
        }
        
    }, 1000/10);
</script>