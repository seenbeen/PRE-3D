#pragma once
#include <SDL.h>

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
			bool _applicationHasQuit;

			int _mouseX;
			int _mouseY;

			bool _mousePreviousLeft;
			bool _mouseLeft;

			bool _mousePreviousWheel;
			bool _mouseWheel;

			bool _mousePreviousRight;
			bool _mouseRight;
			
			int _mouseHScroll;
			int _mouseVScroll;

			Uint8 _keyPreviousState[SDL_NUM_SCANCODES];
			Uint8 _keyState[SDL_NUM_SCANCODES];
		};
	} // namespace InputModule
} // namespace PRE