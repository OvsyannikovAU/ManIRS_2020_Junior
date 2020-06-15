--User functions------------------------------------------
function init()
    --[[Example of using standart functions of robot:
    
    simTime=sim.getSimulationTime() -- get time of simulation
    --simTime - float number in seconds, e.g. simPime=0.563 [s]
        
    --Robot have gripper to take and realise objects:
    releaseObject() --deactivate gripper
    grapObject() -- activate gripper
    
    --Robot have a camera (resolution 256 x 256 pixeles):
    img=getCameraImage() --get full camera image in RGB mode, resolution 256 x 256 pixeles
    --Return number array with lenght=256*256*3=196608 values. Array struct:
    --{pix_0x0_red, pix_0x0_green, pix_0x0_blue, pix_0x1_red, pix_0x1_green, pix_0x1_blue...}
    -- Each value in range 0..1, e.g.
    --{0.80000001192093, 0.7843137383461, 0.76470589637756...}
    
    x=0
    y=0
    pixel=getCameraPixel(x,y) --get RGB pixel values from camera
    --Return number array with lenght 3. Array struct:
    --{pixel_red, pixel_green, pixel_blue}
    -- Each value in range 0..1, e.g.
    --{0.80000001192093, 0.7843137383461, 0.76470589637756}
    --Pixel numering in range 0..255 (camera resolution 256 x 256)
    
    x=0
    x_size=10
    y=0
    y_size=10
    imgField=getCameraField(x, x_size,y, y_size) --get  part of camera image in RGB mode
    --Must be x+x_size<255 and y+y_size<255
    --Pixel numering in range 0..255 (camera resolution 256 x 256)
    --Return number array with lenght x_size*y_size*3. Array struct:
    --{pix_X_Y_red, pix_X_Y_green, pix_X_Y_blue, pix_X_Y+1_red, pix_X_Y+1_green, pix_X_Y+1_blue...}
    -- Each value in range 0..1, e.g.
    --{0.80000001192093, 0.7843137383461, 0.76470589637756...}
    
    
    --Move robot link:
    posX=95 -- mm 0..190
    posY=85 -- mm 0..170
    posZ=95 -- mm 0..190
    setPosX(posX) --set joint X to position
    setPosY(posY) --set joint Y to position
    setPosZ(posZ) --set joint Z to position
    setPositions(posX,posY,posZ) --set all joints to positions.
    
    kp=0.1    --float num
    ki=0.01   --float num
    kd=0.1    --float num
    speed=180 --mm/s
    link=linkX
    setPID(link, kp, ki, kd) --set new PID-parameters to link
    resetPID(link) --set to link default parameters kp=0.1, ki=0, kd=0
    setMaxSpeed(link, speed) --set maximum speed to link
    
    --See also:
    --ReadMe for information about robot and simulator
    --https://coppeliarobotics.com/helpFiles/en/apiFunctionListCategory.htm - list of all Lua regular API function
    ]]--
end

function simulationStepStarted()
    
end

function simulationStepDone()

end

function cleanup()

end
--Put your functions here







--system functions------------------------------------------
function sysCall_init()
    require "Programming/Lua/RobotFunctions"
    roboInit()
    init() 
end
function sysCall_sensing()
    X_enc=sim.getJointPosition(linkX)*1000
    Y_enc=sim.getJointPosition(linkY)*1000
    Z_enc=sim.getJointPosition(linkZ)*1000
    simulationStepDone()
end

function sysCall_actuation()
    simulationStepStarted()
end

function sysCall_cleanup()
    cleanup()
end

function remoteResetDyn(args)
    local spd=args[2]
    JT=sim.getJointType(args[1])
    joint=sim.getObjectName(args[1])
    if JT>-1 then
        if JT==sim.joint_revolute_subtype then spd=args[2]*math.pi/180 end
        if JT==sim.joint_prismatic_subtype then spd=args[2]*0.001 end
    end
    sim.setObjectFloatParameter(args[1], sim.jointfloatparam_upper_limit, spd)
    local res=sim.resetDynamicObject(args[1])
	return res
end