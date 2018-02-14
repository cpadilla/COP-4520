#include "stdafx.h"
#include "Node.h"

template<class T>
Node<T>::Node() {

}

template<class T>
Node<T>::Node(T value)
{
	this->value = value;
	//this->next = NULL;
}

template<class T>
Node<T>::~Node() {

}
