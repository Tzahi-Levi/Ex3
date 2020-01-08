//
// Created by ZachiLoy on 08/01/2020.
//

#ifndef EX3_UNIQUEARRAYIMP_H
#define EX3_UNIQUEARRAYIMP_H

#include <functional>

template <class Element, class Compare>
UniqueArray<Element, Compare>::UniqueArray(unsigned int size) : size(size), nextIndex(0), currentNumberOfElements(0),
                                                                dataInfo(new bool[size]), data(new Element*[size]){
    for (int i=0; i<size ; i++){
        dataInfo[i] = false;
    }
}
template <class Element, class Compare>
UniqueArray<Element, Compare>::UniqueArray(const UniqueArray& other) : size(other.size), nextIndex(other.nextIndex),
                                                                       currentNumberOfElements(other.currentNumberOfElements),
                                                                       dataInfo(new bool[other.size]), data(new Element*[other.size]) {
    for (int i=0; i<nextIndex; i++){
        dataInfo[i] = other.dataInfo[i];
        //TODO מניחים שיש פה אופרטור השמה לאלמנט האם זה בסדר?
        *data[i] = *other.data[i];
    }
    for (int i=nextIndex; i<size; i++) {
        dataInfo[i] = false;
    }
}
template <class Element, class Compare>
UniqueArray<Element, Compare>::~UniqueArray(){
    delete [] dataInfo;
    delete[] data;

}

template <class Element, class Compare>
unsigned int UniqueArray<Element, Compare>::insert(const Element& element){

    for (unsigned int i=0; i<nextIndex; i++) { //This loop checks if the element is already exist (and didn't removed)
        if (dataInfo[i] and Compare(*data[i], element)) {
            return i;
        }
    }
    for (unsigned int i=0; i<nextIndex; i++) {//This loop checks if there is a free place inside the array (not at the end)
        if(!dataInfo[i]){
            //TODO יש פה הנחה שיש אופרטור השמה
            *data[i] = element;
            dataInfo[i] = true;
            currentNumberOfElements++;
            return i;
        }
    }
    if(nextIndex>=size){
        throw UniqueArrayIsFullException();
    }
    dataInfo[nextIndex] = true;
    currentNumberOfElements++;
    return nextIndex++;
}
template <class Element, class Compare>
bool UniqueArray<Element, Compare>::getIndex(const Element& element, unsigned int& index) const{
    for (unsigned int i=0; i<nextIndex ; i++) {
        if (Compare(*data[i], element)) {
            index = i;
            return true;
        }
    }
    return false;
}

template <class Element, class Compare>
const Element* UniqueArray<Element, Compare>::operator[](const Element& element) const {
    int tempIndex = 0;
    if(getIndex(element,tempIndex)){
        return data[tempIndex];
    }
    return  NULL;
}

template <class Element, class Compare>
bool UniqueArray<Element, Compare>::remove(const Element& element){
    for (int i = 0; i <nextIndex ; ++i) {
        if(dataInfo[i] and Compare(*data[i], element)) {
            dataInfo[i] = false;
            currentNumberOfElements--;
            return true;
        }
    }
    return false;
}
template <class Element, class Compare>
unsigned int UniqueArray<Element, Compare>::getCount() const{
    return currentNumberOfElements;
}
template <class Element, class Compare>
unsigned int UniqueArray<Element, Compare>::getSize() const {
    return size;
}

#endif //EX3_UNIQUEARRAYIMP_H
