from math import pi

class ManRobot:
	linkX=0
	linkY=0
	linkZ=0
	cam=0	
	X_enc=0.0
	Y_enc=0.0
	Z_enc=0.0
	client=0
	simTime=0
	cam_image=[]
	__simState=0
	
	def __init__(self, cl):
		self.client=cl
		errHand, self.linkX=self.client.simxGetObjectHandle('X', self.client.simxServiceCall() )
		errHand, self.linkY=self.client.simxGetObjectHandle('Y', self.client.simxServiceCall() )
		errHand, self.linkZ=self.client.simxGetObjectHandle('Z', self.client.simxServiceCall() )
		errHand, self.cam=self.client.simxGetObjectHandle('RoboCamera', self.client.simxServiceCall() )
		
		self.client.simxGetJointPosition(self.linkX, self.client.simxDefaultSubscriber(self.f_X_enc) )
		self.client.simxGetJointPosition(self.linkY, self.client.simxDefaultSubscriber(self.f_Y_enc) )
		self.client.simxGetJointPosition(self.linkZ, self.client.simxDefaultSubscriber(self.f_Z_enc) )
		self.client.simxGetSimulationTime( self.client.simxDefaultSubscriber(self.getSimTime) )
		self.client.simxGetSimulationState( self.client.simxDefaultSubscriber(self.getSimState) )
		self.client.simxGetVisionSensorImage(self.cam, False, self.client.simxDefaultSubscriber(self.getCameraImage,1) )
		
	def setPosX(self, pos=0):
		p=pos
		if p>190:
			p=190
		if p<0:
			p=0
		errPos=self.client.simxSetJointTargetPosition(self.linkX, p*0.001, self.client.simxDefaultPublisher() )
		return errPos

	def setPosY(self, pos=0):
		p=pos
		if p>170:
			p=170
		if p<0:
			p=0
		errPos=self.client.simxSetJointTargetPosition(self.linkY, p*0.001, self.client.simxDefaultPublisher() )
		return errPos
	
	def setPosZ(self, pos=0):
		p=pos
		if p>190:
			p=190
		if p<0:
			p=0
		errPos=self.client.simxSetJointTargetPosition(self.linkZ, p*0.001, self.client.simxDefaultPublisher() )
		return errPos
	
	def setPositions(self, posX=0, posY=0, posZ=0):
		self.setPosX(posX)
		self.setPosY(posY)
		self.setPosZ(posZ)
        #return self.setPosX(posX) and self.setPosY(posY) and self.setPosZ(posZ)
	
	def grapObject(self):
		errSig=self.client.simxSetIntSignal('succtionActive', 1, self.client.simxDefaultPublisher() )
		return errSig
	
	def releaseObject(self):
		errSig=self.client.simxSetIntSignal('succtionActive', 0, self.client.simxDefaultPublisher() )
		return errSig
	
	def getCameraImage(self, msg):
		if msg[0] and self.__simState>0:
			self.cam_image=msg[2]
	
	def setCameraResolution(self, x, y):
		resX=x
		resY=y
		if resX<1: resX=1
		if resX>1024: resX=1024
		if resY<1: resY=1
		if resY>1024: resY=1024
		errParX=self.client.simxSetObjectIntParameter(self.cam, 1002, resX, self.client.simxDefaultPublisher() )
		errParY=self.client.simxSetObjectIntParameter(self.cam, 1003, resY, self.client.simxDefaultPublisher() )
	
	def setPID(self,link,kp,ki,kd):
		self.client.simxSetObjectFloatParameter(link, 2002, kp, self.client.simxDefaultPublisher() )
		self.client.simxSetObjectFloatParameter(link, 2003, ki, self.client.simxDefaultPublisher() )
		self.client.simxSetObjectFloatParameter(link, 2004, kd, self.client.simxDefaultPublisher() )
	
	def resetPID(self,link):
		self.setPID(link, 0.1,0,0)
		
	def setMaxSpeed(self,link, speed):
		#self.client.simxSetObjectFloatParameter(link, 2017, speed, self.client.simxDefaultPublisher() )
		args=[link, speed]
		ret=self.client.simxCallScriptFunction('remoteResetDyn@ManIRS_junior_robot','sim.scripttype_customizationscript',args,self.client.simxServiceCall())
		return ret
		
	def getSimTime(self, msg):
		if msg[0] and self.__simState>0:
			self.simTime=msg[1]
		
	def f_X_enc(self,msg):
		if msg[0] and self.__simState>0:
			self.X_enc=msg[1]*100
	
	def f_Y_enc(self,msg):
		if msg[0] and self.__simState>0:
			self.Y_enc=msg[1]*1000
	
	def f_Z_enc(self,msg):
		if msg[0] and self.__simState>0:
			self.Z_enc=msg[1]*1000
			
	def getSimState(self, msg):
		if msg[0]:
			self.__simState=msg[1]