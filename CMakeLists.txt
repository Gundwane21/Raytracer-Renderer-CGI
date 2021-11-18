cmake_minimum_required(VERSION 3.14)
project(Graph_Hw1)

set(CMAKE_CXX_STANDARD 17)


include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories(.)
include_directories(code_template)

add_executable(Graph_Hw1
        code_template/parser.cpp
        code_template/parser.h
        code_template/ppm.cpp
        code_template/ppm.h
        code_template/raytracer.cpp
        code_template/tinyxml2.cpp
        code_template/tinyxml2.h
        code_template/ray.cpp
        code_template/ray.h
        code_template/vec3.h
        sample_scenes_v2/inputs/berserker.xml
        sample_scenes_v2/inputs/bunny.xml
        sample_scenes_v2/inputs/car.xml
        sample_scenes_v2/inputs/cornellbox.xml
        sample_scenes_v2/inputs/dragon_lowres.xml
        sample_scenes_v2/inputs/horse_and_mug.xml
        sample_scenes_v2/inputs/killeroo.xml
        sample_scenes_v2/inputs/low_poly.xml
        sample_scenes_v2/inputs/mirror_spheres.xml
        sample_scenes_v2/inputs/monkey.xml
        sample_scenes_v2/inputs/simple.xml
        sample_scenes_v2/inputs/simple_reflectance.xml
        sample_scenes_v2/inputs/simple_shading.xml
        sample_scenes_v2/outputs/bfc_off/berserker.ppm
        sample_scenes_v2/outputs/bfc_off/bunny.ppm
        sample_scenes_v2/outputs/bfc_off/Car.ppm
        sample_scenes_v2/outputs/bfc_off/Car_front.ppm
        sample_scenes_v2/outputs/bfc_off/cornellbox_front.ppm
        sample_scenes_v2/outputs/bfc_off/cornellbox_inverse.ppm
        sample_scenes_v2/outputs/bfc_off/cornellbox_top.ppm
        sample_scenes_v2/outputs/bfc_off/dragon_lowres.ppm
        sample_scenes_v2/outputs/bfc_off/horse_and_mug.ppm
        sample_scenes_v2/outputs/bfc_off/killeroo.ppm
        sample_scenes_v2/outputs/bfc_off/low_poly_scene.ppm
        sample_scenes_v2/outputs/bfc_off/mirror_spheres.ppm
        sample_scenes_v2/outputs/bfc_off/monkey.ppm
        sample_scenes_v2/outputs/bfc_off/simple.ppm
        sample_scenes_v2/outputs/bfc_off/simple_reflectance.ppm
        sample_scenes_v2/outputs/bfc_off/simple_shading.ppm
        sample_scenes_v2/outputs/bfc_on/berserker.ppm
        sample_scenes_v2/outputs/bfc_on/bunny.ppm
        sample_scenes_v2/outputs/bfc_on/Car.ppm
        sample_scenes_v2/outputs/bfc_on/Car_front.ppm
        sample_scenes_v2/outputs/bfc_on/cornellbox_front.ppm
        sample_scenes_v2/outputs/bfc_on/cornellbox_inverse.ppm
        sample_scenes_v2/outputs/bfc_on/cornellbox_top.ppm
        sample_scenes_v2/outputs/bfc_on/dragon_lowres.ppm
        sample_scenes_v2/outputs/bfc_on/horse_and_mug.ppm
        sample_scenes_v2/outputs/bfc_on/killeroo.ppm
        sample_scenes_v2/outputs/bfc_on/low_poly_scene.ppm
        sample_scenes_v2/outputs/bfc_on/mirror_spheres.ppm
        sample_scenes_v2/outputs/bfc_on/monkey.ppm
        sample_scenes_v2/outputs/bfc_on/simple.ppm
        sample_scenes_v2/outputs/bfc_on/simple_reflectance.ppm
        sample_scenes_v2/outputs/bfc_on/simple_shading.ppm code_template/Sphere.cpp code_template/Sphere.h code_template/utility.h code_template/camera_bundle.cpp code_template/camera_bundle.h)

add_executable(Graph_Hw1_Tester
        code_template/parser.cpp
        code_template/parser.h
        code_template/ppm.cpp
        code_template/ppm.h
        code_template/tinyxml2.cpp
        code_template/tinyxml2.h
        code_template/ray.cpp
        code_template/ray.h
        code_template/vec3.h
        sample_scenes_v2/inputs/berserker.xml
        sample_scenes_v2/inputs/bunny.xml
        sample_scenes_v2/inputs/car.xml
        sample_scenes_v2/inputs/cornellbox.xml
        sample_scenes_v2/inputs/dragon_lowres.xml
        sample_scenes_v2/inputs/horse_and_mug.xml
        sample_scenes_v2/inputs/killeroo.xml
        sample_scenes_v2/inputs/low_poly.xml
        sample_scenes_v2/inputs/mirror_spheres.xml
        sample_scenes_v2/inputs/monkey.xml
        sample_scenes_v2/inputs/simple.xml
        sample_scenes_v2/inputs/simple_reflectance.xml
        sample_scenes_v2/inputs/simple_shading.xml
        sample_scenes_v2/outputs/bfc_off/berserker.ppm
        sample_scenes_v2/outputs/bfc_off/bunny.ppm
        sample_scenes_v2/outputs/bfc_off/Car.ppm
        sample_scenes_v2/outputs/bfc_off/Car_front.ppm
        sample_scenes_v2/outputs/bfc_off/cornellbox_front.ppm
        sample_scenes_v2/outputs/bfc_off/cornellbox_inverse.ppm
        sample_scenes_v2/outputs/bfc_off/cornellbox_top.ppm
        sample_scenes_v2/outputs/bfc_off/dragon_lowres.ppm
        sample_scenes_v2/outputs/bfc_off/horse_and_mug.ppm
        sample_scenes_v2/outputs/bfc_off/killeroo.ppm
        sample_scenes_v2/outputs/bfc_off/low_poly_scene.ppm
        sample_scenes_v2/outputs/bfc_off/mirror_spheres.ppm
        sample_scenes_v2/outputs/bfc_off/monkey.ppm
        sample_scenes_v2/outputs/bfc_off/simple.ppm
        sample_scenes_v2/outputs/bfc_off/simple_reflectance.ppm
        sample_scenes_v2/outputs/bfc_off/simple_shading.ppm
        sample_scenes_v2/outputs/bfc_on/berserker.ppm
        sample_scenes_v2/outputs/bfc_on/bunny.ppm
        sample_scenes_v2/outputs/bfc_on/Car.ppm
        sample_scenes_v2/outputs/bfc_on/Car_front.ppm
        sample_scenes_v2/outputs/bfc_on/cornellbox_front.ppm
        sample_scenes_v2/outputs/bfc_on/cornellbox_inverse.ppm
        sample_scenes_v2/outputs/bfc_on/cornellbox_top.ppm
        sample_scenes_v2/outputs/bfc_on/dragon_lowres.ppm
        sample_scenes_v2/outputs/bfc_on/horse_and_mug.ppm
        sample_scenes_v2/outputs/bfc_on/killeroo.ppm
        sample_scenes_v2/outputs/bfc_on/low_poly_scene.ppm
        sample_scenes_v2/outputs/bfc_on/mirror_spheres.ppm
        sample_scenes_v2/outputs/bfc_on/monkey.ppm
        sample_scenes_v2/outputs/bfc_on/simple.ppm
        sample_scenes_v2/outputs/bfc_on/simple_reflectance.ppm
        sample_scenes_v2/outputs/bfc_on/simple_shading.ppm code_template/Sphere.cpp code_template/Sphere.h code_template/utility.h code_template/camera_bundle.cpp code_template/camera_bundle.h code_template/tester.cpp)

enable_testing()

add_executable(
        ray_test
        code_template/ray.cpp
        code_template/ray.h
        code_template/vec3.h
        code_template/ray_test.cpp
)

target_link_libraries(
        ray_test
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(ray_test)