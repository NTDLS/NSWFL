///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Copyright © NetworkDLS 2010, All rights reserved
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NSWFL_VectorT_H_
#define _NSWFL_VectorT_H_
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace NSWFL {
	namespace Collections {
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		template <class T>
		class VectorT
		{
		public:
			T * *_Items;
			int _Allocated;
			int _Used;

		public:
			VectorT()
			{
				this->_Allocated = 10;
				this->_Used = 0;
				this->_Items = (T **)calloc(this->_Allocated, sizeof(T));
			}

			~VectorT()
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
			T *Add(T value)
			{
				return AddGetIndex(value, sizeof(value), NULL);
			}

			//Adds a new item to the vector.
			T *Add(T value, int length)
			{
				return AddGetIndex(value, length, NULL);
			}

			//Adds a new item to the vector.
			T *AddEmpty(int *outIndex)
			{
				if (this->_Used == this->_Allocated)
				{
					//Keep a 10 percent overage for allocation efficiency.
					this->_Allocated += (int)(this->_Allocated > 100 ? (this->_Allocated / 10.0f) : 10.0f);
					this->_Items = (T **)realloc(this->_Items, this->_Allocated * sizeof(T));
				}

				if (outIndex)
				{
					*outIndex = this->_Used;
				}

				this->_Items[this->_Used] = (T *)calloc(1, sizeof(T));

				return this->_Items[this->_Used++];
			}

			//Adds a new item to the vector.
			T *AddGetIndex(T value, int length, int *outIndex)
			{
				if (this->_Used == this->_Allocated)
				{
					//Keep a 10 percent overage for allocation efficiency.
					this->_Allocated += (int)(this->_Allocated > 100 ? (this->_Allocated / 10.0f) : 10.0f);
					this->_Items = (T **)realloc(this->_Items, this->_Allocated * sizeof(T));
				}

				this->_Items[this->_Used] = (T *)calloc(length, 1);
				if (value)
				{
					memcpy(this->_Items[this->_Used], &value, length);
				}

				if (outIndex)
				{
					*outIndex = this->_Used;
				}

				return this->_Items[this->_Used++];
			}

			//Removes all but 10 percent of allocated overhead.
			void Compact()
			{
				int iCompact = this->_Used + (int)(this->_Used > 100 ? (this->_Used / 10.0f) : 10.0f);
				if (iCompact < this->_Allocated)
				{
					this->_Allocated = iCompact;
					this->_Items = (T **)realloc(this->_Items, this->_Allocated * sizeof(T));
				}
			}

			//Deletes all values from the vector matching on value.
			//Returns the count of items which were deleted.
			int DeleteAll(T value)
			{
				int iCount = 0;
				for (int i = this->_Used - 1; i >= 0 && this->_Used > 0;)
				{
					if (value == (*(this->_Items[i])))
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
			bool DeleteOne(T value)
			{
				for (int i = this->_Used - 1; i >= 0; i--)
				{
					if (value == (*(this->_Items[i])))
					{
						this->DeleteAt(i);
						return true;
					}
				}
				return false;
			}

			//Determines if a given value is in the vector.
			//Returns true if an item was found, otherwise false.
			bool Contiains(T value)
			{
				for (int i = this->_Used - 1; i >= 0; i--)
				{
					if (value == (*(this->_Items[i])))
					{
						return true;
					}
				}
				return false;
			}

			//Finds the index of a given value.
			//Returns the index of the found value, otherwise returns -1.
			int IndexOf(T value)
			{
				for (int i = this->_Used - 1; i >= 0; i--)
				{
					if (value == (*(this->_Items[i])))
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
					free((void *)this->_Items[this->_Used - 1]);
					this->_Used--;
				}
			}

			//Returns a pointer to the value by index.
			T at(int index)
			{
				if (index >= 0 && index < this->_Used)
				{
					return *this->_Items[index];
				}
				return (T)NULL;
			}

			void SortAscending()
			{
				bool bItemsMoved = true;

				while (bItemsMoved)
				{
					bItemsMoved = false;

					for (int i = 0; i < this->_Used - 1; i++)
					{
						if ((*(this->_Items[i])) > (*(this->_Items[i + 1])))
						{
							T tTemp = (*(this->_Items[i]));
							(*(this->_Items[i])) = (*(this->_Items[i + 1]));
							(*(this->_Items[i + 1])) = tTemp;

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
						if ((*(this->_Items[i])) < (*(this->_Items[i + 1])))
						{
							T tTemp = (*(this->_Items[i]));
							(*(this->_Items[i])) = (*(this->_Items[i + 1]));
							(*(this->_Items[i + 1])) = tTemp;

							bItemsMoved = true;
						}

					}
				}
			}
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	} //namespace::Collections
} //namespace::NSWFL
#endif
