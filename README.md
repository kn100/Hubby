# Assembly of USB WiFi hub
There are USB hubs out there which allow you to switch between two connected computers. The idea is that you can connect it to a work machine as well as a personal machine, and then 'switch' the devices connected to the hub between these two computers with a button press.

What if you don't want to press the button though - and instead wish you could switch your hub using the internet? Home assistant? Bash script that executes when the machine boots? That's what this project describes.

Provided are build instructions for the electronics portion, as well as basic firmware you can run on the ESP32 that powers this project. Also provided are 3D models that will allow you 3D print a neat enclosure for the project, but this part is optional.

## Required parts for electronic portion of project
* 1x UGREEN USB 3.0 Sharing Switch 2 Computers 4 Ports [like this](https://www.amazon.ca/UGREEN-Computers-Peripheral-Switcher-One-Button/dp/B01N6GD9JO/).
* 4x length ~10cm lengths of wire (optionally with a small connector in the middle to allow for easy disassembly)
* 1x Transistor (I used a BC327 but any similar transistor should do)
* 3x 1k ohm resistor 
* 8x m2x3x3.5 heat set inserts

## Required parts for 3d printed portion of project
* 4x m2x4 screws with heads that are larger than 3mm in diameter
* 4x m2x8 screws with heads that have a diameter of ~3mm.
* Some electrical tape (optional)
* ~70g of filament

## Firmware preparation
1. Edit `src/main.cpp` so that it reflects your Wifi SSID and password. You may also wish to change the references to Mac and PC - if you're perhaps connecting two linux boxes or something.
2. Flash the firmware to your ESP32. I recommend platformIO.

## Electronics preparation
1. Disassemble your uGreen USB Hub. It is held together with clips that can easily be dislodged with a guitar pick or similar run along the seams.
2. Solder wires to the four points described below
   - One to the right hand side of the rightmost LED (Referred to later as Wire 1)
   - One on either side of the left hand side of the switch (Referred to later as Wire 2 and 3, where wire 2 is the wire closest to the 4 USB ports)
   - One to the right hand side of the large capacitor in the bottom left. (Referred to later as Wire 4)
3. Solder your ESP32 to a 5cm x 7cm Protoboard so that one side has at least 4 rows of pins free, and that the other side has at least 2 rows of pins free.
> **Note before you continue: before you start attaching wires to your esp32, you may want to consider running them through the hole in the enclosure designed for them - if you didn't use wires with connectors that can fit through said hole. If your connectors do not fit through the hole, put them on the side of the enclosure where the ESP32 goes - there will be more room in there once you assemble everything**

4. Attach wire 4 to the 5v pin of your ESP32.
5. Attach a 1kohm resistor to pin 13 of your ESP32, and attach the other side somewhere where you can later attach the middle pin of your transistor.
6. Attach a transistor so that its middle leg (Collector) is attached to this resistor.
7. Attach Wire 2 to the right hand leg of your transistor (The Emitter)
8. Attach Wire 3 to the left hand leg of your transistor (The Base)
9. Now we're going to construct a voltage divider. Attach one leg of a resistor to pin 19.
10. Attach the second leg of that first resistor, along with the first leg of your second resistor to Wire 1. 
11. Finally, attach the final free leg to a ground pin on your ESP32.

## 3d printed assembly
Provided are 5 models. `ESPEnclosure.stl` is the bottom portion of the build, and will hold the ESP32 + protoboard setup we built earlier. `HubEnclosure.stl` is the middle layer, and is a complete replacement for the default enclosure. `Button.stl` is a simple button that sits inside the `TopPlate.stl`. Also provided is `ButtonSpacer.stl` which is just a 0.2mm 12mm disk you can glue to the underside of your button if your button wasn't tall enough to stay in place or rattles too much. 

Print wise, print however you see fit, supports aren't likely to be necessary for any of the models. I suggest around 15% infill, with a 3 wall thickness, with 3 top and bottom layers. You may wish to print your `TopPlate.stl` at a higher density if you'd like to obscure the internal LEDs better. I printed at 0.2mm layer height.

1. Insert the USB hub into the portion of the enclosure designed for it. It should slip right over the standoffs. 
2. Now you've verified USB hub fitment - install the 8 heat set inserts, four on either side of the USB hub enclosure.
3. Install the ESP32/Protoboard combo into the ESP32 holder and then screw it to the bottom of the Hub enclosure using your m2x8 screws.
4. Insert the button into the top cover, and for ease of installation place some electrical tape over the bottom of it to keep it in place (optional).
5. Place top cover over the top of the USB hub enclosure and screw in using the m2x4 screws.

## Interacting with the hub.
1. Figure out what IP address your ESP32 has. You can usually easily do this through your routers administration panel.
2. Visit that IP address in your browser.
3. Alternatively, if you wish to integrate the hub into something else (Perhaps HomeAssistant!), you can directly fire requests at <ip-address>/mac or <ip-address>/pc in order to switch between the two.

## Bonus round - Using systemd inhibitor locks to automate switching the hub
What if your personal linux machine would just switch the hub over to your Mac when it went to sleep, and yank control back by switching it back to PC when it wakes back up?
Check out https://github.com/kn100/sleepwake/ - it does just that. You'll just need to change the IP address to whatever your hubs is, and magically you no longer even have
to think about it anymore.