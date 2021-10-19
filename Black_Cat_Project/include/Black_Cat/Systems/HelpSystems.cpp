#include "Black_Cat/Systems/HelpSystems.h"
#include "RenderSystem.h"
#include "Black_Cat/Systems/TransformSystem.h"

void BLK_Cat::HelpSystemsUpdate(entt::registry& registry, float dt, Display& display)
{
	if (display.GetMouseDown(Display::right))
	{
		auto view = registry.view<Quad, Duplicate>();
		for (auto entity : view)
		{
			DuplicateEntity(registry, entity, display);
		}
		
	}
	
}

void BLK_Cat::DuplicateEntity(entt::registry& registry, entt::entity entity, Display& display)
{
	entt::entity quad = registry.create();

	registry.emplace<BLK_Cat::Quad>(quad);
	registry.emplace<BLK_Cat::Shader>(quad, registry.get<Shader>(entity)._filename);
	registry.emplace<BLK_Cat::Transform>(quad, glm::vec3(display.GetMousePos().x, (float_t)display.GetScreenHeight() - display.GetMousePos().y, 0), glm::vec3(),
					glm::vec3(registry.get<Transform>(entity)._scale.x, registry.get<Transform>(entity)._scale.y, 1.0));

	registry.emplace<BLK_Cat::CameraOrtho>(quad, (float_t)0, (float_t)display.GetScreenWidth(), (float_t)0, (float_t)display.GetScreenHeight(), -1.0f, 1.0f, glm::vec3());
	registry.emplace<BLK_Cat::Texture>(quad, registry.get<Texture>(entity)._filename);

	Quad& quad2 = registry.get<Quad>(quad);
	Shader& shader = registry.get<Shader>(quad);
	Texture& texture = registry.get<Texture>(quad);
	CreateQuad(quad2);
	InitShaderHandler(shader);
	InitTextureHandler(texture);

	Transform& tranform = registry.get<Transform>(quad);
	tranform._model = GetModel(tranform);
	tranform.count = 0.0f;

	CameraOrtho& camera = registry.get<CameraOrtho>(quad);
	camera._viewOrtho = GetViewOrtho(camera);
}
