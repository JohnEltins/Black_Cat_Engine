#include <iostream>

#include <Black_Cat/BLK_Cat.h>
#include "../res/Scenes/TestScene.h"

#define WIDTH 800
#define HEIGHT 600

int main(int argc, char** argv)
{
    Engine* blkCat = new Engine(WIDTH, HEIGHT, std::string("Black_Cat_Engine"));
    

    BLK_Cat::Scene* scene1 = new TestScene(blkCat->getEngineManager()->addNewEntity(), blkCat->getEngineManager(), WIDTH, HEIGHT);

    //blkCat->activateScene(scene1);

    blkCat->init();
   
    return 0;
}