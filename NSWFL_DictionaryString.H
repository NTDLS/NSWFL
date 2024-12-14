///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2023, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_DictionaryString_H_
#define _NSWFL_DictionaryString_H_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

namespace NSWFL {
	namespace Collections {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		class DictionaryString
		{
		public:
			struct VECTORVALUE {
				char *Key;
				char *Value;
				int Length;
			};

			VECTORVALUE *_Items;
			int _Allocated;
			int _Used;

		public:
			DictionaryString()
			{
				this->_Allocated = 10;
				this->_Used = 0;
				this->_Items = (VECTORVALUE *)calloc(this->_Allocated, sizeof(VECTORVALUE));
			}

			~DictionaryString()
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
			VECTORVALUE *Upsert(char *key, char *value)
			{
				return UpsertGetIndex(key, value, sizeof(value), NULL);
			}

			//Adds a new item to the vector.
			VECTORVALUE *Upsert(char *key, char *value, int length)
			{
				return UpsertGetIndex(key, value, length, NULL);
			}

			//Adds a new item to the vector.
			VECTORVALUE *UpsertGetIndex(char *key, char *value, int length, int *outIndex)
			{
				int iExistingIndex = IndexOfKey(key);

				if (iExistingIndex >= 0)
				{
					free(this->_Items[iExistingIndex].Value);
					this->_Items[iExistingIndex].Value = _strdup(value);
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

					this->_Items[this->_Used].Key = _strdup(key);
					this->_Items[this->_Used].Value = _strdup(value);

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
			int DeleteAllValues(char *value)
			{
				int iCount = 0;
				for (int i = this->_Used - 1; i >= 0 && this->_Used > 0;)
				{
					if (strcmp(value, this->_Items[i].Value) == 0)
					{
						this->DeleteAt(i);
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
			bool DeleteFirstValue(char *value)
			{
				for (int i = this->_Used - 1; i >= 0; i--)
				{
					if (strcmp(value, this->_Items[i].Value) == 0)
					{
						this->DeleteAt(i);
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
			bool ContiainsValue(char *value)
			{
				for (int i = this->_Used - 1; i >= 0; i--)
				{
					if (strcmp(value, this->_Items[i].Value) == 0)
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
			bool DeleteAt(int index)
			{
				if (index >= 0 && index < this->_Used)
				{
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
					free(this->_Items[this->_Used - 1].Value);
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
				int iIndex = IndexOfKey(key);
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
