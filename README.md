# sendCAN2msg
Encode/send and receive/decode CAN 2.0x messages for Raspberry Pi with SKPang PiCAN2 CAN bus board
<p>This software is intended to control the behaviour of the <a href="http://skpang.co.uk/catalog/pican2-canbus-board-for-raspberry-pi-23-p-1475.html">skPang PiCAN2 CAN-Bus shield board</a> family with chipset <a href="http://ww1.microchip.com/downloads/en/devicedoc/21801e.pdf">Microchip MCP2515</a> as CAN controler.</p>

<h1>Features</h1>
<p>The software is developped in C++ and has been tested during long months under heavy conditions. Initialy the project was developped in Python but in the real life it was too slow and messages were lost.</p>
<p>Unlike most other applications that use a network interface like can0, sendCAN2msg work directly with the CAN controler. All interrupts are handled and used for sending or receiving messages.</p>
<p>On a Raspberry Pi B+ delay between messagges to be sent is less than 4µs. A receiving rate of more than ten message each 0.1 second is not a problem. There is no loss, buffers are never overflowed.</p>
<p>All parameters are in a single .ini file. The .ini filename can be choosen at startup or set to its default value. You can adjust all of the CAN protocol parameters as well as MCP2515 registers and timing values.</p>
<p>The only things you have to do is putting your stuff in the main and use functions to send single or multiple frame messages</p>
<p>The incoming messages are filtered and after acceptance are set in a known structure. You have an example for decoding it.</p>
<p>According to the debug level all is logged to syslog.</p>

<h1>CAN controler</h1>
<p>All af the MCP2515 registers are configurable in the .ini file. Default values are given for optimal operations.</p>
<p>The meaningfull parameters concern :</p>
<ul>
	<li>SPI bus. You can adjust the frequency. The latency issues of SPI bus described on the web have not been noticed.</li>
	<li>According to your bus characteristics (length, speed, nodes ...) the calculations of segments length according to sample point position and synchonization jump width are done</li>
	<li>Interrupt on incoming / outgoing messages are set via MCP2515 registers and handled. Interrupts are usefull for signaling warnings or errors from TEC / REC register, finding a free TX buffer or signaling a full RX buffer.</li>
	<li>Acceptance filters are easy to use, just write 29 bits length masks and filters in .ini file.</li>
</ul>

<h1>prerequisites</h1>
<ul>
	<li>SPI interface enabled. This can be done with <code>raspi-config->interfacing options->SPI</code></li>
	<li>wiringPi library. This library is standard in Raspbian. Check if it is present <code>dpkg -l | grep -i wiring</code></li>
</ul>

<h1>Quick start guide</h1>
<p>There is a <code>main.c </code> which simulate every 0.2 second a single frame message with an engine tachometer, the boost presure and the engine trim/tilt. A second multi frame message (4 frames) every 1 s simulate the oil presure, oil temp, coolant temp, alternator voltage, fuel rate, engine hours, coolant presure, fuel presure, engine percent load and engine percent torque. All of this messages according to NMEA 2000 and verified with an <a href="https://www.actisense.com/product/nmea-2000-to-pc-interface-ngt-1/">Actisense probe</a>.</p>
<p>At the console prompt :</p>
<pre>
<code>
make
./sendRPM
</code>
</pre>
<p>In a second display follow the syslog flow : <code>tail -f /var/log/syslog</code>
<p>Haver fun ...</p>
</p>

<h1>Acknowledgements</h1>
<p>Thanks to David Harting for his work on the MCP2515 object library for Arduino/Genuino.</p>
<p>Nothing would have been possible without the <a href="http://wiringpi.com/">wiringPi</a> library.</p>
