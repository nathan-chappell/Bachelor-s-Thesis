void f() {}

struct B {
  B() { f(); }
};

template<typename T> 
void g(T) {
  f();
}

int main() {
  B b;
  B *bp = new B();
  g<int>(0);
}
