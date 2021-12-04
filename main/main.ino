#include <LinkedList.h>
#include "CapSense.h"

#define tonePin A5

//notes
unsigned int currOct = 1;                                       //which octave to use, DO NOT CHANGE, not supported yet
const int notes[3][8] = {                                       //each frequency of a note from index 0 to 7 goes from C to C in one octave
    {//low
        66,
        73,
        83,
        87,
        98,
        110,
        124,
        132
    },

    {//default
        132,
        147,
        165,
        175,
        196,
        220,
        247,
        262
    },

    {//high
        262,
        294,
        330,
        349,
        392,
        440,
        494,
        523
    }
};
LinkedList<int> keys;                                           //holds all notes to be played

//input
const int thsd = 400;                                           //threshold value
CapSense pins[] = {                                             //all pins that will be using capacitive sense
    CapSense(2,3, thsd),
    CapSense(2,4, thsd),
    CapSense(5,6, thsd),
    CapSense(5,7, thsd),
    CapSense(8,9, thsd),
    CapSense(8,10, thsd),
    CapSense(11,12, thsd),
    CapSense(11,13, thsd),
};
CapSense modulatorPin = CapSense(A0,A1,thsd);                   //pin to modulate the sounds produced
unsigned const int pinsSize = sizeof(pins)/sizeof(pins[0]);     //calculate the length of an array

void setup() {
    Serial.begin(9600);                                         //debug
}

void Remove(int index) {                                        //simple Linear search method that removes keys to be played
    int length = sizeof(notes)/sizeof(notes[0]);

    for(unsigned int i = 0 ; i < length ; i++) {                //check all octaves
        int target = notes[i][index];
        for(int i = 0 ; i < keys.size() ; i++) {                //linear search through array
            if(target == keys.get(i)) {
                keys.remove(i);                                 //if the right frequency is found, remove
            }
        }
    }
}

int mod = 0;                                                    //pitch change for option 2
int option = 1;                                                 //what type of modulation should be used 1 = entire octave change | 2 = pitch modulation
void modulate(int opt) {
    long value = modulatorPin.read();
    Serial.print(value);

    switch (opt)
    {
    case 1:                                                     //octave change option
        if(value > thsd) { 
            currOct = 2;
        } else {
            currOct = 1;
        }
        break;

    case 2:                                                     //modulate pitch option
        value = value > 200 ? value : 0;
        mod = value/80;
        break;
    }
}

void loop() {
    modulate(option);

    int length = sizeof(pins)/sizeof(pins[0]);
    for(unsigned int i = 0 ; i < length ; i++) {
        CapSense& cs = pins[i];
        cs.read();
        
        if( i < (sizeof(pins)/sizeof(pins[0]) - 1)) {           //debug purposes
            Serial.print(cs.read());
            Serial.print(" | ");
        }
        else{
            Serial.println(cs.read());
        }

        if(cs.rose()) {
            keys.unshift(notes[currOct][i]);                    //add correct note to the top of the keys to be played
        }
        if(cs.fell()) {
            Remove(i);                                          //remove the note assigned to the key in the keys to be played
        }
    }

    // Serial.println(keys.size());
    if(keys.size() != 0) {
        
        if(option == 1)
            tone(tonePin,keys.get(0));                          //access the top of the list which has the most recent note played
        else
            tone(tonePin, keys.get(0) + mod);                   //for when the modulation is in use
    } else {
        noTone(tonePin);
    }
}
