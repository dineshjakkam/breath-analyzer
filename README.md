# breath-analyzer
Lost track of how many beers you had? I can tell you..

## Overview
* The MQ-3 sensor connected to the Nucleo Development board thorugh ADC peripheral helps in detecting the alochol levels in breath.
* On detecting alochol in air, the system sends notification to the mobile device via the BLE module _using notification property_.
* The system starts sampling the MQ-3 sensor data, only if atleast one mobile device is connected to the system through BLE.
* Due to the poor MQ-3 sensor documentation and calibration data avaiable, that notifications that the system send to the mobile device is not 100 % accurate.
* The system can detect the presence of alochol with much accuracy, but the level of alocohol present is just the approximations.
* **This project doesn't tell accurately how sober you are, so do not take these results to judge your driving ability. Ofcourse you should never drink and drive**

### Key Learnings:
1. BLE complete stack
2. Analog sesnor integration to microcontroller
3. Configuring ADC peripherals

### See documentation for more details:
1. <a href="docs/bluetooth-low-energy.pdf">Project report</a>
2. <a href="docs/bluetooth-low-energy.txt">Configuration file</a>
