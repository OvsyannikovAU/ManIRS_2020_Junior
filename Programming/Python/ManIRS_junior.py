import os
os.add_dll_directory(r'C:\Program Files\CoppeliaRobotics\CoppeliaSimEdu')

import b0RemoteApi
from RoboFunctions import ManRobot
import time


with b0RemoteApi.RemoteApiClient('b0RemoteApi_pythonClient','b0RemoteApi_manirs') as client:
    doNextStep=True
    robot=ManRobot(client)
    step=0 #переменная для подсчета шагов симуляции

    def init():
		#пример использования функций управления роботом:
        #подробнее о всех функицях, их параметрах и возвращаемых значения см ReadMe

        #энкодеры:
        #robot.X_enc #положение привода X
        #robot.Y_enc #положение привода Y
        #robot.Z_enc #положение привода Z
        
        #simTime=robot.simTime
        
        #posX=190  #мм в диапазоне 0..190
        #posY=170  #мм в диапазоне 0..170
        #posZ=190 #мм в диапазоне 0..190
        #robot.setPosX(posX)
        #robot.setPosY(posY)
        #robot.setPosZ(posZ)
        #robot.setPositions(posX, posY, posZ)

        #kp=0.1
        #ki=0.01
        #kd=0.1
        #robot.setPID(robot.linkX,kp,ki,kd) #установить параметры ПИД-регулятора для выбранного звена
        #robot.resetPID(robot.linkX)        #сбросить параметры ПИД-регулятора (kp=0.1, ki=0, kd=0)

        #robot.grapObject()    #включить схват
        #robot.releaseObject() #выключить схват

        #img=robot.cam_image
        time.time()
        
	#See also:
	#ReadMe for information about robot and simulator
	#https://coppeliarobotics.com/helpFiles/en/b0RemoteApi-python.htm - list of all Python B0 remote API function
	
    def simulationStepStarted(msg):
        #бездействие
        time.time()
        
    def simulationStepDone(msg):
        #бездействие
        global step #выводим текущий шаг
        print(step) #просто для демонстрации, можно отключить
        global doNextStep #а это обязательный участок кода
        doNextStep=True   #для синхронизации с основным потоком ниже
	
    def cleanup():
	#бездействие
        time.time()
		
    client.simxSynchronous(True)
    client.simxGetSimulationStepStarted(client.simxDefaultSubscriber(simulationStepStarted))
    client.simxGetSimulationStepDone(client.simxDefaultSubscriber(simulationStepDone))	
    res=client.simxStartSimulation(client.simxDefaultPublisher())
    init()
	#Put your main action here:
	
    startTime=time.time()
    startStep=step
    while robot.simTime<5: #крутить цикл 5 секунд с начала симуляции
        #варианты условий для выхода из цикла:
        #time.time()<startTime+5 #крутить 5 секунд саму программу
        #step-startStep<100 #крутить цикл 100 шагов симуляции
        if doNextStep:
            doNextStep=False
            #действия в цикле, синхронизированном с симулятором

            step=step+1
            client.simxSynchronousTrigger()
        client.simxSpinOnce()
	
    #тут могут быть несинхронизированные с симулятором действия,
    #блокирующие функции:
    #time.sleep(надолго)
	
    #End of simulation:
    cleanup()
    client.simxStopSimulation(client.simxDefaultPublisher())
	
