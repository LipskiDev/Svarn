#include <Svarn.h>


class Sandbox : public Svarn::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};

Svarn::Application* Svarn::CreateApplication()
{
	return new Sandbox();
}