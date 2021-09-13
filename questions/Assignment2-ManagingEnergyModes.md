Please include your answers to the questions below with your submission, entering into the space below each question
See [Mastering Markdown](https://guides.github.com/features/mastering-markdown/) for github markdown formatting if desired.

**Note: All average currents should be taken at a time scale of 200mS/div. See the pop-up menu in the lower-right corner of the Energy Profiler window**

**1. Fill in the below chart based on currents measured in each energy mode, replacing each TBD with measured values.  Use the [Selecting Ranges](https://www.silabs.com/documents/public/user-guides/ug343-multinode-energy-profiler.pdf) feature of the profiler to select the appropriate measurement range.  Your measurements should be accurate to 10%**

Energy Mode | Period average current (uA) | Average Current with LED off (uA) | Average Current with LED On (uA)
------------| ----------------------------|-----------------------------------|-------------------------
EM0         |          4940               |          4900                     |         5380
EM1         |          3490               |          3460                     |         3940
EM2         |         43.18               |          4.08                     |         497.58
EM3         |         47.26               |          4.15                     |         511.5

**2. Fill in the below chart based on time measured period and LED on time measurements for EM2 and EM3 modes, replacing each TBD with measured values.  Use the [Selecting Ranges](https://www.silabs.com/documents/public/user-guides/ug343-multinode-energy-profiler.pdf) feature of the profiler to select the appropriate measurement range.  When measuring time, measure from peak current to peak current.  Your LED on time measurement should be accurate to 1ms and your period measurement should be accurate to within 10ms**

Energy Mode | Period (s) | LED On Time (ms) |
------------| -----------|-------------------
EM2         |  2.25      |        176
EM3         |  2.24      |        174


**3. ScreenShots**  

***EM0***  
Period average current    
![em0_avg_current_period][em0_avg_current_period]    
Average Current with LED ***off***  
![em0_avg_current_ledoff][em0_avg_current_ledoff]   
Average Current with LED ***on***  
![em0_avg_current_ledon][em0_avg_current_ledon]   

***EM1***  
Period average current    
![em1_avg_current_period][em1_avg_current_period]      
Average Current with LED ***off***  
![em1_avg_current_ledoff][em1_avg_current_ledoff]   
Average Current with LED ***on***  
![em1_avg_current_ledon][em1_avg_current_ledon]    

***EM2***  
Period average current  
![em2_avg_current_period][em2_avg_current_period]     
Average Current with LED ***off***  
![em2_avg_current_ledoff][em2_avg_current_ledoff]   
Average Current with LED ***on***  
![em2_avg_current_ledon][em2_avg_current_ledon]    
LED measurement - Period   
![em2_led_period][em2_led_period]   
LED measurement - LED on time   
![em2_led_ledOnTime][em2_led_ledOnTime]  

***EM3***  
Period average current    
![em3_avg_current_period][em3_avg_current_period]  
Average Current with LED ***off***  
![em3_avg_current_period][em3_avg_current_ledoff]     
Average Current with LED ***on***  
![em3_avg_current_period][em3_avg_current_ledon]   
LED measurement - Period   
![em3_led_period][em3_led_period]   
LED measurement - LED on time   
![em3_led_ledOnTime][em3_led_ledOnTime]    

[em0_avg_current_period]: https://github.com/CU-ECEN-5823/ecen5823-assignment2-shni9045/blob/master/questions/screenshots/EMO/FULL_ss1.png  "em0_avg_current_period"
[em0_avg_current_ledoff]: https://github.com/CU-ECEN-5823/ecen5823-assignment2-shni9045/blob/master/questions/screenshots/EMO/OFF_ss1.png  "em0_avg_current_ledoff"
[em0_avg_current_ledon]: https://github.com/CU-ECEN-5823/ecen5823-assignment2-shni9045/blob/master/questions/screenshots/EMO/ON_ss1.png  "em0_avg_current_ledon"

[em1_avg_current_period]: https://github.com/CU-ECEN-5823/ecen5823-assignment2-shni9045/blob/master/questions/screenshots/EM1/FULL_SS1.png "em1_avg_current_period"
[em1_avg_current_ledoff]: https://github.com/CU-ECEN-5823/ecen5823-assignment2-shni9045/blob/master/questions/screenshots/EM1/OFF_SS1.png "em1_avg_current_ledoff"
[em1_avg_current_ledon]: https://github.com/CU-ECEN-5823/ecen5823-assignment2-shni9045/blob/master/questions/screenshots/EM1/ON_SS1.png "em1_avg_current_ledon"

[em2_avg_current_period]: https://github.com/CU-ECEN-5823/ecen5823-assignment2-shni9045/blob/master/questions/screenshots/EM2/FULL_ss1.png "em2_avg_current_period"
[em2_avg_current_ledoff]: https://github.com/CU-ECEN-5823/ecen5823-assignment2-shni9045/blob/master/questions/screenshots/EM2/OFF_ss1.png "em2_avg_current_ledoff"
[em2_avg_current_ledon]: https://github.com/CU-ECEN-5823/ecen5823-assignment2-shni9045/blob/master/questions/screenshots/EM2/ON_ss1.png "em2_avg_current_ledon"
[em2_led_period]: https://github.com/CU-ECEN-5823/ecen5823-assignment2-shni9045/blob/master/questions/screenshots/EM2/TotalPeriod_ss1.png "em2_led_period"
[em2_led_ledOnTime]:  https://github.com/CU-ECEN-5823/ecen5823-assignment2-shni9045/blob/master/questions/screenshots/EM2/PeriodON_ss1.png "em2_led_ledOnTime"

[em3_avg_current_period]: https://github.com/CU-ECEN-5823/ecen5823-assignment2-shni9045/blob/master/questions/screenshots/EM3/FULL_SS1.png  "em3_avg_current_period"
[em3_avg_current_ledoff]: https://github.com/CU-ECEN-5823/ecen5823-assignment2-shni9045/blob/master/questions/screenshots/EM3/OFF_SS1.png "em3_avg_current_ledoff"
[em3_avg_current_ledon]: https://github.com/CU-ECEN-5823/ecen5823-assignment2-shni9045/blob/master/questions/screenshots/EM3/ON_SS1.png  "em3_avg_current_ledon"
[em3_led_period]: https://github.com/CU-ECEN-5823/ecen5823-assignment2-shni9045/blob/master/questions/screenshots/EM3/Tperiod_SS1.png "em3_led_period"
[em3_led_ledOnTime]: https://github.com/CU-ECEN-5823/ecen5823-assignment2-shni9045/blob/master/questions/screenshots/EM3/OPeriod_SS1.png "em3_led_ledOnTime"
