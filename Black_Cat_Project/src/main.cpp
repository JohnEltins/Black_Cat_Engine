#include <iostream>

#include <Black_Cat/BLK_Cat.h>


int main(int argc, char** argv)
{
    BLK_Cat::EntityManager mgr;

    BLK_Cat::Entity canvas(mgr.addNewEntity(), &mgr);
    canvas.addComponent<BLK_Cat::Canvas>(800, 600, std::string("Hello World"));

    BLK_Cat::Vertex vertices[] = {
        BLK_Cat::Vertex(glm::vec3(-0.3, -0.3, 0.0), glm::vec2(0.0, 0.0)),
        BLK_Cat::Vertex(glm::vec3(0.0, 0.3, 0.0),  glm::vec2(0.5, 1.0)),
        BLK_Cat::Vertex(glm::vec3(0.3, -0.3, 0.0), glm::vec2(1.0, 0.0))
    };

    BLK_Cat::Vertex vertices2[] = {
        BLK_Cat::Vertex(glm::vec3(0.3, 0.3, 0.0), glm::vec2(0.0, 0.0)),
        BLK_Cat::Vertex(glm::vec3(0.0, -0.3, 0.0),  glm::vec2(0.5, 1.0)),
        BLK_Cat::Vertex(glm::vec3(-0.3, 0.3, 0.0), glm::vec2(1.0, 0.0))
    };

    BLK_Cat::Entity tri(mgr.addNewEntity(), &mgr);
    tri.addComponent<BLK_Cat::Mesh>(vertices, sizeof(vertices) / sizeof(vertices[0]));
    tri.addComponent<BLK_Cat::Shader>("./res/basicShader2");
    tri.addComponent<BLK_Cat::Texture>("./res/bricks.jpg");
    tri.addComponent< BLK_Cat::Transform>();

    BLK_Cat::Entity tri2(mgr.addNewEntity(), &mgr);
    tri2.addComponent<BLK_Cat::Mesh>(vertices2, sizeof(vertices2) / sizeof(vertices2[0]));
    tri2.addComponent<BLK_Cat::Shader>("./res/basicShader");
    tri2.addComponent<BLK_Cat::Transform>();


    mgr.registerSystem<BLK_Cat::Renderer>(&mgr);
    mgr.registerSystem<BLK_Cat::ShaderHandler>(&mgr);
    mgr.registerSystem<BLK_Cat::TextureHandler>(&mgr);
    mgr.registerSystem<BLK_Cat::TransformHandler>(&mgr);
    mgr.registerSystem<BLK_Cat::Draw>(&mgr);

    mgr.init();

    mgr.render(canvas.getID());
    return 0;
}