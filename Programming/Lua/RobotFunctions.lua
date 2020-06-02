function roboInit()
	linkX=sim.getObjectHandle('X')
    linkY=sim.getObjectHandle('Y')
    linkZ=sim.getObjectHandle('Z')
    cam=sim.getObjectHandle('RoboCamera')
    X_enc=sim.getJointPosition(linkX)*1000
    Y_enc=sim.getJointPosition(linkY)*1000
    Z_enc=sim.getJointPosition(linkZ)*1000
end

function setPosX(pos)
    local p=pos
    if p>190 then p=190 end
    if p<0 then p=0 end
    sim.setJointTargetPosition(linkX, p*0.001)
end

function setPosY(pos)
local p=pos
    if p>170 then p=170 end
    if p<0 then p=0 end
    sim.setJointTargetPosition(linkY, p*0.001)
end

function setPosZ(pos)
    local p=pos
    if p>190 then p=190 end
    if p<0 then p=0 end
    sim.setJointTargetPosition(linkZ, p*0.001)
end

function setPositions(posX, posY, posZ)
    setPosX(posX)
    setPosY(posY)
    setPosZ(posZ)
end

function grapObject()
    sim.setIntegerSignal('succtionActive', 1)
end

function releaseObject()
    sim.setIntegerSignal('succtionActive', 0)
end

function getCameraImage()
    local img=sim.getVisionSensorImage(cam, 0,0,0,0,0)
    return img
end

function getCameraPixel(x, y)
    local img=sim.getVisionSensorImage(cam, x,y,1,1,0)
    return img
end

function getCameraField(x, x_size, y, y_size)
    local img=sim.getVisionSensorImage(cam, x,y,x_size,y_size,0)
    return img
end

function setCameraResolution(x, y)
	local resX=x
	local resY=y
	if resX<1 then resX=1 end
	if resX>1024 then resY=1024 end
	if resY<1 then resY=1 end
	if resY>1024 then resY=1024 end
	local resultX = sim.setObjectInt32Parameter(cam,sim.visionintparam_resolution_x, resX)
	local resultY = sim.setObjectInt32Parameter(cam,sim.visionintparam_resolution_y, resY)
	return resultX and resultY
end

function setPID(link,kp,ki,kd)
    sim.setObjectFloatParameter(link, sim.jointfloatparam_pid_p ,kp)
    sim.setObjectFloatParameter(link, sim.jointfloatparam_pid_i ,ki) 
    sim.setObjectFloatParameter(link, sim.jointfloatparam_pid_d ,kd) 
end

function resetPID(link)
    setPID(link, 0.1,0,0)
end

function setMaxSpeed(link, speed)
	local spd=speed
	JT=sim.getJointType(link)
    if JT>-1 then
        if JT==sim.joint_revolute_subtype then spd=speed*math.pi/180 end
        if JT==sim.joint_prismatic_subtype then spd=speed*0.001 end
    end
    sim.setObjectFloatParameter(link, sim.jointfloatparam_upper_limit, spd)
	sim.resetDynamicObject(link)
end