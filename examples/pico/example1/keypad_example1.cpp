/**
 * Copyright (c) 2020 SparkFun Electronics
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"
#include "qwiic_keypad.h"

KEYPAD myKeypad;


int main() {

     // setup stdio. Output device is controlled in CMakeLists.txt file
    stdio_init_all();

	printf("Starting KEYPAD Test\n\n");

    // Init the system 

    if(!myKeypad.begin()){
    	printf("Error starting the qwiic keypad device. Aboarting\n");
  		return 0;
    }

    char chButton;
    while (1) {

        myKeypad.updateFIFO();  // necessary for keypad to pull button from stack to readable register
        chButton = myKeypad.getButton();

        switch(chButton){

            case -1:
                printf("No keypad deteced\n");
                sleep_ms(1000);
                break;
            case '#':
                printf("\n");
                break;
            case '*':
                printf(" ");
                break;
            case 0:
                break;
            default:
                printf("%c", chButton);
                break;
        }
  
        sleep_ms(50);

    }

    return 0;
}
