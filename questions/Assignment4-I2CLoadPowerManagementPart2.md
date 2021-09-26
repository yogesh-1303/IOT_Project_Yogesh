Please include your answers to the questions below with your submission, entering into the space below each question
See [Mastering Markdown](https://guides.github.com/features/mastering-markdown/) for github markdown formatting if desired.

*Be sure to take measurements with logging disabled to ensure your logging logic is not impacting current/time measurements.*

*Please include screenshots of the profiler window detailing each current measurement captured.  See the file Instructions to add screenshots in assignment.docx in the ECEN 5823 Student Public Folder.* 

1. What is the average current per period?
   Answer: 14.48 Microamperes(uA)
   <br>Screenshot:  
   ![Avg_current_per_period] [Avg_current_per_period] 

2. What is the average current when the Si7021 is Powered Off?
   Answer: 2.44 Microamperes(uA)
   <br>Screenshot:  
   ![Avg_current_LPM_Off] [Avg_current_LPM_Off]  

3. What is the average current when the Si7021 is Powered On?
   Answer: 366.2  Microamperes(uA)
   <br>Screenshot:  
   ![Avg_current_LPM_On] [Avg_current_LPM_On]  

4. How long is the Si7021 Powered On for 1 temperature reading?
   Answer: 98 Milliseconds(mS)
   <br>Screenshot:  
   ![duration_lpm_on] [duration_lpm_on]

5. Compute what the total operating time of your design for assignment 4 would be in hours, assuming a 1000mAh battery power supply?
   Answer: My design has an average current consumption per period of 14.48 uA per 3 Second period. 
 
   Total operating time on 1000 mAH battery = Battery Capacity/Load Current = 1000 / 0.01448 = 69,060 Hours = 7.8835616 Years
   
   
6. How has the power consumption performance of your design changed since the previous assignment?
   Answer:
   When Si7021 was interfaced using polling I2C method in the previous assignment the Average Period Current consumotion was 166 Microamperes compared and when Si7021 was interfaced using interrupt based I2C method the Average Period Current consumption was 14.48 MicroAmperes.  
   Hence, there was a decrease of 91.27% in average energy consumed per period. Therefore, there was a significant reduction in energy consumption for Total,ON and OFF period of Si7021 with I2C interrupt method.
   
7. Describe how you have tested your code to ensure you are sleeping in EM1 mode during I2C transfers.
   Answer:
    <br>Screenshot:  
   ![analysis] [analysis]
   
   By analyzing the energy diagram from energy profiler it can be confirmed that EFR32 is sleeping in EM1 mode during I2C transfers-
   At time instant T1 Si7021 is turned ON and at time instant T2 EFR32 sleeps to EM3 for Power Up period. At time instant T3 EFR32 performs Write transfer and sleeps to EM1 energy mode until transfer is complete.  
   At time instant T4 EFR32 sleeps for 10.8 mS in EM3 energy mode while waiting for value to be ready. At time instant T5 EFR32 performs I2C read transfer and sleeps to EM1 mode until read is complete, following which it sleeps to EM3.  
   Empirically through trial and error I have tested that the I2C peripheral of EFR32 worked only in EM1 mode and no energy mode below that which the temperature readings from Si7021 confirmed.
   
   [Avg_current_per_period]: https://github.com/CU-ECEN-5823/ecen5823-assignment4-shni9045/blob/master/questions/screenshots/Assignment_4/PERIOD_AVG.png
   [Avg_current_LPM_Off]: https://github.com/CU-ECEN-5823/ecen5823-assignment4-shni9045/blob/master/questions/screenshots/Assignment_4/OFF_AVG.png
   [Avg_current_LPM_On]: https://github.com/CU-ECEN-5823/ecen5823-assignment4-shni9045/blob/master/questions/screenshots/Assignment_4/ON_AVG.png
   [duration_lpm_on]: https://github.com/CU-ECEN-5823/ecen5823-assignment4-shni9045/blob/master/questions/screenshots/Assignment_4/ON_AVG.png 
   [analysis]: https://github.com/CU-ECEN-5823/ecen5823-assignment4-shni9045/blob/master/questions/screenshots/Assignment_4/Question_7.png

