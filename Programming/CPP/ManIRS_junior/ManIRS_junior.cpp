// ManIRS_junior.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <string>
#include "b0RemoteApi.h"
#include "ManRobot.h"


bool doNextStep = true;
bool runInSynchronousMode = true;
int step = 0;
b0RemoteApi client("b0RemoteApi_c++Client", "b0RemoteApi_manirs");
ManRobot robot(&client);

void init()
{
    //пример использования функций управления роботом:
    //подробнее о всех функицях, их парметрах и возвращаемых значения см ReadMe

    //энкодеры:
    //float encX=robot.X_enc;
    //float encY=robot.Y_enc;
    //float encZ=robot.Z_enc;

    //float simTime = robot.simTime;

    //float posX = 95;  //миллиметров в диапазоне 0..190
    //float posY = 85;  //миллиметров в диапазоне 0..170
    //float posZ = 95; //миллиметров в диапазоне 0..190
    //robot.setPosX(posX);
    //robot.setPosY(posY);
    //robot.setPosZ(posZ);
    //robot.setPositions(posX, posY, posZ);

    //float kp = 0.1;
    //float ki = 0.01;
    //float kd = 0.1;
    //robot.setPID(robot.linkX, kp, ki, kd);
    //robot.resetPID(robot.linkX);

    //float speed = 100; //max speed in mm/s or deg/s
    //robot.setMaxSpeed(robot.linkX, speed);

    //robot.grapObject();    //включить схват
    //robot.releaseObject(); //выключить схват

    //int resX = 2; //X resolution of RoboCamera in pixels 1...1024
    //int resY = 2; //Y resolution of RoboCamera in pixels 1...1024
    //robot.setCameraResolution(resX, resY);

    //std::string img = robot.cam_image; //кадр с камеры возвращается как строка длинной 196608 символов
    //Каждые три символа надо переводить в byte/int и обрабатывать как RGB-компоненты одного пикселя
	
	//См. также:
	//ReadMe для более подробной информации о роботе и симуляторе
	//https://coppeliarobotics.com/helpFiles/en/b0RemoteApi-cpp.htm - список всех C++ B0 remote API функций
}

void simulationStepStarted(std::vector<msgpack::object>* msg)
{

}

void simulationStepDone(std::vector<msgpack::object>* msg)
{
    //выводим текущий шаг, просто для демонстрации:
    std::cout << step << std::endl;
    doNextStep = true;
}

void cleanup()
{

}

void stepSimulation()
{
    if (runInSynchronousMode)
    {
        while (!doNextStep)
            client.simxSpinOnce();
        doNextStep = false;
        //действия в цикле, синхронизированном с симулятором

        step = step + 1;
        client.simxSynchronousTrigger();
    }
    else
        client.simxSpinOnce();
}

int main()
{
    if (runInSynchronousMode)
        client.simxSynchronous(true);

    client.simxGetSimulationStepStarted(client.simxDefaultSubscriber(simulationStepStarted));
    client.simxGetSimulationStepDone(client.simxDefaultSubscriber(simulationStepDone));
    client.simxStartSimulation(client.simxDefaultPublisher());
    init();
    //Put your main action here:

    int startStep = step;
    while (robot.simTime < 5)
        stepSimulation();
	
	//тут могут быть несинхронизированные с симулятором действия,
    //и блокирующие функции
	
    //End of simulation:
    cleanup();
    client.simxStopSimulation(client.simxDefaultPublisher());
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
