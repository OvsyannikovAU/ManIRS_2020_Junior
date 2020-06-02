#include <iostream>
#include "b0RemoteApi.h"
//#include <msgpack.hpp>

#pragma once
class ManRobot
{
public:
	int linkX = 0;
	int linkY = 0;
	int linkZ = 0;
	int cam = 0;
	float X_enc = 0.0;
	float Y_enc = 0.0;
	float Z_enc = 0.0;
	float simTime = 0.0;
	std::string cam_image="987";

	ManRobot(b0RemoteApi *cl);
	bool setPosX(float pos);
	bool setPosY(float pos);
	bool setPosZ(float pos);
	bool setPositions(float posX, float posY, float posZ);
	bool grapObject();
	bool releaseObject();
	void setPID(int link, float kp, float ki, float kd);
	void resetPID(int link);
	void setMaxSpeed(int link, float speed);
	void getCameraImage(std::vector<msgpack::object>* msg);
	void setCameraResolution(int x, int y);
	void getSimTime(std::vector<msgpack::object>* msg);
	void getXEnc(std::vector<msgpack::object>* msg);
	void getYEnc(std::vector<msgpack::object>* msg);
	void getZEnc(std::vector<msgpack::object>* msg);
	void getSimState(std::vector<msgpack::object>* msg);

private:
	b0RemoteApi *client = NULL;
	int simState = 0;
};

