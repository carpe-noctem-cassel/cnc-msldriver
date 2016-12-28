
//============================================================================
// Name        : ga.cpp
// Author      : Claas Lühring
// Version     : Eins
// Copyright   : 1000€
// Description : Misst das Aussehen einer Referenzfläche bei verschiedenen Gainparametern.
//============================================================================

#include <iostream>

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "../Brightness.h"
#include "../Whitepoint.h"
#include <Configuration.h>
#include "../../driver/imagingsource.h"
#include "../../helpers/KeyHelper.h"

using namespace std;

int main() {

    camera::ImagingSource* cam = NULL;

    //Je nach Kamera muss eine der folgenden Zeilen ein- bzw. auskommentiert sein.
    cam = new camera::ImagingSource("Point Grey");
    //cam = new camera::ImagingSource("Imaging Source");
    cam->setVideoMode(DC1394_VIDEO_MODE_640x480_YUV422);
    cam->init();

    //cam->setFramerate(DC1394_FRAMERATE_30);  //DC1394_FRAMERATE_15

    //Wird nicht gebraucht
    cam->enableAutoShutter(false);
    cam->enableTrigger(false);
    cam->enableGamma(false);
    cam->setGamma(false);
    cam->enableAutoGain(false);
    cam->disableAutoWhiteBalance();

    //Set Hue
    unsigned char ucHue = 180;
    cam->setHue(ucHue);
    std::cout << "Hue: "<< (int)ucHue << std::endl;

    //Set Expo
    unsigned short usExpo = 0;
    cam->setExposure(usExpo);
    std::cout << "Expo :"<<  usExpo << std::endl;

    //Set Saturation
    unsigned char ucSat = 127;
    cam->setSaturation(ucSat);
    std::cout << "Sat: "<< (int) ucSat << std::endl;


    cam->startCapture();
    camera::Frame frame;
    cam->getFrame(frame);

    Brightness *br=Brightness::getInstance(640, 480, cam);
    Whitepoint *wp=Whitepoint::getInstance(640, 480, cam);

    int counter=0;

    unsigned char *a;

    KeyHelper::checkKeyPress();

    while (true){
        counter++;
        cam->getFrame(frame);
        br->process(frame.getImagePtr(), counter);
        wp->process(frame.getImagePtr(), counter);
        br->showRefFlaeche(frame.getImagePtr());
        wp->showRefFlaeche(frame.getImagePtr());
        if (counter%100==0){
            printf("Durchlauf: %i\n",counter);
                }
        KeyHelper::checkKeyPress();
        KeyHelper::checkRemoteKey();
        if(KeyHelper::checkKey('X')){
            cout<<"hier\n\n\n\n"<<endl;
            br->addBrightness(0.5);
        }
        if(KeyHelper::checkKey('x')){br->addBrightness(-0.5);}
    }
    return 0;
}
