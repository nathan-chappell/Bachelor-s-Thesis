void ac() {}
void ab() {}
void aa() {}

void (*ab_p)(void);

void a() {
  aa();
  ab();
  ac();

  ab_p = ab;
  ab_p();

  ab_p = aa;
  ab_p();
}

int main() { a(); }
