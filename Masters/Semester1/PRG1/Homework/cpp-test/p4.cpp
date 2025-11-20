#include <iostream>

class ch {
private:
  char *elements;
  size_t size;
  size_t capacity;

public:
  ch() {
    elements = new char[16];
    size = 0;
    capacity = 16;
  }
  ch(std::initializer_list<char> ls) {
    elements = new char[ls.size()];
    int i = 0;
    for (char li : ls) {
      if (std::isdigit(li)) {
        elements[i++] = li;
      }
    }
    size = i;
    capacity = i;
  }

  char &operator[](size_t idx) { return elements[idx]; }

  void insert(char c) {
    if (std::isdigit(c)) {
      if (size == capacity) {
        char *new_elements = new char[capacity * 2];
        std::copy(elements, elements + capacity, new_elements);
        delete[] elements;
        elements = new_elements;
				capacity *= 2;
      }
			elements[size++] = c;
    } else {
      std::cout << "not a digit" << std::endl;
    }
  }

  void remove(size_t idx) {
    if (idx >= size) {
      return;
    }
    for (size_t i = idx; i < size - 1;) {
      elements[i] = elements[++i];
    }
    --size;
  }

  void print() {
    for (size_t i = 0; i < size; ++i) {
      std::cout << elements[i] << std::endl;
    }
  }
};

int main() {
  ch test{'a', 'b', 'c'};
  test.print();
}
