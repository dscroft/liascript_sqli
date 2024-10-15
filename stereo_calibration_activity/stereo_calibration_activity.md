<!--
author:   David Croft

email:    david.croft@warwick.ac.uk

version:  0.0.1

language: en

narrator: US English Female

comment:  Try to write a short comment about
          your course, multiline is also okay.

link: https://dscroft.github.io/liascript_materials/assets/styles.css
-->

# Stereo Camera Calibrator activity

## Matlab

<details>
<summary>Installed MATLAB</summary>

1. Using the provided laptop.
</details>

<details>
<summary>MATLAB Online</summary>

1. Open a new browser window.
</details>

## Opening Stereo Camera Calibrator

{{1}}
> **Select `APPS` tab at the top of the window.**
>
> ![](images/a.png)

{{2}}
> A series of app icons should appear top of screen.
>
> ![](images/b.png)

{{3}}
> **In `APPS` selector click the ▾ button and search for "Stereo Camera Calibrator".**
>
> ![](images/e.png)

{{4}}
> **Click on the app icon to open the app.**
> 
> - You may be asked to allow matlab.mathworks.com to use your camera. It doesn't matter what you choose, we will not be using the webcam for this activity.

## Loading the images

{{1-99}}
> **Click `New Session`.**

{{2-99}}
> **Click `Add Images`.**

{{3-99}}
> **`Browse` for the `camera 1` images and select the `image_l` directory.**
> 
> - If you are using install MATLAB this will be where ever you unzipped the files earlier.
> - If you are using MATLAB in your browser, this will be the `dvsa` -> `dataset` -> `image_l` directory already provided.

{{4-99}}
> `Browse` for the `camera 2` images and select the `image_r` directory.
> 
> - If you are using install MATLAB this will be where ever you unzipped the images earlier.
> - If you are using MATLAB in your browser, this will be the `dvsa` -> `dataset` -> `image_r` directory already provided.

{{5-99}}
> Change the `Properties` of the checkerboard. 
>
> - `Size of the checkerboard square` should be 100mm (or 10cm).
>
> ![](images/d.png)

{{6-99}}
> Click `OK`.

{{7-99}}
> Wait.
> 
> - If you have done the previous steps you should see the following pop ups appear.
>   1. `Analysing images`.
> 
>   2. `Detection Results`.
> - Leave `Detection Results` open.


## Detection Results

Should give details of the `Total stereo pairs processed` and `Rejected stereo pairs`.

{{1-99}}
> **Click `view images` to see the rejected images.**

**What do you notice about the rejected images?**

- Why did MATLAB not include them in the calibration process?

<details>
<summary>**Hint 1**</summary>

*The rejected images all have the same issue*.

</details>

<details>
<summary>**Hint 2**</summary>

*The rejected pairs have the calibration target missing from one of the pair.*

*At least part of the target is missing.*

*In some cases the entire target is absent.*

</details>

![Images from individual pairs are shown side by side.](images/rejected_images_b.png)

{{2-99}}
> **Click `OK` to close the `Rejected Images` window.**

{{3-99}}
> **Click `OK` to close the `Detection Results` window.**

## Calibration

See that the images for the stereo pairs are different.

- This is because the cameras are in different positions.

See that the checkerboard pattern has been detected in each image.

- Corner points of the inner checkerboard squares are marked with circles.
- Checkerboard pattern is a known size, so the distance between the circles is known.
  - Apparent distance between the circles (in pixels) *something something distance*
- Checkerboard pattern is a rectangle, not a square.
  - To avoid possible confusion regarding the orientation of the target. 

## Calibrate


1. Click `Calibrate`.
  - If you have done the previous steps you should see the following pop ups appear.
    1. `Calibration Progress`
2. Wait.
