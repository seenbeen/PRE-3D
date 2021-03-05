#pragma once
#include <queue>
#include <string>
#include <unordered_map>

namespace PRE
{
	namespace AssetModule
	{
		using std::list;
		using std::string;
		using std::unordered_map;

		/**
		 * Fairly simple LRU-style Asset Manager.
		 * 
		 * Note: If capacity is set to 0, will never evict objects.
		 * 
		 * 1. Get -> attempts to get an associated object.
		 *    On Success, increments the object's refCount and returns object.
		 *    On Failure, returns nullptr.
		 * 2. Store -> associates an object with a key.
		 *    If not enough space exists, will evict objects with refCount
		 *    0, LRU-style until there is enough space.
		 *    If __PRE_DEBUG__, throws on not enough capacity.
		 * 3. Release -> decrements the refCount of an associated object.
		 * 
		 * On termination, evicts all stored objects.
		 **/
		class AssetManager
		{
			AssetManager& operator=(const AssetManager&) = delete;
			AssetManager(const AssetManager&) = delete;

		public:
			typedef void (*Unloader)(void*, void*);

		private:
			struct AssetEntry
			{
				AssetEntry& operator=(const AssetEntry&) = delete;
				AssetEntry(const AssetEntry&) = delete;

				const string key;
				const size_t payloadSize;
				void* const payload;

				const Unloader unloader;
				void* const unloaderArg;

				list<AssetEntry*>::iterator itLRU;

				unsigned int refCount;

				AssetEntry(
					const string& key,
					size_t payloadSize,
					void* payload,
					Unloader unloader,
					void* unloaderArg
				);
			};

		public:
			AssetManager(size_t capacity);
			~AssetManager();

			void Store(
				const string& key,
				size_t payloadSize,
				void* payload,
				Unloader unloader,
				void* unloaderArg
			);
			void* Get(const string& key);
			void Release(const string& key);

		private:
			size_t _capacity;
			size_t _size;

			list<AssetEntry*> _evictionLRU;
			unordered_map<string, AssetEntry*> _entriesAccess;
		};
	} // namespace AssetModule
} // namespace PRE