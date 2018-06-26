from conans import ConanFile, CMake, tools
from os import path

class RoseSliceTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def test(self):
        self.run(path.join("bin", "test_package"))
