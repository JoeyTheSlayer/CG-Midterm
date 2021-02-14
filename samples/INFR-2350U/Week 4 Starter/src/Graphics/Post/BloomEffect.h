#pragma once

#include "Graphics/Post/PostEffect.h"

class BloomEffect : public PostEffect
{
public:
	//Initialize
	//Override Init
	void Init(unsigned width, unsigned height) override;

	//Applies effects
	//passes previous framebuffer
	void ApplyEffect(PostEffect* buffer) override;

	//Getter
	float GetDown() const;
	float GetThresh() const;
	unsigned GetPass() const;

	//Setter
	void SetDown(float down);
	void SetThresh(float thresh);
	void SetPass(unsigned pass);

private:
	float m_down = 2.0f;
	float m_thresh = 0.01f;
	unsigned m_pass = 10;
	glm::vec2 Direction;
};
