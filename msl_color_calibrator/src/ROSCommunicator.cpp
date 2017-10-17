#include "ROSCommunicator.h"

#include <msl/robot/IntRobotID.h>
#include <msl/robot/IntRobotIDFactory.h>
#include <supplementary/IAgentID.h>

#include <supplementary/BroadcastID.h>

#include <iostream>

#include <pthread.h>

using namespace std;

bool ROSCommunicator::rosInitCalled;
bool ROSCommunicator::initialized;
ROSCommunicator *ROSCommunicator::instance;

msl_sensor_msgs::CameraSettings *ROSCommunicator::cameraSettingsMsgs;
msl_sensor_msgs::CameraSettingsRequest *ROSCommunicator::cameraSettingsRequestMsgs;

ros::Subscriber ROSCommunicator::settingSub;
ros::Publisher ROSCommunicator::settingPub;
ros::Publisher ROSCommunicator::settingRequestPub;
ros::NodeHandle *ROSCommunicator::visionNode;

ROSCommunicator *ROSCommunicator::getInstance()
{
    if (instance == NULL)
    {
        instance = new ROSCommunicator();
    }
    return instance;
}

ROSCommunicator::ROSCommunicator()
{
}

void ROSCommunicator::initialize()
{
    if (!rosInitCalled)
    {
        rosInitCalled = true;
        int argc = 0;
        char **argv = NULL;
        ros::init(argc, argv, "CNCalibration");
    }

    visionNode = new ros::NodeHandle();

    cameraSettingsMsgs = new msl_sensor_msgs::CameraSettings();
    auto tmp = new supplementary::BroadcastID(nullptr, 0);
    cameraSettingsMsgs->senderID.id = tmp->toByteVector();
    cameraSettingsRequestMsgs = new msl_sensor_msgs::CameraSettingsRequest();

    settingSub = visionNode->subscribe<msl_sensor_msgs::CameraSettings>("CNCalibration/CameraSettings", 1, &ROSCommunicator::handleSettings);

    settingPub = visionNode->advertise<msl_sensor_msgs::CameraSettings>("CNCalibration/CameraSettings", 1);

    settingRequestPub = visionNode->advertise<msl_sensor_msgs::CameraSettingsRequest>("CNCalibration/CameraSettingsRequest", 1);

    pthread_t thread;
    pthread_create(&thread, NULL, &ROSCommunicator::rosSpin, NULL);

    initialized = true;
}

bool ROSCommunicator::isROScoreRunning()
{
    if (!rosInitCalled)
    {
        rosInitCalled = true;
        int argc = 0;
        char **argv = NULL;
        ros::init(argc, argv, "CNCalibration");
    }
    return ros::master::check();
}

void *ROSCommunicator::rosSpin(void *threadid)
{
    ros::spin();
}

void ROSCommunicator::requestSettings(std::vector<int> &receiverIDs)
{
    cameraSettingsRequestMsgs->receiverIDs.clear();
    if (!receiverIDs.empty())
    {
        msl::robot::IntRobotIDFactory factory;
        for (std::vector<int>::iterator it = receiverIDs.begin(); it != receiverIDs.end(); ++it)
        {
            auto intID = *it;
            std::vector<uint8_t> id;

            for (int i = 0; i < sizeof(int); i++)
            {
                id.push_back(*(((uint8_t *)&intID) + i));
            }
           const msl::robot::IntRobotID* tmpID = factory.create(id);

            cameraSettingsRequestMsgs->receiverIDs.push_back(tmpID);
        }

        settingRequestPub.publish(*cameraSettingsRequestMsgs);
        for (auto it = cameraSettingsRequestMsgs->receiverIDs.begin(); it != cameraSettingsRequestMsgs->receiverIDs.end();)
        {
        	delete &it;
        }
    }
}

void ROSCommunicator::sendSettings(const msl::robot::IntRobotID *receiverID, CameraCalibration::Settings *settings)
{
    cameraSettingsMsgs->receiverID = receiverID;
    cameraSettingsMsgs->useBrightness = settings->useBrightness;
    cameraSettingsMsgs->brightness = settings->brightness;
    cameraSettingsMsgs->exposure = settings->exposure;
    cameraSettingsMsgs->autoWhiteBalance = settings->autoWhiteBalance;
    cameraSettingsMsgs->whiteBalance1 = settings->whiteBalance1;
    cameraSettingsMsgs->whiteBalance2 = settings->whiteBalance2;
    cameraSettingsMsgs->hue = settings->hue;
    cameraSettingsMsgs->saturation = settings->saturation;
    cameraSettingsMsgs->gamma = settings->gamma;
    cameraSettingsMsgs->autoShutter = settings->autoShutter;
    cameraSettingsMsgs->shutter = settings->shutter;
    cameraSettingsMsgs->autoGain = settings->autoGain;
    cameraSettingsMsgs->gain = settings->gain;
    settingPub.publish(*cameraSettingsMsgs);
}

void ROSCommunicator::handleSettings(const msl_sensor_msgs::CameraSettings::ConstPtr &msg)
{
    getInstance()->emit receivedSettings(msg);
}
