#include "Black_Cat/Systems/RenderSystem.h"
#include "Black_Cat/Systems/TransformSystem.h"

void BLK_Cat::RendererInit(entt::registry& registry)
{
	auto viewTriangle = registry.view <Triangle>();
	auto viewQuad = registry.view <Quad>();
	auto viewMesh = registry.view <Mesh>();

	for (auto entity : viewTriangle)
	{
		Triangle& triangle = viewTriangle.get<Triangle>(entity);
		Shader& shader = registry.get<Shader>(entity);
		Texture& texture = registry.get<Texture>(entity);
		CreateTriangle(triangle);
		InitShaderHandler(shader);
		InitTextureHandler(texture);
	}

	for (auto entity : viewQuad)
	{
		Quad& quad = viewQuad.get<Quad>(entity);
		Shader& shader = registry.get<Shader>(entity);
		Texture& texture = registry.get<Texture>(entity);
		CreateQuad(quad);
		InitShaderHandler(shader);
		InitTextureHandler(texture);
	}

	for (auto entity : viewMesh)
	{
		Mesh& mesh = viewMesh.get<Mesh>(entity);
		Shader& shader = registry.get<Shader>(entity);
		Texture& texture = registry.get<Texture>(entity);
		CreateMesh(mesh);
		InitShaderHandler(shader);
		InitTextureHandler(texture);
	}
}

/////////////////////////////////////////////////////////////////////
//////////////////////		Draw Primitives		/////////////////////
/////////////////////////////////////////////////////////////////////

void BLK_Cat::RendererDraw(entt::registry& registry)
{
	auto viewTriangle = registry.view<Triangle, Shader, Texture, Transform, Camera>();

	auto viewQuad = registry.view<Quad, Shader, Texture, Transform, Camera>();
	auto viewQuadOrtho = registry.view<Quad, Shader, Texture, Transform, CameraOrtho>();

	auto viewMesh = registry.view<Mesh, Shader, Texture, Transform, Camera>();

	for (auto entity : viewTriangle)
	{
		Triangle& triangle = viewTriangle.get<Triangle>(entity);
		Shader& shader = viewTriangle.get<Shader>(entity);
		Texture& texture = viewTriangle.get<Texture>(entity);
		Transform& transform = viewTriangle.get<Transform>(entity);
		Camera& camera = viewTriangle.get<Camera>(entity);

		DrawTriangle(triangle, shader, texture, transform, camera);
	}

	for (auto entity : viewQuad)
	{
		Quad& quad = viewQuad.get<Quad>(entity);
		Shader& shader = viewQuad.get<Shader>(entity);
		Texture& texture = viewQuad.get<Texture>(entity);
		Transform& transform = viewQuad.get<Transform>(entity);
		Camera& camera = viewQuad.get<Camera>(entity);

		DrawQuad(quad, shader, texture, transform, camera);
	}

	for (auto entity : viewQuadOrtho)
	{
		Quad& quad = viewQuadOrtho.get<Quad>(entity);
		Shader& shader = viewQuadOrtho.get<Shader>(entity);
		Texture& texture = viewQuadOrtho.get<Texture>(entity);
		Transform& transform = viewQuadOrtho.get<Transform>(entity);
		CameraOrtho& camera = viewQuadOrtho.get<CameraOrtho>(entity);

		DrawQuadOrtho(quad, shader, texture, transform, camera);
	}


	for (auto entity : viewMesh)
	{
		Mesh& mesh = viewMesh.get<Mesh>(entity);
		Shader& shader = viewMesh.get<Shader>(entity);
		Texture& texture = viewMesh.get<Texture>(entity);
		Transform& transform = viewMesh.get<Transform>(entity);
		Camera& camera = viewMesh.get<Camera>(entity);

		DrawMesh(mesh, shader, texture, transform, camera);
	}
}

void BLK_Cat::CreateTriangle(Triangle& triangle)
{
	std::cout << "init triangle" << std::endl;
	Vertex vertices[] = {
				Vertex(glm::vec3(-0.5f, -0.5f, 0.0), glm::vec2(0.0, 0.0)),
				Vertex(glm::vec3(0.0,  0.5f, 0.0),  glm::vec2(0.5, 1.0)),
				Vertex(glm::vec3(0.5f, -0.5f, 0.0),  glm::vec2(1.0, 0.0))
	};

	const uint32_t numVertices = sizeof(vertices) / sizeof(vertices[0]);
	GLuint VBO[numVertices];

	triangle._numVertices = numVertices;

	triangle._vertices = vertices;

	glGenVertexArrays(1, &triangle._VAO);
	glBindVertexArray(triangle._VAO);

	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> texture;

	positions.reserve(numVertices);
	texture.reserve(numVertices);


	for (uint32_t i = 0; i < numVertices; i++)
	{
		positions.push_back(triangle._vertices[i]._pos);
		texture.push_back(triangle._vertices[i]._textCoord);
	}

	glGenBuffers(triangle.NUM_BUFFERS, VBO);

	//buffer de vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBO[triangle.POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(positions[0]), &positions[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//buffer de textCoord
	glBindBuffer(GL_ARRAY_BUFFER, VBO[triangle.TEXTCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(texture[0]), &texture[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
}

void BLK_Cat::CreateQuad(Quad& quad)
{
#if 0
	Vertex vertices[] = {
		Vertex(glm::vec3(0.5f,  0.5f, 0.0), glm::vec2(-1.0, -1.0)),
		Vertex(glm::vec3(0.5f, -0.5f, 0.0), glm::vec2(1.0, -1.0)),
		Vertex(glm::vec3(-0.5f,-0.5f, 0.0), glm::vec2(1.0, 1.0)),
		Vertex(glm::vec3(-0.5f, 0.5f, 0.0), glm::vec2(-1.0, 1.0))
	};
#endif

	Vertex vertices[] = {
		Vertex(glm::vec3(1.0f,  1.0f, 0.0), glm::vec2(-1.0, -1.0)),
		Vertex(glm::vec3(1.0f, -1.0f, 0.0), glm::vec2(1.0, -1.0)),
		Vertex(glm::vec3(-1.0f,-1.0f, 0.0), glm::vec2(1.0, 1.0)),
		Vertex(glm::vec3(-1.0f, 1.0f, 0.0), glm::vec2(-1.0, 1.0))
	};

	uint32_t indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	const uint32_t numVertices = sizeof(vertices) / sizeof(vertices[0]);
	GLuint VBO[numVertices];

	quad._numVertices = numVertices;

	quad._vertices = vertices;

	glGenVertexArrays(1, &quad._VAO);
	glBindVertexArray(quad._VAO);

	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> texture;

	positions.reserve(numVertices);
	texture.reserve(numVertices);

	for (uint32_t i = 0; i < numVertices; i++)
	{
		positions.push_back(quad._vertices[i]._pos);
		texture.push_back(quad._vertices[i]._textCoord);
	}

	glGenBuffers(quad.NUM_BUFFERS, VBO);

	//buffer de vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBO[quad.POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(positions[0]), &positions[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//buffer de textCoord
	glBindBuffer(GL_ARRAY_BUFFER, VBO[quad.TEXTCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(texture[0]), &texture[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	uint32_t EBO;
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void BLK_Cat::CreateMesh(Mesh& mesh)
{
	glGenVertexArrays(1, &mesh._vertexArrayObject);
	glBindVertexArray(mesh._vertexArrayObject);

	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> texture;

	positions.reserve(mesh._numVertices);
	texture.reserve(mesh._numVertices);

	for (uint32_t i = 0; i < mesh._numVertices; i++)
	{
		positions.push_back(mesh._vertices[i]._pos);
		texture.push_back(mesh._vertices[i]._textCoord);
	}

	glGenBuffers(mesh.NUM_BUFFERS, mesh._vertexArrayBuffers);

	//buffer de vertices
	glBindBuffer(GL_ARRAY_BUFFER, mesh._vertexArrayBuffers[mesh.POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, mesh._numVertices * sizeof(positions[0]), &positions[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//buffer de textCoord
	glBindBuffer(GL_ARRAY_BUFFER, mesh._vertexArrayBuffers[mesh.TEXTCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, mesh._numVertices * sizeof(texture[0]), &texture[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh._vertexArrayBuffers[mesh.INDEX_VB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh._numIndices * sizeof(mesh._indices[0]), &mesh._indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void BLK_Cat::DrawTriangle(Triangle& triangle, Shader& shader, Texture& texture, Transform& transform, Camera& camera)
{
	glUseProgram(0);
	glUseProgram(shader._program);

	glm::mat4 model = camera._viewProjection * transform._model;

	glUniformMatrix4fv(shader._uniforms[shader.UNIFORMS::TRANSFORM_U], 1, GL_FALSE, &model[0][0]);

	glActiveTexture(GL_TEXTURE0); //setar textura de uma das 32 unidades
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_2D, texture._texture);

	glBindVertexArray(triangle._VAO);
	glDrawArrays(GL_TRIANGLES, 0, triangle._numVertices);
	glBindVertexArray(0);
}

void BLK_Cat::DrawQuad(Quad& quad, Shader& shader, Texture& texture, Transform& transform, Camera& camera)
{
	glUseProgram(0);
	glUseProgram(shader._program);

	glm::mat4 model = camera._viewProjection * transform._model;

	glUniformMatrix4fv(shader._uniforms[shader.UNIFORMS::TRANSFORM_U], 1, GL_FALSE, &model[0][0]);

	glActiveTexture(GL_TEXTURE0); //setar textura de uma das 32 unidades
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_2D, texture._texture);

	glBindVertexArray(quad._VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void BLK_Cat::DrawQuadOrtho(Quad& quad, Shader& shader, Texture& texture, Transform& transform, CameraOrtho& camera)
{
	glUseProgram(0);
	glUseProgram(shader._program);

	glm::mat4 model = camera._viewOrtho * transform._model;

	glUniformMatrix4fv(shader._uniforms[shader.UNIFORMS::TRANSFORM_U], 1, GL_FALSE, &model[0][0]);

	glActiveTexture(GL_TEXTURE0); //setar textura de uma das 32 unidades
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_2D, texture._texture);

	glBindVertexArray(quad._VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void BLK_Cat::DrawMesh(Mesh& mesh, Shader& shader, Texture& texture, Transform& transform, Camera& camera)
{
	glUseProgram(0);
	glUseProgram(shader._program);

	glm::mat4 model = camera._viewProjection * transform._model;

	glUniformMatrix4fv(shader._uniforms[shader.UNIFORMS::TRANSFORM_U], 1, GL_FALSE, &model[0][0]);


	glActiveTexture(GL_TEXTURE0); //setar textura de uma das 32 unidades
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_2D, texture._texture);


	glBindVertexArray(mesh._vertexArrayObject);
	glDrawElements(GL_TRIANGLES, mesh._numIndices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

/////////////////////////////////////////////////////////////////////
//////////////////////		Handlers	 ////////////////////////////
/////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////
			//////////////////////		Shader	 ////////////////////////////	
			/////////////////////////////////////////////////////////////////

void BLK_Cat::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}

std::string BLK_Cat::LoadShader(const std::string& fileName)
{
	std::ifstream file;
	file.open((fileName).c_str());

	std::string output;
	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else
	{
		std::cerr << "Unable to Load shader: " << fileName << std::endl;
	}

	//std::cout << output << std::endl;
	return output;
}

GLuint BLK_Cat::CreateShader(const std::string txt, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	if (shader == 0)
		std::cerr << "Error: shader creation failed!" << std::endl;

	const GLchar* shaderSourceStrings[1];
	GLint shaderSouceStringLenghts[1];

	shaderSourceStrings[0] = txt.c_str();
	shaderSouceStringLenghts[0] = txt.length();

	glShaderSource(shader, 1, shaderSourceStrings, shaderSouceStringLenghts);
	glCompileShader(shader);

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error shader failed to compile: ");

	return shader;
}

void BLK_Cat::InitShaderHandler(Shader& shader)
{
	shader._program = glCreateProgram();
	shader._shaders[shader.SHADERS::VertexShader] = CreateShader(LoadShader(shader._filename + ".vs"), GL_VERTEX_SHADER);
	shader._shaders[shader.SHADERS::FragmentShader] = CreateShader(LoadShader(shader._filename + ".fs"), GL_FRAGMENT_SHADER);

	for (uint32_t i = 0; i < shader.NUM_SHADERS; i++)
	{
		glAttachShader(shader._program, shader._shaders[i]);
	}

	glBindAttribLocation(shader._program, 0, "position");
	glBindAttribLocation(shader._program, 1, "texCoord");

	glLinkProgram(shader._program);
	CheckShaderError(shader._program, GL_LINK_STATUS, true, "Error shader program failed to link: ");

	glValidateProgram(shader._program);
	CheckShaderError(shader._program, GL_VALIDATE_STATUS, true, "Error shader program is invalid: ");

	shader._uniforms[shader.TRANSFORM_U] = glGetUniformLocation(shader._program, "transform");
}

void BLK_Cat::InitTextureHandler(Texture& texture)
{
	int width, height, numComponents;
	unsigned char* imageData = stbi_load(texture._filename.c_str(), &width, &height, &numComponents, 4); //carregar png 

	if (imageData == NULL)
		std::cerr << "Texture loading failed: " << texture._filename << std::endl;

	glGenTextures(1, &texture._texture);
	glBindTexture(GL_TEXTURE_2D, texture._texture);

	//repetir caso a textura chegue aou final
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//interpolação caso a imagem seja aumentada ou diminuida
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//envia imagem para o buffer de textura na gpu
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

	stbi_image_free(imageData);
}