//
// Created by ZachiLoy on 08/01/2020.
//

#ifndef EX3_UNIQUEARRAYIMP_H
#define EX3_UNIQUEARRAYIMP_H

#include <functional>

template <class Element, class Compare>
UniqueArray<Element, Compare>::UniqueArray(unsigned int size) : size(size), data(new Element* [size]),dataInfo(new bool[size]),
                                                                currentNumberOfElements(0){
	for (unsigned int i = 0; i < size; i++) {
		dataInfo[i] = false;
	}
}
template <class Element, class Compare>
UniqueArray<Element, Compare>::UniqueArray(const UniqueArray& other) :  size(other.size),
                                                                        data(new Element* [other.size]),dataInfo(new bool[other.size]),
																		                                    currentNumberOfElements(other.currentNumberOfElements){
	
	for (unsigned int i = 0; i < size; i++) {
		if (other.dataInfo[i])	{
			data[i] = new Element(*other.data[i]);
		}

		dataInfo[i] = other.dataInfo[i];
	}

}
template <class Element, class Compare>
UniqueArray<Element, Compare>::~UniqueArray() {
	delete[] dataInfo;
	delete[] data;

}

template <class Element, class Compare>
unsigned int UniqueArray<Element, Compare>::insert(const Element& element) {
	Compare cmp = {};
	for (unsigned int i = 0; i < size; i++) { //This loop checks if the element is already exist (and didn't removed)
		if (dataInfo[i] and cmp(*(data[i]), element)) {

			return i;
		}
	}
	for (unsigned int i = 0; i < size; i++) {//This loop checks if there is a free place inside the array (not at the end)
		if (!dataInfo[i]) {
			data[i] = new Element(element);
			dataInfo[i] = true;
			currentNumberOfElements++;
			return i;
		}
	}
	throw UniqueArrayIsFullException();
	return 0;
}
template <class Element, class Compare>
bool UniqueArray<Element, Compare>::getIndex(const Element& element, unsigned int& index) const {
	Compare cmp = {};
	index = 0;
	for (unsigned int i = 0; i < size; i++,index++) {
		if (dataInfo[i] and cmp(*(data[i]), element)) {
			return true;
		}
	}
	return false;
}

template <class Element, class Compare>
const Element* UniqueArray<Element, Compare>::operator[](const Element& element) const {
	unsigned int tempIndex;
	if (getIndex(element, tempIndex)) {
		return data[tempIndex];
	}
	return  NULL;
}

template <class Element, class Compare>
bool UniqueArray<Element, Compare>::remove(const Element& element) {
	Compare cmp = {};
	for (unsigned int i = 0; i < size; ++i) {
		if (dataInfo[i] and cmp(*data[i], element)) {
			dataInfo[i] = false;
			currentNumberOfElements--;
			return true;
		}
	}
	return false;
}
template <class Element, class Compare>
unsigned int UniqueArray<Element, Compare>::getCount() const {
	return currentNumberOfElements;
}
template <class Element, class Compare>
unsigned int UniqueArray<Element, Compare>::getSize() const {
	return size;
}
template <class Element, class Compare>
UniqueArray<Element, Compare> UniqueArray<Element, Compare>::filter(const Filter& f) const {

	UniqueArray<Element, Compare> filteredUA = UniqueArray<Element, Compare>(size);

	for (unsigned int i = 0; i < size; i++) {
		if (dataInfo[i] and f(*data[i])) {
			filteredUA.data[i] = new Element(*data[i]);
			filteredUA.dataInfo[i] = true;
			filteredUA.currentNumberOfElements++;
		}
	}

	return filteredUA;
}
template <class Element, class Compare>
bool UniqueArray<Element, Compare>::isFull() const {
	for (unsigned int i = 0; i < size; i++) {
		if (dataInfo[i] == false) {
			return false;
		}
			
	}
	return true;
}
template <class Element, class Compare>
Element * UniqueArray<Element, Compare>::getElementAtIndex(unsigned int index) const {
	if (dataInfo[index] == false) {
		return NULL;
	}
	return data[index];
}
#endif //EX3_UNIQUEARRAYIMP_H
