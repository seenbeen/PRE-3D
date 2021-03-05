#pragma once

namespace PRE
{
	namespace Core
	{
		struct PREAssetManagerConfig;

		class PREApplicationContext;

		class PREAssetManager
		{
			PREAssetManager& operator=(const PREAssetManager&) = delete;
			PREAssetManager(const PREAssetManager&) = delete;

			friend class PREApplicationContext;

		public:


		private:
			static PREAssetManager& MakePREAssetManager(
				const PREAssetManagerConfig& assetManagerConfig,
				PREApplicationContext& applicationContext
			);

			PREAssetManager(
				PREApplicationContext& applicationContext,
				Renderer& renderer
			);
			~PREAssetManager();

			void Initialize();
			void Update();
			void Shutdown();
		};
	} // namespace Core
} // namespace PRE