#pragma once
#include <queue>

using std::queue;

namespace PRE
{
	namespace Core
	{
		class PREGameObjectWorld;
		class PRETime;
		class PRERendering;
		// class PREPhysics;

		class Application
		{
		public:
			void Run();

		protected:
			virtual void Initialize(PREGameObjectWorld* gameObjectWorld) = 0;

			PRETime* UseTime();
			PRERendering* UseRendering();
			// PREPhysics* UsePhysics();

			void Shutdown();

		private:
			PREGameObjectWorld* _gameObjectWorld;
			PRETime* _time;
			PRERendering* _rendering;
			// PREPhysics* _physics;
		};
	}
}