#include <modules/input/input.h>

#include <cstring>

#include <SDL.h>

namespace PRE
{
	namespace InputModule
	{
		Input::Input()
			:
			_applicationHasQuit(false),
			_mouseX(0),
			_mouseY(0),
			_mouseDX(0),
			_mouseDY(0),
			_mousePreviousLeft(false),
			_mouseLeft(false),
			_mousePreviousWheel(false),
			_mouseWheel(false),
			_mousePreviousRight(false),
			_mouseRight(false),
			_mouseHScroll(0),
			_mouseVScroll(0),
			_keyPreviousState(),
			_keyState()
		{
			// no need to initialize events, video initializes this
		}

		Input::~Input() {}

		void Input::Update()
		{
			_applicationHasQuit = false;

			_mousePreviousLeft = _mouseLeft;
			_mousePreviousWheel = _mouseWheel;
			_mousePreviousRight = _mouseRight;

			std::memcpy(_keyPreviousState, _keyState, SDL_NUM_SCANCODES * sizeof(bool));

			SDL_PumpEvents();

			auto mouseButtons = SDL_GetMouseState(&_mouseX, &_mouseY);
			_mouseLeft = mouseButtons & SDL_BUTTON_LMASK;
			_mouseWheel = mouseButtons & SDL_BUTTON_MMASK;
			_mouseRight = mouseButtons & SDL_BUTTON_RMASK;

			auto keyState = SDL_GetKeyboardState(nullptr);
			for (auto i = 0; i < SDL_NUM_SCANCODES; ++i)
			{
				_keyState[i] = keyState[i];
			}
			_mouseHScroll = 0;
			_mouseVScroll = 0;
			_mouseDX = 0;
			_mouseDY = 0;
			SDL_Event evt;
			while (SDL_PollEvent(&evt))
			{
				switch (evt.type)
				{
				case SDL_QUIT:
					_applicationHasQuit = true;
					break;
				case SDL_MOUSEWHEEL:
					_mouseHScroll = evt.wheel.x;
					_mouseVScroll = evt.wheel.y;
					break;
				case SDL_MOUSEMOTION:
					_mouseDX = evt.motion.xrel;
					_mouseDY = evt.motion.yrel;
					break;
				}
			}
		}

		bool Input::ApplicationHasQuit()
		{
			return _applicationHasQuit;
		}

		void Input::MousePosition(int& mouseX, int& mouseY)
		{
			mouseX = _mouseX;
			mouseY = _mouseY;
		}

		void Input::MouseMotion(int& mouseDX, int& mouseDY)
		{
			mouseDX = _mouseDX;
			mouseDY = _mouseDY;
		}

		bool Input::MouseButtonLeftState()
		{
			return _mouseLeft;
		}

		bool Input::MouseButtonLeftPressed()
		{
			return !_mousePreviousLeft && _mouseLeft;
		}

		bool Input::MouseButtonLeftReleased()
		{
			return _mousePreviousLeft && !_mouseLeft;
		}

		bool Input::MouseWheelState()
		{
			return _mouseWheel;
		}

		bool Input::MouseWheelPressed()
		{
			return !_mousePreviousWheel && _mouseWheel;
		}

		bool Input::MouseWheelReleased()
		{
			return _mousePreviousWheel && !_mouseWheel;
		}

		bool Input::MouseButtonRightState()
		{
			return _mouseRight;
		}

		bool Input::MouseButtonRightPressed()
		{
			return !_mousePreviousRight && _mouseRight;
		}

		bool Input::MouseButtonRightReleased()
		{
			return _mousePreviousRight && !_mouseRight;
		}

		int Input::MouseWheelHScroll()
		{
			return _mouseHScroll;
		}

		int Input::MouseWheelVScroll()
		{
			return _mouseVScroll;
		}

		bool Input::KeyState(unsigned int keyCode)
		{
			return _keyState[keyCode];
		}

		bool Input::KeyPressed(unsigned int keyCode)
		{
			return !_keyPreviousState[keyCode] && _keyState[keyCode];
		}

		bool Input::KeyReleased(unsigned int keyCode)
		{
			return _keyPreviousState[keyCode] && !_keyState[keyCode];
		}

		void Input::SetMousePosition(unsigned int x, unsigned int y)
		{
			SDL_WarpMouseInWindow(nullptr, (int)x, (int)y);
		}

		void Input::LockMouse(bool isLocked)
		{
			SDL_SetRelativeMouseMode(isLocked ? SDL_TRUE : SDL_FALSE);
		}
	} // namespace InputModule
} // namespace PRE