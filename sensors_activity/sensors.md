<!--
author:   David Croft
email:    david.croft@warwick.ac.uk
version:  0.1.0
language: en
narrator: US English Male

classroom: false
icon: https://dscroft.github.io/liascript_materials/assets/logo.svg

@style
.flex-container {
    display: flex;
    flex-wrap: wrap; /* Allows the items to wrap as needed */
    align-items: stretch;
    gap: 20px; /* Adds both horizontal and vertical spacing between items */
}

.flex-child { 
    flex: 1;
    margin-right: 20px; /* Adds space between the columns */
}

@media (max-width: 600px) {
    .flex-child {
        flex: 100%; /* Makes the child divs take up the full width on slim devices */
        margin-right: 0; /* Removes the right margin */
    }
}
@end

-->

# Sensor Metrics and Characterisation

![](assets/image3.png)



## Robot fight?

!?[Robot fight?](https://youtu.be/KwDUMVrHdZg?t=203)



## Learning Outcomes

By the end of this session you will be able to understand:

- How to use (basic) sensor(s) to implement navigation features (line following).
- How to access sensor data.
- How to characterise sensors.
- How sensor limitations can affect function performance.
- Sensor metrics and its measurement.

<!--
style="background-color: firebrick; color: white"
-->
>⚠️**Warning**
>
> Take detailed notes and plots/pictures of your testing activities.
>
> - IMA related!



## Outline

- Learn LEGO EV3 vehicles hardware
- Learn LEGO LEGO EV3 software as well as some basic block programming steps
- Learn LEGO EV3 sensor data logging
- Activities


# LEGO EV3 vehicles hardware

LAB 01 Sensor
Characterisation
LEGO Town

LEGO EV3 vehicles
hardware



## Sensors

![](assets/image6.png)



## EV3 Brick

[EV3 userguide](https://le-www-live-s.legocdn.com/ev3/userguide/1.4.0/ev3_userguide_enus.pdf)

![](assets/image7.png)



## Vehicle Chosen

1. The 2 Large Servo Motors are connected to the wheels of the vehicle.
2. The Medium Servo Motor is connected to the carriage arm at the front of the vehicle.
3. The Colour Sensor is at the front of the vehicle, pointing downwards.
4. The Touch Sensor is at the front of the vehicle facing forward.
5. The Ultrasonic Sensor is at the front of the vehicle.
6. The EV3 Brick is at the top of the vehicle.
7. The Gyro Sensor is mounted on top of the EV3 Brick.
8. The Infrared Sensor is mounted as the 'front face' of the vehicle.

![](assets/image8.png "Vehicle configuration")

<!-- style="display:none" -->
TODO: add numbers to the image.



### Gyro Sensor

![](assets/image10.png "Gyro Sensor")

| Maximum Speed          | Accuracy                             |
| ---------------------- | ------------------------------------ |
| 440 degrees per second | (+/-) 3 degrees for a 90 degree turn |




### Colour Sensor 

Also used as Light Sensor.

![](assets/image12.png "Colour Sensor")

| Mode                      | Description                                                             | Specification                                          | Notes                                                  |
| ------------------------- | ----------------------------------------------------------------------- | ------------------------------------------------------ | ------------------------------------------------------ |
| Colour                    | Recognises 7 colours                                                    | Black, blue, green, yellow, red, white and brown       | Must be held close to the surface and at a right angle |
| Reflected Light Intensity | Measures intensity of a red light reflected back.                       | Scale of 0 to 100                                      | Must be held close to the surface and at a right angle |
| Ambient Light Intensity   | Measures intensity of light from the environment that enters the window | Sample rate of 1kHz/sec                                |                                                        |



### Ultrasonic Sensor

![](assets/image13.png "Ultrasonic Sensor")

| Description                                                           | Detectable Distance  | Accuracy  | Notes                                                                    |
| --------------------------------------------------------------------- | -------------------- | --------- | ------------------------------------------------------------------------ |
| Distance to an object can be measured in either inches or centimeters | Between 3cm to 250cm | (+/-) 1cm | Presence mode detects other ultrasound sources but does not transmit any |



### Infrared Sensor

![](assets/image15.png "Infrared Sensor")

| Mode      | Description                                                                                                                                                             | Working Distance |
| --------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------------- |
| Proximity | Light waves reflected back are used to estimate the distance to the object. It reports the distance as values between 0 (very close) to 100 (far away).                 | Up to 70cm       |
| Beacon    | The sensor will detect a beacon signal matching one of four channels that you specify. Direction shows values between -25 and 25 whilst proximity is between 0 and 100. | Up to 200cm      |



### Touch Sensor

A simple on/off button. Can be used to allow human interference in running of the programme or as a trigger.

![](assets/image17.png "Touch Sensor")



### EV3 Brick

- The EV3 Brick contains an ARM 9 processor.
- 4 Inputs (for reading and recording sensor information) and 4 Outputs (for driving the motors).
- It also has a speaker on board which can be programmed in EV3.
- Can connect using USB, or with the use of WIFI or Bluetooth dongles.
- On board memory storage to log sensor data.

![](assets/image18.png "EV3 Brick")


## Possible Languages

- EV3 - default block programming suite (later in the slides)
- C Language/ROBOT C
- Java
- Python
- Scratch
- MATLAB & Simulink



## LEGO EV3 software

## Introduction to EV3

- Launch LEGO
MINDSTORMS
Education EV3

17

## Introduction to EV3

- Click on 'File'.>'New
Project'.>'Program.'.
. 18. ..
## Action Blocks (Green).

- These bricks allow you to execute various actions
through code. With these blocks, you can control
motor rotations (of both the medium and the
large servo motors) as well as image, sound and
light on the EV3 Brick.

19

## Flow Blocks (Orange)

- These blocks control the flow of your program.
- All programs will begin with the Start Block.
- You can also use these blocks to create different
types of loops with conditions set by sensor data.

20

## Sensor Blocks (Yellow)

- These are the blocks you need to use to read

data from the various sensors available to you.
- Once you lay down the block, you can alter the

settings of the block
- for example, you can set the Ultrasonic Sensor to
detect objects, or to measure the actual distance
from the sensor to an object.

21

## Data Operation Blocks (Red)

- The data operation blocks will allow you to read,
write and compare variables and constants. You
can use these blocks to carry out simple
arithmetic operations and Boolean operations
amongst several others.

22

## Advanced Blocks (Dark Blue)

- These allow manage different files, establish
connections via Bluetooth, log data, leave
comments, and more.

23

# Data logging

Data logging can be crucial in order to understand why and how a particular autonomous vehicle behaves the way it does
It helps to verify whether the software and hardware are behaving in the intended manner

- There are 4 main ways of data logging using the EV3 MINDSTORMS:

1. Live Data Logging
2. Live Data Reading
3. Remote Data Logging
4. Brick Data Logging
5. Autonomous Data Logging

The original official EV3 software is no longer supported due to the Microsoft Silverlight plug-in being discontinued.
The new official EV3 software does not contain equivalent data logging capabilities.

Instead we will be using Matlab to log, plot and analyse data captured from the sensors.

This form of data logging takes place when you add a data logging functionality in your code
itself using the Data Logging block

- For this form of logging, you only need to create a
new project, you do not need to open a 'New
Experiment'.



## Brick Data Logging

This form involves setting up the data collection
parameters on the brick itself instead of using
external software.

- The data can then be viewed on the brick.

<section class="flex-container">
<!-- class="flex-child" style="min-width: 250px" -->
- Start the brick up and go to the 3rd tab in the menu to find 'Brick Datalog'.

<!-- class="flex-child" style="min-width: 250px" -->
![](assets/image34.png)
</section>

------------------------------------

<section class="flex-container">
<!-- class="flex-child" style="min-width: 250px" -->
- Click on Settings Icon to change the configurations.

<!-- class="flex-child" style="min-width: 250px" -->
![](assets/image35.png)
</section>

------------------------------------

<section class="flex-container">
<!-- class="flex-child" style="min-width: 250px" -->
- Click on the icon with numbers to see data from different sensors .

<!-- class="flex-child" style="min-width: 250px" -->
![](assets/image36.png)
</section>

------------------------------------

<section class="flex-container">
<!-- class="flex-child" style="min-width: 250px" -->
- Click on sphere icon to start/stop collecting data.

<!-- class="flex-child" style="min-width: 250px" -->
![](assets/image37.png)
</section>



## Remote Data Logging

Remote Data Logging involves the establishment of a connection between the EV3 brick and external software using either Bluetooth, WIFI, or a USB;
in order to attain real-time data from the sensors



### Example 1 - Test sensors

``` matlab
% test if ev variable already exists, if not, create it
if ~exist('ev', 'var')
    ev = legoev3('USB');
end

fprintf( "Connected Sensors:\n" )
% iterate through connected sensors
for i = 1:length( ev.ConnectedSensors )
    fprintf( "  %d: %s\n", i, ev.ConnectedSensors{i} )
end

% assuming that sensors are connected to default ports
ir = irSensor( ev, 1 );
fprintf( "IR: %d\n", readProximity( ir ) );

color = colorSensor( ev, 2 );
fprintf( "Intensity: %d\n", readLightIntensity( color ) );

gyro = gyroSensor( ev, 3 );
fprintf( "Gyro: %d\n", readRotationAngle( gyro ) );

sonar = sonicSensor( ev, 4 );
fprintf( "Sonar: %f\n", readDistance( sonar ) );
```



### Example 2 - Move motors

``` matlab
% config
speed = 20;

% test if ev variable already exists, if not, create it
if ~exist('ev', 'var')
    ev = legoev3('USB');
end

% initalise and start motors
left = motor( ev, 'A' );
right = motor( ev, 'D' );
start(left);
start(right);

% forward
left.Speed = speed;
right.Speed = speed;

pause( 1 );

% turn left
left.Speed = -speed;

pause( 1 );

% stop
left.Speed = 0;
right.Speed = 0;
```



### Example 3 - Log data

``` matlab
function historicValues = mainloop( ev, updateRate, runFor )
    % config
    if nargin < 2
        updateRate = 10; % Hz
    end
    if nargin < 3
        runFor = 30; % seconds
    end

    % connect to ir sensor
    ir = irSensor( ev, 1 );

    % initalise loop variables
    historicValues = [];
    startTime = tic;
    figure;

    while toc( startTime ) < runFor
        % Read the color sensor
        prox = readProximity( ir );
        historicValues = [historicValues; prox];

        fprintf( 'Proximity: %d\n', prox );

        % Plot the intensity values
        plot( historicValues, 'LineWidth', 2 );
        title( 'Value Over Time' );
        xlabel( 'Reading' );
        ylabel( 'Value' );
        
        % Force MATLAB to update the figure immediately
        drawnow;

        pause( 1.0 / updateRate );
    end
end

% test if ev variable already exists, if not, create it
if ~exist('ev', 'var')
    ev = legoev3('USB');
    %ev = legoev3('bt', '001653514A3D');
end

values = mainloop( ev );
```



### Example 4

``` matlab
function historicValues = mainloop( ev, updateRate, runFor )
    % config
    if nargin < 2
        updateRate = 10; % Hz
    end
    if nargin < 3
        runFor = 30; % seconds
    end

    % connect to sensors
    ir = irSensor( ev, 1 );
    sonar = sonicSensor( ev, 4 );

    % initalise loop variables
    historicValues = [];
    startTime = tic;
    figure;

    while toc( startTime ) < runFor
        prox = readProximity( ir );
        dist = readDistance( sonar );

        % convert sonar to cm
        dist = dist * 100;

        historicValues = [historicValues; prox, dist];

        fprintf( 'Proximity: %d, distance: %f\n', prox, dist );

        % Plot the intensity values
        plot( historicValues, 'LineWidth', 2 );
        title( 'Value Over Time' );
        xlabel( 'Reading' );
        ylabel( 'Value' );

        % add legend
        legend( 'IR', 'Sonar' );
        
        % Force MATLAB to update the figure immediately
        drawnow;

        pause( 1.0 / updateRate );
    end
end

% test if ev variable already exists, if not, create it
if ~exist('ev', 'var')
    ev = legoev3('USB');
end

values = mainloop( ev );
```


# Activities

We are building the sensor suite for our Level 4 LEGO vehicle.

- You will use the info collected today for your IMA.
- To achieve that we are going to decompose our function.

1. Navigation following a pre-defined path (30 min)
2. Look at the sensor data and refine your function (30 min)
3. Sensor characterization & obstacle detection (90 min)
  - NOTE: measurements of the above activity are very important for your PMA.
4. Navigation with pre-defined path and obstacles (remaining time - if any).



## Equipment available

- 1 or 2 robots per group.
- Laptops with TODO installed (do not use home edition!).
- Tiles with different paths.
- Spare batteries and sensors.
- Different obstacles (materials, sizes, shapes).
- Measuring tapes.



### Activity 1 - Constrained navigation

- In an urban environment we imagine to have predefined (and geo fenced) paths for the vehicles.
- Our L4 LEGO vehicles shall be able to navigate the environment following the black line.
- We are going to rely initially on just one sensor (colour sensor).

![](assets/image47.png) ![](assets/image46.png)

- Try to create a program that controls the vehicle
- You are going to use the Colour **Sensor** and the Large Servo Motor **Actuators**.
- Decomposition of the function (remember think computationally!):

  - Your vehicle must be able to detect the black path using the Colour Sensor.
  - Your vehicle must follow this path (being driven by the motors).

TODO path 

Some considerations
===================

- Is your detection robust?

  - What happens if there is less light? 
  - If we have a line of another colour? 
  - If the line is not continuous?
- Is your algorithm robust?

  - Can you compensate for any issues as above?
  - Do you need more sensors? (and therefore more money?)
- Any ideas on how to improve it your basic navigation?



### Activity 2 - Sensor data logging

Understanding data readings is fundamental for autonomous vehicles.

- Using one (or more) of the sensor data logging techniques, download the data from your sensors (colour and gyro).
- Understand your data 

  - *Optional - plot them.*
- Can you use the data to improve your control?

[ ![](assets/image50.jpeg) ] ![](assets/image55.png)

<!-- style="display:none" -->
https://goo.gl/images/mRQUWU https://goo.gl/images/L9msRF



### Activity 3 - Obstacle detection

Once you be are able to navigate your geo fenced area, 
the vehicle needs to be able to detect obstacles.

- We would like it to avoid pedestrians, other vehicles, orange cones, etc.
- To carry out this task you need to characterise your available sensors

![](assets/image58.jpeg)

Obstacle detection
==================

Understand sensor performance:

- What are the noise factors that can affect the performance of the sensors?
- You will be given obstacles of different sizes and different materials.
- You can use different types of sensors: US, colour, IR.
- You have to characterise their performance.

  - Range max and min, resolution, field of view (H & V), material, position
- How are they affected by environmental condition?
  
  - Light?
  - Multiple targets?
  - Materials? 
  - Are there preferred sensor type(s) depending on the target?
- Functional or non-functional testing?

- *HINT: For this activity, you may need to use sensor data logging.*
  - Record as much of your data because you are going to need them in your assignment.



### Activity 4 - Navigation and obstacle detection

You now have to combine what you have learned from the previous activities.

- Your vehicle must follow the black path.
- When there is a detected obstacle in the path your vehicle must stop or go around the obstacle.

![](assets/image60.png)

<!-- display:none -->
https://goo.gl/images/F7iR1z



## URBAN L4 Activity 4 -
Navigation and obstacle detection

- Optimise your use of sensors and their combination
to get through the path as fast as possible without
bumping the obstacles

- The implemented algorithm is a very basic way of

implementing high level sensor fusion
- Do sensors have priority?
- Are their read at the same time?
- How fast they are?
- How promptly do you need to counter act?



# Conclusion

Even if we have used simplified vehicles with few sensors, 
the sensor performance is crucial to achieve simple tasks.

- Data sensor logging can be used to improve our testing efficiency but also the quality of the control algorithm(s)
- We have used very simple control loops.
- The final performance of our sensor(s) will determine the performance of our function(s):

  - Is sensor fusion the solution?
  - Or it is just another engineering tradeoff?

![](assets/image64.png) ![](assets/image65.jpeg)

<!-- style="display:none" -->
https://goo.gl/images/VTTunS https://goo.gl/images/WMyXTM


