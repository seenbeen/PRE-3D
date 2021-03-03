#pragma once
#include <include/modules/input.h>

namespace PRE
{
	namespace Core
	{
		struct PREInputConfig;

		class PREApplicationContext;

		enum class PREKeyCode;

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

			bool KeyState(const PREKeyCode& keyCode);

			bool KeyPressed(const PREKeyCode& keyCode);
			bool KeyReleased(const PREKeyCode& keyCode);

		private:
			static PREInput& MakePREInput(
				const PREInputConfig& inputConfig,
				PREApplicationContext& applicationContext
			);

			PREApplicationContext& _applicationContext;
			Input& _input;

			PREInput(PREApplicationContext& applicationContext, Input& input);
			~PREInput();

			void Initialize();
			void Update();
			void Shutdown();
		};
	} // namespace Core
} // namespace PRE