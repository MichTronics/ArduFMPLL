#### !!!!Scroll down for english!!!! ####

## ArduFMPLL Nederlands ##

Arduino code die moet gaan werken met de PLL ic's TSA5511 / SAA1057.
De TSA5511 zit in de DRFS 6 v2.0 van de Dutch RF Shop. https://dutchrfshop.nl/nl/
De SAA1057 zit in de ECC Tube Oscillator van Fred Wester. http://www.fred-wester.nl/ecc_tube_oscillator.html

### TSA5511 ###

Dit ic kan je instellen met i2c protocol.<br>
Ik heb de code werkend met de DRFS 6 v2.0 kit dit is een 3Meter FM zender die 6 watt geeft van 80Mhz tot 108Mhz.<br>
Deze zender heeft ook een character lcd van 16x2 om de frequentie in te stellen.<br>
En heeft een potmeter om het vermogen in te stellen van 0w tot 6w.<br>
De zender heeft ook een pic16f628a microcontroller om het scherm, knoppen en TSA5511 aan te sturen.<br>

#### TODO ####

Een gebuikersinterface op een character lcd van 20x4.<br>
Die gerbuikersinterface is te bedienen met rotary encoder.<br>
En misschien And maybe gona use a digital potmeter for output power adjustment.

### SAA1057 ###

Ben ik nog aan het uitzoeken eerst die TSA5511 code compleet werkend krijgen dan ga ik met deze chip verder.

#### TODO ####

Code schrijven om die SAA1057 aan te sturen met een arduino.<br><br><br><br>

## ArduFMPLL English ##

Arduino code that is working with PLL chip TSA5511 / SAA1057

### TSA5511 ###

This chip is communcating true i2c.
I got it working with the DRFS 6 v2.0 3Meter FM Transmitter 6Watt.<br>
This pcb you can buy it from https://dutchrfshop.nl/nl/.

#### TODO ####

Make user interface on lcd 2*16.<br>
Use rotary encoder to navigate.<br>
And maybe gona use a digital potmeter for output power adjustment.

### SAA1057 ###

Nothing here yet.

#### TODO ####

Need to make the code to for running with a SAA1057.

