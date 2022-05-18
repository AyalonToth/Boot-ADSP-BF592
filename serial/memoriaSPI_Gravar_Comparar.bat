cd C:\Analog Devices\CrossCore Embedded Studio 2.10.0
cldp -proc ADSP-BF592 -emu 1000 -driver "C:\Users\Ayalon\Documents\GitHub\Boot-ADSP-BF592\serial\Debug\bf592_m25p16_dpia.dxe" -cmd prog -erase affected -format hex -file "C:\Users\Ayalon\Documents\GitHub\ESIG_SW_SPO2\Debug\ESIG_SW_SPO2.ldr" -cmd compare -format hex -file "C:\Users\Ayalon\Documents\GitHub\ESIG_SW_SPO2\Debug\ESIG_SW_SPO2.ldr"

@pause