#include "cf_pch.h"
#include "Application.h"

#include "Coffee/Log.h"

#include <GLFW/glfw3.h>

namespace Coffee {
	Application::Application(){
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
	}
	
	
	Application::~Application(){
	
	}


	void Application::pushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
	}

	void Application::pushOverlay(Layer* overlay) {
		m_LayerStack.PushOverlay(overlay);
	}

	void Application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowClosedEvent>(BIND_EVENT_FN(Application::OnWindowClosed));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run() {
		while (m_Running) { // App Loop
			for (Layer* l : m_LayerStack) {
				l->OnUpdate();
			}

			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}
	
	bool Application::OnWindowClosed(WindowClosedEvent& e) {
		m_Running = false;
		return true;
	}

}
