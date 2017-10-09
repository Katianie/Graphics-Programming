/************************************************************************************
Filename		:   Object.cpp
Content			:	Implementation of the Object class.
Created			:   October 20th, 2014
Modified		:	July 4th, 2016
Original Author :   Tom Heath
Modified By		:	Eddie O'Hagan (Katianie)
Copyright		:   Copyright 2014 Oculus, Inc and Katianie.
All Rights reserved. Licensed under the Apache License, Version 2.0
(the "License"); you may not use this file except in compliance
with the License. You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*************************************************************************************/
#include "../Headers/stdafx.h"
#include "../Headers/Object.h"

/**
 * Uses _aligned_free to remove memory. All classes that inherit
 * from Object will use these functions when new or delete is used.
 * 
 * @author Katianie
 * @date 7/4/2016
 *
 * @param [in,out] ptr If non-null, the pointer to delete.
 *
 */
void Object::operator delete(void* ptr)
{
	_aligned_free(ptr);
}

/**
 * Uses _aligned_malloc to remove memory. All classes that inherit
 * from Object will use these functions when new or delete is used.
 *
 * @author Katianie
 * @date 7/4/2016
 *
 * @param size The size.
 * 
 * @return An aligned piece of memory.
 * 
 */
void* Object::operator new(size_t size)
{
	return _aligned_malloc(size, BYTE_ALIGNMENT);
}

