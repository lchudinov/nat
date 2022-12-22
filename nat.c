#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct N_tag {
  const struct N_tag *prev;  
} N;

typedef struct R_tag {
  const N *p;
  const N *q;
} R;

N* Zero() {
  N *zero = malloc(sizeof(N));
  zero->prev = NULL;
  return zero;
}

N* Succ(const N *n) {
  N *succ = (N*)malloc(sizeof(N));
  succ->prev = n;
  return succ;
}

bool isZero(const N *n) {
  return n->prev == NULL;
}

bool isOne(const N *n) {
  return !isZero(n) && isZero(n->prev);
}

bool equal(const N *a, const N *b) {
  if (isZero(a) && isZero(b)) {
    return true;
  }
  if (isZero(a) && !isZero(b)) {
    return false;
  }
  if (!isZero(a) && isZero(b)) {
    return false;
  }
  return equal(a->prev, b->prev);
}

int toInt(const N *n) {
  int num = 0;
  while (!isZero(n)) {
    num++;
    n = n->prev;
  }
  return num;
}

const N* plus(const N *a, const N *b) {
  if (isZero(b)) {
    return a;
  }
  N* succA = Succ(a);
  return plus(succA, b->prev);
}

const N* mul(const N *a, const N *b) {
  if (isZero(b) || isZero(b)) {
    return Zero();
  }
  if (isOne(b)) {
    return a;
  }
  return plus(a, mul(a, b->prev));
}

const R* _R(const N *p, const N *q) {
  R *r = malloc(sizeof(R));
  r->p = p;
  r->q = q;
  return r;
}

const R* plusR(const R *a, const R *b) {
  R *r = malloc(sizeof(R));
  r->p = plus(mul(a->p, b->q), mul(a->q, b->p));
  r->q = mul(a->q, b->q);
  return r;
}

int main() {
  const N *_0 = Zero();
  const N *_1 = Succ(_0);
  const N *_2 = Succ(_1);
  const N *_4 = plus(_2, _2);
  const N *_8 = plus(_4, _4);
  const N *_32 = mul(_4, _8);
  const N *_10 = Succ(Succ(Succ(Succ(Succ(Succ(Succ(Succ(Succ(Succ(Zero()))))))))));
  const N *_100 = mul(_10, _10);
  const N *_10000 = mul(_100, _100);
  
  const R *_1_2 = _R(_1, _2);
  const R *_1_4 = _R(_1, _4);
  const R *_3_4 = plusR(_1_2, _1_4);

  printf("%d + %d = %d\n", toInt(_2), toInt(_2), toInt(_4));
  printf("%d + %d = %d\n", toInt(_4), toInt(_4), toInt(_8));
  printf("%d * %d = %d\n", toInt(_4), toInt(_8), toInt(_32));
  printf("%d %s %d\n", toInt(_2), equal(_2, _2) ? "==" : "!=", toInt(_2));
  printf("%d %s %d\n", toInt(_1), equal(_1, _2) ? "==" : "!=", toInt(_2));
  printf("%d * %d = %d\n", toInt(_10), toInt(_10), toInt(_100));
  printf("%d * %d = %d\n", toInt(_100), toInt(_100), toInt(_10000));
  printf("%d/%d + %d/%d = %d/%d\n", toInt(_1_2->p), toInt(_1_2->q), toInt(_1_4->p), toInt(_1_4->q), toInt(_3_4->p), toInt(_3_4->q));

  return 0;
}
