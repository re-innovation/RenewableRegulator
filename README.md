# Renewable Regulator

# Overview

This is an open-source project to design a simple, low-cost, Charge Regulator Designs for off-grid renewable energy projects.

A charge-controller protects a re-chargeable battery from over (and sometime under) charging from a renewable energy resource.

This controller will work as a Series and Shunt controller. Future improvements include working as a Maximum Power Point Tracking.
The design is based upon the Arduino IDE and will use one version of the Arduino processors (Arduino Nano initially).
Voltage is measured using a potential divider.
Current is measured using a hall-effect current sensor.
Temperature is measured using a 1-wire digital temperature probe.

A small OLED or LCD screen is used to show the data.
A rotary encoder is used for input data.
Data will also be sent via a serial connection for data-logging use.


## Specifications:

Maximum working voltage: 60V DC
Maximum working current: 50A DC
Measurement accuracy voltage:
Measurement accuracy current:
Temperature: -20 to 150C 0.1C steps



## Contributors

Overview and project concept by Matthew Little (matt@re-innovation.co.uk)
Phase 1 by Arnaud Moulas (arnaud.moulas@gmail.com)
Phase 2 by Yoan Gornes (yoan.gornes@gmail.com)

This project has been part of a larger project for modular power electronics concept initiated by Wind Empowerment:
http://www.wisions.net/projects/exchange-modular-power-to-the-people-democratising-energy-access-through-mo#project121

This section of the project is designed to stand-alone as a charge controller, but the code will be incorporated into the larger project.

# Series Regulator

A series charge controller or series regulator disables further current flow into batteries when they are full.


# Shunt Regulator

A shunt charge controller or shunt regulator diverts excess electricity to an auxiliary or "shunt" load, such as an electric water heater, when batteries are full.

# MPPT Algorithm

At present this function is not included. In the future a DC-DC converter can be used to separately


# Battery types

A rechargeable battery, storage battery, secondary cell, or accumulator is a type of electrical battery which can be charged, discharged into a load, and recharged many times, while a non-rechargeable or primary battery is supplied fully charged, and discarded once discharged. It is composed of one or more electrochemical cells. The term "accumulator" is used as it accumulates and stores energy through a reversible electrochemical reaction. Rechargeable batteries are produced in many different shapes and sizes, ranging from button cells to megawatt systems connected to stabilize an electrical distribution network.
Several different combinations of electrode materials and electrolytes are used, including lead–acid, nickel cadmium (NiCd), nickel metal hydride (NiMH), lithium ion (Li-ion), and lithium ion polymer (Li-ion polymer).


# Data Protocol
