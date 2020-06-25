# TODO: give this a name

Quick and dirty guide to getting data out of an arduino (or other MCU) and plotting it live via serial.  (to be refined later)

Based on: https://www.instructables.com/id/Plotting-Data-From-Arduino/ (plenty of good info here)

Download and install Cypress Bridge Control Panel which is part of the PSoC programmer package (windows only, sorry): https://www.cypress.com/file/505286/download

Set up your code to output UART serial at whatever bitrate you please.  You will want a character to identify the start of a frame, followed by any number of by values.

It will look something like this:

> now = (unsigned int)millis();

> Serial.print("C"); //output to monitor

> Serial.write(now>>8);

> Serial.write(now&0xff);

> Serial.write(int(map(Setpoint,0,255,0,1023))>>8); //output to monitor

> Serial.write(int(map(Setpoint,0,255,0,1023))&0xff); //output to monitor

> Serial.write(int(sensorValue)>>8); //output to monitor

> Serial.write(int(sensorValue)&0xff); //output to monitor

> Serial.write(int(flowValueINH)>>8); //output to monitor

> Serial.write(int(flowValueINH)&0xff); //output to monitor

> Serial.write(int(flowValueEXH)>>8); //output to monitor

> Serial.write(int(flowValueEXH)&0xff); //output to monitor


Open up Bridge Control panel.

In the window you will use the following syntax to tell it what to expect:

`RX8 [h=43] @1Key1 @0Key1 @1Key2 @0Key2 @1Key3 @0Key3 @1Key4 @Key4 @1Key5 @0Key5`

This one matches the code above.  RX8 specifies serial input, [h=43] is the char 'C' at the beginnign of the frame, and the following 10 bytes are defined as high and low bytes of a 16-bit integer defined as key1 to key5.

In the lower part of the window select your COM port.  It should open the connection, if not, click the blue bowtie looking icon to connect or disconnect the port.

In the menu at the top, click Tools > Protocol Configuration.  Select the RX8 (UART) tab and input your comm parameters.

Click the Variable Settings button at the top of the window under the Tools menu (the left most of the 3 color buttons at the end of the tool bar - photos to be added to wiki later).  Click OK.

For each of the keys, check Active and define what type (byte or int) they are, and select plot colors if desired.  Click OK.

Click the repeat button at the bottom left of the window.  You should now see your hex serial data streaming from your MCU.  Click stop to stop it.

## How to save a long series of data:
***


You can save your data by clicking the To File button and it will save a live stream of the data to the File tab at the top of the window.  After you have collected the data you want, go to the File tab and click save file at the bottom.  Fair warning: this has never worked for me.  It never actually saves the file.  I usually end up coping the contents of the window into a text editor and then saving that.  It will be in hex format, separated by spaces.

## How to Plot:
***


Click the repeat button, the click over to the Chart tab. You will see the live data streaming.  To change which data are plotted, use the check boxes to the upper right of the plot.

To clear the data, click the blank plot icon in the toolbar, 4th icon from the right.

Click stop at the bottom left at any time to stop the plotting.

You can save the data in the plot by clicking the 'Export Collected Data' icon in the toolbar, the second from the right.  You can export as an image, or as an excel or CSV.  Note that this seems to be limited to the most recent 10,000 points of data.  If you need to record longer datasets, use the procedure in the section above.






