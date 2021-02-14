#include "BloomEffect.h"

void BloomEffect::Init(unsigned width, unsigned height)
{
	int index = int(_buffers.size());
	_buffers.push_back(new Framebuffer());
	_buffers[index]->AddColorTarget(GL_RGBA8);
	_buffers[index]->AddDepthTarget();
	_buffers[index]->Init(width, height);
	index++;
	_buffers.push_back(new Framebuffer());
	_buffers[index]->AddColorTarget(GL_RGBA8);
	_buffers[index]->Init(unsigned(width / m_down), unsigned(height / m_down));
	index++;
	_buffers.push_back(new Framebuffer());
	_buffers[index]->AddColorTarget(GL_RGBA8);
	_buffers[index]->Init(unsigned(width / m_down), unsigned(height / m_down));
	index++;
	_buffers.push_back(new Framebuffer());
	_buffers[index]->AddColorTarget(GL_RGBA8);
	_buffers[index]->Init(width, height);

	//Loads shaders
	index = int(_shaders.size());
	_shaders.push_back(Shader::Create());
	_shaders[index]->LoadShaderPartFromFile("shaders/passthrough_vert.glsl", GL_VERTEX_SHADER);
	_shaders[index]->LoadShaderPartFromFile("shaders/passthrough_frag.glsl", GL_FRAGMENT_SHADER);
	_shaders[index]->Link();
	index++;
	_shaders.push_back(Shader::Create());
	_shaders[index]->LoadShaderPartFromFile("shaders/passthrough_vert.glsl", GL_VERTEX_SHADER);
	_shaders[index]->LoadShaderPartFromFile("shaders/Post/Bloom_frag.glsl", GL_FRAGMENT_SHADER);
	_shaders[index]->Link();
	index++;
	_shaders.push_back(Shader::Create());
	_shaders[index]->LoadShaderPartFromFile("shaders/passthrough_vert.glsl", GL_VERTEX_SHADER);
	_shaders[index]->LoadShaderPartFromFile("shaders/Post/BlurHorizontal_frag.glsl", GL_FRAGMENT_SHADER);
	_shaders[index]->Link();
	index++;
	_shaders.push_back(Shader::Create());
	_shaders[index]->LoadShaderPartFromFile("shaders/passthrough_vert.glsl", GL_VERTEX_SHADER);
	_shaders[index]->LoadShaderPartFromFile("shaders/Post/BlurVertical_frag.glsl", GL_FRAGMENT_SHADER);
	_shaders[index]->Link();
	index++;
	_shaders.push_back(Shader::Create());
	_shaders[index]->LoadShaderPartFromFile("shaders/passthrough_vert.glsl", GL_VERTEX_SHADER);
	_shaders[index]->LoadShaderPartFromFile("shaders/Post/BloomComposite_frag.glsl", GL_FRAGMENT_SHADER);
	_shaders[index]->Link();
	index++;

	Direction = glm::vec2(1.0f / width, 1.0f / height);
}

void BloomEffect::ApplyEffect(PostEffect* buffer)
{
	//Draw previous buffer to first target
	BindShader(0);

	buffer->BindColorAsTexture(0, 0, 0);

	_buffers[0]->RenderToFSQ();

	buffer->UnbindTexture(0);

	UnbindShader();

	//Performs high pass on first target
	BindShader(1);
	_shaders[1]->SetUniform("u_Threshold", m_thresh);

	BindColorAsTexture(0, 0, 0);

	_buffers[1]->RenderToFSQ();

	UnbindTexture(0);

	UnbindShader();

	//Compute the vertical and horizontal blur
	for (unsigned i = 0; i < m_pass; i++)
	{
		//Horizontal
		BindShader(2);
		_shaders[2]->SetUniform("u_Direction", Direction.x);

		BindColorAsTexture(1, 0, 0);

		_buffers[2]->RenderToFSQ();

		UnbindTexture(0);

		UnbindShader();

		//Vertical
		BindShader(3);
		_shaders[3]->SetUniform("u_Direction", Direction.y);

		BindColorAsTexture(2, 0, 0);

		_buffers[1]->RenderToFSQ();

		UnbindTexture(0);

		UnbindShader();
	}

	//Composite
	BindShader(4);

	buffer->BindColorAsTexture(0, 0, 0);
	BindColorAsTexture(1, 0, 1);

	_buffers[0]->RenderToFSQ();

	UnbindTexture(1);
	UnbindTexture(0);

	UnbindShader();
}

float BloomEffect::GetDown() const
{
	return m_down;
}

float BloomEffect::GetThresh() const
{
	return m_thresh;
}

unsigned BloomEffect::GetPass() const
{
	return m_pass;
}

void BloomEffect::SetDown(float down)
{
	m_down = down;
}

void BloomEffect::SetThresh(float thresh)
{
	m_thresh = thresh;
}

void BloomEffect::SetPass(unsigned pass)
{
	m_pass = pass;
}

