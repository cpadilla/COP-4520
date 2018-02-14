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
