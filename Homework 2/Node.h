#pragma once

template<class T>
struct Node {
public:
	T value;
	Node<T>* next;

	Node<T>();
	Node<T>(T value);
	~Node<T>();
};

template<class T>
Node<T>::Node() {

}

template<class T>
Node<T>::Node(T value)
{
	this->value = value;
}

template<class T>
Node<T>::~Node() {
}
