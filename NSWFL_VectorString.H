///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2023, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_VectorString_H_
#define _NSWFL_VectorString_H_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace NSWFL {
	namespace Collections {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		class VectorString
		{
		public:
			char** _Items;
			int _Allocated;
			int _Used;

		public:
			VectorString()
			{
				this->_Allocated = 10;
				this->_Used = 0;
				this->_Items = (char**)calloc(this->_Allocated, sizeof(char*));
			}

			~VectorString()
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
			char* Add(char* value)
			{
				return AddGetIndex(value, (int)strlen(value), NULL);
			}

			//Adds a new item to the vector.
			char* Add(char* value, int length)
			{
				return AddGetIndex(value, length, NULL);
			}

			//Adds a new item to the vector.
			char* AddEmpty(int* outIndex)
			{
				if (this->_Used == this->_Allocated)
				{
					//Keep a 10 percent overage for allocation efficiency.
					this->_Allocated += (int)(this->_Allocated > 100 ? (this->_Allocated / 10.0f) : 10.0f);
					this->_Items = (char**)realloc(this->_Items, this->_Allocated * sizeof(char*));
				}

				this->_Items[this->_Used] = (char*)calloc(1, sizeof(char*));

				if (outIndex)
				{
					*outIndex = this->_Used;
				}

				this->_Items[this->_Used] = (char*)calloc(1, sizeof(char*));

				return this->_Items[this->_Used++];
			}

			//Adds a new item to the vector.
			char* AddGetIndex(char* value, int length, int* outIndex)
			{
				if (this->_Used == this->_Allocated)
				{
					//Keep a 10 percent overage for allocation efficiency.
					this->_Allocated += (int)(this->_Allocated > 100 ? (this->_Allocated / 10.0f) : 10.0f);
					this->_Items = (char**)realloc(this->_Items, this->_Allocated * sizeof(char*));
				}

				this->_Items[this->_Used] = (char*)calloc(length + 1, 1);
				if (value)
				{
					memcpy(this->_Items[this->_Used], value, length);
					(this->_Items[this->_Used])[length] = '\0';
				}

				if (outIndex)
				{
					*outIndex = this->_Used;
				}

				return this->_Items[this->_Used++];
			}

			void Compact()
			{
				this->_Allocated = this->_Used;
				this->_Items = (char**)realloc(this->_Items, this->_Allocated * sizeof(char*));
			}

			//Deletes all values from the vector matching on value.
			//Returns the count of items which were deleted.
			int DeleteAllValues(char* value)
			{
				int iCount = 0;
				for (int i = this->_Used - 1; i >= 0 && this->_Used > 0;)
				{
					if (strcmp(value, this->_Items[i]) == 0)
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
					if (strcmp(value, this->_Items[i]) == 0)
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
					if (strcmp(value, this->_Items[i]) == 0)
					{
						return true;
					}
				}
				return false;
			}

			//Determines if a given value is in the vector.
			//Returns true if an item was found, otherwise false.
			bool ContiainsI(char* value)
			{
				for (int i = this->_Used - 1; i >= 0; i--)
				{
					if (_strcmpi(value, this->_Items[i]) == 0)
					{
						return true;
					}
				}
				return false;
			}

			//Determines if a given value is in the vector.
			//Returns true if an item was found, otherwise false.
			bool ContiainsNI(char* value, int iLenght)
			{
				for (int i = this->_Used - 1; i >= 0; i--)
				{
					if (_strnicmp(value, this->_Items[i], iLenght) == 0)
					{
						return true;
					}
				}
				return false;
			}

			//Determines if a given value is in the vector.
			//Returns true if an item was found, otherwise false.
			bool ContiainsN(char* value, int iLenght)
			{
				for (int i = this->_Used - 1; i >= 0; i--)
				{
					if (strncmp(value, this->_Items[i], iLenght) == 0)
					{
						return true;
					}
				}
				return false;
			}

			//Finds the index of a given value.
			//Returns the index of the found value, otherwise returns -1.
			int IndexOfFirstValue(char* value)
			{
				for (int i = this->_Used - 1; i >= 0; i--)
				{
					if (strcmp(value, this->_Items[i]) == 0)
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
					free(this->_Items[index]);

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
					free(this->_Items[this->_Used - 1]);
					this->_Used--;
				}
			}

			//Returns a pointer to the value by index.
			char* GetByIndex(int index)
			{
				if (index >= 0 && index < this->_Used)
				{
					return this->_Items[index];
				}
				return (char*)NULL;
			}

			char** ToArray()
			{
				return ToArray(NULL);
			}

			char** ToArray(int* iNumberOfElements)
			{
				char** sArray = (char**)calloc(this->_Used, sizeof(char*));

				if (iNumberOfElements != NULL)
				{
					*iNumberOfElements = this->_Used;
				}

				for (int i = 0; i < _Used; i++)
				{
					int iLength = (int)strlen(this->_Items[i]);
					sArray[i] = (char*)calloc(iLength + 1, 1);
					strcpy_s(sArray[i], iLength + 1, this->_Items[i]);
				}

				return sArray;
			}
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::Collections
} //namespace::NSWFL
#endif
