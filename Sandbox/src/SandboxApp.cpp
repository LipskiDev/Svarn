#include <Svarn.h>
#include <stdio.h>

class Sandbox : public Svarn::Application
{
public:
	Sandbox()
	{
		SV_CORE_INFO("Hello from Sandbox!");
	}

	~Sandbox()
	{

	}
};

Svarn::Application* Svarn::CreateApplication()
{
	return new Sandbox();
}