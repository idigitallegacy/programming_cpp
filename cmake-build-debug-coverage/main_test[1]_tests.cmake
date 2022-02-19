add_test( Point.Creation /mnt/d/labs_prog_cpp/lab1/cmake-build-debug-coverage/main_test [==[--gtest_filter=Point.Creation]==] --gtest_also_run_disabled_tests)
set_tests_properties( Point.Creation PROPERTIES WORKING_DIRECTORY /mnt/d/labs_prog_cpp/lab1/cmake-build-debug-coverage)
add_test( Point.Setting /mnt/d/labs_prog_cpp/lab1/cmake-build-debug-coverage/main_test [==[--gtest_filter=Point.Setting]==] --gtest_also_run_disabled_tests)
set_tests_properties( Point.Setting PROPERTIES WORKING_DIRECTORY /mnt/d/labs_prog_cpp/lab1/cmake-build-debug-coverage)
add_test( Polygon.Init /mnt/d/labs_prog_cpp/lab1/cmake-build-debug-coverage/main_test [==[--gtest_filter=Polygon.Init]==] --gtest_also_run_disabled_tests)
set_tests_properties( Polygon.Init PROPERTIES WORKING_DIRECTORY /mnt/d/labs_prog_cpp/lab1/cmake-build-debug-coverage)
add_test( Polygon.SetAndPush /mnt/d/labs_prog_cpp/lab1/cmake-build-debug-coverage/main_test [==[--gtest_filter=Polygon.SetAndPush]==] --gtest_also_run_disabled_tests)
set_tests_properties( Polygon.SetAndPush PROPERTIES WORKING_DIRECTORY /mnt/d/labs_prog_cpp/lab1/cmake-build-debug-coverage)
add_test( Polygon.SquareAndPerimeter /mnt/d/labs_prog_cpp/lab1/cmake-build-debug-coverage/main_test [==[--gtest_filter=Polygon.SquareAndPerimeter]==] --gtest_also_run_disabled_tests)
set_tests_properties( Polygon.SquareAndPerimeter PROPERTIES WORKING_DIRECTORY /mnt/d/labs_prog_cpp/lab1/cmake-build-debug-coverage)
add_test( CorrectPolygon.Init /mnt/d/labs_prog_cpp/lab1/cmake-build-debug-coverage/main_test [==[--gtest_filter=CorrectPolygon.Init]==] --gtest_also_run_disabled_tests)
set_tests_properties( CorrectPolygon.Init PROPERTIES WORKING_DIRECTORY /mnt/d/labs_prog_cpp/lab1/cmake-build-debug-coverage)
add_test( Trapezium.InitAndChanging /mnt/d/labs_prog_cpp/lab1/cmake-build-debug-coverage/main_test [==[--gtest_filter=Trapezium.InitAndChanging]==] --gtest_also_run_disabled_tests)
set_tests_properties( Trapezium.InitAndChanging PROPERTIES WORKING_DIRECTORY /mnt/d/labs_prog_cpp/lab1/cmake-build-debug-coverage)
add_test( Triangle.InitAndChanging /mnt/d/labs_prog_cpp/lab1/cmake-build-debug-coverage/main_test [==[--gtest_filter=Triangle.InitAndChanging]==] --gtest_also_run_disabled_tests)
set_tests_properties( Triangle.InitAndChanging PROPERTIES WORKING_DIRECTORY /mnt/d/labs_prog_cpp/lab1/cmake-build-debug-coverage)
add_test( Additional.DynamicPolymorphism /mnt/d/labs_prog_cpp/lab1/cmake-build-debug-coverage/main_test [==[--gtest_filter=Additional.DynamicPolymorphism]==] --gtest_also_run_disabled_tests)
set_tests_properties( Additional.DynamicPolymorphism PROPERTIES WORKING_DIRECTORY /mnt/d/labs_prog_cpp/lab1/cmake-build-debug-coverage)
add_test( Variable.Init /mnt/d/labs_prog_cpp/lab1/cmake-build-debug-coverage/main_test [==[--gtest_filter=Variable.Init]==] --gtest_also_run_disabled_tests)
set_tests_properties( Variable.Init PROPERTIES WORKING_DIRECTORY /mnt/d/labs_prog_cpp/lab1/cmake-build-debug-coverage)
add_test( Polynom.Init /mnt/d/labs_prog_cpp/lab1/cmake-build-debug-coverage/main_test [==[--gtest_filter=Polynom.Init]==] --gtest_also_run_disabled_tests)
set_tests_properties( Polynom.Init PROPERTIES WORKING_DIRECTORY /mnt/d/labs_prog_cpp/lab1/cmake-build-debug-coverage)
add_test( Polynom.Maths /mnt/d/labs_prog_cpp/lab1/cmake-build-debug-coverage/main_test [==[--gtest_filter=Polynom.Maths]==] --gtest_also_run_disabled_tests)
set_tests_properties( Polynom.Maths PROPERTIES WORKING_DIRECTORY /mnt/d/labs_prog_cpp/lab1/cmake-build-debug-coverage)
set( main_test_TESTS Point.Creation Point.Setting Polygon.Init Polygon.SetAndPush Polygon.SquareAndPerimeter CorrectPolygon.Init Trapezium.InitAndChanging Triangle.InitAndChanging Additional.DynamicPolymorphism Variable.Init Polynom.Init Polynom.Maths)
