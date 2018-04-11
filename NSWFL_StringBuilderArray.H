///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2010, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_STRINGBUILDERARRAY_H_
#define _NSWFL_STRINGBUILDERARRAY_H_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "NSWFL.H"
#include "NSWFL.H"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace NSWFL {
	namespace String {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		class StringBuilderArray
		{
		public:
			StringBuilder **_Items;
			int _Allocated;
			int _Used;

		public:
			StringBuilderArray()
			{
				this->_Allocated = 10;
				this->_Used = 0;
				this->_Items = (StringBuilder **)calloc(this->_Allocated, sizeof(StringBuilder *));
			}

			~StringBuilderArray()
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
			StringBuilder *Add(char *value)
			{
				return AddGetIndex(value, (int)strlen(value), NULL);
			}

			//Adds a new item to the vector.
			StringBuilder *Add(char *value, int length)
			{
				return AddGetIndex(value, length, NULL);
			}

			//Adds a new item to the vector.
			StringBuilder *AddEmpty(int *outIndex)
			{
				if (this->_Used == this->_Allocated)
				{
					//Keep a 10 percent overage for allocation efficiency.
					this->_Allocated += (int)(this->_Allocated > 100 ? (this->_Allocated / 10.0f) : 10.0f);
					this->_Items = (StringBuilder **)realloc(this->_Items, this->_Allocated * sizeof(StringBuilder *));
				}

				this->_Items[this->_Used] = (StringBuilder *)calloc(1, sizeof(StringBuilder *));

				if (outIndex)
				{
					*outIndex = this->_Used;
				}

				this->_Items[this->_Used] = (StringBuilder *)calloc(1, sizeof(StringBuilder *));

				return this->_Items[this->_Used++];
			}

			//Adds a new item to the vector.
			StringBuilder *AddGetIndex(char *value, int length, int *outIndex)
			{
				if (this->_Used == this->_Allocated)
				{
					//Keep a 10 percent overage for allocation efficiency.
					this->_Allocated += (int)(this->_Allocated > 100 ? (this->_Allocated / 10.0f) : 10.0f);
					this->_Items = (StringBuilder **)realloc(this->_Items, this->_Allocated * sizeof(StringBuilder *));
				}

				this->_Items[this->_Used] = (StringBuilder *)calloc(length + 1, 1);
				if (value)
				{
					this->_Items[this->_Used] = new	StringBuilder(value, length);
				}
				else
				{
					this->_Items[this->_Used] = new	StringBuilder();
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
				this->_Items = (StringBuilder **)realloc(this->_Items, this->_Allocated * sizeof(StringBuilder *));
			}

			//Deletes all values from the vector matching on value.
			//Returns the count of items which were deleted.
			int DeleteAll(char *value)
			{
				int iCount = 0;
				for (int i = this->_Used - 1; i >= 0 && this->_Used > 0;)
				{
					if (strcmp(value, this->_Items[i]->Buffer) == 0)
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
			bool DeleteOne(char *value)
			{
				for (int i = this->_Used - 1; i >= 0; i--)
				{
					if (strcmp(value, this->_Items[i]->Buffer) == 0)
					{
						this->DeleteAt(i);
						return true;
					}
				}
				return false;
			}

			//Determines if a given value is in the vector.
			//Returns true if an item was found, otherwise false.
			bool Contiains(char *value)
			{
				for (int i = this->_Used - 1; i >= 0; i--)
				{
					if (strcmp(value, this->_Items[i]->Buffer) == 0)
					{
						return true;
					}
				}
				return false;
			}

			//Determines if a given value is in the vector.
			//Returns true if an item was found, otherwise false.
			bool ContiainsI(char *value)
			{
				for (int i = this->_Used - 1; i >= 0; i--)
				{
					if (_strcmpi(value, this->_Items[i]->Buffer) == 0)
					{
						return true;
					}
				}
				return false;
			}

			//Determines if a given value is in the vector.
			//Returns true if an item was found, otherwise false.
			bool ContiainsNI(char *value, int iLenght)
			{
				for (int i = this->_Used - 1; i >= 0; i--)
				{
					if (_strnicmp(value, this->_Items[i]->Buffer, iLenght) == 0)
					{
						return true;
					}
				}
				return false;
			}

			//Determines if a given value is in the vector.
			//Returns true if an item was found, otherwise false.
			bool ContiainsN(char *value, int iLenght)
			{
				for (int i = this->_Used - 1; i >= 0; i--)
				{
					if (strncmp(value, this->_Items[i]->Buffer, iLenght) == 0)
					{
						return true;
					}
				}
				return false;
			}

			//Finds the index of a given value.
			//Returns the index of the found value, otherwise returns -1.
			int IndexOf(char *value)
			{
				for (int i = this->_Used - 1; i >= 0; i--)
				{
					if (strcmp(value, this->_Items[i]->Buffer) == 0)
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
					delete this->_Items[index];

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
					delete this->_Items[this->_Used - 1];
					this->_Used--;
				}
			}

			//Returns a pointer to the value by index.
			StringBuilder *at(int index)
			{
				if (index >= 0 && index < this->_Used)
				{
					return this->_Items[index];
				}
				return (StringBuilder *)NULL;
			}

			char **ToArray()
			{
				return ToStringArray(NULL);
			}

			char **ToStringArray(int *iNumberOfElements)
			{
				char **sArray = (char **)calloc(this->_Used, sizeof(char *));

				if (iNumberOfElements != NULL)
				{
					*iNumberOfElements = this->_Used;
				}

				for (int i = 0; i < _Used; i++)
				{
					sArray[i] = (char *)calloc(this->_Items[i]->Length + 1, 1);
					strcpy_s(sArray[i], this->_Items[i]->Length + 1, this->_Items[i]->Buffer);
				}

				return sArray;
			}

			void SortAscending()
			{
				bool bItemsMoved = true;

				while (bItemsMoved)
				{
					bItemsMoved = false;

					for (int i = 0; i < this->_Used - 1; i++)
					{
						if (this->_Items[i]->StrCmp(this->_Items[i + 1]) > 0)
						{
							StringBuilder *temp = this->_Items[i];
							this->_Items[i] = this->_Items[i + 1];
							this->_Items[i + 1] = temp;

							bItemsMoved = true;
						}
					}
				}
			}

			void SortDecending()
			{
				bool bItemsMoved = true;

				while (bItemsMoved)
				{
					bItemsMoved = false;

					for (int i = 0; i < this->_Used - 1; i++)
					{
						if (this->_Items[i]->StrCmp(this->_Items[i + 1]) < 0)
						{
							StringBuilder *temp = this->_Items[i];
							this->_Items[i] = this->_Items[i + 1];
							this->_Items[i + 1] = temp;

							bItemsMoved = true;
						}
					}
				}
			}

		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::String
} //namespace::NSWFL
#endif
