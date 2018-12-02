# sendCAN2msg
Encode/send and receive/decode CAN 2.0x messages for Raspberry Pi with SKPang PiCAN2 CAN bus board
<p>This software is intended to control the behaviour of the <a href="http://skpang.co.uk/catalog/pican2-canbus-board-for-raspberry-pi-23-p-1475.html">skPang PiCAN2 CAN-Bus shield board</a> with chipset <a href="http://ww1.microchip.com/downloads/en/devicedoc/21801e.pdf">Microchip MCP2515</a></p>

<h1>Features</h1>
<p>The software is developped in C++ and has been tested during long months under heavy conditions. Initialy the project was developped in Python but in the real life it was too slow and messages were lost.</p>
<p>All parameters are in a single .ini file. The .ini filename can be choosen at startup or set to its default value. You can adjust all of the CAN protocol parameters as well as MCP2515 registers and timing values.</p>
<p>The only things you have to do is putting your stuff in the main and use functions to send single or multiple frame messages</p>
<p>The incoming messages are filtered and after acceptance are set in a known structure. You have an example for decoding it.</p>
<p>According to the debug level the outputs are logged to syslog.</p>

<h1>CAN controler</h1>
<p>All af the MCP2515 registers are configurable in the .ini file. Default values are given for optimal operations.</p>
<p>The meaningfull parameters concern :</p>
<ul>
	<li>SPI bus</li>
	<li>Calculation of segments length according to sample point position and synchonization jump width</li>
	<li>Interrupt on incoming / outgoing messages</li>
	<li>Acceptance filters</li>
</ul>

<h1>Acknowledgements</h1>
Nothing would have been possible without the <a href="http://wiringpi.com/">wiringPi</a> library.
