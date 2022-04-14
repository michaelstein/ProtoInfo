from conans import ConanFile, CMake, tools
import os


class ProtoInfoConan(ConanFile):
	settings = "os", "compiler", "build_type", "arch"
	generators = "cmake", "cmake_find_package", "virtualenv", "virtualrunenv", "virtualbuildenv"
	
	build_requires = (
		"cmake/3.22.3"
	)
	
	requires = (
		"gtest/cci.20210126",
		"humblelogging/3.0.3@insanefactory/stable",
		"protobuf/3.19.2",
		"qt/6.2.3",
		"ms-gsl/4.0.0",

		"zlib/1.2.12"
	)
	
	default_options = {
		"protobuf:shared": False,
		"qt:shared": True,
		"qt:opengl": "desktop"
	}

	#def config_options(self):
	#	if self.settings.os == "Windows":
	#		self.options["qt"].with_harfbuzz = False

	def imports(self):
		self.copy("*.so.*", dst="bin", src="lib")
		self.copy("*.so", dst="bin", src="lib")
		self.copy("*.dll", dst="bin", src="bin")
		self.copy("*.dylib*", dst="bin", src="lib")
		
		self.copy("*", dst="bin/plugins", src=os.path.join("res", "archdatadir", "plugins"), root_package="qt")
		self.copy("google/protobuf/*.proto", dst="bin/protobuf", src="include", root_package="protobuf")
