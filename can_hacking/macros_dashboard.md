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
        'dippedBeam': 0,
        'brake':      0,
        'drift':      0,
        'highBeam':   0,
        'lock':       0,
        'seatBelt':   0,
        'engineTemp': 0,
        'stab':       0,
        'abs':        0,
        // right circle
        'gas':        0,
        'trunk':      0,
        'bonnet':     0,
        'doors':      0,
        // left circle
        'battery':    0,
        'oil':        0,
        'engineFail': 0
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
    window.dashboard_refresh = setInterval(function()
    {
        let change = ( window.tacho - 0.2 ) * 0.01;
        window.speed = Math.max( 0, Math.min( 1, window.speed + change ) );

        if( !document.getElementById("canvas") )
        {
            clearInterval( window.dashboard_refresh );
            return;
        }

        try
        {
            window.Dashboard.draw( document.getElementById("canvas"), 
                window.speed * 0.95 + ( 0.005 * Math.random() -0.0025 ), 
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

    console.log( "init" );
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