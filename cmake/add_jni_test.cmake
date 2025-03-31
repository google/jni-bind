function(add_jni_test test_name test_source)

    # workaround duplicated target test name "invoke_test"
    if (TARGET ${test_name})
        set(test_name ${test_name}_2)
    endif()

    add_executable(${test_name} ${test_source})

    target_link_libraries(${test_name}
        PRIVATE
            jni_bind
            gmock
            gtest_main)

    add_test(
        NAME ${test_name}
        COMMAND ${test_name})
endfunction()