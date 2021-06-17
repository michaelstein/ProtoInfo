from conans import ConanFile, CMake, tools
import os, time, shutil


class ProtoInfoConan(ConanFile):
	settings = "os", "compiler", "build_type", "arch"
	generators = "cmake", "virtualrunenv", "virtualenv", "virtualbuildenv"
	
	build_requires = (
		"cmake_installer/3.15.5@conan/stable",
		"protoc_installer/3.9.1@bincrafters/stable"
	)
	
	requires = (
		"gtest/1.10.0",
		"humblelogging/3.0.3@insanefactory/stable",
		"protobuf/3.9.1",
		"qt/5.13.2@bincrafters/stable",
		"gsl_microsoft/2.0.0@bincrafters/stable",

		"openssl/1.0.2t",
	)
	
	default_options = {
		"protobuf:shared": False,
		"qt:shared": True,
		"qt:commercial": False,
		"qt:opengl": "desktop",
		"qt:openssl": True,
		"qt:qtcharts": True,
		"qt:qtconnectivity": True,
		"qt:qtgraphicaleffects": True,
		"qt:qtimageformats": True,
		"qt:qtlocation": True,
		"qt:qtmultimedia": True,
		"qt:qtnetworkauth": True,
		"qt:qtquickcontrols2": True,
		"qt:qtserialbus": True,
		"qt:qtserialport": True,
		"qt:qtspeech": True,
		"qt:qttranslations": True,
		"qt:qtvirtualkeyboard": True,
		"qt:with_sqlite3": True,
		"qt:with_mysql": False,
		"qt:with_pq": True,
		"qt:with_odbc": False,
		"qt:with_openal": False,
		"glib:shared": True,
		"zlib:shared": True,
		"lqr:shared": True,
	}

	def config_options(self):
		if self.settings.os == "Windows":
			self.options["qt"].with_harfbuzz = False

	def imports(self):
		self.copy("*.so.*", dst="bin", src="lib")
		self.copy("*.so", dst="bin", src="lib")
		self.copy("*.dll", dst="bin", src="bin")
		self.copy("*.dylib*", dst="bin", src="lib")
		
		self.copy("*", dst="bin/plugins", src="plugins", root_package="Qt")
		self.copy("google/protobuf/*.proto", dst="bin/protobuf", src="include", root_package="protobuf")
