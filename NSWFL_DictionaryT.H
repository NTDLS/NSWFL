///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2023, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_DictionaryT_H_
#define _NSWFL_DictionaryT_H_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace NSWFL {
	namespace Collections {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		template <class T>
		class DictionaryT
		{
		public:
			struct VECTORVALUE {
				char *Key;
				T* Value;
			};

			VECTORVALUE *_Items;
			int _Allocated;
			int _Used;

		public:
			DictionaryT()
			{
				this->_Allocated = 10;
				this->_Used = 0;
				this->_Items = (VECTORVALUE *)calloc(this->_Allocated, sizeof(VECTORVALUE));
			}

			~DictionaryT()
			{
				this->Clear();
				this->_Allocated = 0;
				this->_Used = 0;
				free(this->_Items);
			}

			//Returns the count of used allocation (slots).
			int Count()
			{
				return this->_Used;
			}

			//Returns the number of allocations (slots).
			int Allocated()
			{
				return this->_Allocated;
			}

			//Adds a new item to the vector.
			VECTORVALUE *Upsert(char *sKey, T value)
			{
				return UpsertGetIndex(sKey, value, NULL);
			}

			//Adds a new item to the vector.
			VECTORVALUE *UpsertGetIndex(char *key, T value, int *outIndex)
			{
				int iExistingIndex = IndexOf(key);

				if (iExistingIndex >= 0)
				{
					memcpy((void *)this->_Items[iExistingIndex].Value, &value, sizeof(T));
					return &this->_Items[iExistingIndex];
				}
				else
				{
					if (this->_Used == this->_Allocated)
					{
						//Keep a 10 percent overage for allocation efficiency.
						this->_Allocated += (int)(this->_Allocated > 100 ? (this->_Allocated / 10.0f) : 10.0f);
						this->_Items = (VECTORVALUE *)realloc(this->_Items, this->_Allocated * sizeof(VECTORVALUE));
					}

					this->_Items[this->_Used].Value = (T *)calloc(sizeof(T), 1);
					memcpy((void *)this->_Items[this->_Used].Value, &value, sizeof(T));

					this->_Items[this->_Used].Key = _strdup(key);

					if (outIndex)
					{
						*outIndex = this->_Used;
					}

					return &this->_Items[this->_Used++];
				}
			}

			void Compact()
			{
				this->_Allocated = this->_Used;
				this->_Items = (VECTORVALUE *)realloc(this->_Items, this->_Allocated * sizeof(VECTORVALUE));
			}

			//Deletes all values from the vector matching on value.
			//Returns the count of items which were deleted.
			int DeleteAllValues(T value)
			{
				int iCount = 0;
				for (int i = this->_Used - 1; i >= 0 && this->_Used > 0;)
				{
					if (value == *this->_Items[i].Value)
					{
						this->DeleteAtIndex(i);
						iCount++;
					}
					else
					{
						i--;
					}
				}

				return iCount;
			}

			//Deletes a value from the vector.
			//Returns true if an item was found and removed.
			bool DeleteFirstValue(T value)
			{
				for (int i = this->_Used - 1; i >= 0; i--)
				{
					if (value == *this->_Items[i].Value)
					{
						this->DeleteAtIndex(i);
						return true;
					}
				}
				return false;
			}

			//Deletes a value from the vector.
			//Returns true if an item was found and removed.
			bool RemoveByKey(char *key)
			{
				for (int i = this->_Used - 1; i >= 0; i--)
				{
					if (strcmp(key, this->_Items[i].Key) == 0)
					{
						this->DeleteAt(i);
						return true;
					}
				}
				return false;
			}

			//Determines if a given value is in the vector.
			//Returns true if an item was found, otherwise false.
			bool ContiainsValue(T value)
			{
				for (int i = this->_Used - 1; i >= 0; i--)
				{
					if (value == *this->_Items[i].Value)
					{
						return true;
					}
				}
				return false;
			}

			//Determines if a given value is in the vector.
			//Returns true if an item was found, otherwise false.
			bool ContiainsKey(char *key)
			{
				for (int i = this->_Used - 1; i >= 0; i--)
				{
					if (strcmp(key, this->_Items[i].Key) == 0)
					{
						return true;
					}
				}
				return false;
			}

			//Finds the index of a given value.
			//Returns the index of the found value, otherwise returns -1.
			int IndexOfKey(char *key)
			{
				for (int i = this->_Used - 1; i >= 0; i--)
				{
					if (strcmp(key, this->_Items[i].Key) == 0)
					{
						return i;
					}
				}
				return -1;
			}

			//Deletes a value from the vector by it's index.
			//Returns true if an item was deleted. 
			bool DeleteAtIndex(int index)
			{
				if (index >= 0 && index < this->_Used)
				{
					free(this->_Items[index].Key);
					free(this->_Items[index].Value);

					this->_Used--;

					if (this->_Used > 0)
					{
						for (int i = index; i < this->_Used; i++)
						{
							this->_Items[i] = this->_Items[i + 1];
						}
					}
				}
				return false;
			}

			//Deletes all items in the vector.
			void Clear()
			{
				while (this->_Used != 0)
				{
					free((void *)this->_Items[this->_Used - 1].Value);
					free((void *)this->_Items[this->_Used - 1].Key);
					this->_Used--;
				}
			}

			//Returns a pointer to the value by index.
			VECTORVALUE *GetByIndex(int index)
			{
				if (index >= 0 && index < this->_Used)
				{
					return &this->_Items[index];
				}
				return NULL;
			}

			//Returns a pointer to the value by key.
			VECTORVALUE *GetByKey(char *key)
			{
				int iIndex = IndexOf(key);
				if (iIndex >= 0)
				{
					return &this->_Items[iIndex];
				}
				return NULL;
			}
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::Collections
} //namespace::NSWFL
#endif
