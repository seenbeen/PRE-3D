#include <core/subsystems/input/preinput.h>

#include <include/modules/input.h>

#include <core/subsystems/input/preinputconfig.h>
#include <core/subsystems/input/prekeycode.h>

namespace PRE
{
	namespace Core
	{
		using PRE::InputModule::Input;

		bool PREInput::ApplicationHasQuit()
		{
			return _input.ApplicationHasQuit();
		}

		void PREInput::MousePosition(int& mouseX, int& mouseY)
		{
			_input.MousePosition(mouseX, mouseY);
		}

		void PREInput::MouseMotion(int& mouseDX, int& mouseDY)
		{
			_input.MouseMotion(mouseDX, mouseDY);
		}

		bool PREInput::MouseButtonLeftState()
		{
			return _input.MouseButtonLeftState();
		}

		bool PREInput::MouseButtonLeftPressed()
		{
			return _input.MouseButtonLeftPressed();
		}

		bool PREInput::MouseButtonLeftReleased()
		{
			return _input.MouseButtonLeftReleased();
		}

		bool PREInput::MouseWheelState()
		{
			return _input.MouseWheelState();
		}

		bool PREInput::MouseWheelPressed()
		{
			return _input.MouseWheelPressed();
		}

		bool PREInput::MouseWheelReleased()
		{
			return _input.MouseWheelReleased();
		}

		bool PREInput::MouseButtonRightState()
		{
			return _input.MouseButtonRightState();
		}

		bool PREInput::MouseButtonRightPressed()
		{
			return _input.MouseButtonRightPressed();
		}

		bool PREInput::MouseButtonRightReleased()
		{
			return _input.MouseButtonRightReleased();
		}

		int PREInput::MouseWheelHScroll()
		{
			return _input.MouseWheelHScroll();
		}

		int PREInput::MouseWheelVScroll()
		{
			return _input.MouseWheelVScroll();
		}

		bool PREInput::KeyState(const PREKeyCode& keyCode)
		{
			return _input.KeyState((unsigned int)keyCode);
		}

		bool PREInput::KeyPressed(const PREKeyCode& keyCode)
		{
			return _input.KeyPressed((unsigned int)keyCode);
		}

		bool PREInput::KeyReleased(const PREKeyCode& keyCode)
		{
			return _input.KeyReleased((unsigned int)keyCode);
		}

		void PREInput::SetMousePosition(unsigned int x, unsigned int y)
		{
			_input.SetMousePosition(x, y);
		}

		void PREInput::LockMouse(bool isLocked)
		{
			_input.LockMouse(isLocked);
		}

		PREInput& PREInput::MakePREInput(
			const PREInputConfig& inputConfig,
			PREApplicationContext& applicationContext
		)
		{
			auto& input = *(new Input());
			return *(new PREInput(applicationContext, input));
		}

		PREInput::PREInput(
			PREApplicationContext& applicationContext,
			Input& input
		)
			:
			_applicationContext(applicationContext),
			_input(input) {}

		PREInput::~PREInput()
		{
			delete &_input;
		}

		void PREInput::Initialize() {}

		void PREInput::Update()
		{
			_input.Update();
		}

		void PREInput::Shutdown() {}
	} // namespace Core
} // namespace PRE