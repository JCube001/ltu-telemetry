# AeroC2

A [MAVLink](http://qgroundcontrol.org/mavlink/start) based protocol for the
Command and Control (C2) of MAVs.

## Purpose

The goal of this project is to redesign the MAVLink MAV marshaling protocol
in such a way that it will perform better in systems where there is a stronger
focus on gathering telemetry.

## Install

For Arduino users, you can use this library by placing the entire AeroC2
folder into your libraries folder and then include "AeroC2.h" in your sketch.

For everyone else, simply copy the contents on the "inc" folder into your
project. Then include the header file "aeroc2.h".

## Changes from MAVLink

* Tweaked API.
* Enhanced Doxygen comments.
* The ICAO approved CRC-32Q checksum is used instead of X25.
* Larger minimum message frame size.
* Removed the separate logical channels portion of the parser state machine.
* Enumerated message type IDs.
* Changed the set of common messages.
