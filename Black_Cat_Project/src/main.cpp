#include <iostream>

#include <Black_Cat/BLK_Cat.h>

int main(int argc, char** argv)
{
    BLK_Cat::EntityManager mgr;

    BLK_Cat::Entity canvas(mgr.addNewEntity(), &mgr);
    canvas.addComponent<BLK_Cat::Canvas>(800, 600, std::string("Black_Cat_Engine"));


    BLK_Cat::Entity tri(mgr.addNewEntity(), &mgr);

    tri.addComponent<BLK_Cat::Triangle>();
    tri.addComponent<BLK_Cat::Drawable>();
    tri.addComponent<BLK_Cat::Shader>("./res/basicShader2");
    tri.addComponent<BLK_Cat::Texture>("./res/bricks.jpg");
    tri.addComponent< BLK_Cat::Transform>(glm::vec3(), glm::vec3(), glm::vec3(0.2,0.2,1.0));

    mgr.registerSystem<BLK_Cat::Renderer2D>(&mgr);
    mgr.registerSystem<BLK_Cat::DrawTriangle>(&mgr);
    mgr.registerSystem<BLK_Cat::ShaderHandler>(&mgr);
    mgr.registerSystem<BLK_Cat::TextureHandler>(&mgr);
    mgr.registerSystem<BLK_Cat::TransformHandler>(&mgr);
    
    mgr.init();

    mgr.render(canvas.getID());
    return 0;
}