#include "ManRobot.h"

ManRobot::ManRobot(b0RemoteApi *cl)
{
	client = cl;
	std::vector<msgpack::object>* reply1 = client->simxGetObjectHandle("X", client->simxServiceCall());
	linkX = b0RemoteApi::readInt(reply1, 1);
	std::vector<msgpack::object>* reply2 = client->simxGetObjectHandle("Y", client->simxServiceCall());
	linkY = b0RemoteApi::readInt(reply2, 1);
	std::vector<msgpack::object>* reply3 = client->simxGetObjectHandle("Z", client->simxServiceCall());
	linkZ = b0RemoteApi::readInt(reply3, 1);
	std::vector<msgpack::object>* reply4 = client->simxGetObjectHandle("RoboCamera", client->simxServiceCall());
	cam = b0RemoteApi::readInt(reply4, 1);

	boost::function<void(std::vector<msgpack::object>*)> fp1 = boost::bind(&ManRobot::getXEnc, this, _1);
	client->simxGetJointPosition(linkX, client->simxDefaultSubscriber(fp1, 1));
	boost::function<void(std::vector<msgpack::object>*)> fp2 = boost::bind(&ManRobot::getYEnc, this, _1);
	client->simxGetJointPosition(linkY, client->simxDefaultSubscriber(fp2, 1));
	boost::function<void(std::vector<msgpack::object>*)> fp3 = boost::bind(&ManRobot::getZEnc, this, _1);
	client->simxGetJointPosition(linkZ, client->simxDefaultSubscriber(fp3, 1));
	boost::function<void(std::vector<msgpack::object>*)> fp7 = boost::bind(&ManRobot::getSimState, this, _1);
	client->simxGetSimulationState(client->simxDefaultSubscriber(fp7, 1));
	boost::function<void(std::vector<msgpack::object>*)> fp8 = boost::bind(&ManRobot::getSimTime, this, _1);
	client->simxGetSimulationTime(client->simxDefaultSubscriber(fp8, 1));
	boost::function<void(std::vector<msgpack::object>*)> fp9 = boost::bind(&ManRobot::getCameraImage, this, _1);
	client->simxGetVisionSensorImage(cam, false, client->simxDefaultSubscriber(fp9, 1));
}

bool ManRobot::grapObject()
{
	bool errSig = client->simxSetIntSignal("succtionActive", 1, client->simxDefaultPublisher());
	return errSig;
}

bool ManRobot::releaseObject()
{
	bool errSig = client->simxSetIntSignal("succtionActive", 0, client->simxDefaultPublisher());
	return errSig;
}

bool ManRobot::setPosX(float pos)
{
	float p = pos;
	if (p > 190) { p = 190; }
	if (p < 0) { p = 0; }
	bool errPos = client->simxSetJointTargetPosition(linkX, p * 0.001, client->simxDefaultPublisher());
	return errPos;
}

bool ManRobot::setPosY(float pos)
{
	float p = pos;
	if (p > 170) { p = 170; }
	if (p < 0) { p = 0; }
	bool errPos = client->simxSetJointTargetPosition(linkY, p * 0.001, client->simxDefaultPublisher());
	return errPos;
}

bool ManRobot::setPosZ(float pos)
{
	float p = pos;
	if (p > 190) { p = 190; }
	if (p < 0) { p = 0; }
	bool errPos = client->simxSetJointTargetPosition(linkZ, p * 0.001, client->simxDefaultPublisher());
	return errPos;
}

bool ManRobot::setPositions(float posX, float posY, float posZ)
{
	bool resX = setPosX(posX);
	bool resY = setPosY(posY);
	bool resZ = setPosZ(posZ);
	return  resX && resY && resZ;
}

void ManRobot::setPID(int link, float kp, float ki, float kd)
{
	client->simxSetObjectFloatParameter(link, 2002, kp, client->simxDefaultPublisher());
	client->simxSetObjectFloatParameter(link, 2003, ki, client->simxDefaultPublisher());
	client->simxSetObjectFloatParameter(link, 2004, kd, client->simxDefaultPublisher());
}

void ManRobot::resetPID(int link)
{
	setPID(link, 0.1, 0, 0);
}

void ManRobot::setMaxSpeed(int link, float speed)
{
	//client->simxSetObjectFloatParameter(link, 2017, speed, client->simxDefaultPublisher());

	std::tuple <int, float > args(link, speed);
	std::stringstream packedArgs;
	msgpack::pack(packedArgs, args);
	std::vector<msgpack::object>* reply=client->simxCallScriptFunction("remoteResetDyn@ManIRS_junior_robot", "sim.scripttype_customizationscript", packedArgs.str().c_str(), packedArgs.str().size(), client->simxServiceCall());
}

void ManRobot::getCameraImage(std::vector<msgpack::object>* msg)
{
	bool errRead = b0RemoteApi::readInt(msg, 0);
	if (!errRead && simState > 0) 
	{
		cam_image=b0RemoteApi::readByteArray(msg, 2);
	} 
}

void ManRobot::setCameraResolution(int x, int y)
{
	int resX = x;
	int resY = y;
	if (resX < 1) { resX = 1; }
	if (resX > 1024) { resX = 1024; }
	if (resY < 1) { resY = 1; }
	if (resY > 1024) { resY = 1024; }
	client->simxSetObjectIntParameter(cam, 1002, resX, client->simxDefaultPublisher());
	client->simxSetObjectIntParameter(cam, 1003, resY, client->simxDefaultPublisher());
}

void ManRobot::getXEnc(std::vector<msgpack::object>* msg)
{
	bool errRead = b0RemoteApi::readInt(msg, 0);
	if (!errRead && simState>0) { X_enc = b0RemoteApi::readFloat(msg, 1)*1000; }
}

void ManRobot::getYEnc(std::vector<msgpack::object>* msg)
{
	bool errRead = b0RemoteApi::readInt(msg, 0);
	if (!errRead && simState > 0) { Y_enc = b0RemoteApi::readFloat(msg, 1) * 1000; }
}

void ManRobot::getZEnc(std::vector<msgpack::object>* msg)
{
	bool errRead = b0RemoteApi::readInt(msg, 0);
	if (!errRead && simState > 0) { Z_enc = b0RemoteApi::readFloat(msg, 1) * 1000; }
}

void ManRobot::getSimState(std::vector<msgpack::object>* msg)
{
	bool errRead= b0RemoteApi::readInt(msg, 0);
	if (!errRead) { simState = b0RemoteApi::readInt(msg, 1); }
}

void ManRobot::getSimTime(std::vector<msgpack::object>* msg)
{
	bool errRead = b0RemoteApi::readInt(msg, 0);
	if (!errRead) { simTime = b0RemoteApi::readFloat(msg, 1); }
}