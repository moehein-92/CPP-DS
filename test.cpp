
#include <iostream>

class Stack{
	public:
		Stack();
		bool push(int x);
		int pop();
		bool isEmpty();
    // other lines omitted
};

class Queue{
	public:
		Queue();
		bool enqueue(int x);
		int dequeue();
		bool isEmpty();
    // other lines omitted
};

int main() {
    Stack s = Stack();
    Queue q = Queue();

    for(int i = 0; i < 5; i++){
    	s.push(i);
    	q.enqueue(i);
    }

    for(int i=0; i < 5; i++){
    	s.push(q.dequeue());
    	q.enqueue(s.pop());
    }

    while (!q.isEmpty())
        std::cout << q.dequeue() << " ";
}
