#include <modules/asset/assetmanager.h>

#include <queue>
#include <string>
#include <unordered_map>

#ifdef __PRE_DEBUG__
#include <assert.h>
#endif

namespace PRE
{
	namespace AssetModule
	{
		using std::queue;
		using std::string;
		using std::unordered_map;

		AssetManager::AssetEntry::AssetEntry(
			const string& key,
			size_t payloadSize,
			void* payload,
			Unloader unloader,
			void* unloaderArg
		)
			:
			key(key),
			payloadSize(payloadSize),
			payload(payload),
			unloader(unloader),
			unloaderArg(unloaderArg),
			itLRU(),
			refCount(0) {}

		AssetManager::AssetManager(size_t capacity)
			:
			_capacity(capacity),
			_size(0) {}

		AssetManager::~AssetManager()
		{
			for (auto it = _entriesAccess.begin(); it != _entriesAccess.end(); ++it)
			{
				auto pEntry = it->second;

#ifdef __PRE_DEBUG__
				assert(pEntry->refCount == 0);
#endif

				pEntry->unloader(pEntry->unloaderArg, pEntry->payload);
				delete pEntry;
			}
		}

		void AssetManager::Store(
			const string& key,
			size_t payloadSize,
			void* payload,
			AssetManager::Unloader unloader,
			void* unloaderArg
		)
		{
			auto itEntry = _entriesAccess.find(key);

#ifdef __PRE_DEBUG__
			assert(itEntry == _entriesAccess.end());
			assert(payloadSize != 0);
			assert(payload != nullptr);
#endif

			if (_capacity != 0)
			{
				while (_capacity - _size < payloadSize)
				{

#ifdef __PRE_DEBUG__
					assert(_evictionLRU.size() != 0);
#endif

					auto pEntry = _evictionLRU.front();
					_evictionLRU.pop_front();
					_entriesAccess.erase(pEntry->key);
					pEntry->unloader(pEntry->unloaderArg, pEntry->payload);
					_size -= pEntry->payloadSize;
					delete pEntry;
				}
			}
			_size += payloadSize;

			auto pNewEntry = new AssetEntry(
				key,
				payloadSize,
				payload,
				unloader,
				unloaderArg
			);

			_entriesAccess[key] = pNewEntry;
			_evictionLRU.push_front(pNewEntry);
			pNewEntry->itLRU = _evictionLRU.begin();
		}

		void* AssetManager::Get(const string& key)
		{
			auto itEntry = _entriesAccess.find(key);
			if (itEntry == _entriesAccess.end())
			{
				return nullptr;
			}
			
			auto pEntry = itEntry->second;
			if (pEntry->refCount == 0)
			{
				_evictionLRU.erase(pEntry->itLRU);
			}
			++pEntry->refCount;

			return pEntry->payload;
		}

		void AssetManager::Release(const string& key)
		{
			auto itEntry = _entriesAccess.find(key);

#ifdef __PRE_DEBUG__
			assert(itEntry != _entriesAccess.end());
#endif

			auto pEntry = itEntry->second;
			--pEntry->refCount;
			if (pEntry->refCount == 0)
			{
				_evictionLRU.push_back(pEntry);
				pEntry->itLRU = --_evictionLRU.end();
			}
		}
	} // namespace AssetModule
} // namespace PRE