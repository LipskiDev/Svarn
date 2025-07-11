#include <Svarn.h>
#include <stdio.h>

class Sandbox : public Svarn::Application
{
public:
	Sandbox()
	{
		printf("Hello from Sandboxxxx");
	}

	~Sandbox()
	{

	}
};

Svarn::Application* Svarn::CreateApplication()
{
	return new Sandbox();
}