/*
  This is a library written for the SparkFun Qwiic Keypad
  SparkFun sells these at its website: www.sparkfun.com
  Do you like this library? Help support SparkFun. Buy a board!
  https://www.sparkfun.com/products/15168

  Written by Pete Lewis @ SparkFun Electronics, 3/12/2019
  Much of the code originally came from SparkX version,
  Located here: https://github.com/sparkfunX/Qwiic_Keypad

  The Qwiic Keypad is a I2C controlled 12 button keypad

  https://github.com/sparkfun/SparkFun_Qwiic_Keypad_Arduino_Library

  Development environment specifics:
  Arduino IDE 1.8.8

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <stdio.h>
#include <string.h>
#include "qwiic_i2c.h"
#include "qwiic_keypad.h"

Qwiic_I2C qwiic;

//Constructor
KEYPAD::KEYPAD(uint8_t deviceAddress)
{
    _deviceAddress = deviceAddress;
}

//Initializes the I2C connection
//Returns false if board is not detected
bool KEYPAD::begin()
{

    // init I2C.

    if(!qwiic.init()){
        printf("Error: I2C subsystem failed to initialize.");
        return false;
    }
  return isConnected();

}

//Returns true if I2C device ack's
bool KEYPAD::isConnected(){

  return qwiic.pingDevice(_deviceAddress);
}

//Change the I2C address of this address to newAddress
bool KEYPAD::setI2CAddress(uint8_t newAddress)
{
    if (!(8 <= newAddress && newAddress <= 119)){
        // bad address.
        printf("Invalid Keypad address\n");
        return false;
     }
  
    qwiic.writeRegister(_deviceAddress, KEYPAD_CHANGE_ADDRESS, newAddress);
    sleep_ms(100); // allow time for slave device to write new address to EEPROM

    uint8_t tmp = _deviceAddress;
    //Once the address is changed, we need to change it in the library
    _deviceAddress = newAddress;

    if(!isConnected()){
        printf("[KEYPAD] - Unable to Change I2C address\n");

        _deviceAddress = tmp;
        return false;
    }
    return true;
  
}

////Returns the button at the top of the stack (aka the oldest button)
uint8_t KEYPAD::getButton(){

    return qwiic.readRegister(_deviceAddress, KEYPAD_BUTTON);
}

//Returns the number of milliseconds since the current button in FIFO was pressed.
uint16_t KEYPAD::getTimeSincePressed(){

    uint16_t MSB = qwiic.readRegister(_deviceAddress, KEYPAD_TIME_MSB);

    return MSB << 8 | qwiic.readRegister(_deviceAddress, KEYPAD_TIME_LSB); 
}

//Returns a string of the firmware version number
uint16_t KEYPAD::getVersion(){

    uint16_t MSB = qwiic.readRegister(_deviceAddress, KEYPAD_VERSION1);

    return MSB << 8 | qwiic.readRegister(_deviceAddress, KEYPAD_VERSION2);
}

// "commands" keypad to plug in the next button into the registerMap
// note, this actually sets the bit0 on the updateFIFO register
void KEYPAD::updateFIFO(){

    qwiic.writeRegister(_deviceAddress, KEYPAD_UPDATE_FIFO, 0x01); // set bit0, commanding keypad to update fifo
}	
