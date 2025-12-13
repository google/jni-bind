package com.jnibind.test;

import static com.google.common.truth.Truth.assertThat;
import static org.junit.Assert.assertThrows;

import org.junit.AfterClass;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;

@RunWith(JUnit4.class)
public final class ExceptionTest {
  static {
    System.load(
        System.getenv("JAVA_RUNFILES")
            + "/_main/javatests/com/jnibind/test/libexception_test_jni.so");
  }

  static native void jniTearDown();

  @AfterClass
  public static void doShutDown() {
    jniTearDown();
  }

  native void jniThrowException();

  @Test
  public void testJniThrowException() {
    Exception exception = assertThrows(Exception.class, () -> {
        jniThrowException();
    });
    assertThat(exception).hasMessageThat().isEqualTo("Test failed with unmet requirements");
  }

  native void jniThrowCustomException();

  @Test
  public void testJniThrowCustomException() {
    CustomException exception = assertThrows(CustomException.class, () -> {
        jniThrowCustomException();
    });
    assertThat(exception).hasMessageThat().isEqualTo("Custom failure message");
  }

  native void jniThrowExceptionFromStaticMethod();

  @Test
  public void testJniThrowExceptionFromStaticMethod() {
    Exception exception = assertThrows(Exception.class, () -> {
        jniThrowExceptionFromStaticMethod();
    });
    assertThat(exception).hasMessageThat().isEqualTo("Built Exception");
  }
}
