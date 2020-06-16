# ChiLOG
This is a simple library which implements logging features in plain text and a predefined CSV format suitable to use with the values sent by Kelly motor controller.

This library keeps a file containing all previously created log files and determines the name of the next log file based on the prefix given in setNames() function and a number as: \<prefix\>\<number\>.\<extension\>. This number is determined automatically using the name of the previous log file. This is to make it independent of the system time which might be wrong (if RTC is not present; the log files will get over written).

###Note
CAN_logger.c file may not be compiled without BSP-Yocto-i.MX6-PD18.1.0 for phyBoard-Mira-i.MX6.
