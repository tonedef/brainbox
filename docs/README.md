>[!IMPORTANT]
I am not a physician and make no guarantees about this device's effectiveness, safety, or intended function. It uses pulsing light and sound, which may pose risks for individuals with epilepsy or other neurological conditions. Anyone choosing to build or use this device, in whole or in part, should consult their physician beforehand. By engaging with this device or any related information, you accept full responsibility for all risks. Always discuss medical concerns with your doctor first.

# What is wavebox?
Wavebox is a simple device that produces 40Hz audio and light pulses designed to delay and slow the progression of Alzheimer's/dementia. It's based on promising research done at MIT: 
https://news.mit.edu/2022/small-studies-40hz-sensory-stimulation-confirm-safety-suggest-alzheimers-benefits-1213 

Again, I cannot ensure that this will happen. There are ongoing clinic trials for this, so the efficacy is still being evaluated.

<img src="./img/board.jpg" alt="working board" width="400">

It's meant to be as simple as possible for anyone to use. A dimmer to control how bright the light is, a volume control for the sound, and a single button to start/stop the device (though two other buttons, one on the encoder and one on left side exist for expansion or to repurpose this for any number of other uses, like a radio). It also has a web interface accessible thru WiFi, and the code can be upgraded/modified to support all kinds of things such as:
- making it an alarm clock, so that it runs without user intervention
- tracking usage
- playing reminders for other mental activities
- Repurposing for anything else that could use a couple of buttons and a mono audio output

The MCU being used is a Wemos S2 mini board. While you can use a Wemos C3, or an older D1 (significant code changes required), the S2 is ideal, as the GPIO pin being used to supply the PWM audio (which here is just a simple square wave), is also the DAC output. This makes it easy to switch over to an 8bit DAC output to support significantly more audio options thru software.

It's designed using pre-built dev boards and a few through-hole components to make it easy for hobbyists to make. The most challanging thing to build will be the housing, but really anything box like can be used for that.


# Getting started

## Electronic component BOM
Get all the necessary components for the electronics. You don't have to get the exact brand I've listed, but the values, and footprint of the component is important. I've made a few of these with generic no-name encoders and pots for example. The 0.96" I2C displays all seem to have slight variations in hole/pin placements, which I've tried to account for in the PCB's mounting and through-hole pads.

- 1x PCB board. Send the grb_v_x_x.zip to your PCB mfg. This is the latest confirmed [build](https://github.com/tonedef/wavebox/blob/main/KiCad/40Hz_S2/grb_v_0_3.zip). Also happy to orchestrate a group buy if folks are interested (minimum order is usually 5)
- 1x [Wemos S2 mini](https://www.amazon.com/HiLetgo-ESP32-S2FN4R2-ESP32-S2-Type-C-Connect/dp/B0B291LZ99)
- 1x  N-Channel MOSFETs [IRF540](https://www.amazon.com/dp/B0CBKGPJML) .
- 1x 100 Ohm resistor. A [kit](https://www.amazon.com/dp/B07L851T3V/ref=sspa_dk_detail_0) is probably the best bet for this if you don't have one already
- 1x 10k Ohm resistor (would be in the kit above)
- 1x 1n4007 Diode. Again, a [kit](https://www.amazon.com/HiLetgo-100pcs-Schottky-Rectifier-Assorted/dp/B07VHMCJK1/) if you don't have one already
- 1x 10k Ohm 15mm PCB vertical [potentiometer](https://www.amazon.com/dp/B0DN1DHWLR).
- 1x 20mm Rotary [encoder](https://www.amazon.com/dp/B07D3DF8TK) (with or without switch). 
- 1x 0.96in 4 pin [I2C display](https://www.amazon.com/MakerFocus-Display-Communication-SSD1315-Raspberry/dp/B0DR2CMYGM)
- 1x PAM8302 [audio dev board](https://www.amazon.com/Audio-Development-Tools-Adafruit-Amplifier/dp/B00PY2YSI4)
- 5V [LED strip](https://www.amazon.com/dp/B0DPKMQNJJ) (pay attention to the Voltage, most are 12V). You can use any color, but I'd recommend a warm white like 3000K.
- 2x 6x6x12 Momentary tactile [buttons](https://www.amazon.com/uxcell-6x6x12mm-Momentary-Tactile-Button/dp/B07JLV6WSF/)
- 1x Box of [caps](https://www.amazon.com/Tactile-Button-Plastic-Replacement-Breadboard/dp/B0B52B8S63) (don't need a whole box, but aren't really sold individually usually)
- 1x Small 4 Ohm [speaker](https://www.amazon.com/dp/B0CCP2Q168) (8ohm should also work)
- 1x [5V power supply](https://www.amazon.com/100-240V-Extension-Replacement-5-5x2-5mm-Security/dp/B0DGFSQS97) walwart. You can also just use the USB-C connector on the Wemos S2 mini, which is great for easy reprogramming, but limits the amperage you can draw on the LED out (only an issue if you need more than ~500mA). 
- 1x DC [jack](https://www.amazon.com/dp/B07CTCLKPP) that fits your 5V walwart

- 2x M2x6mm standoffs. A [kit](https://www.amazon.com/300pcs-Standoff-Column-Spacer-Assortment/dp/B07B9X1KY6/) is good, if you don't have one already.
- Assorted 2.54mm male and female [headers](https://www.amazon.com/dp/B0774VBJ3J) (most/all of these should already be included in the components above). You can of course use [JTS-XH](https://www.amazon.com/Taiss-560PCS-Connector-Adapter-Housing/dp/B09ZTWCZ3K) connectors for the power in, LED out, and  audio out, or just wire them directly.
 
 ## Case
 There's plenty of ways to do this. The easiest is to just a get a cast acrylic box that's roughly 4"x4"x4" and has roughly 1/8" (3mm) thick walls. I simply made boxes out of wood I had lying around, and grabbed a cheap piece of 1/8" scrap cast acrylic from TAP Plastics for the face plate. Cast acrylic usually has a paper backing as opposed to a plastic film. Cast acrylic is generally easier to machine.

 # Assembly
 ## software
 You will need to know how to connect your Wemos S2 to your PC and upload the code to it. There are plenty of tutorials on how to do this. The board needs to be in the [DFU mode](https://www.wemos.cc/en/latest/tutorials/s2/get_started_with_arduino_s2.html) to get picked up, and you need the espressif boards in your Arduino IDE, along with a bunch of libraries:
 - ESP32 boards [installation](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/)
 - ezButton (library from ArduinoGetStarted.com)
 - ESP Rotary (library from Lennart Hennigs)
 - Adafruit_GFX (library from Adafruit)
 - Adafruit_SSD1306 (library from Adafruit)
  
  Libraries are installed thru the Arudino IDE interface. It's been a while since I installed the things I need, so my appologies if I've missed anything. If you get errors, it should point to what's missing, and you'll have to  look for it and install the library.

  ## hardware (main board)
  - install the female headers for the Wemos board
  - solder the male pins to all the dev boards (Wemos, audio, and display). DON'T solder to any of these to the PCB just yet, but do dry fit them to make sure the pins are properly aligned and adjust if needed.
  - if your using connectors for the power in, audio out, and LED out, solder them now
  - solder all the remaining components, including the female headers for the Wemos board, to the PCB except for the display
  - attach the standoffs to the display.
  - As the OLED 0.96 displays aren't fully standardized on pin placement and mounting holes, the PCB has generous sized mounting holes and pin pads. Fit the display on the PCB and align it.
  - screw in the display's attached standoffs to the PCB
  - keeping the display parallel to the PCB, solder the display pins to the PCB (they may not protrude all the way thru the board, but that's fine)
 
 ## hardware (front panel)
  - any material can be used, but the easiest is a 1/8" thick 4x4" piece of clear cast acrylic.
  - [SVG and PDF](https://github.com/tonedef/wavebox/tree/main/images) faceplates can either be printed to guide manual cutting or used in CAD for CNC
  - note: the faceplate has offset mounting as opposed to the corners, so that you're not screwing the faceplate into a 45 joint of the housing, assuming you're making this out of wood with faces cut at a 45 degree bevel

 ## hardware (housing)
  - 3/4" 4x18" wood board works
  - cut 4, 4x4 sections at a 45 degree bevel
  - set a bit or a blade to 3/8" deep and run one side of all four sections about 1/8" in from the edge (this will make a 1/8" inset for your faceplate) 
  - in the center of one of the 4x4 pieces, cut a hole for the speaker (check the diameter you need for your speaker, it should be about 2")
  - on the same board as the speaker, cut a small 1/8 hole or so for the LED wires about 1/2 from the top edge in line with the center of the speaker
  - on the same board as the speaker, at the opposite end of the LED hole, cut a mount hole for your DC plug or a slot for a USB cable (if you plan to inset a bottom face into the box, leave enough room to accomodate your plug/cable)
  - glue and/or nail all four boards together so they form a box
  - finish the box however you like

 ## hardware (DC jack, LED, and speaker)
  - leaving the tape on, wrap the LED strip around the top edge of the box and cut the strip at the nearest cut point where the strip doesn't overlap
  - attach wire leads to the LED strip, the DC jack (if using), and the speaker, make them long enough to attach to the PCB with room to spare so you can connect everything easily
  - feed the LED leads thru the hole cut for the LED wires
  - take the tape off the LED strip and tape the strip around the box, then lift up each end of the strip, apply a drop of superglue and press them back down (most tape backings don't hold long term)
  - attach speaker and DC jack to box
  - attach leads to the PCB board
  - attach faceplat to PCB board, and then attach the faceplate to the box

