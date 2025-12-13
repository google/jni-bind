package com.jnibind.test;

public class ClassWithStaticMethod {
  public static Exception build() {
    return new Exception("Built Exception");
  }
}
