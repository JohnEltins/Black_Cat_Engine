#include <iostream>

#include <Black_Cat/BLK_Cat.h>
//#include "../res/Scenes/TestScene.h"

#define WIDTH 960
#define HEIGHT 540

int main(int argc, char** argv)
{
    Engine* blkCat = Engine::GetInstace(WIDTH, HEIGHT, std::string("Black_Cat_Engine"));
    
    //BLK_Cat::Scene* scene1 = new TestScene(blkCat->getEngineManager()->addNewEntity(), blkCat->getEngineManager(), WIDTH, HEIGHT);

    //blkCat->activateScene(scene1);

    blkCat->init();
   
    return 0;
}