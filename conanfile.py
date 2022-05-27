from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout


class TestConan(ConanFile):
    name = "QtHttpServer"
    version = "0.1"

    # Optional metadata
    license = "GPL3"
    author = "?"
    url = "https://github.com/qt-labs/qthttpserver"
    description = "<Description of Test here>"
    topics = ("QT", "http", "server")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "src/*"

    requires = "qt/6.2.4"

    generators = "cmake_find_package", "cmake_paths"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

#    def package_info(self):
#        self.cpp_info.libs = ["test"]
