#### !!!! SCROLL DOWN FOR ENGLISH !!!! ####

## ArduFMPLL Nederlands ##

Arduino code die moet gaan werken met de PLL ic's TSA5511 / SAA1057.<br>
Om het werkend te krijgen ga ik het proberen met 2 type printen die hieronder vermeld staan.<br>
De TSA5511 zit in de DRFS 6 v2.0 van de Dutch RF Shop. https://dutchrfshop.nl/nl/.<br>
De SAA1057 zit in de ECC Tube Oscillator van Fred Wester. http://www.fred-wester.nl/ecc_tube_oscillator.html.<br>

### TSA5511 Pic ###

Dit ic kan je instellen met i2c protocol.<br>
Deze zender heeft ook een character lcd van 16x2 om de frequentie in te stellen.<br>
Ook heeft hij 3 knoppen om de frequentie in te stellen van 86Mhz tot 108Mhz.<br>
En heeft een potmeter om het vermogen in te stellen van 0w tot 6w.<br>
De zender heeft ook een pic16f628a microcontroller om het scherm, knoppen en TSA5511 aan te sturen.<br>

### TSA5511 Arduino ###

Als kan werken op elke arduino met i2c uitgang.<br>
Ik maak momenteel gebruik van een Arduino Uno.<br>
En een 20 * 4 character lcd om de frequentie in te stellen van 80Mhz tot 108Mhz, en ook die is op i2c bus aangesloten.<br>
Rotary Encoder om alles in te stellen met een menu.<br>
DRFS 6 v2.0 gebruik ik om mijn code te testen en is ook aangesloten op i2c bus.<br>
De code werkt helemaal op nog wat kleine dingetjes na zoals band selectie.<br>

#### TODO ####

Schema maken en toevoegen hoe alles aan elkaar vast.<br>
Band selectie nog toevoegen.<br>
Misschien ga ik ook nog een digitale potmeter gebruiken om het vermogen te kunnen regelen.<br>

### SAA1057 ###

Ben ik nog aan het uitzoeken eerst die TSA5511 code compleet werkend krijgen dan ga ik met deze chip verder.

#### TODO ####

Code schrijven om die SAA1057 aan te sturen met een arduino.<br><br><br><br>

## ArduFMPLL English ##

Arduino code that is working with PLL chip TSA5511 / SAA1057

### TSA5511 ###

This chip is communcating true i2c.<br>
I got it working with the DRFS 6 v2.0 3Meter FM Transmitter 6Watt.<br>
This pcb you can buy it from https://dutchrfshop.nl/nl/.

#### TODO ####

Make user interface on lcd 20 * 4.<br>
Use rotary encoder to navigate.<br>
And maybe gona use a digital potmeter for output power adjustment.

### SAA1057 ###

Nothing here yet.

#### TODO ####

Need to make the code to for running with a SAA1057.

