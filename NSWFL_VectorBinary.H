///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2023, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_VectorBinary_H_
#define _NSWFL_VectorBinary_H_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace NSWFL {
	namespace Collections {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		class VectorBinary
		{
		public:
			struct VECTORVALUE {
				char* Value;
				int Length;
			};

			VECTORVALUE* _Items;
			int _Allocated;
			int _Used;

		public:
			VectorBinary()
			{
				this->_Allocated = 10;
				this->_Used = 0;
				this->_Items = (VECTORVALUE*)calloc(this->_Allocated, sizeof(VECTORVALUE));
			}

			~VectorBinary()
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
			VECTORVALUE* Add(char* value)
			{
				return AddGetIndex(value, sizeof(value), NULL);
			}

			//Adds a new item to the vector.
			VECTORVALUE* Add(char* value, int length)
			{
				return AddGetIndex(value, length, NULL);
			}

			//Adds a new item to the vector.
			VECTORVALUE* AddEmpty(int* outIndex)
			{
				if (this->_Used == this->_Allocated)
				{
					//Keep a 10 percent overage for allocation efficiency.
					this->_Allocated += (int)(this->_Allocated > 100 ? (this->_Allocated / 10.0f) : 10.0f);
					this->_Items = (VECTORVALUE*)realloc(this->_Items, this->_Allocated * sizeof(VECTORVALUE));
				}

				int length = 0;
				this->_Items[this->_Used].Value = (char*)calloc(length + 1, 1);
				(this->_Items[this->_Used].Value)[length] = '\0';
				this->_Items[this->_Used].Length = length;

				if (outIndex)
				{
					*outIndex = this->_Used;
				}


				return &this->_Items[this->_Used++];
			}

			//Adds a new item to the vector.
			VECTORVALUE* AddGetIndex(char* value, int length, int* outIndex)
			{
				if (this->_Used == this->_Allocated)
				{
					//Keep a 10 percent overage for allocation efficiency.
					this->_Allocated += (int)(this->_Allocated > 100 ? (this->_Allocated / 10.0f) : 10.0f);
					this->_Items = (VECTORVALUE*)realloc(this->_Items, this->_Allocated * sizeof(VECTORVALUE));
				}

				this->_Items[this->_Used].Value = (char*)calloc(length + 1, 1);
				if (value)
				{
					memcpy(this->_Items[this->_Used].Value, value, length);
					(this->_Items[this->_Used].Value)[length] = '\0';
					this->_Items[this->_Used].Length = length;
				}

				if (outIndex)
				{
					*outIndex = this->_Used;
				}

				return &this->_Items[this->_Used++];
			}

			void Compact()
			{
				this->_Allocated = this->_Used;
				this->_Items = (VECTORVALUE*)realloc(this->_Items, this->_Allocated * sizeof(VECTORVALUE));
			}

			//Deletes all values from the vector matching on value.
			//Returns the count of items which were deleted.
			int DeleteAllValues(char* value)
			{
				int iCount = 0;
				for (int i = this->_Used - 1; i >= 0 && this->_Used > 0;)
				{
					if (strcmp(value, this->_Items[i].Value) == 0)
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
			bool DeleteFirstValue(char* value)
			{
				for (int i = this->_Used - 1; i >= 0; i--)
				{
					if (strcmp(value, this->_Items[i].Value) == 0)
					{
						this->DeleteAtIndex(i);
						return true;
					}
				}
				return false;
			}

			//Determines if a given value is in the vector.
			//Returns true if an item was found, otherwise false.
			bool Contiains(char* value)
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

			//Finds the index of a given value.
			//Returns the index of the found value, otherwise returns -1.
			int IndexOfValue(char* value)
			{
				for (int i = this->_Used - 1; i >= 0; i--)
				{
					if (strcmp(value, this->_Items[i].Value) == 0)
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
			VECTORVALUE* GetValueAtIndex(int index)
			{
				if (index >= 0 && index < this->_Used)
				{
					return &this->_Items[index];
				}
				return NULL;
			}
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::Collections
} //namespace::NSWFL
#endif
