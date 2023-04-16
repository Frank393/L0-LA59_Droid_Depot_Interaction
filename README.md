# L0-LA59_Droid_Depot_Interaction

This project makes the Hasbro Star Wars: Obi-Wan Kenobi LL-LA59 (Lola) Droid Electronic Figure interact with the droid from Galaxy Edge Droid Depot and the lands Bluetooth beacons.

Demo Video: https://youtu.be/LgOxleKGDac
## Hardware required

The board used in this project is the [Seeed Studio XIAO ESP32C3](https://www.seeedstudio.com/Seeed-XIAO-ESP32C3-p-5431.html). It was selected because of its compact size.
## Circuit

For power the BAT contact pad on the back of the board will be connected to the respected negative and positive from the main Lola board with the positive side having a switch connected in between. This way there is no need for a second power source.

For the interaction the bouton that Lola uses to make sounds has 2 contact pads next to it. The left pad will be connected to the ground and the right one will be connected to D1 in the board.
