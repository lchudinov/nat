#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

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

N* One() {
  return Succ(Zero());
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

bool less(const N *a, const N *b) {
  if (isZero(a) && isZero(b)) {
    return false;
  }
  if (isZero(a) && !isZero(b)) {
    return true;
  }
  if (!isZero(a) && isZero(b)) {
    return false;
  }
  return less(a->prev, b->prev);
}

bool lessOrEqual(const N *a, const N *b) {
  if (isZero(a) && isZero(b)) {
    return true;
  }
  if (isZero(a) && !isZero(b)) {
    return true;
  }
  if (!isZero(a) && isZero(b)) {
    return false;
  }
  return lessOrEqual(a->prev, b->prev);
}

bool greater(const N *a, const N *b) {
  if (isZero(a) && isZero(b)) {
    return false;
  }
  if (isZero(a) && !isZero(b)) {
    return false;
  }
  if (!isZero(a) && isZero(b)) {
    return true;
  }
  return greater(a->prev, b->prev);
}

bool greaterOrEqual(const N *a, const N *b) {
  if (isZero(a) && isZero(b)) {
    return true;
  }
  if (isZero(a) && !isZero(b)) {
    return false;
  }
  if (!isZero(a) && isZero(b)) {
    return true;
  }
  return greaterOrEqual(a->prev, b->prev);
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

const N* minus(const N *a, const N *b) {
  assert(greaterOrEqual(a, b));
  if (isZero(b)) {
    return a;
  }
  return minus(a->prev, b->prev);
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

const N* divN(const N *a, const N *b) {
  assert(!isZero(b));
  if (isOne(b)) {
    return a;
  }
  if (greaterOrEqual(a, b)) {
    return plus(One(), divN(minus(a, b), b));
  } else {
    return Zero();
  }
}

const N* modN(const N *a, const N *b) {
  assert(!isZero(b));
  if (equal(a, b)) {
    return Zero();
  }
  if (less(a, b)) {
    return a;
  }
  return modN(minus(a, b), b);
}

const N* gcd(const N *a, const N *b) {
  if (isZero(a)) {
    return b;
  }
  if (isZero(b)) {
    return a;
  }
  return gcd(b, modN(a, b));
}

const R* _R(const N *p, const N *q) {
  R *r = malloc(sizeof(R));
  r->p = p;
  r->q = q;
  return r;
}

const R* simplifyR(const R *r) {
  const N *gcd_pq = gcd(r->p, r->q);
  if (!isZero(gcd_pq)) {
    const N* p = divN(r->p, gcd_pq);
    const N* q = divN(r->q, gcd_pq);
    return _R(p, q);
  }
  return r;
}

const R* plusR(const R *a, const R *b) {
  R *r = malloc(sizeof(R));
  r->p = plus(mul(a->p, b->q), mul(a->q, b->p));
  r->q = mul(a->q, b->q);
  return simplifyR(r);
}

int main() {
  const N *_0 = Zero();
  const N *_1 = Succ(_0);
  const N *_2 = Succ(_1);
  const N *_4 = plus(_2, _2);
  const N *_6 = plus(_4, _2);
  const N *_8 = plus(_4, _4);
  const N *_32 = mul(_4, _8);
  const N *_10 = Succ(Succ(Succ(Succ(Succ(Succ(Succ(Succ(Succ(Succ(Zero()))))))))));
  const N *_100 = mul(_10, _10);
  const N *_10000 = mul(_100, _100);
  const N *_6_gcd_8 = gcd(_6, _8);
  
  const R *_1_2 = _R(_1, _2);
  const R *_1_4 = _R(_1, _4);
  const R *_1_8 = _R(_1, _8);
  const R *_3_4 = plusR(_1_2, _1_4);
  const R *_5_8 = plusR(_1_2, _1_8);
  
  const N *_32div10 = divN(_32, _10);
  const N *_32mod10 = modN(_32, _10);

  printf("%d + %d = %d\n", toInt(_2), toInt(_2), toInt(_4));
  printf("%d + %d = %d\n", toInt(_4), toInt(_4), toInt(_8));
  printf("%d * %d = %d\n", toInt(_4), toInt(_8), toInt(_32));
  printf("%d %s %d\n", toInt(_2), equal(_2, _2) ? "==" : "!=", toInt(_2));
  printf("%d %s %d\n", toInt(_1), equal(_1, _2) ? "==" : "!=", toInt(_2));
  printf("%d * %d = %d\n", toInt(_10), toInt(_10), toInt(_100));
  printf("%d * %d = %d\n", toInt(_100), toInt(_100), toInt(_10000));
  printf("%d/%d + %d/%d = %d/%d\n", toInt(_1_2->p), toInt(_1_2->q), toInt(_1_4->p), toInt(_1_4->q), toInt(_3_4->p), toInt(_3_4->q));
  printf("%d/%d + %d/%d = %d/%d\n", toInt(_1_2->p), toInt(_1_2->q), toInt(_1_8->p), toInt(_1_8->q), toInt(_5_8->p), toInt(_5_8->q));
  printf ("%d / %d = %d\n", toInt(_32), toInt(_10), toInt(_32div10));
  printf ("%d %% %d = %d\n", toInt(_32), toInt(_10), toInt(_32mod10));
  printf ("gcd (%d, %d) = %d\n", toInt(_6), toInt(_8), toInt(_6_gcd_8));
  return 0;
}
