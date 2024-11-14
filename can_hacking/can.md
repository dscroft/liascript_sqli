<!--
author:   André Dietrich
email:    LiaScript@web.de
version:  0.3.2
language: en
narrator: US English Male

logo:     logo.jpg

import: can_hacking/macros_webserial.md
-->

# aaa

## Connect 

@WebSerial.defaultManager

<span id="data"></span>

<script>
    window.connection.on("arduino-alive",function(value){
        send.output(value);
    });

    "LIA: wait"
</script>


## Page 2

@WebSerial.defaultManager

Other contents

