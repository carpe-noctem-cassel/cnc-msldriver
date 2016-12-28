/*
 * $Id: CompassValueHelper.h 1935 2007-03-19 19:50:12Z phbaer $
 *
 *
 * Copyright 2005,2006 Carpe Noctem, Distributed Systems Group,
 * University of Kassel. All right reserved.
 *
 * The code is derived from the software contributed to Carpe Noctem by
 * the Carpe Noctem Team.
 *
 * The code is licensed under the Carpe Noctem Userfriendly BSD-Based
 * License (CNUBBL). Redistribution and use in source and binary forms,
 * with or without modification, are permitted provided that the
 * conditions of the CNUBBL are met.
 *
 * You should have received a copy of the CNUBBL along with this
 * software. The license is also available on our website:
 * http://carpenoctem.das-lab.net/license.txt
 *
 *
 * <description>
 */
#ifndef CompassValueHelper_H
#define CompassValueHelper_H

#include <boost/thread/mutex.hpp>
//#include <NetAddress.h>
#include <msl_sensor_msgs/CompassInfo.h>
#include <string>
#include "ros/ros.h"

#include "../global/Types.h"


class CompassValueHelper{


    public:
        CompassValueHelper();
        ~CompassValueHelper();

        void integrateData(int value);
        int getCompassData();
        int getCompassData2();

        static CompassValueHelper * getInstance();

    protected:

        static CompassValueHelper * instance_;

        ros::Subscriber sub;
        void init();
        void cleanup();

        int compassValue;
        int updateCycles;

        ros::AsyncSpinner* spinner;

        void handleCompassInfo(const msl_sensor_msgs::CompassInfo::ConstPtr& message);

        boost::mutex mutex;

        bool workWithoutCompass;
};

#endif

