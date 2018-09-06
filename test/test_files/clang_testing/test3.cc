struct B {
  void f() {}
  virtual void vf() {}
};

struct D : public B {
  void vf() override;
};

void D::vf() {}

int main() {
  B *bp;
  bp = new B();
  bp->f();
  bp->vf();
  delete bp;
  bp = new D();
  bp->f();
  bp->vf();
}
