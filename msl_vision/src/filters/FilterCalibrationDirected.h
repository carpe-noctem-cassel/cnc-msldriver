/*
 * $Id: FilterCalibrationDirected.h 1531 2006-08-01 21:36:57Z phbaer $
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
#ifndef FilterCalibrationDirected_H
#define FilterCalibrationDirected_H

#include "Filter.h"


class FilterCalibrationDirected  : public Filter {


    public:
        FilterCalibrationDirected(int width, int height);
        ~FilterCalibrationDirected();

        unsigned char * process(unsigned char * src, int width, int height);

    protected:

        void init();
        void cleanup();

        int imWidth;
        int imHeight;


};




#endif

