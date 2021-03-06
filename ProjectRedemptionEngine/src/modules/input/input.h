#pragma once

namespace PRE
{
	namespace InputModule
	{
		class Input
		{
			Input& operator=(const Input&) = delete;
			Input(const Input&) = delete;

		public:
			Input();
			~Input();

			void Update();

			bool ApplicationHasQuit();

			void MousePosition(int& mouseX, int& mouseY);
			void MouseMotion(int& mouseDX, int& mouseDY);

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

			// TODO: Integrate SDL_Window ptr across Rendering and Input modules
			void SetMousePosition(unsigned int x, unsigned int y);
			void LockMouse(bool isLocked);

		private:
			static const int SDL_NUM_SCANCODES = 512;

			bool _applicationHasQuit;

			int _mouseX;
			int _mouseY;

			int _mouseDX;
			int _mouseDY;

			bool _mousePreviousLeft;
			bool _mouseLeft;

			bool _mousePreviousWheel;
			bool _mouseWheel;

			bool _mousePreviousRight;
			bool _mouseRight;
			
			int _mouseHScroll;
			int _mouseVScroll;

			bool _keyPreviousState[SDL_NUM_SCANCODES];
			bool _keyState[SDL_NUM_SCANCODES];
		};
	} // namespace InputModule
} // namespace PRE