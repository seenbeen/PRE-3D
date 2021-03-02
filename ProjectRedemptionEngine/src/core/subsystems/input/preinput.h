#pragma once
#include <include/modules/input.h>

namespace PRE
{
	namespace Core
	{
		class PREApplicationContext;

		using PRE::InputModule::Input;

		class PREInput
		{
			PREInput& operator=(const PREInput&) = delete;
			PREInput(const PREInput&) = delete;

			friend class PREApplicationContext;
		
		public:
			bool ApplicationHasQuit();

			void MousePosition(int& mouseX, int& mouseY);

			bool MouseButtonLeftState();

			bool MouseButtonLeftPressed();
			bool MouseButtonLeftReleased();

			bool MouseWheelState();

			bool MouseWheelPressed();
			bool MouseWheelReleased();

			bool MouseButtonRightState();

			bool MouseButtonRightPressed();
			bool MouseButtonRightReleased();

			int MouseWheelHScroll();
			int MouseWheelVScroll();

			bool KeyState(unsigned int keyCode);

			bool KeyPressed(unsigned int keyCode);
			bool KeyReleased(unsigned int keyCode);

		private:
			Input& _input;

			PREInput();
			~PREInput();

			void Initialize();
			void Update();
			void Shutdown();
		};
	} // namespace Core
} // namespace PRE