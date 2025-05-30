>[!IMPORTANT]
I am not a physician and make no guarantees about this device's effectiveness, safety, or intended function. It uses pulsing light and sound, which may pose risks for individuals with epilepsy or other neurological conditions. Anyone choosing to build or use this device, in whole or in part, should consult their physician beforehand. By engaging with this device or any related information, you accept full responsibility for all risks. Always discuss medical concerns with your doctor first.

# What is Memini?
Memini is a simple device that produces 40Hz audio and light pulses designed to delay and slow the progression of Alzheimer's/dementia. It's based on promising research done at MIT: 
https://news.mit.edu/2022/small-studies-40hz-sensory-stimulation-confirm-safety-suggest-alzheimers-benefits-1213 (again, I cannot ensure that this will happen.  There are ongoing clinic trials for this, and I have no guarantee that the designs here will be able to achieve that)

It's meant to be as simple as possible for anyone to use. A dimmer to control how bright the light is, a volume control for the sound, and a single button to start/stop the device. It also has a web interface accessible thru WiFi, and the code can be upgraded/modified to support all kinds of things such as:
- making it an alarm clock, so that it runs without user intervention
- tracking usage
- playing reminders for other mental activities
- Repurposing for anything else that could use a couple of buttons and a mono audio output

The MCU being used is a Wemos S2 mini board. While you can use a Wemos C3, or an older D1 (code changes required), the S2 is ideal, as it includes a DAC output which makes it easy to modify the existing on/off square wave audio pulse to support significantly more audio options thru software upgrades.

It's designed using pre-built dev boards and a few through-hole components to make it easy for hobbyists to make. The most challanging thing to build will be the housing, but really anything box like can be used for that.

# Getting started

## Electronic component BOM
Get all the necessary components for the electronics. You don't have to get the exact brand I've listed, but the values, and footprint of the component is important. I've made a few of these with generic no-name encoders and pots for example. The 0.96" I2C displays all seem to have slight variations in hole/pin placements, which I've tried to account for in the PCB's mounting and through-hole pads.

- 1x PCB board. Send the grb_v_x_x.zip to your PCB mfg. This is the latest confirmed [build](https://github.com/tonedef/Memini/blob/main/KiCad/40Hz_S2/grb_v_0_3.zip). Also happy to orchestrate a group buy if folks are interested (minimum order is usually 5)
- 1x [Wemos S2 mini](https://www.amazon.com/HiLetgo-ESP32-S2FN4R2-ESP32-S2-Type-C-Connect/dp/B0B291LZ99)
- 1x  N-Channel logic level MOSFET like the [CSD19531KCS](https://www.digikey.com/en/products/detail/texas-instruments/CSD19531KCS/4437459). If you're using a short LED strip (< 400mm), you can _probably_ get away with generic MOSFETs, like the ubiquitous [IRF540](https://www.amazon.com/dp/B0CBKGPJML), which I've tried sucessfully.
- 1x 100 Ohm resistor. A [kit](https://www.amazon.com/dp/B07L851T3V/ref=sspa_dk_detail_0) is probably the best bet for this if you don't have one already
- 1x 10k Ohm resistor (would be in the kit above)
- 1x 1n4007 Diode. Again, a [kit](https://www.amazon.com/HiLetgo-100pcs-Schottky-Rectifier-Assorted/dp/B07VHMCJK1/) if you don't have one already
- 1x 10k Ohm 15mm PCB vertical [potentiometer](https://www.amazon.com/dp/B0DN1DHWLR).
- 1x 20mm Rotary [encoder](https://www.amazon.com/dp/B07D3DF8TK) (with or without switch). 
- 1x 0.96in 4 pin [I2C display](https://www.amazon.com/MakerFocus-Display-Communication-SSD1315-Raspberry/dp/B0DR2CMYGM)
- 1x PAM8302 [audio dev board](https://www.amazon.com/Audio-Development-Tools-Adafruit-Amplifier/dp/B00PY2YSI4)
- 5V [LED strip](https://www.amazon.com/dp/B0DPKMQNJJ) (pay attention to the Voltage, most are 12V). You can use any color, but I'd recommend a warm white like 3000K.
- 2x 6x6 Momentary tactile buttons
  - ~3mm or thinner faceplate: 9mm height [buttons with caps](https://www.amazon.com/TWTADE-6%C3%976%C3%979mm-Tactile-Button-Switch/dp/B07CGCPGPT) is the bare minimum
  - Greater than 3mm, or just for a "chunkier" button: 12mm [buttons](https://www.amazon.com/uxcell-6x6x12mm-Momentary-Tactile-Button/dp/B07JLV6WSF/) and [caps](https://www.amazon.com/Tactile-Button-Plastic-Replacement-Breadboard/dp/B0B52B8S63)
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
  - dry fit the male pins to the audio dev board and the PCB (don't solder just yet)
  - solder the male pins to all the dev boards (Wemos, audio, and display). DON'T solder to any of these to the PCB just yet, but do dry fit them to make sure the pins are properly aligned and adjust if needed.
  - if your using connectors for the power in, audio out, and LED out, solder them now
  - solder all the remaining components to the PCB except for the display
  - attach the standoffs to the display.
  - As the OLED 0.96 displays aren't fully standardized on pin placement and mounting holes, the PCB has generous sized mounting holes and pin pads. Fit the display on the PCB and align it.
  - screw in the display's attached standoffs to the PCB
  - keeping the display parallel to the PCB, solder the display pins to the PCB
 
 ## hardware (DC jack, LED, and speaker)
