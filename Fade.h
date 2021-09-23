#pragma once
class Fade
{
public:
	Fade();
	static void FadeIn(int _fadeOutFinishFlag, int _graphHandle);
	static void FadeOut(int _fadeOutFinishFlag, int _graphHandle);
};

