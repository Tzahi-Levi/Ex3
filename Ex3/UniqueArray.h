#ifndef MTMPARKINGLOT_UNIQUEARRAY_H
#define MTMPARKINGLOT_UNIQUEARRAY_H

#include <functional>

template <class Element, class Compare = std::equal_to<Element>>
class UniqueArray {
	unsigned int size;
	Element** data;
	bool* dataInfo;
	int currentNumberOfElements;
public:
	UniqueArray(unsigned int size);
	UniqueArray(const UniqueArray& other);
	~UniqueArray();
	UniqueArray& operator=(const UniqueArray& uniqueArray) = delete;
	unsigned int insert(const Element& element);
	bool getIndex(const Element& element, unsigned int& index) const;
	const Element* operator[] (const Element& element) const;
	bool remove(const Element& element);
	unsigned int getCount() const;
	unsigned int getSize() const;
	bool isFull() const;
	Element * getElementAtIndex(unsigned int index) const ;


	class Filter {
	public:
		virtual bool operator() (const Element&) const = 0;
	};
	UniqueArray filter(const Filter& f) const;

	class UniqueArrayIsFullException:std::exception {};

};
#include "UniqueArrayImp.h"

#endif //MTMPARKINGLOT_UNIQUEARRAY_H
