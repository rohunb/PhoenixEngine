prebuildcommands { 
	os.copyfile("Libraries/Dll/x32/assimp.dll", "Build/Debug/assimpd.dll"),
	os.copyfile("Libraries/Dll/x32/ikpFlac.dll", "Build/Debug/ikpFlac.dll"),
	os.copyfile("Libraries/Dll/x32/ikpMP3.dll", "Build/Debug/ikpMP3.dll"),
	os.copyfile("Libraries/Dll/x32/irrklang.dll", "Build/Debug/irrklang.dll"),
	
	os.copyfile("Libraries/Dll/x32/assimp.dll", "Build/Release/assimp.dll"),
	os.copyfile("Libraries/Dll/x32/ikpFlac.dll", "Build/Release/ikpFlac.dll"),
	os.copyfile("Libraries/Dll/x32/ikpMP3.dll", "Build/Release/ikpMP3.dll"),
	os.copyfile("Libraries/Dll/x32/irrklang.dll", "Build/Release/irrklang.dll")
}
