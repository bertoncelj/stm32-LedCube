# RGB LED Cube 4x4x4 TicTacToe 

Cube when user can play 4x4x4 TicTacToe game vs computer which uses MinMax algorithem.
Software runs on STM32F4DISCOVERY kit with high performance microcontroller STM32F407VGT6.
LEDS are control by TLC5940 chip.

## Cube
[Album](https://imgur.com/a/et8Eu76)


### Description

Runs on 3 TLC5940, which each one control one of three colors. TLC have 16 outputs where is connected LED katode, and it can be pushed to the ground. 4 layers (16 leds in one layer) in hight, toggeld by MOSFETS, one layer at the time.
Game TicTacToe can be play on where user with 3 potenciometers add his own coordinates in. Computer uses [MinMax] logic to calculate his next move. First one to 4 in a row wins!

### Datasheet

* [STM32F4DISCOVERY](http://www.st.com/content/ccc/resource/technical/document/user_manual/70/fe/4a/3f/e7/e1/4f/7d/DM00039084.pdf/files/DM00039084.pdf/jcr:content/translations/en.DM00039084.pdf) - general inforamtion
* [TLC5940](http://www.ti.com/lit/ds/symlink/tlc5940.pdf) - manual
* [Shematic](https://cdn.instructables.com/ORIG/F7Q/NBEM/H1QMLAYD/F7QNBEMH1QMLAYD.png) - for Arduino

## Installing

What things you need to install the software and how to install them
* IDE using are [System Workbench for STM32, AC6 Tool](http://www.openstm32.org/System%2BWorkbench%2Bfor%2BSTM32) and new [TrueSTUDIO](http://www.st.com/en/development-tools/truestudio.html)
* Exported project [Download]()


### Built With

* [TM_Libraries](http://stm32f4-discovery.net/2014/05/all-stm32f429-libraries-at-one-place/) - Dependency
* [STM32 Standard Peripheral Libraries](http://www.st.com/en/embedded-software/stm32-standard-peripheral-libraries.html?querycriteria=productId=LN1939) - Main lib



## Running the tests TicTacToe MinMax algorithem

Test in terminal Console-Application. Play vs computer with 2 layer depth recursive function. Run ConsoleApp_main.c in termianl:
```
make ConsoleApp_main
```
Run it:
```
./ConsoleApp_main
```
Play by type number from 1 to 64. Imagin 2D board as 3D cube stack layer on top of layer.

## Authors

* **Tine Beroncelj** - tine.bertoncelj@gmail.com
